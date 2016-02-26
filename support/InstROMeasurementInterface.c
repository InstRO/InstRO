#include "InstROMeasurementInterface.h"

void __instro_init() {}

void __instro_fini() {}

void __instro_start_function(size_t construct_hash, void *callsite) {
	__cyg_profile_func_enter((void *) construct_hash, callsite);
}

void __instro_end_function(size_t construct_hash, void *callsite) {
	__cyg_profile_func_exit((void *) construct_hash, callsite);
}

void __instro_start_simple_statement(size_t construct_hash) {
	__cyg_profile_func_enter((void *) construct_hash, 0);
}

void __instro_end_simple_statement(size_t construct_hash) {
	__cyg_profile_func_exit((void *) construct_hash, 0);
}

void __instro_start_loop(size_t construct_hash) {
	__cyg_profile_func_enter((void *) construct_hash, 0);
}

void __instro_end_loop(size_t construct_hash) {
	__cyg_profile_func_exit((void *) construct_hash, 0);
}

void __instro_start_scope(size_t construct_hash) {
	__cyg_profile_func_enter((void *) construct_hash, 0);
}

void __instro_end_scope(size_t construct_hash) {
	__cyg_profile_func_exit((void *) construct_hash, 0);
}

void __instro_start_conditional(size_t construct_hash) {
	__cyg_profile_func_enter((void *) construct_hash, 0);
}

void __instro_end_conditional(size_t construct_hash) {
	__cyg_profile_func_exit((void *) construct_hash, 0);
}

void __instro_start_expression(size_t construct_hash) {
	__cyg_profile_func_enter((void *) construct_hash, 0);
}

void __instro_end_expression(size_t construct_hash) {
	__cyg_profile_func_exit((void *) construct_hash, 0);
}
