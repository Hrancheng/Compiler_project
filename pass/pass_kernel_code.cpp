#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;

namespace {
  struct DetachReattachKernelizePass : public FunctionPass {
    static char ID;
    DetachReattachKernelizePass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      std::vector<Instruction*> instructionsToKernelize;
      bool inKernelRegion = false;

      Module *mod = F.getParent();
      LLVMContext &Context = F.getContext();

      // Declare CUDA APIs
      declareCudaFunctions(mod);

      for (auto &B : F) {
        for (auto &I : B) {
          if (auto *call = dyn_cast<CallInst>(&I)) {
            if (call->getCalledFunction()->getName() == "detach") {
              inKernelRegion = true;
              continue;
            }
            if (call->getCalledFunction()->getName() == "reattach") {
              inKernelRegion = false;
              Function *kernelFunc = generateKernel(F, instructionsToKernelize);
              insertKernelLaunch(F, kernelFunc);
              instructionsToKernelize.clear();
              continue;
            }
          }
          if (inKernelRegion) {
            instructionsToKernelize.push_back(&I);
          }
        }
      }
      return false;
    }

    void declareCudaFunctions(Module *mod) {
      LLVMContext &Context = mod->getContext();
      FunctionCallee cudaMallocFunc = mod->getOrInsertFunction("cudaMalloc",
        FunctionType::get(IntegerType::getInt32Ty(Context), {PointerType::get(Type::getInt8PtrTy(Context), 0), IntegerType::getInt64Ty(Context)}, false));
      FunctionCallee cudaMemcpyFunc = mod->getOrInsertFunction("cudaMemcpy",
        FunctionType::get(IntegerType::getInt32Ty(Context), {PointerType::get(Type::getInt8PtrTy(Context), 0), PointerType::get(Type::getInt8PtrTy(Context), 0), IntegerType::getInt64Ty(Context), IntegerType::getInt32Ty(Context)}, false));
      FunctionCallee cudaFreeFunc = mod->getOrInsertFunction("cudaFree",
        FunctionType::get(IntegerType::getInt32Ty(Context), {PointerType::get(Type::getInt8PtrTy(Context), 0)}, false));
    }

    Function *generateKernel(Function &F, std::vector<Instruction*> &instructions) {
      FunctionCallee kernelFunc = F.getParent()->getOrInsertFunction(
        "cudaKernel", FunctionType::get(Type::getVoidTy(F.getContext()), {}, false)
      );

      BasicBlock *BB = BasicBlock::Create(F.getContext(), "entry", cast<Function>(kernelFunc.getCallee()));
      IRBuilder<> builder(BB);
      
      for (auto *Inst : instructions) {
        builder.Insert(Inst->clone());
      }

      builder.CreateRetVoid();

      verifyFunction(*cast<Function>(kernelFunc.getCallee()), &errs());
      return cast<Function>(kernelFunc.getCallee());
    }

    void insertKernelLaunch(Function &F, Function *kernelFunc) {
      IRBuilder<> builder(&F.getEntryBlock().front());
      builder.CreateCall(kernelFunc, {});
      declareCudaFunctions(F.getParent());
    }
  };
}

char DetachReattachKernelizePass::ID = 0;
static RegisterPass<DetachReattachKernelizePass> X("detachreattach", "CUDA Kernelize Detach and Reattach Pass", false, false);
