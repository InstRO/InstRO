#ifdef USE_ROSE_DUMMY
class SgNode {};
class SgProject: public SgNode{}
class SgExpression: public SgNode{};
SgExpression * isSgExpression(SgNode *);
#endif
