#include <instro/adapterbuilder/OpenMPAdapterBuilder.h>

using namespace InstRO;

void OMP_AdapterBuilder::buildForNode (SgNode* node,Selector * decidingSelector){
        // Assert that we have a basic block as body if we have a body at all
        if(isSgOmpBodyStatement(node) && !isSgOmpForStatement(node)){
                if(!isSgBasicBlock(isSgOmpBodyStatement(node)->get_body())){
                        // Body is not a basic block - build a new one and connect it to the AST
                        SgBasicBlock* newBody = SageBuilder::buildBasicBlock(isSgOmpBodyStatement(node)->get_body());
                        isSgOmpBodyStatement(node)->set_body(newBody);
                        newBody->set_parent(node);
                }
        } else if (isSgOmpForStatement(node)) {
                if(!isSgBasicBlock(isSgForStatement(isSgOmpForStatement(node)->get_body())->get_loop_body())){
                        // Body is not a basic block - build a new one and connect it to the AST
                        SgBasicBlock* newBody = SageBuilder::buildBasicBlock(isSgForStatement(isSgOmpForStatement(node)->get_body())->get_loop_body());
                        isSgForStatement(isSgOmpForStatement(node)->get_body())->set_loop_body(newBody);
                        newBody->set_parent(isSgOmpForStatement(node)->get_body());
                }

        }

        // Distribute the nodes to the correct special handlers
        if(isSgOmpParallelStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpParallelStatement(node)->get_body()), isSgOmpParallelStatement(node));

        } else if (isSgOmpForStatement(node)){
                handleOpenMP(isSgForStatement(isSgOmpForStatement(node)->get_body()), isSgOmpForStatement(node));

        } else if (isSgOmpSectionsStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpSectionsStatement(node)->get_body()), isSgOmpSectionsStatement(node));

        } else if (isSgOmpSectionStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpSectionStatement(node)->get_body()), isSgOmpSectionStatement(node));

        } else if (isSgOmpSingleStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpSingleStatement(node)->get_body()), isSgOmpSingleStatement(node));

        } else if (isSgOmpTaskStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpTaskStatement(node)->get_body()), isSgOmpTaskStatement(node));

        } else if (isSgOmpMasterStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpMasterStatement(node)->get_body()), isSgOmpMasterStatement(node));

        } else if (isSgOmpCriticalStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpCriticalStatement(node)->get_body()), isSgOmpCriticalStatement(node));

        } else if (isSgOmpOrderedStatement(node)){
                handleOpenMP(isSgBasicBlock(isSgOmpOrderedStatement(node)->get_body()),isSgOmpOrderedStatement(node));

        } else if (isSgOmpAtomicStatement(node)){
                handleOpenMP(isSgExprStatement(isSgOmpAtomicStatement(node)->get_body()), isSgOmpAtomicStatement(node));

        } else if (isSgOmpBarrierStatement(node)){
                handleOpenMP(isSgOmpBarrierStatement(node));

        } else if (isSgOmpTaskwaitStatement(node)){
                handleOpenMP(isSgOmpTaskwaitStatement(node));

        } else if (isSgOmpFlushStatement(node)){
                handleOpenMP(isSgOmpFlushStatement(node));

        }
}
