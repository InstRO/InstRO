#ifndef INSTRO_CORE_PASSIMPLEMENTATION_H
#define INSTRO_CORE_PASSIMPLEMENTATION_H

#include "instro/core/ConstructSet.h"

namespace InstRO {
// We need a forward declaration of Pass, since Pass and PassImplementation are circular
class Pass;

namespace Core {

/**
 * This class is the user-interface to create his own pass.
 * One needs to inherit from this class and implement a compiler dependent pass.
 * Using the getInput(passId) one can retrieve the ConstructSet of one of the
 * predecessors.
 */
class PassImplementation {
	friend class InstRO::Pass;

 public:
	/** Since the Pass Implementation no longer knows of this ChannelConfiguration object, it can now be default
	 * constructed */
	PassImplementation() {}

	virtual ~PassImplementation() {}

	/**
	 * The init method can be used to allocate ressources independent from the compiler and predeccors within the
	 * instrumentation graph, like configuration files etc.
	 */
	virtual void init() {}	// provide default impl because this is the default case

	/**
	 * The method were the actual work is done.
	 */
	virtual void execute() = 0;

	/**
	 * Counterpart to the init method. Should be used to close / deallocate ressources which have been allocated in
	 * init().
	 * Note: Deallocating the outputSet is prohibited during finalize.
	 */
	virtual void finalize() {}	// provide default impl because this is the default case

	/** Frees the output ConstructSet. Automatically called. */
	void releaseOutput() { outputSet.clear(); }

	/** returns the ConstructSet generated by this pass */
	ConstructSet *getOutput() { return &outputSet; };

	/** returns a set of constructs, which haven been altered by this pass */
	const ConstructSet *getCollisionSet() const { return &collisionSet; };

 protected:
	/** The generated set of constructs the entity selected */
	ConstructSet outputSet;

	/** This set is used to track alterations to the AST and notify which nodes have been invalidated */
	ConstructSet collisionSet;

	/** Accessor method for the ConstructSet retrieved from input channel (counting from 0) */
	const ConstructSet *getInput(int channel);

 private:
	Pass *managingPass;
};

}	// namespace Core
}	// namespace InstRO

#endif
