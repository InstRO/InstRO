#ifndef INSTRUMENTOR_OPENMP_H__
#define INSTRUMENTOR_OPENMP_H__

#include "../common.h"

namespace InstRO{
/**
 * Selects all openmp-statements and -directives.
 * Only works if there actually are open-mp-directives in the ast. So they have to be lowered by adding -rose:openmp:ast_only when executing the translator
 * \ingroup Selector
 * \author InstRO
 */
class OpenMPSelector : public OutOfOrderSelector{
	public:
		void visit(SgNode* node);

		// May be used for fine-grained selection of specific omp-statements and -directives
#if 0
		void selectParallel();		/**< \brief Select parallel statements when passing over the AST */
		void unselectParallel();	/**< \brief Do no select parallel statements when passing over the AST */
		bool getSelectParallel();	/**< \brief Return selectionstatus of parallel statements */

		void selectFor();			/**< \brief Select for statements when passing over the AST */
		void unselectFor();			/**< \brief Do no select for statements when passing over the AST */
		bool getSelectFor();		/**< \brief Return selectionstatus of for statements */

		void selectSections();		/**< \brief Select sections statements when passing over the AST */
		void unselectSections();	/**< \brief Do no select sections statements when passing over the AST */
		bool getSelectSections();	/**< \brief Return selectionstatus of sections statements */

		void selectSection();		/**< \brief Select section statements when passing over the AST */
		void unselectSection();		/**< \brief Do no select section statements when passing over the AST */
		bool getSelectSection();	/**< \brief Return selectionstatus of section statements */

		void selectSingle();		/**< \brief Select single statements when passing over the AST */
		void unselectSingle();		/**< \brief Do no select single statements when passing over the AST */
		bool getSelectSingle();		/**< \brief Return selectionstatus of single statements */

		void selectTask();			/**< \brief Select task statements when passing over the AST */
		void unselectTask();		/**< \brief Do no select task statements when passing over the AST */
		bool getSelectTask();		/**< \brief Return selectionstatus of task statements */

		void selectMaster();		/**< \brief Select master statements when passing over the AST */
		void unselectMaster();		/**< \brief Do no select master statements when passing over the AST */
		bool getSelectMaster();		/**< \brief Return selectionstatus of master statements */

		void selectCritical();		/**< \brief Select critical statements when passing over the AST */
		void unselectCritical();	/**< \brief Do no select critical statements when passing over the AST */
		bool getSelectCritical();	/**< \brief Return selectionstatus of critical statements */

		void selectOrdered();		/**< \brief Select ordered statements when passing over the AST */
		void unselectOrdered();		/**< \brief Do no select ordered statements when passing over the AST */
		bool getSelectOrdered();	/**< \brief Return selectionstatus of ordered statements */

		void selectAtomic();		/**< \brief Select atomic statements when passing over the AST */
		void unselectAtomic();		/**< \brief Do no select atomic statements when passing over the AST */
		bool getSelectAtomic();		/**< \brief Return selectionstatus of atomic statements */

		void selectBarrier();		/**< \brief Select barrier statements when passing over the AST */
		void unselectBarrier();		/**< \brief Do no select barrier statements when passing over the AST */
		bool getSelectBarrier();	/**< \brief Return selectionstatus of barrier statements */

		void selectTaskwait();		/**< \brief Select taskwait statements when passing over the AST */
		void unselectTaskwait();	/**< \brief Do no select taskwait statements when passing over the AST */
		bool getSelectTaskwait();	/**< \brief Return selectionstatus of taskwait statements */

		void selectFlush();			/**< \brief Select flush statements when passing over the AST */
		void unselectFlush();		/**< \brief Do no select flush statements when passing over the AST */
		bool getSelectFlush();		/**< \brief Return selectionstatus of flush statements */
#endif
};

}
#endif // INSTRUMENTOR_OPENMP_H__

