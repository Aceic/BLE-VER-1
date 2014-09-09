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

  sc_event.h --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_event.h#1 $

 *****************************************************************************/

#ifndef SC_EVENT_H
#define SC_EVENT_H


#include "sc_kernel_ids.h"
#include "sc_simcontext.h"
#include "sc_logic.h"

// BEGIN MODELSIM ADDITIONS
#include "scmtiapi.h"
#include "scmti.h"
#include "scmti_msg.h"
#include "ptrarray.h"
// END MODELSIM ADDITIONS

namespace sc_core {

// forward declarations
class sc_clock;
class sc_event_timed;
class sc_event_list;
class sc_event_or_list;
class sc_event_and_list;

// friend function declarations
    int sc_notify_time_compare( const void*, const void* );

// ----------------------------------------------------------------------------
//  CLASS : sc_event
//
//  The event class.
// ----------------------------------------------------------------------------

class sc_event
{
    friend class sc_clock;
    friend class sc_event_list;
    friend class sc_event_timed;
    friend class sc_simcontext;
    friend class sc_process_b;
    friend class sc_method_process;
    // BEGIN MODELSIM ADDITIONS
    friend class sc_method_debug_process;
    // END MODELSIM ADDITIONS
    friend class sc_thread_process;
    template<typename IF> friend class sc_signal;
    friend class sc_signal<bool>;
    friend class sc_signal<sc_dt::sc_logic>;

    // BEGIN MODELSIM ADDITIONS
    // add the counting method of the event triggers
public:
    int mti_get_num_static_triggers() const {
        return m_methods_static.size() + m_threads_static.size();
    }

    int mti_get_num_dynamic_triggers() const {
        return m_methods_dynamic.size() + m_threads_dynamic.size();
    }
    // END MODELSIM ADDITIONS
public:
    /* event that should trigger certain clock edge process */
    enum mtiClockEdgeTypeT {
        MTI_UNKNOWN_CLK_EVENT, 
        MTI_POSEDGE_CLK_EVENT, 
        MTI_NEGEDGE_CLK_EVENT,
        MTI_BOTHEDGE_CLK_EVENT,
        MTI_INTERNAL_POSEDGE_CLK_EVENT,
        MTI_INTERNAL_NEGEDGE_CLK_EVENT
    };

    sc_event();
    ~sc_event();

    void cancel();

    void notify();
    void notify( const sc_time& );
    void notify( double, sc_time_unit );

    void notify_delayed();
    void notify_delayed( const sc_time& );
    void notify_delayed( double, sc_time_unit );

    sc_event_or_list&  operator | ( const sc_event& ) const;
    sc_event_and_list& operator & ( const sc_event& ) const;

private:

    void add_static( sc_method_handle ) const;
    void add_static( sc_thread_handle ) const;
    void add_dynamic( sc_method_handle ) const;
    void add_dynamic( sc_thread_handle ) const;

    void notify_internal( const sc_time& );
    void notify_next_delta();

    // BEGIN MODELSIM ADDITIONS
    void notify_clock_event(const sc_time&);
    // END MODELSIM ADDITIONS

    bool remove_static( sc_method_handle ) const;
    bool remove_static( sc_thread_handle ) const;
    bool remove_dynamic( sc_method_handle ) const;
    bool remove_dynamic( sc_thread_handle ) const;

    void reset();

    // START MODELSIM MODIFICATIONS
    void trigger( int isImmediateNotify = 0 );
    void triggerClock();
    // END MODELSIM MODIFICATIONS


private:

    enum notify_t { NONE, DELTA, TIMED };

    sc_simcontext*  m_simc;
    notify_t        m_notify_type;
    int             m_delta_event_index;
    sc_event_timed* m_timed;

    // START MODELSIM MODIFICATIONS

    mutable ptrarray<sc_method_handle> m_methods_static;
    mutable ptrarray<sc_method_handle> m_methods_dynamic;
    mutable ptrarray<sc_thread_handle> m_threads_static;
    mutable ptrarray<sc_thread_handle> m_threads_dynamic;

    // END MODELSIM MODIFICATIONS

private:

    // disabled
    sc_event( const sc_event& );
    sc_event& operator = ( const sc_event& );

	// BEGIN modelsim additions
 	sc_event(sc_clock* clk, mtiClockEdgeTypeT type);
	void mti_resume_clock_edge_eval() const;

public:

	friend void scmti_trigger_event(void* sc_event);
	friend void scmti_trigger_clock_event(void* sc_event);

	void mti_set_is_prim_channel_event(bool is_prim_channel_event);
	void mti_set_is_clock_event(sc_clock* eval_clk, mtiClockEdgeTypeT type);

private:

	mtiObjectHandleT m_mti_event_id;
	bool             m_mti_is_prim_channel_event;
	sc_clock*          m_mti_clock;
	mtiClockEdgeTypeT  m_mti_clk_edge_type;

	// END MODELSIM ADDITIONS
};

extern sc_event sc_non_event; // Event that never happens.

// ----------------------------------------------------------------------------
//  CLASS : sc_event_timed
//
//  Class for storing the time to notify a timed event.
// ----------------------------------------------------------------------------

class sc_event_timed
{
    friend class sc_event;
    friend class sc_simcontext;

    friend int sc_notify_time_compare( const void*, const void* );

private:

    sc_event_timed( sc_event* e, const sc_time& t )
        : m_event( e ), m_notify_time( t )
        {}

    ~sc_event_timed()
        { if( m_event != 0 ) { m_event->m_timed = 0; } }

    sc_event* event() const
        { return m_event; }

    const sc_time& notify_time() const
        { return m_notify_time; }

    static void* operator new( std::size_t )
        { return allocate(); }

    static void operator delete( void* p, std::size_t )
        { deallocate( p ); }

private:

    // dedicated memory management
    static void* allocate();
    static void  deallocate( void* );

private:

    sc_event* m_event;
    sc_time   m_notify_time;

private:

    // disabled
    sc_event_timed();
    sc_event_timed( const sc_event_timed& );
    sc_event_timed& operator = ( const sc_event_timed& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// BEGIN MODELSIM ADDITIONS
inline
sc_event::sc_event(sc_clock* clk, mtiClockEdgeTypeT type)
: m_simc( sc_get_curr_simcontext() ),
  m_notify_type( NONE ),
  m_delta_event_index( -1 ),
  m_timed( 0 )
{
        m_mti_event_id = mti_CreateScEvent(this, 1);
	m_mti_is_prim_channel_event = false;
	assert(mti_IsVoptMode() || m_mti_event_id);
	m_mti_clock = clk;
	m_mti_clk_edge_type = type;  
}
// END MODELSIM ADDITIONS

inline
sc_event::sc_event()
: m_simc( sc_get_curr_simcontext() ),
  m_notify_type( NONE ),
  m_delta_event_index( -1 ),
  m_timed( 0 )
{
	// BEGIN MODELSIM ADDITIONS
	m_mti_event_id = mti_CreateScEvent(this, 0);
	m_mti_is_prim_channel_event = false;
	assert(mti_IsVoptMode() || m_mti_event_id);
	m_mti_clock = NULL;
	m_mti_clk_edge_type = MTI_UNKNOWN_CLK_EVENT;

	// END MODELSIM ADDITIONS
}

inline
sc_event::~sc_event()
{
    cancel();
}


inline
void
sc_event::notify( double v, sc_time_unit tu )
{
	// START MODELSIM MODIFICATIONS
	sc_dt::uint64 delay = 0;

	// Check if a delta event is already schedule.
	if( m_notify_type == DELTA )
		return;

	/*
	 * If the notify time unit matches the resolution both converted
	 * to femto seconds, avoid creation of a sc_time object.  A sc_time
	 * object a required if the time value needs conversion to resolution
	 * units.
	 */
	if (time_values[tu] == m_simc->m_time_params->time_resolution) {
		if( v == 0.0 ) {
			//
			// Do not schedule an event for a zero delay notification
			// from a primitive channel.  This optimization does not
			// work when the notification is coming from another
			// process.  The process that is triggered might be already
			// in the active_processes list and not executed yet in
			// the current delta.  A process can be put in the active
			// processes list once. 
			//
			if (m_mti_is_prim_channel_event) {
				//
				// If a timed event is already scheduled in some future
				// time, then cancel that event and schedule a new delta
				// event.
				//
				if( m_notify_type == TIMED )
					cancel();

				// Trigger to schedule processes at the next delta.
				trigger();

			} else {
				mti_ScheduleScEvent(m_mti_event_id, (void *)&delay);
				m_notify_type = DELTA;
			}
			return;
		}

		//
		// Schedule an event at a future time.  If the event is already
		// scheduled at a time further in the future than the current 
		// schedule time, then the old event is canceled and rescheduled.
		//
		delay = (sc_dt::uint64)v;
		mti_ScheduleScEvent(m_mti_event_id, (void *)&delay);
		m_notify_type = TIMED;
	} else {
		notify( sc_time( v, tu, m_simc ) );
	}

	// END MODELSIM MODIFICATIONS
}

inline
void
sc_event::notify_internal( const sc_time& t )
{
	// START MODELSIM MODIFICATIONS

    sc_dt::uint64 delay = 0;
    if( t == SC_ZERO_TIME ) {
        //
        // Do not schedule an event for a zero delay notification
        // from a primitive channel.  This optimization does not
        // work when the notification is coming from another
        // process.  The process that is triggered might be already
        // in the active_processes list and not executed yet in
        // the current delta.  A process can be put in the active
        // processes list once.
        //
        if (m_mti_is_prim_channel_event) {
            trigger();
        } else {
            mti_ScheduleScEvent(m_mti_event_id, (void *)&delay);
            m_notify_type = DELTA;
        }
    } else {
        m_notify_type = TIMED;

        //
        // Schedule an event with the specified delay.
        //
        delay = t.value();
        mti_ScheduleScEvent(m_mti_event_id, (void *)&delay);
    }

    // END MODELSIM MODIFICATIONS
}

inline
void
sc_event::notify_next_delta()
{
	if( m_notify_type != NONE )
        SC_REPORT_ERROR( SC_ID_NOTIFY_DELAYED_, 0 );

    // START MODELSIM MODIFICATIONS

    //
    // Do not schedule an event for a zero delay notification
    // from a primitive channel.  This optimization does not
    // work when the notification is coming from another
    // process.  The process that is triggered might be already
    // in the active_processes list and not executed yet in
    // the current delta.  A process can be put in the active
    // processes list once.
    //

    if (m_mti_is_prim_channel_event) {
        trigger();
    } else {
        sc_dt::uint64 delay = 0;
        mti_ScheduleScEvent(m_mti_event_id, (void *)&delay);
        m_notify_type = DELTA;
    }

    // END MODELSIM MODIFICATIONS
}

// START MODELSIM ADDITIONS
inline
void
sc_event::notify_clock_event(const sc_time& t)
{
    sc_dt::uint64 delay = 0;
    delay = t.value();

    mti_ScheduleScClockEvent(m_mti_event_id, (void *)&delay);
    m_notify_type = TIMED;
}
// END MODELSIM ADDITIONS

inline
void
sc_event::add_static( sc_method_handle method_h ) const
{
    m_methods_static.push_back( method_h );
}

inline
void
sc_event::add_static( sc_thread_handle thread_h ) const
{
    m_threads_static.push_back( thread_h );
}

inline
void
sc_event::add_dynamic( sc_method_handle method_h ) const
{
    m_methods_dynamic.push_back( method_h );
    if (m_mti_clock) {
        mti_resume_clock_edge_eval();
    }
}

inline
void
sc_event::add_dynamic( sc_thread_handle thread_h ) const
{
    m_threads_dynamic.push_back( thread_h );
    if (m_mti_clock) {
        mti_resume_clock_edge_eval();
    }
}

// ----------------------------------------------------------------------------
//  Deprecated functional notation for notifying events.
// ----------------------------------------------------------------------------

extern void notify( sc_event& e );
extern void notify( const sc_time& t, sc_event& e );
extern void notify( double v, sc_time_unit tu, sc_event& e );

// ----------------------------------------------------------------------------
//  CLASS : sc_event_list
//
//  Base class for lists of events.
// ----------------------------------------------------------------------------

class sc_event_list
{
    friend class sc_process_b;
    friend class sc_method_process;
    // BEGIN MODELSIM ADDITIONS
    friend class sc_method_debug_process;
    // END MODELSIM ADDITIONS
    friend class sc_thread_process;

protected:

    void push_back( const sc_event& );

    sc_event_list( const sc_event&,
                   bool and_list_,
                   bool auto_delete_ = false );

    int size() const;
    bool and_list() const;

    void add_dynamic( sc_method_handle );
    void add_dynamic( sc_thread_handle );
    void remove_dynamic( sc_method_handle, const sc_event* );
    void remove_dynamic( sc_thread_handle, const sc_event* );

    void auto_delete();

private:

    // START MODELSIM MODIFICATIONS

    ptrarray<const sc_event*> m_events;

    // END MODELSIM MODIFICATIONS

    bool                        m_and_list;
    bool                        m_auto_delete;

private:

    // disabled
    sc_event_list();
    sc_event_list( const sc_event_list& );
    sc_event_list& operator = ( const sc_event_list& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_event_list::sc_event_list( const sc_event& e,
                              bool and_list_,
                              bool auto_delete_ )
: m_and_list( and_list_ ),
  m_auto_delete( auto_delete_ )
{
    m_events.push_back( &e );
}


inline
int
sc_event_list::size() const
{
    return m_events.size();
}

inline
bool
sc_event_list::and_list() const
{
    return m_and_list;
}


inline
void
sc_event_list::auto_delete()
{
    if( m_auto_delete ) {
        delete this;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_event_or_list
//
//  OR list of events.
// ----------------------------------------------------------------------------

class sc_event_or_list
: public sc_event_list
{
    friend class sc_event;
    friend class sc_process_b;
    friend class sc_method_process;
    // BEGIN MODELSIM ADDITIONS
    friend class sc_method_debug_process;
    // END MODELSIM ADDITIONS
    friend class sc_thread_process;

protected:

    sc_event_or_list( const sc_event&, bool auto_delete_ = false );

public:

    sc_event_or_list& operator | ( const sc_event& );

private:

    // disabled
    sc_event_or_list();
    sc_event_or_list( const sc_event_or_list& );
    sc_event_or_list& operator = ( const sc_event_or_list& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_event_or_list::sc_event_or_list( const sc_event& e,
                                    bool auto_delete_ )
: sc_event_list( e, false, auto_delete_ )
{}


inline
sc_event_or_list&
sc_event_or_list::operator | ( const sc_event& e )
{
    push_back( e );
    return *this;
}


// sc_event

inline
sc_event_or_list&
sc_event::operator | ( const sc_event& e2 ) const
{
    sc_event_or_list* el = new sc_event_or_list( *this, true );
    el->push_back( e2 );
    return *el;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_event_and_list
//
//  AND list of events.
// ----------------------------------------------------------------------------

class sc_event_and_list
: public sc_event_list
{
    friend class sc_event;
	friend class sc_process_b;
    friend class sc_method_process;
    // BEGIN MODELSIM ADDITIONS
    friend class sc_method_debug_process;
    // END MODELSIM ADDITIONS
    friend class sc_thread_process;

protected:

    sc_event_and_list( const sc_event&, bool auto_delete_ = false );

public:

    sc_event_and_list& operator & ( const sc_event& );

private:

    // disabled
    sc_event_and_list();
    sc_event_and_list( const sc_event_and_list& );
    sc_event_and_list& operator = ( const sc_event_and_list& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_event_and_list::sc_event_and_list( const sc_event& e,
                                      bool auto_delete_ )
: sc_event_list( e, true, auto_delete_ )
{}


inline
sc_event_and_list&
sc_event_and_list::operator & ( const sc_event& e )
{
    push_back( e );
    return *this;
}


// sc_event

inline
sc_event_and_list&
sc_event::operator & ( const sc_event& e2 ) const
{
    sc_event_and_list* el = new sc_event_and_list( *this, true );
    el->push_back( e2 );
    return *el;
}

} // namespace sc_core

#endif

// Taf!
