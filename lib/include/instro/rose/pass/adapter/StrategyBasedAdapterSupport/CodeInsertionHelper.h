#ifndef CODE_INSERTION_HELPER_H__
#define CODE_INSERTION_HELPER_H__

#include <rose.h>

#include <vector>

namespace InstRO {

/**
 * \brief Abstraction to insert code at a specific point
 * 
 * Code is inserted at a point which is configured at creation time
 * 
 * It is not allowed to delete a configured insertion target as long as
 * the instance of CodeInsertionHelper is alive
 * 
 * It is not allowed to delete intermediate inserted code as long as the
 * instance is alive
 * 
 * Do not derive from this class!
 * 
 * \ingroup Util
 * 
 * \author Matthias Zoellner
 */
class CodeInsertionHelper {

public:
	/**
	 * Values to determine where a new node is placed.
	 * For prepend / append, new nodes are placed in inner scope.
	 * The before / after values indicate, that the new nodes are placed at the same level.
	 */
	enum RelativePosition {
		prepend,
		append,
		before,
		after
	};

	/**
	 * Main constructor with support for various strategies
	 * 
	 * First implementation: The caller has to insert a hidden SgDeclarationStatement
	 * to the AST, which is used as relative base for subsequent calls to
	 * SageInterface::insertStatementAfter(...)
	 */
	CodeInsertionHelper(SgDeclarationStatement* anchor);

	/**
	 * Support more relative positions for subsequent statements
	 */
	CodeInsertionHelper(SgStatement* anchor, RelativePosition insertionPosition);

	CodeInsertionHelper(const std::vector<std::pair<SgStatement*, RelativePosition> >& anchorList);

	/**
	 * Constructor with support for multiple anchor nodes
	 */
	CodeInsertionHelper(const std::vector<SgDeclarationStatement*>& anchorList);

	/** Standard copy constructor */
	CodeInsertionHelper(const CodeInsertionHelper& other);

	/** Standard copy operator */
	CodeInsertionHelper& operator =(const CodeInsertionHelper& other);

	/** Standard destructor */
	~CodeInsertionHelper();

	/** Insert statement according to the configuration */
	void pushStatement(SgStatement* node);

private:
	/** Default constructor is not available */
	CodeInsertionHelper();

	std::vector<std::pair<SgStatement*, RelativePosition> > currentAnchors;
};

} // namespace InstRO


#endif // CODE_INSERTION_HELPER_H__

