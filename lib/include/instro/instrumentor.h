/*
 * This is our user-level interface. A typical use case in our minds is
 * int main(...){
 *   InstRO myinstro = ROSE::getInstRO();
 *   ROSE::InstROFactory fac = myinstro.getFactory();
 *   Pass *sel = fac.getSelectionPass(myinstro.getPassManager());
 *   Pass *adapter = fac.getAdapterPass(myinstro.getPassManager(), sel);
 *   myinstro.run();
 * }
 */
namespace InstRO {

class PassManager;

class Pass {
 public:
	/** What exactly is our public interface? */
	virtual void run();
};

class PassFactory {
 public:
	virtual Pass *createFunctionDefinitionSelector() = 0;
	virtual Pass *createCygProfileAdapter(Pass *inputSelector) = 0;
};

class InstRO {
 public:
	virtual PassFactory *getFactory() = 0;

 protected:
	PassManager *pm;
};
}
