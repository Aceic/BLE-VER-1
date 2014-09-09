/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_module.h -- Base class of all hierarchical modules and channels.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Ali Dasdan, Synopsys, Inc.
  Description of Modification: - Implementation of operator() and operator,
                                 positional connection method.
                               - Implementation of error checking in
                                 operator<<'s.
                               - Implementation of the function test_module_prm.
                               - Implementation of set_stack_size().

      Name, Affiliation, Date: Gene Bushuyev, Synopsys, Inc.
  Description of Modification: - Change implementation for VC6.

      Name, Affiliation, Date: Andy Godorich, Forte
                               Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: inherit from sc_process_host as a part of
                               implementing dynamic processes

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_module.h#1 $

 *****************************************************************************/

#ifndef SC_MODULE_H
#define SC_MODULE_H

#include "sc_kernel_ids.h"
#include "sc_process.h"
#include "sc_module_name.h"
#include "sc_sensitive.h"
#include "sc_time.h"
#include "sc_wait.h"
#include "sc_wait_cthread.h"
#include "sc_process_handle.h"
#include "sc_list.h"
#include "sc_string.h"

// BEGIN MODELSIM ADDITIONS
#include "scmti_msg.h"
// END MODELSIM ADDITIONS

namespace sc_core {

// BEGIN MODELSIM ADDITIONS
extern int mti_is_elab_boundary_top;

const int MTI_IS_BOUNDARY_TOP_MODULE = 0x01;
const int MTI_IS_FOREIGN_MODULE = 0x02;
const int MTI_IS_FOREIGN_MODULE_ELABORATED = 0x04;

// END MODELSIM ADDITIONS

class sc_name_gen;
template<class T> class sc_in;
template<class T> class sc_signal;

// ----------------------------------------------------------------------------
//  STRUCT : sc_bind_proxy
//
//  Struct for temporarily storing a pointer to an interface or port.
//  Used for positional binding.
// ----------------------------------------------------------------------------

struct sc_bind_proxy
{
    sc_interface* iface;
    sc_port_base* port;
    
    sc_bind_proxy();
    sc_bind_proxy( sc_interface& );
    sc_bind_proxy( sc_port_base& );
};


extern const sc_bind_proxy SC_BIND_PROXY_NIL;


// ----------------------------------------------------------------------------
//  CLASS : sc_module
//
//  Base class for all structural entities.
// ----------------------------------------------------------------------------

class sc_module
: public sc_object, public sc_process_host
{
    friend class sc_module_name;
    friend class sc_module_registry;
    friend class sc_object;
    friend class sc_port_registry;
	friend class sc_process_b;
    friend class sc_simcontext;

public:

    sc_simcontext* sc_get_curr_simcontext()
	{ return simcontext(); }

    // to generate unique names for objects in an MT-Safe way
    const char* gen_unique_name( const char* basename_, bool preserve_first );

    virtual const char* kind() const
        { return "sc_module"; }

protected:
  
    // called by construction_done 
    virtual void before_end_of_elaboration();

    void construction_done();

    // called by elaboration_done (does nothing by default)
    virtual void end_of_elaboration();

    void elaboration_done( bool& );

    // called by start_simulation (does nothing by default)
    virtual void start_of_simulation();

    void start_simulation();

    // called by simulation_done (does nothing by default)
    virtual void end_of_simulation();

    void simulation_done();

    void sc_module_init();

    // constructor
    sc_module( const char* nm );
    sc_module( const std::string& nm );
    sc_module( const sc_module_name& nm ); /* for those used to old style */
    sc_module();

public:

    // destructor
    virtual ~sc_module();

    // positional binding methods

    sc_module& operator << ( sc_interface& );
    sc_module& operator << ( sc_port_base& );

    sc_module& operator , ( sc_interface& interface_ )
        { return operator << ( interface_ ); }

    sc_module& operator , ( sc_port_base& port_ )
        { return operator << ( port_ ); }

    // operator() is declared at the end of the class.

	const ::std::vector<sc_object*>& get_child_objects() const;

protected:

    void add_child_object( sc_object* );
    void remove_child_object( sc_object* );

    // this must be called by user-defined modules
    void end_module();


    // to prevent initialization for SC_METHODs and SC_THREADs
    void dont_initialize();

    // positional binding code - used by operator ()

    void positional_bind( sc_interface& );
    void positional_bind( sc_port_base& );

    // set reset sensitivity for SC_CTHREADs
    void reset_signal_is( const sc_in<bool>& port, bool level );
    void reset_signal_is( const sc_signal_in_if<bool>& iface, bool level );

    // static sensitivity for SC_THREADs and SC_CTHREADs

    void wait()
        { ::sc_core::wait( simcontext() ); }


    // dynamic sensitivity for SC_THREADs and SC_CTHREADs

    void wait( const sc_event& e )
        { ::sc_core::wait( e, simcontext() ); }

    void wait( sc_event_or_list& el )
	{ ::sc_core::wait( el, simcontext() ); }

    void wait( sc_event_and_list& el )
	{ ::sc_core::wait( el, simcontext() ); }

    void wait( const sc_time& t )
        { ::sc_core::wait( t, simcontext() ); }

    void wait( double v, sc_time_unit tu )
        { ::sc_core::wait( sc_time( v, tu, simcontext() ), simcontext() ); }

    void wait( const sc_time& t, const sc_event& e )
        { ::sc_core::wait( t, e, simcontext() ); }

    void wait( double v, sc_time_unit tu, const sc_event& e )
        { ::sc_core::wait( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void wait( const sc_time& t, sc_event_or_list& el )
        { ::sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_or_list& el )
        { ::sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void wait( const sc_time& t, sc_event_and_list& el )
        { ::sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_and_list& el )
        { ::sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // static sensitivity for SC_METHODs

    void next_trigger()
	{ ::sc_core::next_trigger( simcontext() ); }


    // dynamic sensitivty for SC_METHODs

    void next_trigger( const sc_event& e )
        { ::sc_core::next_trigger( e, simcontext() ); }

    void next_trigger( sc_event_or_list& el )
        { ::sc_core::next_trigger( el, simcontext() ); }

    void next_trigger( sc_event_and_list& el )
        { ::sc_core::next_trigger( el, simcontext() ); }

    void next_trigger( const sc_time& t )
        { ::sc_core::next_trigger( t, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu )
        { ::sc_core::next_trigger( sc_time( v, tu, simcontext() ), simcontext() ); }

    void next_trigger( const sc_time& t, const sc_event& e )
        { ::sc_core::next_trigger( t, e, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, const sc_event& e )
        { ::sc_core::next_trigger( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void next_trigger( const sc_time& t, sc_event_or_list& el )
        { ::sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_or_list& el )
        { ::sc_core::next_trigger( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void next_trigger( const sc_time& t, sc_event_and_list& el )
        { ::sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_and_list& el )
        { ::sc_core::next_trigger( sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // for SC_METHODs and SC_THREADs and SC_CTHREADs

    bool timed_out()
	{ return ::sc_core::timed_out(); }


    // for SC_CTHREADs

    void halt()
        { ::sc_core::halt( simcontext() ); }

    void wait( int n )
        { ::sc_core::wait( n, simcontext() ); }

    void at_posedge( const sc_signal_in_if<bool>& s )
	{ ::sc_core::at_posedge( s, simcontext() ); }

    void at_posedge( const sc_signal_in_if<sc_dt::sc_logic>& s )
	{ ::sc_core::at_posedge( s, simcontext() ); }

    void at_negedge( const sc_signal_in_if<bool>& s )
	{ ::sc_core::at_negedge( s, simcontext() ); }

    void at_negedge( const sc_signal_in_if<sc_dt::sc_logic>& s )
	{ ::sc_core::at_negedge( s, simcontext() ); }

    // Catch uses of watching:
    void watching( bool expr )
        { SC_REPORT_ERROR(SC_ID_WATCHING_NOT_ALLOWED_,""); }

    // These are protected so that user derived classes can refer to them.
    sc_sensitive     sensitive;
    sc_sensitive_pos sensitive_pos;
    sc_sensitive_neg sensitive_neg;

    // Function to set the stack size of the current (c)thread process.
    void set_stack_size( std::size_t );

    int append_port( sc_port_base* );

private:
    sc_module( const sc_module& );

private:

    bool                       m_end_module_called;
    std::vector<sc_port_base*>* m_port_vec;
    int                        m_port_index;
    sc_name_gen*               m_name_gen;
    std::vector<sc_object*>    m_child_objects;
	sc_module_name*            m_module_name_p;

public:

    void defunct() { }

    // positional binding methods (cont'd)

    void operator () ( const sc_bind_proxy& p001,
		       const sc_bind_proxy& p002 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p003 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p004 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p005 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p006 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p007 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p008 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p009 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p010 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p011 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p012 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p013 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p014 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p015 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p016 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p017 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p018 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p019 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p020 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p021 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p022 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p023 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p024 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p025 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p026 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p027 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p028 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p029 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p030 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p031 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p032 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p033 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p034 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p035 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p036 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p037 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p038 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p039 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p040 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p041 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p042 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p043 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p044 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p045 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p046 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p047 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p048 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p049 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p050 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p051 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p052 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p053 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p054 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p055 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p056 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p057 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p058 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p059 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p060 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p061 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p062 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p063 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p064 = SC_BIND_PROXY_NIL );


	// BEGIN MODELSIM ADDITIONS

private:
    mtiObjectHandleT           m_mti_region_id;
    mtiObjectHandleT           m_mti_parent_region_id;
	int                        m_mti_module_flags;
    void*                      m_mti_parameter_list;

    void* mti_find_parameter_object(
        const char* func_name,
        const char* param_name);

protected:

    void mti_set_region_id( mtiObjectHandleT region_id )
        { m_mti_region_id = region_id; }

	void mti_set_region_id_on_ports();

public:
    int mti_is_region() { return 1; }
     
    mtiObjectHandleT mti_get_region_id()
        { return m_mti_region_id; }

    mtiObjectHandleT mti_get_parent_region_id()
        { return m_mti_parent_region_id; }

    bool mti_is_boundary_top_module()
        { return (m_mti_module_flags & MTI_IS_BOUNDARY_TOP_MODULE); }

    bool mti_is_foreign_module()
        { return (m_mti_module_flags & MTI_IS_FOREIGN_MODULE); }

	bool mti_is_foreign_module_elaborated()
		{ return (m_mti_module_flags & MTI_IS_FOREIGN_MODULE_ELABORATED); }

	void mti_set_foreign_module_elaborated()
		{ m_mti_module_flags |= MTI_IS_FOREIGN_MODULE_ELABORATED; }

    int sc_get_int_param(const char* param_name, int* status = NULL);
    int sc_get_param(const char* param_name, int& param_value);

    double sc_get_real_param(const char* param_name, int* status = NULL);
    int sc_get_param(const char* param_name, double& param_value);

    //
    // Returns the string value of a parameter in binary, octal, decimal,
    // hexadecimal, or ASCII format. format_char is only useful for getting parameter
    // value from Verilog parent. It can be 'b' or 'B' for binary,
    // 'o' or 'O' for octal, 'd' or 'D' for decimal, 'h' or 'H' for hexadecimal,
    // 'a' or 'A' for ASCII format.
    //
	std::string sc_get_string_param(
        const char* param_name,
        char format_char = 'a', 
		int* status = NULL);

    int sc_get_param(
        const char* param_name,
		std::string& param_value,
        char format_char = 'a');
	// END MODELSIM ADDITIONS
};

extern sc_module* sc_module_dynalloc(sc_module*);
#define SC_NEW(x)  ::sc_core::sc_module_dynalloc(new x);


// -----------------------------------------------------------------------------
// SOME MACROS TO SIMPLIFY SYNTAX:
// -----------------------------------------------------------------------------

#define SC_MODULE(user_module_name)                                           \
    struct user_module_name : ::sc_core::sc_module

#define SC_CTOR(user_module_name)                                             \
    typedef user_module_name SC_CURRENT_USER_MODULE;                          \
    user_module_name( ::sc_core::sc_module_name )

// the SC_HAS_PROCESS macro call must be followed by a ;
#define SC_HAS_PROCESS(user_module_name)                                      \
    typedef user_module_name SC_CURRENT_USER_MODULE


#define declare_method_process(handle, name, host_tag, func)        \
    {		                                                    \
        ::sc_core::sc_process_handle handle =                      \
	    sc_core::sc_get_curr_simcontext()->create_method_process( \
		name,  false, SC_MAKE_FUNC_PTR( host_tag, func ), \
		this, 0 ); \
        this->sensitive << handle;                                        \
        this->sensitive_pos << handle;                                    \
        this->sensitive_neg << handle;                                    \
    }

// BEGIN MODELSIM ADDITIONS

#define declare_method_debug_process(handle, name, host_tag, func)  \
    {		                                                    \
        ::sc_core::sc_process_handle handle =                       \
            sc_core::sc_get_curr_simcontext()->create_method_debug_process( \
                name,  false, SC_MAKE_FUNC_PTR( host_tag, func ), \
                this, 0 ); \
        this->sensitive << handle;                                        \
        this->sensitive_pos << handle;                                    \
        this->sensitive_neg << handle;                                    \
    }

// END MODELSIM ADDITIONS

#define declare_thread_process(handle, name, host_tag, func)        \
    {                                                               \
        ::sc_core::sc_process_handle handle =                      \
	     sc_core::sc_get_curr_simcontext()->create_thread_process( \
                 name,  false,           \
                 SC_MAKE_FUNC_PTR( host_tag, func ), this, 0 ); \
        this->sensitive << handle;                                        \
        this->sensitive_pos << handle;                                    \
        this->sensitive_neg << handle;                                    \
    }

#define declare_cthread_process(handle, name, host_tag, func, edge) \
    {                                                               \
        ::sc_core::sc_process_handle handle =                     \
	     sc_core::sc_get_curr_simcontext()->create_cthread_process( \
            name,  false,          \
                     SC_MAKE_FUNC_PTR( host_tag, func ), this, 0 ); \
        this->sensitive.operator() ( handle, edge );\
    }

#define SC_CTHREAD(func, edge)                                                \
    declare_cthread_process( func ## _handle,                                 \
                             #func,                                           \
                             SC_CURRENT_USER_MODULE,                          \
                             func,                                            \
                             edge )

// BEGIN MODELSIM MODIFICATIONS

#define SC_METHOD(func)                                                       \
    if (SC_GETKF(SC_KF_SCDPI_DEBUG)) {                                        \
        declare_method_debug_process( func ## _handle,                        \
                                      #func,                                  \
                                      SC_CURRENT_USER_MODULE,                 \
                                      func )                                  \
    } else {                                                                  \
        declare_method_process( func ## _handle,                              \
                                #func,                                        \
                                SC_CURRENT_USER_MODULE,                       \
                                func )                                        \
    }

#define SC_METHOD_ORIG(func)                                                  \
    declare_method_process( func ## _handle,                                  \
                            #func,                                            \
                            SC_CURRENT_USER_MODULE,                           \
                            func )

// END MODELSIM MODIFICATIONS

#define SC_THREAD(func)                                                       \
    declare_thread_process( func ## _handle,                                  \
                            #func,                                            \
                            SC_CURRENT_USER_MODULE,                           \
                            func )



// ----------------------------------------------------------------------------
//  TYPEDEFS
// ----------------------------------------------------------------------------

typedef sc_module sc_channel;
typedef sc_module sc_behavior;

// BEGIN MODELSIM ADDITIONS

// Kept this for backward compatibility with ModelSim 5.8
#define SC_MTI_BIND_NAME

// END MODELSIM ADDITIONS

} // namespace sc_core

#endif
