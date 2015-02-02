#ifndef INSTRUMENTOR_COMMON_H__
#define INSTRUMENTOR_COMMON_H__

// Our own exceptions!
#include "instro/exception/instroexception.h"

#include "instrumentor.h"
#include "utility/functionals.h"

namespace InstRO{
/**
 * \brief Implements the Instrumentor interface by storing the selectors and let them traverse the ast.
 *
 * \ingroup Instrumentor
 * \author InstRO
 */
class BasicInstrumentor : public Instrumentor{
public:
    BasicInstrumentor();

    virtual void select();			/**< \brief Lets every registered selector traverse the ast*/
    virtual void instrument();			/**< \brief Lets the registered adapterbuilder traverse the ast*/
    virtual void transform(){};                 /** \brief Lets the registered transformer traverse the ast*/

    virtual void setProject(SgProject* project); /**<  \brief Sets the project to project */
    virtual SgProject* getProject(); /**< \brief Get the actual project node */

    virtual void addSelector(Selector* sel); /**< \brief Adds a Selector to the instrumentor */
    virtual void removeSelector(Selector* sel);/**< \brief Removes a selector from the instrumentor*/

    virtual void addAdapterBuilder(AdapterBuilder* builder); /**< \brief Adds an adapterbuilder to the instrumentor */
    virtual void removeAdapterBuilder(AdapterBuilder* builder);/**< \brief Removes an adapterbuilder from the instrumentor */

    virtual void addTransformer(Transformer* transformer);
    virtual void removeTransformer(Transformer* transformer);

private:
    SgProject* project; // Holds project
    std::list<Selector*> selectorList; // holds a list of selector
    std::list<AdapterBuilder*> adapterBuilderList; // holds a list of adapter builders
    std::list<Transformer*> transformerList; // holds a list of all transformers

    bool hasSelector(Selector* sel);
};

/*
 * #############
 * # Selectors #
 * #############
 */

/**
 * \brief Takes care of traversing the AST, only one of the functions called before and after descending into the subtree has to be implemented
 * \ingroup Selector
 * \author InstRO
 */
class PrePostOrderSelector : public Selector, public AstPrePostProcessing{
public:
    virtual void selectionBegin(SgProject* project);		/**< \brief Gets called before the selection process starts. Might be used for initialization */
    virtual void selectionEnd(SgProject* project);		/**< \brief Gets called after the selection process has finished. Might be used for cleaning up */
    /** \brief Gets called before visiting the children of a node. Either this or postOrderVisit should be implemented. Otherwise the selector is useless */
    virtual void preOrderVisit(SgNode* n);
    /** \brief Gets called after visiting the children of a node. Either this or preOrderVisit should be implemented. Otherwise the selector is useless */
    virtual void postOrderVisit(SgNode *n);

    void traverseAST(SgNode* start);				/**< \brief Starts the traversal of the AST. */
};

/**
 * \brief Takes care of traversing the AST, only visit(SgNode* n) has to be implemented.
 *
 * Simply walks over all nodes in the AST with no specific order.
 * \ingroup Selector
 * \author InstRO
 */
class OutOfOrderSelector : public Selector, public AstSimpleProcessing{
public:
    virtual void selectionBegin(SgProject* project);		/**< \brief Gets called before the selection process starts. Might be used for initialization */
    virtual void selectionEnd(SgProject* project);		/**< \brief Gets called after the selection process has finished. Might be used for cleaning up */
    virtual void visit(SgNode *n) = 0;	/**< \brief Gets called for every node in the AST */

    void traverseAST(SgNode* start);	/**< \brief Starts the traversal of the AST. */
};


/**
 * \brief The purpose is to retrieve all nodes which have as Rose ast attribute corresponding to "attribName".
 * Nodes which fulfill this criterion will be marked as selected by this AttributeSelector, so
 * selectors processing the ast after this selector are able to locate such matches.
 *
 * \XXX 2013-10-10 JP: Is this selector really in use? (JP: e.g. CompoundSelector inherits from it!)
 *
 * \ingroup Selector
 * \author InstRO
 */
class AttributeSelector : public Selector{
public:
    /** As Rose-ast-attributes are connected to a string, this constructor takes a string to determine which attribute to retrieve */
    AttributeSelector(std::string attribName = ROSE_AST_CONTAINER_ATTRIB_NAME);
    /** Starts the traversal over the nodes starting at start */
    virtual void traverseAST(SgNode *start);
    /** Processes the list of nodes which have an attribute for the attribName string, just mark them as selected! */
    virtual void processNode(SgNode* n); // JPL: This goes to protected?
    /** returns an empty std::vector */
/*    virtual std::vector<ASTAttributeContainer::SharedAttributePointerType > getSelectionDecisionMarkers(); */

protected:
    /** Starts the processing of nodes starting at the startNode (which can be the SgProject as well) */
    virtual void startNodeProcessing(SgNode* startNode);
    ~AttributeSelector(){};

private:
    std::string attribName; // The name of the attribute to be retrieved by this AttributeSelector
    Rose_STL_Container<SgNode*> nodesList; /** Holds all found nodes which have the searched ast attribute */
};


/**
 * \brief Implements the ASTReWriter interface
 *
 * When registering an ASTReWriter to the BasicInstrumentor, the project member is set automatically.
 *
 * \todo 2013-10-26 JP: Introduce another method, to have startModificationPass not needing to be virtual?
 *
 * \ingroup Astrewriter
 * \author Jan-Patrick Lehr
 */
class ASTReWriterImpl : public ASTReWriter {
public:

	ASTReWriterImpl(Selector* decidingSelector);

	/**
	 * Implements a very basic checking of the internal state.
	 * It checks whether the decidingSelector is NULL or the project is NULL. If either is true an exception is thrown
	 *
	 * \throws exception
	 */
	virtual void validateInternalState();
	/*
	 * sets the internally stored SgProject pointer. This method is implicitly called, when the ASTReWriter is added to an instrumentor instance.
	 */
	void setProject(SgProject* project);

protected:
	/** Proxy method to switch between different possibilities to generate the node set */
	virtual std::vector<SgNode*> generateSelectedNodeSet();

	/** Query function to decide whether node has to be instrumented or not */
	bool isNodeSelected(SgNode* node);

	/** To retrieve the Selector this ASTModifier listens to */
	const Selector* getSelector();

private:
	/** Holds a SgProject */
	SgProject* project;

	/** Is the selector, the modifier should respect. */
	Selector* decidingSelector;
};


/**
 * \brief Instruments all nodes selected by the selectors the specific adapter builder instance should respect.
 *
 * If more than one Selector is of interest for a specific AdapterBuilder instance, one can use the CompoundSelector, to
 * give only one %Selector to the %AdapterBuilder.
 *
 * \todo 2013-10-26 JP: Implement the decision trees // 2014-08 RN: outdated?
 *
 * \ingroup Adapterbuilder
 * \author InstRO
 * \author roman
 */
class AdapterBuilder : public ASTReWriterImpl{
public:
	/* \param the selector whose markers are used for the transformation */
	AdapterBuilder(Selector* decidingSelector) :
			ASTReWriterImpl(decidingSelector) {
	}

	/** Starts the modification pass */
	virtual void startModificationPass();

protected:

	/** Applies AdapterBuilder to a single node */
	virtual void buildForNode(SgNode* node) = 0;

	/** called after all marked nodes were handled */
	virtual void modificationEnd() {}
};

/**
 * Interface for transformation steps, e.g. Preparation for recursion-depth instrumentation.
 *
 * \ingroup Transformer
 * \author InstRO
 */
class Transformer : public ASTReWriterImpl{
public:

	/** Inherited from ASTModifier. Starts transformation */
	virtual void startModificationPass();

	/** Gets called from within startModificationPass() */
	virtual void transform() = 0;
};


}
#endif // INSTRUMENTOR_COMMON_H__
