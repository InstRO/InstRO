/*
 * This file declares the standard InstRO Measurement Interface.
 *
 * The interface contains function declarations for all possible instrumentable constructs.
 * With each having start/end pairs.
 *
 * As this interface is more likely a C-style interface we use snake case here.
 */

#include "stddef.h"

/*
 * Check for GNU compiler to use GNU attributes
 */
#ifdef __GNUC__
#define _INSTRO_WEAK_DECL __attribute__((weak))
#else
#define _INSTRO_WEAK_DECL
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Called before any hook is called.
 * If construction needs to take place things go here.
 */
void __instro_init() _INSTRO_WEAK_DECL;

/*
 * Called before program exits.
 * Clean up code should be placed inside this function
 */
void __instro_fini() _INSTRO_WEAK_DECL;

/*
 * Resembles GNU __cyg_profile_enter function.
 * Gets called at the start of an instrumented function.
 */
void __instro_start_function(size_t construct_hash, void *callsite) _INSTRO_WEAK_DECL;

/*
 * Resembles GNU __cyg_profile_exit function.
 * Gets called before a function is left.
 */
void __instro_end_function(size_t construct_hash, void *callsite) _INSTRO_WEAK_DECL;


/*
 * Gets called before an instrumented statement is executed.
 */
void __instro_start_simple_statement(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called after an instrumentd statement was executed.
 */
void __instro_end_simple_statement(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called before an instrumented loop starts execution.
 */
void __instro_start_loop(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called after an instrumented loop exited execution.
 */
void __instro_end_loop(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called when entering an instrumented scope.
 */
void __instro_start_scope(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called before an instrumented scope is left.
 */
void __instro_end_scope(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called before an instrumented conditional statement is executed.
 */
void __instro_start_conditional(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called after an instrumented conditional statement is left.
 */
void __instro_end_conditional(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called before an expression is evaluated.
 */
void __instro_start_expression(size_t construct_hash) _INSTRO_WEAK_DECL;

/*
 * Gets called after an expression was evaluated.
 */
void __instro_end_expression(size_t construct_hash) _INSTRO_WEAK_DECL;

#ifdef __cplusplus
}
#endif
