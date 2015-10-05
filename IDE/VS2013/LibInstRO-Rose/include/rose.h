#ifdef USE_ROSE_DUMMY

#ifndef ROSE_H
#define ROSE_H

#include <iterator>
#include <vector>

#include <string>
class ROSE_VisitorPatternDefaultBase {};
class Sg_File_Info {
 public:
	int get_line();
	int get_col();
};
class SgNode {
 public:
	void accept(ROSE_VisitorPatternDefaultBase&);
	SgNode* get_parent();
	std::string unparseToString() { return std::string(); }
	std::string class_name();
	std::string get_name();
};
class SgStatement : public SgNode {};
class SgQualifiedName {
 public:
	std::string getString();
};
class SgFunctionDeclaration : public SgNode {
 public:
	SgQualifiedName get_qualified_name();
};
class SgBasicBlock : public SgNode {
 public:
	Sg_File_Info* get_startOfConstruct();
	Sg_File_Info* get_endOfConstruct();
	int cfgIndexForEnd();
};
class SgFunctionDefinition : public SgNode {
 public:
	SgBasicBlock* get_body();
	SgFunctionDeclaration* get_declaration();
};
class SgIfStmt : public SgNode {
 public:
	SgStatement* get_conditional();
};
class SgSwitchStatement : public SgStatement {};
class SgForStatement : public SgStatement {};
class SgScopeStatement : public SgStatement {};
class SgDeclarationStatement : public SgStatement {};
class SgWhileStmt : public SgStatement {};
class SgDoWhileStmt : public SgStatement {};

class SgExpression : public SgNode {};
class SgVariableDefition : public SgNode {};
class SgVariableDeclaration : public SgNode {
 public:
	SgVariableDefition* get_definition();
};
class SgFile : public SgNode {};
class SgSourceFile : public SgNode {};

class SgSymbol : public SgNode {
 public:
	SgVariableDeclaration* get_declaration();
};
class SgFunctionRefExp : public SgNode {
 public:
	SgFunctionDeclaration* getAssociatedFunctionDeclaration();
	SgSymbol* get_symbol();
};
class SgMemberFunctionRefExp : public SgNode {
 public:
	SgFunctionDeclaration* getAssociatedMemberFunctionDeclaration();
};
class SgFunctionParameterList : public SgNode {};
SgFunctionRefExp* isSgFunctionRefExp(SgNode*);
SgFunctionDefinition* isSgFunctionDefinition(SgNode*);
SgMemberFunctionRefExp* isSgMemberFunctionRefExp(SgNode*);

class SgProject : public SgNode {
 public:
	void unparse();
};
class SgExprListExp : public SgNode {};
class SgFunctionRefExp : public SgNode {};

SgProject* frontend(int argv, char** argc);

bool V_SgFunctionDefinition();
bool V_SgForStatement();
bool V_SgTemplateFunctionDefinition();
bool V_SgNode();

namespace NodeQuery {
std::vector<SgNode*> querySubTree(SgNode*, bool (*function)());
}

template <class T>
class Rose_STL_Container : public std::vector<T> {};

template <class Payload>
class Iterator {
 public:
	Payload operator*();
	Iterator(const Iterator&);
	~Iterator();
	Iterator& operator=(const Iterator&);
	bool operator!=(const Iterator&);
	Iterator& operator++();	// prefix increment
	Iterator operator*() const;
	friend void swap(Iterator& lhs, Iterator& rhs);	// C++11 I think
};
template <class Foo>
class MyContainer {
 public:
	Iterator<Foo> begin();
	Iterator<Foo> end();
};
namespace SageInterface {

template <class T>
T* getEnclosingNode(SgNode* start, bool stop);
template <class T>
Rose_STL_Container<T*> querySubTree(SgNode*, bool (*function)());
}

class AstPrePostProcessing {
 public:
	void traverse(SgNode*);
};
class SgVarRefExpr : public SgExpression {
 public:
	SgSymbol* get_symbol();
};

SgVarRefExpr* isSgVarRefExp(SgNode*);

SgIfStmt* isSgIfStmt(SgNode*);
SgExpression* isSgExpression(SgNode*);
SgDoWhileStmt* isSgDoWhileStmt(SgNode*);
SgWhileStmt* isSgWhileStmt(SgNode*);
SgSwitchStatement* isSgSwitchStatement(SgNode*);
SgForStatement* isSgForStatement(SgNode*);
SgBasicBlock* isSgBasicBlock(SgNode*);
SgDeclarationStatement* isSgDeclarationStatement(SgNode*);
SgVariableDeclaration* isSgVariableDeclaration(SgNode*);
SgFunctionDefinition* isSgFunctionDefinition(SgNode*);
SgFunctionDeclaration* isSgFunctionDeclaration(SgNode*);
SgScopeStatement* isSgScopeStatement(SgNode*);
SgStatement* isSgStatement(SgNode*);
SgFile* isSgFile(SgNode*);
SgProject* isSgProject(SgNode*);
SgSourceFile* isSgSourceFile(SgNode*);

SgExprListExp* isSgExprListExp(SgNode*);
SgFunctionRefExp* isSgFunctionRefExp(SgNode*);
class CFGNode;
class CFGEdge {
 public:
	CFGNode target();
};
class CFGNode {
 public:
	bool operator<(const CFGNode) const;
	bool isInteresting();
	SgNode* getNode();
	std::string toString();
	int getIndex();
	Rose_STL_Container<CFGEdge> outEdges();
};
class VirtualCFG {
 public:
	static CFGNode cfgBeginningOfConstruct(SgNode*);
};

#endif
#endif
