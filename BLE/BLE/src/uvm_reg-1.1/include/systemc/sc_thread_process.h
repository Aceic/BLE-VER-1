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

  sc_thread_process.h -- Thread process declarations

  Original Author: Andy Goodrich, Forte Design Systems, 4 August 2005


 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_thread_process.h#1 $

 *****************************************************************************/

#if !defined(sc_thread_process_h_INCLUDED)
#define sc_thread_process_h_INCLUDED

#include "sc_spawn_options.h"
#include "sc_process.h"
#include "sc_cor.h"
#include "sc_event.h"
#include "sc_except.h"
#include "sc_reset.h"


namespace sc_core {

class sc_event_and_list;
class sc_event_or_list;
class sc_reset;

// friend function declarations
    void sc_thread_cor_fn( void* );
    void sc_cthread_cor_fn( void* );
    void sc_set_stack_size( sc_thread_handle, std::size_t );
    class sc_event;
    class sc_join;
    class sc_module;
    class sc_process_handle;
    class sc_process_table;
    class sc_simcontext;
    class sc_runnable;
    sc_cor* get_cor_pointer( sc_process_b* process_p );

    void wait( sc_simcontext* );
    void wait( const sc_event&,
              sc_simcontext* );
    void wait( sc_event_or_list&,
              sc_simcontext* );
    void wait( sc_event_and_list&,
              sc_simcontext* );
    void wait( const sc_time&,
              sc_simcontext* );
    void wait( const sc_time&, const sc_event&,
              sc_simcontext* );
    void wait( const sc_time&, sc_event_or_list&,
              sc_simcontext* );
    void wait( const sc_time&, sc_event_and_list&,
              sc_simcontext* );

//==============================================================================

void sc_set_stack_size( sc_thread_handle thread_h, std::size_t size );

//==============================================================================
// sc_thread_process -
//
//==============================================================================
class sc_thread_process : public sc_process_b {
    friend void sc_thread_cor_fn( void* );
    friend void sc_cthread_cor_fn( void* );
    friend void sc_set_stack_size( sc_thread_handle, std::size_t );
    friend class sc_event;
    friend class sc_join;
    friend class sc_module;
    friend class sc_process_handle;
    friend class sc_process_table;
    friend class sc_simcontext;
    friend class sc_runnable;
    friend sc_cor* get_cor_pointer( sc_process_b* process_p );

    friend void wait( sc_simcontext* );
    friend void wait( const sc_event&,
              sc_simcontext* );
    friend void wait( sc_event_or_list&,
              sc_simcontext* );
    friend void wait( sc_event_and_list&,
              sc_simcontext* );
    friend void wait( const sc_time&,
              sc_simcontext* );
    friend void wait( const sc_time&, const sc_event&,
              sc_simcontext* );
    friend void wait( const sc_time&, sc_event_or_list&,
              sc_simcontext* );
    friend void wait( const sc_time&, sc_event_and_list&,
              sc_simcontext* );

  public:
    sc_thread_process( const char* name_p, bool free_host,
        SC_ENTRY_FUNC method_p, sc_process_host* host_p,
        const sc_spawn_options* opt_p );

    virtual ~sc_thread_process();

    virtual const char* kind() const
        { return "sc_thread_process"; }

// BEGIN MODELSIM ADDITIONS

    void disable_thread();

// END MODELSIM ADDITIONS

  protected:
    virtual void kill_process();
    sc_thread_handle next_exist();
    sc_thread_handle next_runnable();
    virtual void prepare_for_simulation();
    inline bool ready_to_run();
    void set_next_exist( sc_thread_handle next_p );
    void set_next_runnable( sc_thread_handle next_p );

    void set_stack_size( std::size_t size );
    inline void suspend_me();

    bool trigger_dynamic( sc_event* );

    void wait( const sc_event& );
    void wait( sc_event_or_list& );
    void wait( sc_event_and_list& );
    void wait( const sc_time& );
    void wait( const sc_time&, const sc_event& );
    void wait( const sc_time&, sc_event_or_list& );
    void wait( const sc_time&, sc_event_and_list& );
    void wait_cycles( int n=1 );

  protected:
    void add_monitor( sc_process_monitor* monitor_p );
    void remove_monitor( sc_process_monitor* monitor_p);
    void signal_monitors( int type = 0 );

  protected:
    sc_cor*                          m_cor_p;        // Thread's coroutine.
    static sc_cor*                   m_dead_cor_p;   // Coroutine to delete.
    std::vector<sc_process_monitor*> m_monitor_q;    // Thread monitors.
    std::size_t                      m_stack_size;   // Thread stack size.
    int                              m_wait_cycle_n; // # of waits to be done.

  private: // disabled
    sc_thread_process( const sc_thread_process& );
    const sc_thread_process& operator = ( const sc_thread_process& );

    // BEGIN MODELSIM ADDITIONS
protected:

	int m_mti_proc_flag;  // Process flags
	sc_thread_state m_mti_thread_state;
	static sc_cor* m_mti_peeking_thread;

public:

    friend void mti_execute_thread(void*);
    friend void scmti_execute_switch_thread(void*, void*);
    friend void scmti_execute_sc_main(void*, int, unsigned int);
    friend void sc_start(const sc_time&);
	friend void mti_handle_sc_start(mtiScStartModeT , const sc_time*);
    void sc_mti_set_to_run() {
        m_mti_thread_state = MTI_SC_READY_;
    }
    sc_thread_state sc_mti_get_state() {
        return m_mti_thread_state;
    }

	void mti_set_sc_main_thread_flag() {
		m_mti_proc_flag |= SC_MTI_PROC_IS_SC_MAIN;
	}

	void mti_reset_sc_main_thread_flag() {
		m_mti_proc_flag &= ~SC_MTI_PROC_IS_SC_MAIN;
	}

	int mti_is_sc_main_thread() {
		if ((m_mti_proc_flag & SC_MTI_PROC_IS_SC_MAIN) != 0)
			return 1;
		return 0;
	}

    void sc_mti_set_peeking_thread(sc_cor* handle) {
        m_mti_peeking_thread = handle;
    }
    sc_cor* sc_mti_get_peeking_thread() {
        return m_mti_peeking_thread;
    }

    void sc_mti_block_thread();

    int sc_mti_thread_is_blocked();

    // END MODELSIM ADDITIONS


};



//------------------------------------------------------------------------------
//"sc_thread_process::ready_to_run"
//
//------------------------------------------------------------------------------
inline bool sc_thread_process::ready_to_run()
{
    if ( (m_throw_type == THROW_NONE) && ( m_wait_cycle_n > 0 ) )
    {
        --m_wait_cycle_n;
        return false;
    }
    return true;
}


//------------------------------------------------------------------------------
//"sc_thread_process::set_stack_size"
//
//------------------------------------------------------------------------------
inline void sc_thread_process::set_stack_size( std::size_t size )
{
    assert( size );
    m_stack_size = size;

	// BEGIN MODELSIM ADDITIONS
    m_mti_proc_flag |= SC_MTI_PROC_IS_STACK_SIZE_SET;
    // END MODELSIM ADDITIONS

}

//------------------------------------------------------------------------------
//"sc_thread_process::suspend_me"
//
// This method suspends this object instance in favor of the next runnable
// process. Upon awakening we check to see if an exception should be thrown.
//------------------------------------------------------------------------------

inline void sc_thread_process::suspend_me()
{
	// BEGIN MODELSIM ADDITION
	m_mti_thread_state = MTI_SC_WAIT_;
    sc_simcontext* simc_p = simcontext();
	sc_cor* next_cor = simc_p->next_cor();

	while (1) {
    	simc_p->cor_pkg()->yield( next_cor );
		/* If thread is resumed temporarily for peeking stack and locals info, 
		   then yield back to the original thread which yielded to this thread. */
		if ((next_cor = sc_mti_get_peeking_thread())) {
			mti_cdbg_suspend_me_wrapper();
			m_mti_thread_state = MTI_SC_WAIT_;
		} else 
			break;
	}
	// END MODELSIM ADDITION
    switch ( m_throw_type )
    {
      case THROW_NONE:
        break;
      case THROW_RESET:
        throw sc_user();
      default:
        break;
    }
}

//------------------------------------------------------------------------------
//"sc_thread_process::wait"
//
//------------------------------------------------------------------------------
inline
void
sc_thread_process::wait( const sc_event& e )
{
    e.add_dynamic( this );
    m_trigger_type = EVENT;
    suspend_me();
}

inline
void
sc_thread_process::wait( sc_event_or_list& el )
{
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST;
    suspend_me();
}

inline
void
sc_thread_process::wait( sc_event_and_list& el )
{
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t )
{
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    m_trigger_type = TIMEOUT;
    suspend_me();
}

// BEGIN MODELSIM ADDITION
inline void
sc_thread_process::sc_mti_block_thread()
{
    m_trigger_type = TIMEOUT;
    suspend_me();
}

inline int
sc_thread_process::sc_mti_thread_is_blocked()
{
    if (m_trigger_type == TIMEOUT) {
        return 1;
    }
    return 0;
}

// END MODELSIM_ADDITION

inline
void
sc_thread_process::wait( const sc_time& t, const sc_event& e )
{
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    e.add_dynamic( this );
    m_event_p = &e;
    m_trigger_type = EVENT_TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, sc_event_or_list& el )
{
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST_TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, sc_event_and_list& el )
{
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST_TIMEOUT;
    suspend_me();
}

//------------------------------------------------------------------------------
//"sc_thread_process::wait_cycles"
//
// This method suspends this object instance for the specified number of cycles.
// A cycle is defined as the event the thread is set up to staticly wait on.
// The field m_wait_cycle_n is set to one less than the number of cycles to
// be waited for, since the value is tested before being decremented in
// the simulation kernel.
//------------------------------------------------------------------------------
inline
void
sc_thread_process::wait_cycles( int n )
{
    m_wait_cycle_n = n-1;
    suspend_me();
}

//------------------------------------------------------------------------------
//"sc_thread_process::miscellaneous support"
//
//------------------------------------------------------------------------------
inline
void sc_thread_process::add_monitor(sc_process_monitor* monitor_p)
{
    m_monitor_q.push_back(monitor_p);
}


inline
void sc_thread_process::remove_monitor(sc_process_monitor* monitor_p)
{
    int mon_n = m_monitor_q.size();

    for ( int mon_i = 0; mon_i < mon_n; mon_i++ )
    {
    if  ( m_monitor_q[mon_i] == monitor_p )
        {
            m_monitor_q[mon_i] = m_monitor_q[mon_n-1];
            m_monitor_q.resize(mon_n-1);
        }
    }
}

inline
void sc_thread_process::set_next_exist(sc_thread_handle next_p)
{
    m_exist_p = next_p;
}

inline
sc_thread_handle sc_thread_process::next_exist()
{
    return (sc_thread_handle)m_exist_p;
}

inline
void sc_thread_process::set_next_runnable(sc_thread_handle next_p)
{
    m_runnable_p = next_p;
}

inline
sc_thread_handle sc_thread_process::next_runnable()
{
    return (sc_thread_handle)m_runnable_p;
}

inline sc_cor* get_cor_pointer( sc_process_b* process_p )
{
    sc_thread_handle thread_p = DCAST<sc_thread_handle>(process_p);
	sc_cor* cor_p = NULL;

	if (thread_p)
		cor_p = thread_p->m_cor_p;

	return cor_p;
}

} // namespace sc_core

#endif // !defined(sc_thread_process_h_INCLUDED)
