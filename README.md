# background
### C -> clang => LLVM IR (GCC use)-> GCC => Assembly
### C (including loop or recursion) -> Tapir front end tool => Tapir LLVM IR -> Tapir back end tool => Assembly (https://cilk.mit.edu/tapir/)
### CUDA -> clang => LLVM IR (LCC use)-> LCC => PTX
# goal
### try to create an LLVM pass Tapir LLVM IR -> pass => LLVM IR (LCC use)v=> PTX

# thoughts
### Tapir-LLVM includes three new instructions besides those of original LLVM:
### detach: child task beginning point, how to translate? 
### deattch: child task ending point, how to translate?
### sync: waiting for all child tasks to finish. how to translate?
