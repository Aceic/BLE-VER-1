/* $Id: //dvt/mti/rel/10.0b/src/systemc/scmtiapi.h#1 $ */

#ifndef SCMTIAPI_H
#define SCMTIAPI_H

namespace sc_core {
/********************************************************
    Callback functions called by vsim kernel
********************************************************/

void scmti_load_mti_func_tray();

void scmti_initialize();

void scmti_set_property_for_ini_var(
	void* value,
    int ini_var);

void scmti_execute_method_process(void *sc_method);
void scmti_execute_method_debug_process(void *sc_method_debug);
void scmti_execute_thread_process(void *sc_thread);
void scmti_execute_switch_thread(void *sc_thread, void* current_thread);

int scmti_get_thread_state(void *thread);
void scmti_trigger_event(void *sc_event);
void scmti_trigger_clock_event(void *sc_clock_event);

void scmti_update_primitive_channel(void *sc_prim_channel);

void scmti_clear_primitive_channel_update_requested_flag(void *sc_prim_channel);

void scmti_get_primitive_channel_value(
	void *sc_prim_channel_obj,
	void **value,
	int is_driving_value);

void scmti_set_primitive_channel_value(
	void *sc_prim_channel_obj,
	void *value,
	int is_driving_value);

void scmti_fanout_to_primitive_channel(
	void *sc_prim_channel_obj,
	void *value);

void scmti_get_primitive_channel_type(
	void *sc_prim_channel_obj,
	void **type);

int scmti_bind_name(
	void* sc_object_handle,
	char* name);

int scmti_is_valid_module(
	void* sc_module_handle);

int scmti_is_valid_prim_channel(
	void* prim_channel_handle);

int scmti_is_valid_port(
	void* sc_port_handle);

int scmti_is_valid_export(
	void* sc_export_handle);

void scmti_delete_module(void* sc_module_handle);

void scmti_call_start_of_simulation();

void scmti_call_end_of_simulation();

void scmti_trace_cycle(int delta_cycle);

void scmti_get_variable_value_size(
	void* var_handle,
	int* has_construction_param,
	int* elem_num_bits,
	int* elem_num_integer_bits);

void scmti_get_variable_value(
	void* var_handle,
	void** value);

void scmti_set_variable_value(
        void* var_handle,
        void* value);

void scmti_delete_module_dynalloc_list();

char* scmti_get_object_typename(void* obj_handle);

char* scmti_get_object_name(void* obj_handle);

void* scmti_get_object_region_id(void* obj_handle);

int scmti_is_typename_checked(void* obj_handle);

char* scmti_block_systemc_thread(void*);

void scmti_wakeup_systemc_thread(void* procObj, void* threadObj, int is_immediate_notify, int isScThread);

int  scmti_disable_systemc_thread(void* threadObj);

void  scmti_set_incr_default_stack_size();

void scmti_execute_sc_main(void* sc_main_func_ptr, int is_start_elab, unsigned int stack_size);
} // namespace sc_core

#endif
