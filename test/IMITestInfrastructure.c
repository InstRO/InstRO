/**
 * TODO Please help to find a better name for this file...
 */

#include "InstROMeasurementInterface.h"
#include "stdio.h"
#include "stdlib.h"

#define ENTERS(x) ENTER(0, x)
#define EXITS(x) EXIT(0, x)

#ifdef CDEBUG
#define ENTER_F(id, x) \
	printf("Entering "); \
	push(id);            \
	PRINT(#x);           \
	printf(" construct id %zu\n", id);

#define EXIT_F(id, x) \
	printf("Exiting "); \
	pop(id);            \
	PRINT(#x);          \
	printf(" construct id %zu\n", id);

#define ENTER(id, x)   \
	printf("Entering "); \
	PRINT(#x) printf("\n");
#define EXIT(id, x)   \
	printf("Leaving "); \
	PRINT(#x) printf("\n");

#define PRINT(x) printf(x);

#else	// CDEBUG

#define ENTER_F(id, x) push(id);

#define EXIT_F(id, x) pop(id);

#define ENTER(id, x)
#define EXIT(id, x)

#endif	// CDEBUG
				/**
				 * We use this as a tracking stack for the identifiers pused and pop while running an instrumented version of a piece of
				 * code.
				 * Right now, this solely checks whether the order is right.
				 */
#define MAX_STACK_SIZE 100000
size_t globalStack[MAX_STACK_SIZE];
int cur = 0;

void push(size_t id) {
	if (cur >= MAX_STACK_SIZE) {
		exit(-1);
	}
	globalStack[cur] = id;
	cur++;
}

void pop(size_t id) {
	if (globalStack[cur - 1] != id) {
		fprintf(stderr, "Awaited Identifier %zu but got Identifier %zu\n", globalStack[cur - 1], id);
		exit(-1);
	}
	cur--;
}

void __instro_init() {
	ENTERS(instro_init)
	EXITS(instro_init)
}

void __instro_fini() {
	ENTERS(instro_fini)
	EXITS(instro_fini)
}

void __instro_start_function(size_t construct_hash, void *callsite) {
	ENTER_F(construct_hash, instro_start_function)
	EXIT(construct_hash, instro_start_function)
}

void __instro_end_function(size_t construct_hash, void *callsite) {
	ENTER(construct_hash, instro_end_function)
	EXIT_F(construct_hash, instro_end_function)
}

void __instro_start_simple_statement(size_t construct_hash) {
	ENTER_F(construct_hash, instro_start_simple_statement)
	EXIT(construct_hash, instro_start_simple_statement)
}

void __instro_end_simple_statement(size_t construct_hash) {
	ENTER(construct_hash, instro_end_simple_statement)
	EXIT_F(construct_hash, instro_end_simple_statement)
}

void __instro_start_loop(size_t construct_hash) {
	ENTER_F(construct_hash, instro_start_loop)
	EXIT(construct_hash, instro_start_loop)
}

void __instro_end_loop(size_t construct_hash) {
	ENTER(construct_hash, instro_end_loop)
	EXIT_F(construct_hash, instro_end_loop)
}

void __instro_start_scope(size_t construct_hash) {
	ENTER_F(construct_hash, instro_start_scope)
	EXIT(construct_hash, instro_start_scope)
}

void __instro_end_scope(size_t construct_hash) {
	ENTER(construct_hash, instro_end_scope)
	EXIT_F(construct_hash, instro_end_scope)
}

void __instro_start_conditional(size_t construct_hash) {
	ENTER_F(construct_hash, instro_start_condition)
	EXIT(construct_hash, instro_start_condition)
}

void __instro_end_conditional(size_t construct_hash) {
	ENTER(construct_hash, instro_end_condition)
	EXIT_F(construct_hash, instro_end_condition)
}

void __instro_start_expression(size_t construct_hash) {
	ENTER_F(construct_hash, instro_start_expression)
	EXIT(construct_hash, instro_start_expression)
}

void __instro_end_expression(size_t construct_hash) {
	ENTER(construct_hash, instro_end_expression)
	EXIT_F(construct_hash, instro_end_expression)
}
