#ifndef INSTRO_CORE_VISUALPASSMANAGER_H
#define INSTRO_CORE_VISUALPASSMANAGER_H

#include <vector>
#include <iostream>
#include <fstream>
// CI: This Class Extends the PassManger to enable output of the configuration graph
#include "instro/core/PassManager.h"
#include "instro/core/SimplePassManager.h"
namespace InstRO{

class Pass;

namespace Ext{
	class VisualizingPassManager: public PassManagement::SimplePassManager
{
	public:
		void outputConfiguration(std::string fileName)
		{
			std::ostream & out=std::cout;
			out << "graph InstROConfiguration {" << std::endl;
			for(auto & passEnvelope:passList)
			{
				void * pointer=reinterpret_cast<void*>(getPass(passEnvelope));
				bool inputOn,outputOn;
				inputOn=hasInputDependencies(getPass(passEnvelope));
				outputOn=hasOutputDependencies(getPass(passEnvelope));
				if (inputOn && outputOn)
				{
					// it is a filter node
					out << pointer << " [label=\"" << getPass(passEnvelope)->passName() << "\", shape=\"diamond\"];" << std::endl;
				}
				else if (inputOn && !outputOn)
				{
					// it is an adapter
					out << pointer << " [label=\"" << getPass(passEnvelope)->passName() << "\", shape=\"box\", peripheries=2]];" << std::endl;

				}
				else if (!inputOn && outputOn)
				{
					out << pointer << " [label=\"" << getPass(passEnvelope)->passName() << "\", shape=box];" << std::endl;
				}
				else
					out << pointer << " [label=\"" << getPass(passEnvelope)->passName() << "\", shape=circle];" << std::endl;
			}

			for(auto & passEnvelope:passList)
			{
				void * dest=reinterpret_cast<void*>(getPass(passEnvelope));
				for (auto & source:getPredecessors(passEnvelope))
				{
					void * src=reinterpret_cast<void*>(source);
					out <<src <<" -> "<< dest << " [];" << std::endl;
				}
			}
			out << "}" << std::endl;
		}
	};


} // End namespace PassManagement
} // End namespace InstRO

#endif