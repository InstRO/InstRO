#ifndef FUNCTIONPRINTERADAPTERBUILDER_H
#define FUNCTIONPRINTERADAPTERBUILDER_H

#include "GenericFunctionAdapterBuilder.h"

namespace InstRO{

/**
 * This AdapterBuilder simply prints all selected methods.
 * \ingroup Adapterbuilder
 * \author Roman Neß
 */
class FunctionPrinterAdapterBuilder : public GenericFunctionAdapterBuilder{
        public:
                /** \name Constructor/Destructor */
                /** \{ */

                FunctionPrinterAdapterBuilder(Selector* sel, bool printToFile=false);
                ~FunctionPrinterAdapterBuilder();
                /** \} */

                void buildForNode(SgNode* node, Selector* decidingSelector);

                void buildForNode(SgNode* node);

        private:
                Selector* decidingSelector;

                bool printToFile;
};


}
#endif
