#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

// Use the new LLVM pass manager since the legacy one is phasing out, and
// we don't need to deal with inheritance when creating a new passs
namespace {

  struct CountFnBBPass : public PassInfoMixin<CountFnBBPass> {
    // Module granularity and analysis manager, since we need to pass 
    // through every function
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
      unsigned functionCount = 0;
      unsigned basicBlockCount = 0;
      
      // We have functions inside a module, and basic bodies inside each 
      // function.
      // Loop through functions by reference so we don't copy the objects.
      for (Function &F : M) {
        functionCount++;

        // Functions maintains their basic body count in LLVM18, so this is
        // constant time, more efficient than looping through the basic
        // bodies
        basicBlockCount += F.size();
      }
      
      // Print to C's stderr
      errs() 
        << "The program has a total of " << functionCount << " functions and "
        << basicBlockCount << " basic blocks.\n";

      // We didn't change the IR, so all cached analysis results are still
      // valid
      return PreservedAnalyses::all();
    }
  };

}


// Register the pass as a pass manager plugin so that we can manually
// run the pass with opt.
// Dynamically linked plugin, so that we can compile the pass as
// a shared library (.so file)
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    // Plugin metadata
    LLVM_PLUGIN_API_VERSION, "CountFnBBPass", LLVM_VERSION_STRING,
    // anonymous function to insert CountFnBBPass to the pipeline
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          // intercept the pass pipeline on the pass with the
          // expected name, so that CountFnBBPass runs during that pass.
          // This means that our pass will only run if the specified pipeline
          // contains the pass called count-fn-bb-pass
          if (Name == "count-fn-bb-pass") {
            MPM.addPass(CountFnBBPass());
            return true;
          }
          return false;
        });
    }};
}
