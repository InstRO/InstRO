/*
 * This file declares the standard InstRO Measurement Interface.
 *
 * The interface contains function declarations for all possible instrumentable constructs.
 * With each having start/end pairs.
 *
 * As this interface is more likely a C-style interface we use snake case here.
 */

/*
 * Resembles GNU __cyg_profile_enter function.
 * Gets called at the start of an instrumented function.
 */
void __instro_start_function(long construct_hash, void *callsite);

/*
 * Resembles GNU __cyg_profile_exit function.
 * Gets called before a function is left.
 */
void __instro_end_function(long construct_hash, void *callsite);


/*
 * Gets called before an instrumented statement is executed.
 */
void __instro_start_statement(long construct_hash);

/*
 * Gets called after an instrumentd statement was executed.
 */
void __instro_end_statement(long construct_hash);

/*
 * Gets called before an instrumented loop starts execution.
 */
void __instro_start_loop(long construct_hash);

/*
 * Gets called after an instrumented loop exited execution.
 */
void __instro_end_loop(long construct_hash);

/*
 * Gets called when entering an instrumented scope.
 */
void __instro_start_scope(long construct_hash);

/*
 * Gets called before an instrumented scope is left.
 */
void __instro_end_scope(long construct_hash);

/*
 * Gets called before an instrumented conditional statement is executed.
 */
void __instro_start_conditional(long construct_hash);

/*
 * Gets called after an instrumented conditional statement is left.
 */
void __instro_end_conditional(long construct_hash);

/*
 * Gets called before an expression is evaluated.
 */
void __instro_start_expression(long construct_hash);

/*
 * Gets called after an expression was evaluated.
 */
void __instro_end_expression(long construct_hash);
