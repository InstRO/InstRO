/**
 * TODO Please help to find a better name for this file...
 */

#include "InstROMeasurementInterface.h"
#include "stdio.h"

#define ENTER(x)       \
	printf("Entering "); \
	PRINT(#x);
#define EXIT(x)       \
	printf("Exiting "); \
	PRINT(#x);
#define PRINT(x) \
	printf(x);     \
	printf("\n");

void __instro_init() {
	// TODO Implement me
	ENTER(instro_init)
	EXIT(instro_init)
}

void __instro_fini() {
	// TODO Implement me
	ENTER(instro_fini)
	EXIT(instro_fini)
}

void __instro_start_function(size_t construct_hash, void *callsite) {
	// TODO Implement me
	ENTER(instro_start_function)
	EXIT(instro_start_function)
}

void __instro_end_function(size_t construct_hash, void *callsite) {
	// TODO Implement me
	ENTER(instro_end_function)
	EXIT(instro_end_function)
}

void __instro_start_simple_statement(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_start_simple_statement)
	EXIT(instro_start_simple_statement)
}

void __instro_end_simple_statement(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_end_simple_statement)
	EXIT(instro_end_simple_statement)
}

void __instro_start_loop(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_start_loop)
	EXIT(instro_start_loop)
}

void __instro_end_loop(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_end_loop)
	EXIT(instro_end_loop)
}

void __instro_start_scope(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_start_scope)
	EXIT(instro_start_scope)
}

void __instro_end_scope(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_end_scope)
	EXIT(instro_end_scope)
}

void __instro_start_condition(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_start_condition)
	EXIT(instro_start_condition)
}

void __instro_end_condition(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_end_condition)
	EXIT(instro_end_condition)
}

void __instro_start_expression(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_start_expression)
	EXIT(instro_start_expression)
}

void __instro_end_expression(size_t construct_hash) {
	// TODO Implement me
	ENTER(instro_end_expression)
	EXIT(instro_end_expression)
}
