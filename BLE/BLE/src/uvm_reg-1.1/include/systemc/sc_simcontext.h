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

  sc_simcontext.h -- Definition of the simulation context class.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems 20 May 2003
  Description of Modification: - phase callbacks
                               - sc_stop mode

      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: - support for dynamic process
                               - support for sc export registry
                               - new member methods elaborate(), 
				 prepare_to_simulate(), and initial_crunch()
				 that are invoked by initialize() in that order
                               - add sc_get_last_created_process_handle() for 
				 use before simulation starts
                               
      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               3 March, 2004
  Description of Modification: add sc_get_curr_process_kind()

      Name, Affiliation, Date: 
  Description of Modification: 
                               
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_simcontext.h#1 $

 *****************************************************************************/

#ifndef SC_SIMCONTEXT_H
#define SC_SIMCONTEXT_H

#include "sc_cmnhdr.h"
#include "sc_process.h"
#include "sc_time.h"
#include "sc_pq.h"
#include "sc_list.h"

// BEGIN MODELSIM ADDITIONS
//
// The extern variables below are defined in vsim 
// and will be visible to the shared library once 
// its loaded by vsim. The memory image of these 
// are assumed to match in vsim and SystemC code. 
// Care should be taken while porting to new 
// platforms.
//

#include "ptrarray.h"

#ifndef WIN32
extern mtiUInt32T sc_iteration;
#endif
// END MODELSIM ADDITIONS

namespace sc_core {

// forward declarations

class sc_cor;
class sc_cor_pkg;
class sc_event;
class sc_event_timed;
class sc_export_registry;
class sc_module;
class sc_module_name;
class sc_module_registry;
class sc_name_gen;
class sc_object;
class sc_object_manager;
class sc_process_handle;
class sc_port_base;
class sc_port_registry;
class sc_prim_channel;
class sc_prim_channel_registry;
class sc_process_table;
class sc_signal_bool_deval;
class sc_trace_file;
class sc_runnable;
class sc_process_host;

// BEGIN MODELSIM ADDITIONS
class mti_oomr_obj;
class sc_export_base;
// END MODELSIM ADDITIONS


struct sc_curr_proc_info
{
    sc_process_b* process_handle;
    sc_curr_proc_kind kind;
    sc_curr_proc_info() : process_handle( 0 ), kind( SC_NO_PROC_ ) {}
};

typedef const sc_curr_proc_info* sc_curr_proc_handle;

// friend function declarations

sc_dt::uint64 sc_delta_count();
const std::vector<sc_object*>& sc_get_top_level_objects(
const sc_simcontext* simc_p);
bool sc_is_running( const sc_simcontext* simc_p );
bool sc_end_of_simulation_invoked();
bool sc_start_of_simulation_invoked();
void    sc_set_time_resolution( double, sc_time_unit );
sc_time sc_get_time_resolution();
void    sc_set_default_time_unit( double, sc_time_unit );
sc_time sc_get_default_time_unit();
bool sc_pending_activity_at_current_time();


// simulation status codes

const int SC_SIM_OK        = 0;
const int SC_SIM_ERROR     = 1;
const int SC_SIM_USER_STOP = 2;

enum sc_stop_mode {          // sc_stop modes:
    SC_STOP_FINISH_DELTA,
    SC_STOP_IMMEDIATE
};
extern void sc_set_stop_mode( sc_stop_mode mode );
extern sc_stop_mode sc_get_stop_mode();

// BEGIN MODELSIM ADDITIONS

extern int mti_access;

// END MODELSIM ADDITIONS

// ----------------------------------------------------------------------------
//  CLASS : sc_simcontext
//
//  The simulation context.
// ----------------------------------------------------------------------------

class sc_simcontext
{
    friend class sc_event;
    friend class sc_module;
    friend class sc_object;
    friend class sc_time;
    friend class sc_clock;
    friend class sc_method_process;
    // BEGIN MODELSIM ADDITION
    friend class sc_method_debug_process;
    // END MODELSIM ADDITION
    friend class sc_process_b;
	friend class sc_prim_channel;
    friend class sc_thread_process;
    friend sc_dt::uint64 sc_delta_count();
    friend const std::vector<sc_object*>& sc_get_top_level_objects(
        const sc_simcontext* simc_p);
    friend bool sc_is_running( const sc_simcontext* simc_p );

    friend bool sc_end_of_simulation_invoked();
	friend bool sc_pending_activity_at_current_time();
    friend bool sc_start_of_simulation_invoked();

    void init();
    void clean();

public:

    sc_simcontext();
    ~sc_simcontext();

    void initialize( bool = false );
    void cycle( const sc_time& );
    void simulate( const sc_time& duration );
    void stop();
    void end();
    void reset();

    int sim_status() const;
	bool elaboration_done() const;

    sc_object_manager* get_object_manager();

    void hierarchy_push( sc_module* );
    sc_module* hierarchy_pop();
    sc_module* hierarchy_curr() const;

    // BEGIN MODELSIM ADDITION
    sc_module*  curr_module_context(int allow_outoftheblue) const;
    // END MODELSIM ADDITION

    sc_object* first_object();
    sc_object* next_object();
    sc_object* find_object( const char* name );

    sc_module_registry* get_module_registry();
    sc_port_registry* get_port_registry();
    sc_export_registry* get_export_registry();
    sc_prim_channel_registry* get_prim_channel_registry();

    // to generate unique names for objects in an MT-Safe way
    const char* gen_unique_name( const char* basename_, 
                                 bool preserve_first = false 
                               );

    // process creation
    sc_process_handle create_cthread_process( 
	const char* name_p, bool free_host, SC_ENTRY_FUNC method_p, 
	sc_process_host* host_p, const sc_spawn_options* opt_p );

    sc_process_handle create_method_process( 
	const char* name_p, bool free_host, SC_ENTRY_FUNC method_p, 
	sc_process_host* host_p, const sc_spawn_options* opt_p );

    // BEGIN MODELSIM ADDITION
    sc_process_handle create_method_debug_process( 
	const char* name_p, bool free_host, SC_ENTRY_FUNC method_p, 
	sc_process_host* host_p, const sc_spawn_options* opt_p );
    // END MODELSIM ADDITION

    sc_process_handle create_thread_process( 
	const char* name_p, bool free_host, SC_ENTRY_FUNC method_p, 
	sc_process_host* host_p, const sc_spawn_options* opt_p );

    sc_curr_proc_handle get_curr_proc_info();
    sc_object* get_current_writer() const;
    bool write_check() const;
    void set_curr_proc( sc_process_b* );
    void reset_curr_proc();

    int next_proc_id();

    void add_trace_file( sc_trace_file* );

    friend void    sc_set_time_resolution( double, sc_time_unit );
    friend sc_time sc_get_time_resolution();
    friend void    sc_set_default_time_unit( double, sc_time_unit );
    friend sc_time sc_get_default_time_unit();

    const sc_time& time_stamp() const;

	sc_dt::uint64 delta_count() const;
	bool event_occurred( sc_dt::uint64 last_change_count ) const;
    bool is_running() const;
    bool update_phase() const;
    bool get_error();
    void set_error();

    sc_cor_pkg* cor_pkg()
        { return m_cor_pkg; }
    sc_cor* next_cor();

	const ::std::vector<sc_object*>& get_child_objects() const;

    void elaborate();
    void prepare_to_simulate();
    inline void initial_crunch( bool no_crunch );
    const sc_time next_time(); 

private:

    void add_child_object( sc_object* );
    void remove_child_object( sc_object* );

    void crunch(bool once=false);

    int add_delta_event( sc_event* );
    void remove_delta_event( sc_event* );
    void add_timed_event( sc_event_timed* );

    void trace_cycle( bool delta_cycle );

    const ::std::vector<sc_object*>& get_child_objects_internal() const;

    void push_runnable_method( sc_method_handle );
    void push_runnable_thread( sc_thread_handle );

    void push_runnable_method_front( sc_method_handle );
    void push_runnable_thread_front( sc_thread_handle );

    sc_method_handle pop_runnable_method();
    sc_thread_handle pop_runnable_thread();

    void remove_runnable_method( sc_method_handle );
    void remove_runnable_thread( sc_thread_handle );

    void do_sc_stop_action();

private:

    enum execution_phases {
	    phase_initialize = 0,
	    phase_evaluate,
	    phase_update,
	    phase_notify
	};
    sc_object_manager*         m_object_manager;

    sc_module_registry*        m_module_registry;
    sc_port_registry*          m_port_registry;
    sc_export_registry*        m_export_registry;
    sc_prim_channel_registry*  m_prim_channel_registry;

    sc_name_gen*               m_name_gen;

    sc_process_table*          m_process_table;
    sc_curr_proc_info          m_curr_proc_info;
    sc_object*                 m_current_writer;
    bool                       m_write_check;
    int                        m_next_proc_id;

	std::vector<sc_object*>    m_child_objects;

	std::vector<sc_event*>     m_delta_events;
    sc_ppq<sc_event_timed*>*   m_timed_events;

    ptrarray<sc_trace_file*>   m_trace_files;
    bool                       m_something_to_trace;

    sc_runnable*               m_runnable;

    sc_time_params*            m_time_params;
    sc_time                    m_curr_time;

	sc_dt::uint64              m_delta_count;
    bool                       m_forced_stop;
    bool                       m_ready_to_simulate;
    bool                       m_elaboration_done;
    execution_phases           m_execution_phase;
    bool                       m_error;
    bool                       m_in_simulator_control;   
    bool                       m_end_of_simulation_called;
    bool                       m_start_of_simulation_called;


    sc_event*                  m_until_event;

    sc_cor_pkg*                m_cor_pkg; // the simcontext's coroutine package
    sc_cor*                    m_cor;     // the simcontext's coroutine

private:

    // disabled
    sc_simcontext( const sc_simcontext& );
    sc_simcontext& operator = ( const sc_simcontext& );


	// BEGIN MODELSIM ADDITIONS

public:

	friend class sc_foreign_module;
	friend double sc_simulation_time();
	friend void scmti_trace_cycle(int delta_cycle);
	friend void scmti_execute_sc_main(void*, int, unsigned int);

	bool mti_is_bind_name_debug() { return m_mti_bind_name_debug; }
	bool mti_is_incr_default_stack_size() { return m_mti_incr_default_stack_size;}
	void mti_set_incr_default_stack_size(bool status) {
		m_mti_incr_default_stack_size = status;
	}
	bool mti_is_valid_module(sc_module* module) const;
	bool mti_is_valid_prim_channel(sc_prim_channel* prim_channel) const;
	bool mti_is_valid_port(sc_port_base* port) const;
	bool mti_is_valid_export(sc_export_base* ex) const;
	sc_dt::uint64 mti_now() const { return *m_mti_now; }
	bool mti_is_creating_dynamic_proc() const;
	void mti_set_exception_status(bool exception_status);
	bool mti_is_exception_active();
	bool mti_is_something_to_trace();
	bool mti_is_stop_custom_debug_register() { return m_mti_stop_registering_custom_debug; }
	void mti_call_start_of_simulation();

	bool mti_control_foreign_signal(
        sc_prim_channel* sc_signal_obj,
        const char* hdl_name,
        int is_scvconnect_call = 0);

    bool mti_observe_foreign_signal(
        sc_prim_channel* sc_signal_obj,
        const char* hdl_name,
        int is_scvconnect_call = 0);

	void mti_set_write_check(bool write_check) 
		{ m_write_check = write_check; }

private:
	mti_oomr_obj* mti_get_oomr_obj(sc_prim_channel* sig);
    void mti_process_signal_hierarchical_references();
	void mti_create_thread_package();

private:

	bool                       m_mti_bind_name_debug;
	bool                       m_mti_incr_default_stack_size;

	sc_dt::uint64*             m_mti_now; // Handle to current simulation time 
										  // stored in vsim.

	bool                       m_mti_is_creating_dynamic_proc;
	bool                       m_mti_is_exception_thrown;
	bool                       m_mti_stop_registering_custom_debug;

	sc_plist<mti_oomr_obj*>* m_mti_control_observe_list;


	// END MODELSIM ADDITIONS
};

// Not MT safe.

extern sc_simcontext* sc_curr_simcontext;
#ifdef PURIFY
extern sc_simcontext* sc_get_curr_simcontext();
#else
extern sc_simcontext* sc_default_global_context;

inline sc_simcontext*
sc_get_curr_simcontext()
{
    if( sc_curr_simcontext == 0 ) {
        sc_default_global_context = new sc_simcontext;
        sc_curr_simcontext = sc_default_global_context;
    }
    return sc_curr_simcontext;
}
#endif // PURIFY


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
bool
sc_simcontext::elaboration_done() const
{
    return m_elaboration_done;
}


inline
int
sc_simcontext::sim_status() const
{
    if( m_error ) {
        return SC_SIM_ERROR;
    }
    if( m_forced_stop ) {
        return SC_SIM_USER_STOP;
    }
    return SC_SIM_OK;
}


inline
sc_object_manager*
sc_simcontext::get_object_manager()
{
    return m_object_manager;
}

inline
sc_module_registry*
sc_simcontext::get_module_registry()
{
    return m_module_registry;
}

inline
sc_port_registry*
sc_simcontext::get_port_registry()
{
    return m_port_registry;
}

inline
sc_export_registry*
sc_simcontext::get_export_registry()
{
    return m_export_registry;
}

inline
sc_prim_channel_registry*
sc_simcontext::get_prim_channel_registry()
{
    return m_prim_channel_registry;
}


inline
sc_curr_proc_handle
sc_simcontext::get_curr_proc_info()
{
    return &m_curr_proc_info;
}


inline
int
sc_simcontext::next_proc_id()
{
    return ( ++ m_next_proc_id );
}


inline
const sc_time&
sc_simcontext::time_stamp() const
{
	// START MODELSIM MODIFICATIONS
	static sc_time scCurrentTime;
	scCurrentTime = sc_time(*m_mti_now, false);
	return scCurrentTime;
	// END MODELSIM MODIFICATIONS
}


inline 
bool
sc_simcontext::event_occurred(sc_dt::uint64 last_change_count) const
{
// BEGIN MODELSIM MODIFICATIONS
#ifdef WIN32
	return (sc_dt::uint64)mti_ScDelta() == last_change_count;
#else
	return (sc_dt::uint64)::sc_iteration == last_change_count;
#endif
// END MODELSIM MODIFICATIONS
}

inline
bool
sc_simcontext::update_phase() const
{
    return m_execution_phase == phase_update;
}

inline
void
sc_simcontext::set_error()
{
    m_error = true;
}


inline
bool
sc_simcontext::get_error()
{
    return m_error;
}

inline
int
sc_simcontext::add_delta_event( sc_event* e )
{
    m_delta_events.push_back( e );
    return ( m_delta_events.size() - 1 );
}

inline
void
sc_simcontext::add_timed_event( sc_event_timed* et )
{
    m_timed_events->insert( et );
}

inline sc_object* 
sc_simcontext::get_current_writer() const
{
    return m_current_writer;
}

inline bool 
sc_simcontext::write_check() const
{
    return m_write_check;
}


// BEGIN MODELSIM ADDITIONS

inline
bool
sc_simcontext::mti_is_something_to_trace()
{
	return m_something_to_trace;
}

// END MODELSIM ADDITIONS

// ----------------------------------------------------------------------------

class sc_process_handle;
sc_process_handle sc_get_current_process_handle();

inline
sc_process_b*
sc_get_current_process_b()
{
    return sc_get_curr_simcontext()->get_curr_proc_info()->process_handle;
}

// THE FOLLOWING FUNCTION IS DEPRECATED IN 2.1
extern sc_process_b* sc_get_curr_process_handle();

inline
sc_curr_proc_kind
sc_get_curr_process_kind()
{
    return sc_get_curr_simcontext()->get_curr_proc_info()->kind;
}


inline int sc_get_simulator_status()
{
    return sc_get_curr_simcontext()->sim_status();
}


// Generates unique names within each module.
extern
const char*
sc_gen_unique_name( const char* basename_, bool preserve_first = false );


// Set the random seed for controlled randomization -- not yet implemented
extern
void
sc_set_random_seed( unsigned int seed_ );


extern void sc_start();
extern void sc_start( const sc_time& duration );
extern void sc_start( double duration );
extern void sc_stop();

extern void sc_initialize();
extern void sc_cycle( const sc_time& duration );

extern const sc_time& sc_time_stamp();  // Current simulation time.
extern double sc_simulation_time();     // Current time in default time units.

inline
const std::vector<sc_object*>& sc_get_top_level_objects(
    const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
    return simc_p->m_child_objects;
}

extern sc_object* sc_find_object(
    const char* name, sc_simcontext* simc_p = sc_get_curr_simcontext() );

inline
sc_dt::uint64 sc_delta_count()
{
// BEGIN MODELSIM MODIFICATIONS
#ifdef WIN32
	return (sc_dt::uint64)mti_ScDelta();
#else
	return (sc_dt::uint64)::sc_iteration;
#endif
// END MODELSIM MODIFICATIONS
}

inline 
bool sc_is_running( const sc_simcontext* simc_p = sc_get_curr_simcontext() )
{
    return simc_p->m_ready_to_simulate;
}

inline
void
sc_start( double duration, sc_time_unit time_unit )
{
    sc_start( sc_time( duration, time_unit ) );
}

inline
void
sc_cycle( double duration, sc_time_unit time_unit )
{
    sc_cycle( sc_time( duration, time_unit ) );
}

// for backward compatibility with 1.0
inline
void
sc_cycle( double duration )  // in default time units
{
    sc_cycle( sc_time( duration, true ) );
}

inline
bool
sc_end_of_simulation_invoked()
{
        return sc_get_curr_simcontext()->m_end_of_simulation_called;
}


inline
bool 
sc_start_of_simulation_invoked()
{
        return sc_get_curr_simcontext()->m_start_of_simulation_called;
}

} // namespace sc_core
#endif
