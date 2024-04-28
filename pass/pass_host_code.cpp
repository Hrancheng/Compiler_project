#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;

namespace {
    struct CudaHostFunctionPass : public ModulePass {
        static char ID;
        CudaHostFunctionPass() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            LLVMContext &Context = M.getContext();
            FunctionCallee hostFunc = declareHostFunction(M);
            Function *hostFunction = cast<Function>(hostFunc.getCallee());
            
            populateHostFunction(hostFunction, M);

            verifyFunction(*hostFunction, &errs());
            return true;
        }

        FunctionCallee declareHostFunction(Module &M) {
            LLVMContext &Context = M.getContext();
            return M.getOrInsertFunction("cudaHostFunction",
                FunctionType::get(Type::getVoidTy(Context), {}, false));
        }

        void populateHostFunction(Function *F, Module &M) {
            IRBuilder<> builder(&F->getEntryBlock());

            // Find kernel functions
            FunctionCallee kernel1 = M.getFunction("cudaKernel1");
            FunctionCallee kernel2 = M.getFunction("cudaKernel2");

            if (!kernel1 || !kernel2) {
                errs() << "Kernel functions not found in the module\n";
                return;
            }

            // Declare CUDA memory management functions
            FunctionCallee cudaMalloc = M.getOrInsertFunction("cudaMalloc",
                FunctionType::get(IntegerType::getInt32Ty(M.getContext()), {PointerType::get(IntegerType::getInt8PtrTy(M.getContext()), 0), IntegerType::getInt64Ty(M.getContext())}, false));
            FunctionCallee cudaMemcpy = M.getOrInsertFunction("cudaMemcpy",
                FunctionType::get(IntegerType::getInt32Ty(M.getContext()), {PointerType::get(IntegerType::getInt8PtrTy(M.getContext()), 0), PointerType::get(IntegerType::getInt8PtrTy(M.getContext()), 0), IntegerType::getInt64Ty(M.getContext()), IntegerType::getInt32Ty(M.getContext())}, false));
            FunctionCallee cudaFree = M.getOrInsertFunction("cudaFree",
                FunctionType::get(IntegerType::getInt32Ty(M.getContext()), {PointerType::get(IntegerType::getInt8PtrTy(M.getContext()), 0)}, false));

            // Memory allocation for device
            Value *deviceMem = builder.CreateAlloca(IntegerType::getInt8PtrTy(M.getContext()), 0, "deviceMem");
            Value *size = ConstantInt::get(IntegerType::getInt64Ty(M.getContext()), 1024); // Assume 1024 bytes for now
            builder.CreateCall(cudaMalloc, {deviceMem, size});

            // Memory transfer (host to device, device to host as needed)
            // Assume we have a host memory pointer already: hostMem
            Value *hostMem = builder.CreateAlloca(IntegerType::getInt8PtrTy(M.getContext()), 0, "hostMem");
            builder.CreateCall(cudaMemcpy, {deviceMem, hostMem, size, ConstantInt::get(IntegerType::getInt32Ty(M.getContext()), 1)}); // 1 for cudaMemcpyHostToDevice

            // Call kernels
            builder.CreateCall(kernel1, {deviceMem}); // Assume kernel takes device memory as argument
            builder.CreateCall(kernel2, {deviceMem});

            // Free device memory
            builder.CreateCall(cudaFree, {deviceMem});

            // Return from the host function
            builder.CreateRetVoid();
        }
    };
}

char CudaHostFunctionPass::ID = 0;
static RegisterPass<CudaHostFunctionPass> Y("cudaHostFunction", "Generate CUDA Host Function to Call Kernels", false, false);
