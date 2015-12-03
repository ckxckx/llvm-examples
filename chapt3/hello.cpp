#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/ErrorOr.h"

using namespace llvm;

static cl::opt<std::string> FileName(cl::Positional, cl::desc("Bitcode file"), cl::Required);

int main(int argc, char *argv[])
{
	cl::ParseCommandLineOptions(argc, argv, "LLVM hello world\n");
	LLVMContext context;
	ErrorOr<std::unique_ptr<MemoryBuffer>> mb = MemoryBuffer::getFile(FileName);
	if (std::error_code ec = mb.getError()) {
		errs() << ec.message();
		return -1;
	}

	ErrorOr<std::unique_ptr<llvm::Module> > m = parseBitcodeFile(mb.get()->getMemBufferRef(), context);
	if (std::error_code ec = m.getError()) {
		errs() << "Error reading bitcode: " << ec.message() << "\n";
		return -1;
	}

	for (Module::const_iterator i = m.get()->getFunctionList().begin(), 
		e = m.get()->getFunctionList().end(); i != e; ++i) {
		if (!i->isDeclaration()) {
			outs() << i->getName() << " has " << i->size() << " basic block(s).\n";
		}
	}
	return 0;
}