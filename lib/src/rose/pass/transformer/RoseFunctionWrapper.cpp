#include "instro/rose/pass/transformer/RoseFunctionWrapper.h"

#include "instro/rose/core/RoseConstructSet.h"

#include "instro/rose/utility/ScopeStackGuard.hpp"
#include "instro/rose/utility/FunctionRenamer.h"
#include "instro/utility/Logger.h"

#include <sstream>
#include <boost/functional/hash.hpp>

using namespace SageInterface;
using namespace SageBuilder;

using namespace InstRO;
using namespace InstRO::Rose;
using namespace InstRO::Rose::Transformer;

RoseFunctionWrapper::RoseFunctionWrapper(NameTransformer nameTransformer)
		: RosePassImplementation(),
			nameTrafo(nameTransformer),
			defPrefix(),
			wrapPrefix(),
			mainScope(nullptr),
/*			inputPass(input),*/
			renamingPass(nullptr)

{}

RoseFunctionWrapper::RoseFunctionWrapper(NameTransformer nameTransformer,
																				 const std::string &definitionPrefix, const std::string &wrapperPrefix)
		: RosePassImplementation(),
			nameTrafo(nameTransformer),
			defPrefix(definitionPrefix),
			wrapPrefix(wrapperPrefix),
			mainScope(nullptr)
/*			inputPass(input),
			renamingPass(renaming) */{}

RoseFunctionWrapper::~RoseFunctionWrapper() {}

void RoseFunctionWrapper::init() {
	findMainScope();
	if (mainScope) {
		initMainScope(mainScope);
	}
}

void RoseFunctionWrapper::execute() {
	// determine the nodes which are used as starting points for the search of function calls
	RoseNodeList funCallSearchSP;
	if (renamingPass) {
		auto nodeSet = retrieveNodes(1);
//		auto nodeSet = getInput(1);
		funCallSearchSP.insert(funCallSearchSP.end(), nodeSet.begin(), nodeSet.end());
	} else {
		// search the whole project if no renaming pass has been specified
		if (SgProject *project = SageInterface::getProject()) {
			funCallSearchSP.push_back(project);
		} else {
			logIt(ERROR) << "Failed to find the current SgProject, no function calls will be renamed" << std::endl;
		}
	}

	// process input into function declarations which are to be wrapped
	auto inputNodes = retrieveNodes(0);
//	auto inputNodes = getInput(0);

	std::unordered_set<SgFunctionDeclaration *> inputFunctions;
	inputFunctions.reserve(inputNodes.size());
	for (SgNode *node : inputNodes) {
		SgFunctionDeclaration *funDecl = findInputDeclaration(node);
		if (funDecl) {
			inputFunctions.insert(isSgFunctionDeclaration(funDecl->get_firstNondefiningDeclaration()));
		}
	}

	// wrap each function
	for (SgFunctionDeclaration *funDecl : inputFunctions) {
		wrapFunction(funDecl, funCallSearchSP);
	}
}
#if 0
InstRO::Core::ChannelConfiguration RoseFunctionWrapper::createChannelConfig(InstRO::Pass *input,
																																						InstRO::Pass *renaming) {
	std::vector<InstRO::Pass *> passes{input};

	auto channelConfig =
			InstRO::Core::ChannelConfiguration(passes.begin(), passes.end(), ::InstRO::Core::ConstructTraitType::CTExpression,
																				 ::InstRO::Core::ConstructTraitType::CTFunction);
	if (renaming) {
		channelConfig.setConstructLevel(renaming, ::InstRO::Core::ConstructTraitType::CTExpression,
																		::InstRO::Core::ConstructTraitType::CTGlobalScope);
	}

	return channelConfig;
}
#endif

RoseFunctionWrapper::RoseNodeSet RoseFunctionWrapper::retrieveNodes(int channel) {
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface cs(getInput(channel));
	RoseNodeSet nodes;
	nodes.reserve(cs.size());

	for (auto construct : cs) {
		auto rc = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);
		nodes.insert(rc->getNode());
	}

	return nodes;
}

void RoseFunctionWrapper::findMainScope() {
	mainScope = nullptr;

	if (SgProject *project = SageInterface::getProject()) {
		if (SgFunctionDeclaration *mainDecl = findMain(project)) {
			mainScope = getGlobalScope(mainDecl);
		}
	}

	if (!mainScope) {
		logIt(ERROR) << "Failed to find main function, wrappers are placed inside the respective translation unit"
								 << std::endl;
	}
}

void RoseFunctionWrapper::initMainScope(SgScopeStatement *scope) {
	// Subclasses should provide meaningful initialization logic
}

std::string RoseFunctionWrapper::getDefinitionPrefix() const { return defPrefix; }
void RoseFunctionWrapper::setDefinitionPrefix(const std::string &prefix) { defPrefix = prefix; }

std::string RoseFunctionWrapper::getWrapperPrefix() const { return wrapPrefix; }
void RoseFunctionWrapper::setWrapperPrefix(const std::string &prefix) { wrapPrefix = prefix; }

void RoseFunctionWrapper::wrapFunction(SgFunctionDeclaration *fDecl, const RoseNodeList &funCallSearchStartPoints) {
	if (isSgMemberFunctionDeclaration(fDecl)) {
		logIt(WARN) << "Creating wrappers for class member functions has not yet been explicitly implemented and "
									 "will probably not work..."
								<< std::endl;
	}

	// attempt to get the function definition if one is available
	SgFunctionDefinition *fDefinition = nullptr;
	if (SgFunctionDeclaration *defFunDecl = isSgFunctionDeclaration(fDecl->get_definingDeclaration())) {
		fDecl = defFunDecl;
		fDefinition = defFunDecl->get_definition();
	}

	SgScopeStatement *fDeclScope = fDecl->get_scope();
	std::string filePostfix;
	SgScopeStatement *targetScope;	// the scope where the wrapper will be placed
	// if the scope of the main function has not been found, the wrappers are created in the scope of the translation
	// unit.
	// in order to avoid duplicate function definitions at link time a postfix depending on the current source file is
	// appended.
	if (!mainScope) {
		filePostfix = generatePostfix(fDecl);
		targetScope = getGlobalScope(fDeclScope);
	} else {
		targetScope = mainScope;
	}
	// push the target scope onto the scope stack of ROSE & make sure that it is popped after the guard is destroyed
	InstRO::Rose::Utility::ScopeStackGuard targetScopeGuard(targetScope);

	// generate names: apply the NameTransformer to the original name and create the names for the definition (only used
	// if one is available) and wrapper
	SgName originalFunctionName = fDecl->get_name();
	std::string transformedName = nameTrafo(originalFunctionName.getString());
	std::string definitionName = defPrefix + originalFunctionName.getString() + filePostfix;
	std::string wrapperName = wrapPrefix + originalFunctionName.getString() + filePostfix;

	InstRO::Rose::Utility::FunctionRenamer funcRenamer(fDecl);
	// we only need to rename function calls if the name of the wrapper function differs from the original name
	if (!wrapPrefix.empty() || !filePostfix.empty()) {
		// query AST for function calls to the original function declaration: search the whole project or only the sub trees
		// specified by the renaming selector
		bool foundNone = funcRenamer.findFunctionCalls(funCallSearchStartPoints).empty();
		if (foundNone) {
			logIt(WARN) << "Wrapper '" << wrapperName
									<< "' is not used since there are no function calls which have to be renamed" << std::endl;
		}
	}

	// check if this is a defining function declaration -> rename it according to the pre- and postfix for definitions
	bool renamedFunctionDefinition = false;
	if (fDefinition) {
		// skip the node if there is no prefix for either the definition or the wrapper -> definition & wrapper would share
		// the same name!
		if (defPrefix.empty() && wrapPrefix.empty()) {
			logIt(ERROR) << "Skipping node due to missing prefix for renaming: " << originalFunctionName << std::endl;
			return;
		} else {
			funcRenamer.renameFunctionDefinition(definitionName);
			renamedFunctionDefinition = true;
		}
	}

	// create defining wrapper function declaration
	SgFunctionDeclaration *wrapperDecl =
			buildDefiningFunctionDeclaration(wrapperName, fDecl->get_orig_return_type(), cloneFunctionParameterList(fDecl));
	appendStatement(wrapperDecl);

	// rename function calls to match the wrapper
	funcRenamer.redirectFunctionCalls(wrapperDecl);

	// if we are dealing with a function definition, we call it instead of the name generated by the name transformer
	std::string &funName = (fDefinition == nullptr) ? transformedName : definitionName;

	// check if a forward declaration has to be inserted for the function called inside the wrapper.
	// Note: must be done before building the body of the wrapper because building the function call will create an entry
	// in the symbol table
	SgFunctionSymbol *transFunSym = lookupFunctionSymbolInParentScopes(funName, wrapperDecl->get_type());
	if (!transFunSym) {
		// build the forward declaration:
		SgFunctionDeclaration *forwardDecl = buildNondefiningFunctionDeclaration(wrapperDecl);
		forwardDecl->set_name(funName);

		insertStatementBefore(wrapperDecl, forwardDecl);
	}

	buildWrapperBody(fDecl, wrapperDecl, funName);

	// add the created wrapper, renamed function calls and the definition (if it has been renamed) to the output of this
	// pass
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface output(&outputSet);
	auto &constructProvider = InstRO::Rose::Core::RoseConstructProvider::getInstance();
	output.put(constructProvider.getConstruct(wrapperDecl->get_definition()));
	for (auto *funCall : funcRenamer.getFoundFunctionCalls()) {
		output.put(constructProvider.getConstruct(funCall));
	}
	if (renamedFunctionDefinition) {
		output.put(constructProvider.getConstruct(fDefinition));
	}
}

SgFunctionDeclaration *RoseFunctionWrapper::findInputDeclaration(SgNode *node) {
	switch (node->variantT()) {
		case V_SgFunctionCallExp: {
			SgFunctionCallExp *fCallExp = isSgFunctionCallExp(node);
			SgFunctionDeclaration *aFDecl = fCallExp->getAssociatedFunctionDeclaration();

			if (!aFDecl) {
				logIt(ERROR) << "Skipping node due to missing associated function declaration: " << get_name(node) << std::endl;
				return nullptr;
			} else {
				return aFDecl;
			}
		}
		case V_SgFunctionRefExp: {
			SgFunctionDeclaration *aFDecl = isSgFunctionRefExp(node)->getAssociatedFunctionDeclaration();
			if (!aFDecl) {
				logIt(ERROR) << "Skipping node due to missing associated function declaration: " << get_name(node) << std::endl;
				return nullptr;
			} else {
				return aFDecl;
			}
		}
		case V_SgFunctionDeclaration:
			return isSgFunctionDeclaration(node);
		case V_SgFunctionDefinition: {
			SgFunctionDefinition *fDef = isSgFunctionDefinition(node);
			return fDef->get_declaration();
		}
		default:
			logIt(ERROR) << "Skipping node due to invalid input type: " << node->class_name() << std::endl;
			return nullptr;
	}
}

std::string RoseFunctionWrapper::generatePostfix(SgFunctionDeclaration *fDecl) {
	boost::hash<std::string> sHash;
	std::stringstream sstream;
	sstream << "_";

	// we want the name of the source file, not the header
	SgFile *sourceFile = getEnclosingNode<SgFile>(fDecl);
	std::size_t filenameHash = sHash(sourceFile->get_file_info()->get_filenameString());

	sstream << std::hex << filenameHash;
	return sstream.str();
}

SgExprListExp *RoseFunctionWrapper::buildFunctionCallArguments(const SgInitializedNamePtrList *argList) {
	SgExprListExp *fCallArgs = buildExprListExp();

	for (SgInitializedNamePtrList::const_iterator argIt = argList->begin(); argIt != argList->end(); ++argIt) {
		appendExpression(fCallArgs, buildVarRefExp((*argIt)->get_name()));
	}

	return fCallArgs;
}

SgFunctionParameterList *RoseFunctionWrapper::cloneFunctionParameterList(const SgFunctionDeclaration *fDec) {
	const SgInitializedNamePtrList &fArgs = fDec->get_args();
	SgFunctionParameterList *reDecPList = buildFunctionParameterList();

	for (SgInitializedNamePtrList::const_iterator argIt = fArgs.begin(); argIt != fArgs.end(); ++argIt) {
		appendArg(reDecPList, buildInitializedName((*argIt)->get_name(), (*argIt)->get_type()));
	}

	return reDecPList;
}

void RoseFunctionWrapper::buildWrapperBody(SgFunctionDeclaration *fDec, SgFunctionDeclaration *wrapperDec,
																					 const std::string &functionToCall) {
	// place the following statements in the body of the wrapper definition
	InstRO::Rose::Utility::ScopeStackGuard wrapperBodyGuard(wrapperDec->get_definition()->get_body());

	SgFunctionRefExp *calledRef = buildFunctionRefExp(functionToCall, fDec->get_type());
	SgExprListExp *args = buildFunctionCallArguments(&wrapperDec->get_args());
	SgFunctionCallExp *orgCall = buildFunctionCallExp(calledRef, args);

	SgType *retType = wrapperDec->get_orig_return_type();

	// void functions do not need returns
	if (isSgTypeVoid(retType)) {
		appendStatement(buildExprStatement(orgCall));
	} else {
		appendStatement(buildReturnStmt(orgCall));
	}
}

std::string RoseFunctionWrapper::IdentityNameTransformer::operator()(const std::string &name) { return name; }

// MPIFunctionWrapper

RoseMPIFunctionWrapper::RoseMPIFunctionWrapper()
		: RoseFunctionWrapper(PMPINameTransformer()) {}

RoseMPIFunctionWrapper::RoseMPIFunctionWrapper(const std::string &definitionPrefix, const std::string &wrapperPrefix)
		: RoseFunctionWrapper(PMPINameTransformer(), definitionPrefix, wrapperPrefix) {}

std::string RoseMPIFunctionWrapper::PMPINameTransformer::operator()(const std::string &mpiName) {
	return "P" + mpiName;
}

void RoseMPIFunctionWrapper::initMainScope(SgScopeStatement *scope) {
	// look for MPI_Comm to determine if MPI declarations are available
	if (!lookupTypedefSymbolInParentScopes("MPI_Comm", scope)) {
		insertHeader("mpi.h", PreprocessingInfo::after, false, scope);
	}
}
