#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct DetachReplacePass : public ModulePass {
    static char ID;
    DetachReplacePass() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
      bool modified = false;

      for (Function &F : M) {
        if (containsDetach(F)) {
          createNewKernel(M);
          F.eraseFromParent();
          modified = true;
          break; // 假设只处理一个包含 detach 的函数
        }
      }

      return modified;
    }

    bool containsDetach(Function &F) {
      for (auto &B : F) {
        for (auto &I : B) {
          if (CallInst *CI = dyn_cast<CallInst>(&I)) {
            if (CI->getCalledFunction() && CI->getCalledFunction()->getName() == "detach") {
              return true;
            }
          }
        }
      }
      return false;
    }

    void createNewKernel(Module &M) {
      LLVMContext &Context = M.getContext();
      FunctionType *FuncType = FunctionType::get(Type::getVoidTy(Context), false);
      Function *KernelFunc = Function::Create(FuncType, Function::ExternalLinkage, "kernel", &M);

      BasicBlock *Entry = BasicBlock::Create(Context, "entry", KernelFunc);
      IRBuilder<> Builder(Entry);

      // 添加内核函数的代码，可以使用示例中的模板
      // 添加更多的IR代码构建逻辑

      // 确保新生成的函数是有效的
      verifyFunction(*KernelFunc);
    }
  };
}

char DetachReplacePass::ID = 0;
static RegisterPass<DetachReplacePass> X("detach-replace", "Replace Functions containing 'detach' with new kernel", false, false);
