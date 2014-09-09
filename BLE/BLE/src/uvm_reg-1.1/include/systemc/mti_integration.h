/* $Id: //dvt/mti/rel/10.0b/src/systemc/mti_integration.h#1 $ */

#ifndef MTI_INTEGRATION
#define MTI_INTEGRATION

#ifndef MTI_SYSTEMC
#define MTI_SYSTEMC
#endif

namespace sc_core {
	extern void message_function(const char* msg);
} // namespace sc_core

#define SC_MODULE_EXPORT(w) \
	extern "C" void* mti__##w(const char* sc_module_name, int is_boundary_top) \
	{ \
		sc_core::mti_is_elab_boundary_top = is_boundary_top; \
		try \
		{ \
			return new w(sc_module_name); \
		} \
		catch( const std::exception& rep ) \
		{ \
			mti_PrintScMessage((char*)rep.what()); \
			mti_ScError(); \
		} \
		catch( const char* s ) \
		{ \
			sc_core::message_function(s); \
			mti_ScError(); \
		} \
		catch( ... ) \
		{ \
			sc_core::message_function("UNKNOWN SYSTEMC EXCEPTION OCCURED"); \
			mti_ScError(); \
		} \
		return NULL; \
	}


#ifdef MTI_BIND_SC_MEMBER_FUNCTION
#include "member_fptr_bind.h"
#endif


/* These are old interfaces for 6.2beta. The support may be removed in the future. */

#ifndef REMOVE_OLD_INTERFACE_SUPPORT
#define sc_export_function(name, fptr) \
    mti_RegisterScImportFunc(name, NULL, (void*) (fptr), 0);

#define SC_EXPORT_FUNCTION(name) \
    mti_RegisterScImportFunc(#name, NULL, (void*) (name), 0);

#ifdef MTI_BIND_SC_MEMBER_FUNCTION

#define sc_export_member_function(name, objInst, fptr) \
    mti_RegisterScImportFunc(name, objInst, \
                             mti_member_fptr_bind(objInst, fptr) , 0);
#define SC_EXPORT_MEMBER(name, fptr) \
    mti_RegisterScImportFunc(name, this, \
                             mti_member_fptr_bind(this, fptr), 0);
#endif
#endif


/********************************************************************************
 * Interfaces related to registering a cpp function as SystemVerilog DPI imports
 ********************************************************************************/

#define sc_dpi_register_cpp_function(name, fptr) \
    mti_RegisterScImportFunc(name, NULL, (void*) (fptr), 0);

#define sc_dpi_register_cpp_function_ex(name, fptr, stack_size) \
    mti_RegisterScImportFunc(name, NULL, (void*) (fptr), stack_size);

#define SC_DPI_REGISTER_CPP_FUNCTION(name) \
    mti_RegisterScImportFunc(#name, NULL, (void*) (name), 0);

#ifdef MTI_BIND_SC_MEMBER_FUNCTION
#define sc_dpi_register_cpp_member_function(name, objInst, fptr) \
    mti_RegisterScImportFunc(name, objInst, \
                             mti_member_fptr_bind(objInst, fptr) , 0);

#define sc_dpi_register_cpp_member_function_ex(name, objInst, fptr, stack_size) \
    mti_RegisterScImportFunc(name, objInst, \
                             mti_member_fptr_bind(objInst, fptr) , stack_size);

#define SC_DPI_REGISTER_CPP_MEMBER_FUNCTION(name, fptr) \
    mti_RegisterScImportFunc(name, this, \
                             mti_member_fptr_bind(this, fptr), 0);
#endif

#define sc_dpi_set_stack_size(size) \
    mti_RegisterScImportFunc(NULL, NULL, NULL, size);



/*******************************************************************************
 * Interfaces related to registering a cpp callback function for custom debug
 ******************************************************************************/


typedef void (*sc_mti_custom_debug_cb)(void*, char*, char);

/*
 * Registration function for custom debug. This function takes the following
 * arguments:
 * objInst: Handle to the object being debuged using custom debug. This can be
 *          a handle to a channel or a module member variable.
 * length:  Maximum string length to be reserved for this object.
 * cb_func: Callback function to be called to get the custom debug value.
 */

#define SC_MTI_REGISTER_CUSTOM_DEBUG(objInst, length, cb_func) \
	sc_mti_register_custom_debug(objInst, length, cb_func, NULL, __FILE__, __LINE__);

#define SC_MTI_REGISTER_NAMED_CUSTOM_DEBUG(objInst, length, cb_func, name) \
	sc_mti_register_custom_debug(objInst, length, cb_func, name, __FILE__, __LINE__);

extern void sc_mti_register_custom_debug(
		void* objInst,
		size_t length,
		sc_mti_custom_debug_cb cb_func,
		const char* optional_name = NULL,
		const char* filename = NULL,
		int lineno = 0);

/* SignalSpy Functions */

// Mirror the value of the SystemC signal onto an existing SystemC signal,
// VHDL signal or Verilog register.
extern void init_signal_spy(   
						const char* src_name,
                        const char* dest_name,
                        int verbose = 0,
                        int control_state = -1);

// Disable the associated init_signal_spy.
extern void disable_signal_spy(
						const char* src_name,
                        const char* dest_name,
                        int verbose = 0);

// Enable the associated init_signal_spy.
extern void enable_signal_spy( 
						const char* src_name,
                        const char* dest_name,
                        int verbose = 0);

// Force the value specified onto an existing SystemC signal.
extern void signal_force(  
					const char* dest_name,
                    const char* value,
                    const long long rel_time = 0,
                    int force_type = 0,
                    const long long cancel_period = -1,
                    int verbose = 0);

// Release any force that was applied to an existing SystemC signal.
extern void signal_release(const char* dest_name, int verbose = 0);

// Drive the value of a SystemC signal onto an existing SystemC signal,
// VHDL signal or Verilog net.
extern void init_signal_driver(
						const char* src_name,
                        const char* dest_name,
                        const long long delay = 0,
                        int delay_type = 0,
                        int verbose = 0);


//
// Activate and execute all SC processes that are currently activated using
// immediate notification and keep spinning on SC threads until everything is
// quiescent. The calling SV thread is suspend to execute the pending
// SC threads and woken up again once all the SC threads finish execution. 
// Note that only SC threads activated by immediate notification are executed
// in this cycle. Channels scheduled for update from the executed processes are 
// updated but any other process triggered due to normal notification are not executed. 
// This function can only be called from a DPI import task.
//
extern int mti_spin_sc();

#endif
