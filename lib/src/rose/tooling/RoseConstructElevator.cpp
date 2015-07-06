#include <memory>

#include "instro/core/Helper.h"

#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseConstructElevator.h"

#include "rose.h"

namespace InstRO {
	namespace Rose{
		namespace Tooling {
			namespace ConstructElevator {
				namespace ConstructElevatorHelper{

					struct CLExpressionPraedicate{
						bool operator()(SgNode * n) const
						{
							if (isSgExpression(n) != nullptr) return true;
							return false;
						}
					};

					struct CLStatementPraedicate{
					
						bool operator()(SgNode * n) const
						{
							if (isSgStatement(n) != nullptr) return true;
							return false;
						}
					};

					struct CLLoopPraedicate{
						bool operator()(SgNode * n) const
						{
							if (isSgDoWhileStmt(n) != nullptr) return true;
							if (isSgWhileStmt(n) != nullptr) return true;
							if (isSgForStatement(n) != nullptr) return true;
							return false;
						}
					};

					struct CLConditionalPraedicate{
						bool operator()(SgNode * n) const
						{
							if (isSgIfStmt(n) != nullptr) return true;
							if (isSgSwitchStatement(n) != nullptr) return true;
							return false;
						}
					};

					struct CLScopePraedicate{
						bool operator()(SgNode * n) const
						{
							if (isSgBasicBlock(n) != nullptr) return true;
							return false;
						}
					};

					struct CLSimplePraedicate{
						bool operator()(SgNode * n) const
						{
							if (!CLStatementPraedicate()(n)) return false;
							if (CLFunctionPraedicate()(n)) return false;
							if (CLScopePraedicate()(n)) return false;
							if (CLConditionalPraedicate()(n)) return false;
							if (CLLoopPraedicate()(n)) return false;
							if (CLFileScopePraedicate()(n)) return false;
							return true;
						}
					};

					struct CLFunctionPraedicate{
						bool operator()(SgNode * n) const
						{
							if (isSgFunctionDefinition(n) != nullptr) return true;
							return false;
						}
					};

					struct CLFileScopePraedicate{
						bool operator()(SgNode * n) const
						{
							if (isSgFile(n) != nullptr) return true;
							return false;
						}
					};

					struct CLGlobalScopePraedicate{
						bool operator()(SgNode * n) const
						{
							if (isSgGlobal(n) != nullptr) return true;
							return false;
						}
					};



						template <class Predicate> 
						std::shared_ptr<InstRO::Rose:Core::RoseConstruct> raiseConstruct(InstRO::Rose::Core::RoseConstruct * src, Predicate pred){
							SgNode *current=src->getNode();
							// CI Walk the AST upwards, until the current node is either NULL or it is the desired construct type
							while (current != nullptr && !pred(current))
							{
								current = current->get_parent();
							}
							// geth the corresponding construct from the RoseConstructProvider
							return InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(current);
						}

						template <class Predicate>
						std::set<std::shared_ptr<InstRO::Rose:Core::RoseConstruct> > lowerConstruct(InstRO::Rose::Core::RoseConstruct * src, Predicate pred){
							std::set<std::shared_ptr<InstRO::Rose:Core::RoseConstruct> > retSet;
							SgNode *current = src->getNode();
							// rose-query sub-tree with the right selectors
							Rose_STL_Container<SgNode*> nodes = SageInterface::querySubTree<SgBinaryOp>(funcDef, V_SgNode);
							for (auto node:nodes)
							{
								// check if the node is of the desired type ... and add it to the outputset
								if (pred(node)) retSet.insert(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));

							}
							
							return retSet ;
						}

				}
				using namespace ConstructElevatorHelper;
				
				std::unique_ptr<InstRO::Core::ConstructSet> ConstructElevator::raise(InstRO::Core::ConstructSet *inputCS, InstRO::Core::ConstructLevelType cl) {
					InstRO::Rose::Core::RoseConstruct * roseConstruct;
					InstRO::InfracstructureInterface::ConstructSetCompilerInterface input(inputCS);
					std::unique_ptr<InstRO::Core::ConstructSet> newConstructSet = std::make_unique<InstRO::Core::ConstructSet>();
					InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(newConstructSet.get());
					for (auto construct : input){
						std::shared_ptr<InstRO::Rose::Core::RoseConstruct> newConstruct;
						roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct.get());
						if (roseConstruct == nullptr)
							throw std::string("A non InstRO::Rose::Core::RoseConstruct in the ROSE interace. Either multiple compiler interfaces are used, or programming error");
						switch (cl){
						case InstRO::Core::ConstructLevelType::CLExpression:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLStatement:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLLoop:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLConditional:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLScope:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLSimple:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLFunction:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLFileScope:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLGlobalScope:
							newConstruct = raiseConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						}
						output.put(newConstruct);

					}
					return newConstructSet;

				};
					// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
				std::unique_ptr<InstRO::Core::ConstructSet> ConstructElevator::lower(InstRO::Core::ConstructSet *inputCS, InstRO::Core::ConstructLevelType cl)		{
					InstRO::Rose::Core::RoseConstruct * roseConstruct;
					InstRO::InfracstructureInterface::ConstructSetCompilerInterface input(inputCS);
					std::unique_ptr<InstRO::Core::ConstructSet> newConstructSet = std::make_unique<InstRO::Core::ConstructSet>();
					InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(newConstructSet.get());

					//CI: check each input construct seperately
					for (auto construct : input){
						std::set<std::shared_ptr<InstRO::Rose::Core::RoseConstruct> > newConstructs;
						//CI: make sure it is a ROSE construct
						roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct.get());
						if (roseConstruct == nullptr)
							throw std::string("A non InstRO::Rose::Core::RoseConstruct in the ROSE interace. Either multiple compiler interfaces are used, or programming error");
						// Usea different target predicate, depending on the input level
						switch (cl){
						case InstRO::Core::ConstructLevelType::CLExpression:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLStatement:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLLoop:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLConditional:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLScope:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLSimple:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLFunction:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLFileScope:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						case InstRO::Core::ConstructLevelType::CLGlobalScope:
							newConstructs = lowerConstruct(roseConstruct, CLFunctionPraedicate());
							break;
						}
						for (auto newConstruct: newConstructs)
							output.put(newConstruct);

					}
					return newConstructSet;
				}
			}
		}
	}
}