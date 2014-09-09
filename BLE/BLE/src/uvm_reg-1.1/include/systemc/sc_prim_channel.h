/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  sc_prim_channel.h -- Abstract base class of all primitive channel classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte,
                               Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: phase callbacks
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_prim_channel.h#1 $

 *****************************************************************************/

#ifndef SC_PRIM_CHANNEL_H
#define SC_PRIM_CHANNEL_H

#include "sc_object.h"
#include "sc_wait.h"
#include "sc_wait_cthread.h"
#include "ptrarray.h"

// BEGIN MODELSIM ADDITIONS
#include "scmti_msg.h"
// END MODELSIM ADDITIONS

namespace sc_core {

// BEGIN MODELSIM ADDITIONS
extern const char* const MTI_INTERNAL_CHANNEL_NAME_PREFIX;
// END MODELSIM ADDITIONS



// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel
//
//  Abstract base class of all primitive channel classes.
// ----------------------------------------------------------------------------

class sc_prim_channel
: public sc_object
{
    friend class sc_prim_channel_registry;

public:
    enum { list_end = 0xdb };
public:
    virtual const char* kind() const
        { return "sc_prim_channel"; }

    inline bool update_requested() 
	{ return m_update_next_p != (sc_prim_channel*)list_end; }

    // request the update method (to be executed during the update phase)
    inline void request_update();

protected:

    // constructors
    sc_prim_channel();
    explicit sc_prim_channel( const char* );

    // destructor
    virtual ~sc_prim_channel();

    // the update method (does nothing by default)
    virtual void update();

    // called by construction_done (does nothing by default)
    virtual void before_end_of_elaboration();

    // called by elaboration_done (does nothing by default)
    virtual void end_of_elaboration();

    // called by start_simulation (does nothing by default)
    virtual void start_of_simulation();

    // called by simulation_done (does nothing by default)
    virtual void end_of_simulation();

protected:

    // to avoid calling sc_get_curr_simcontext()


    // static sensitivity for SC_THREADs and SC_CTHREADs

    void wait()
        { sc_core::wait( simcontext() ); }


    // dynamic sensitivity for SC_THREADs and SC_CTHREADs

    void wait( const sc_event& e )
        { sc_core::wait( e, simcontext() ); }

    void wait( sc_event_or_list& el )
	{ sc_core::wait( el, simcontext() ); }

    void wait( sc_event_and_list& el )
	{ sc_core::wait( el, simcontext() ); }

    void wait( const sc_time& t )
        { sc_core::wait( t, simcontext() ); }

    void wait( double v, sc_time_unit tu )
        { sc_core::wait( sc_time( v, tu, simcontext() ), simcontext() ); }

    void wait( const sc_time& t, const sc_event& e )
        { sc_core::wait( t, e, simcontext() ); }

    void wait( double v, sc_time_unit tu, const sc_event& e )
        { sc_core::wait( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void wait( const sc_time& t, sc_event_or_list& el )
        { sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_or_list& el )
        { sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void wait( const sc_time& t, sc_event_and_list& el )
        { sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_and_list& el )
        { sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void wait( int n )
        { sc_core::wait( n, simcontext() ); }


    // static sensitivity for SC_METHODs

    void next_trigger()
	{ sc_core::next_trigger( simcontext() ); }


    // dynamic sensitivity for SC_METHODs

    void next_trigger( const sc_event& e )
        { sc_core::next_trigger( e, simcontext() ); }

    void next_trigger( sc_event_or_list& el )
        { sc_core::next_trigger( el, simcontext() ); }

    void next_trigger( sc_event_and_list& el )
        { sc_core::next_trigger( el, simcontext() ); }

    void next_trigger( const sc_time& t )
        { sc_core::next_trigger( t, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu )
        { sc_core::next_trigger( sc_time( v, tu, simcontext() ), simcontext() ); }

    void next_trigger( const sc_time& t, const sc_event& e )
        { sc_core::next_trigger( t, e, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, const sc_event& e )
        { sc_core::next_trigger( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void next_trigger( const sc_time& t, sc_event_or_list& el )
        { sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_or_list& el )
        { sc_core::next_trigger( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void next_trigger( const sc_time& t, sc_event_and_list& el )
        { sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_and_list& el )
        { sc_core::next_trigger( sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // for SC_METHODs and SC_THREADs and SC_CTHREADs

    bool timed_out()
	{ return sc_core::timed_out( simcontext() ); }


    // delta count maintenance

    sc_dt::uint64 delta_count()
	{ return sc_delta_count(); }

private:

    // called during the update phase of a delta cycle (if requested)
    void perform_update();

    // called during the update phase of a delta cycle (if requested but denied)
    void clear_update_requested_flag();

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

    // disabled
    sc_prim_channel( const sc_prim_channel& );
    sc_prim_channel& operator = ( const sc_prim_channel& );

private:

    sc_prim_channel_registry* m_registry;
    sc_prim_channel*          m_update_next_p; // Next entry in update list.

	// BEGIN MODELSIM ADDITIONS

public:

	friend void scmti_update_primitive_channel(void*);
	friend void scmti_clear_primitive_channel_update_requested_flag(void*);
	friend void scmti_get_primitive_channel_value(void*, void**, int);
	friend void scmti_set_primitive_channel_value(void*, void*, int);
	friend void scmti_fanout_to_primitive_channel(void*, void*);
	friend void scmti_get_primitive_channel_type(void*, void**);

    mtiObjectHandleT mti_get_prim_channel()
        { return m_mti_prim_channel; }

private:

	virtual void* mti_get_channel_value(int is_driving_value);

	virtual void mti_set_channel_value(
			void* value,
			int is_driving_value);

	void mti_record_parent_region_id();

    bool mti_is_boundary_top()
        { return m_mti_is_boundary_top; }

	bool mti_is_dummy_signal()
		{ return m_mti_is_dummy_signal; }

	/*
	 * By default, the object type is not available since construction
	 * parameters are ususally not required:
	 */
	virtual void mti_get_construction_params(
		int* elem_num_bits,
		int* elem_num_integer_bits,
		int* fifo_num_elem,
		int* hasConstructionParams,
		int* hasElementConstructionParams)
	{
		*hasConstructionParams = 0;
		*hasElementConstructionParams = 0;
	}

    bool mti_is_resolved()
        { return m_mti_is_resolved; }

	void mti_prim_channel_init();

	void mti_delayed_activate();

	virtual mtiTypeIdT mti_get_channel_type()
		{ return NULL; }

private:
	mtiObjectHandleT          m_mti_prim_channel;
	bool                      m_mti_is_boundary_top;
	sc_module*                m_mti_parent_module;
    bool                      m_mti_is_dummy_signal;

protected:
    bool                      m_mti_is_resolved;
	bool					  m_mti_is_new_val_resolution_done;

	// END MODELSIM ADDITIONS
};


// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel_registry
//
//  Registry for all primitive channels.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

class sc_prim_channel_registry
{
    friend class sc_simcontext;

public:

    void insert( sc_prim_channel& );
    void remove( sc_prim_channel& );

    int size() const
        { return m_prim_channel_vec.size(); }

    inline void request_update( sc_prim_channel& );

	bool pending_updates() const
		{ return m_update_list_p !=
		(sc_prim_channel*)sc_prim_channel::list_end;
	}

private:

    // constructor
    explicit sc_prim_channel_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_prim_channel_registry();

    // called during the update phase of a delta cycle
    inline void perform_update();

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

    // disabled
    sc_prim_channel_registry();
    sc_prim_channel_registry( const sc_prim_channel_registry& );
    sc_prim_channel_registry& operator = ( const sc_prim_channel_registry& );

private:

    sc_simcontext*                m_simc;

    // BEGIN MODELSIM MODIFICATIONS
    ptrarray<sc_prim_channel*>    m_prim_channel_vec;
    // END MODELSIM MODIFICATIONS

    sc_prim_channel*              m_update_list_p;

	// BEGIN MODELSIM ADDITIONS

	void mti_create_signals();
	bool mti_is_valid_prim_channel(sc_prim_channel* prim_channel) const;

	// END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel_registry
//
//  Registry for all primitive channels.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

inline
void
sc_prim_channel_registry::request_update( sc_prim_channel& prim_channel_ )
{
    prim_channel_.m_update_next_p = m_update_list_p;
    m_update_list_p = &prim_channel_;
}


// called during the update phase of a delta cycle

inline
void
sc_prim_channel_registry::perform_update()
{
    sc_prim_channel* next_p; // Next update to perform.
    sc_prim_channel* now_p;  // Update now performing.

    now_p = m_update_list_p;
    m_update_list_p = (sc_prim_channel*)sc_prim_channel::list_end;
    for ( ; now_p != (sc_prim_channel*)sc_prim_channel::list_end; 
    	now_p = next_p )
    {
        next_p = now_p->m_update_next_p;
	now_p->perform_update();
    }
}

// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel
//
//  Abstract base class of all primitive channel classes.
// ----------------------------------------------------------------------------

// request the update method (to be executed during the update phase)

inline
void
sc_prim_channel::request_update()
{
    if( ! m_update_next_p ) {
		// BEGIN MODELSIM ADDITIONS
		if (m_mti_prim_channel != NULL)
			mti_ActivateScPrimChannel( m_mti_prim_channel );
		// END MODELSIM ADDITIONS

		m_update_next_p = this;
		m_mti_is_new_val_resolution_done = false;
    }
}


// called during the update phase of a delta cycle (if requested)

inline
void
sc_prim_channel::perform_update()
{
    update();
	m_mti_is_new_val_resolution_done = false;
    m_update_next_p = 0;
}


// BEGIN MODELSIM ADDITIONS


// when an update has been requested on a forced signal,
// clear the m_update_next_p pointer, but do not update

inline
void
sc_prim_channel::clear_update_requested_flag()
{
    m_update_next_p = 0;
	m_mti_is_new_val_resolution_done = false;
}


inline
void
sc_prim_channel::mti_delayed_activate()
{
	if (m_update_next_p && m_mti_prim_channel != NULL) {
		mti_ActivateScPrimChannel( m_mti_prim_channel );
	}
}


// END MODELSIM ADDITIONS


} // namespace sc_core

#endif

// Taf!
