/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_clock.h -- The clock channel.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_clock.h#1 $

 *****************************************************************************/

#ifndef SC_CLOCK_H
#define SC_CLOCK_H


#include "sc_signal.h"
#include "sc_module.h"
#include "sc_signal_ifs.h"
#include "sc_trace.h"


namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_clock
//
//  The clock channel.  sc_clock is derived from sc_signal<bool> in 
//  SystemC 2.1.  In ModelSim sc_clock continues to be a hierarchical
//  channel as in SystemC 2.0.1 to make the variables in the clock
//  representable in the GUI. All the additional member functions added
//  to sc_clock in 2.1 has been incorporated in this implementation.
//
// ----------------------------------------------------------------------------
class sc_clock;
extern "C" void debug_clock_eval(sc_clock*, const char* msg);

class sc_clock
: public sc_signal_in_if<bool>,
  public sc_module
{
public:

    friend void debug_clock_eval(sc_clock*, const char* msg);

    // constructors

    sc_clock();

    explicit sc_clock( sc_module_name name_ );

    sc_clock( sc_module_name name_,
	      const sc_time& period_,
	      double         duty_cycle_ = 0.5,
	      const sc_time& start_time_ = SC_ZERO_TIME,
	      bool           posedge_first_ = true );

    sc_clock( sc_module_name name_,
	      double         period_v_,
	      sc_time_unit   period_tu_,
	      double         duty_cycle_ = 0.5 );

    sc_clock( sc_module_name name_,
	      double         period_v_,
	      sc_time_unit   period_tu_,
	      double         duty_cycle_,
	      double         start_time_v_,
	      sc_time_unit   start_time_tu_,
	      bool           posedge_first_ = true );

    // for backward compatibility with 1.0
    sc_clock( sc_module_name name_,
	      double         period_,            // in default time units
	      double         duty_cycle_ = 0.5,
	      double         start_time_ = 0.0,  // in default time units
	      bool           posedge_first_ = true );


    // destructor (does nothing)
    virtual ~sc_clock();


    // interface methods

    // get the default event
    virtual const sc_event& default_event() const;


    // get the value changed event
    virtual const sc_event& value_changed_event() const;

    // get the positive edge event
    virtual const sc_event& posedge_event() const;

    // get the negative edge event
    virtual const sc_event& negedge_event() const;


    // read the current value
    virtual const bool& read() const;

    // reset creation
    virtual sc_reset* is_reset() const
        { return sig->is_reset(); }

    // get a reference to the current value (for tracing)
    virtual const bool& get_data_ref() const
        { sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there a value changed event?
    virtual bool event() const;

    // was there a positive edge event?
    virtual bool posedge() const;

    // was there a negative edge event?
    virtual bool negedge() const;


    // other methods

    operator const bool& () const
	{ return read(); }


    // get the period
    const sc_time& period() const
	{ return m_period; }

    // get the duty cycle
    double duty_cycle() const
	{ return m_duty_cycle; }

	// get the current time / clock characteristics

    bool posedge_first() const
        { return m_posedge_first; }

    sc_time start_time() const
        { return m_start_time; }

    // get the current time
    static const sc_time& time_stamp();


    void trace( sc_trace_file* tf ) const
#ifdef DEBUG_SYSTEMC
	{ sc_trace( tf, get_data_ref(), name() ); }
#else
	{}
#endif


    virtual void print( ::std::ostream& = ::std::cout) const;
    virtual void dump( ::std::ostream& = ::std::cout) const;


    virtual const char* kind() const
        { return "sc_clock"; }

    // for backward compatibility with 1.0

    sc_signal_in_if<bool>& signal()
	{ return *this; }

    const sc_signal_in_if<bool>& signal() const
	{ return *this; }

    static void start( const sc_time& duration )
	{ sc_start( duration ); }

    static void start( double v, sc_time_unit tu )
	{ sc_start( v, tu ); }

    static void start( double duration = -1 )
	{ sc_start( duration ); }

    static void stop()
	{ sc_stop(); }

    // create a clock edge eval process
    void mti_create_edge_eval_process(bool is_posedge);

    void mti_activate_clock_edge_eval(sc_event::mtiClockEdgeTypeT type);

    void mti_resume_clock_edge_eval(sc_event::mtiClockEdgeTypeT type);

protected:

    void end_of_elaboration();

    // the actual clock eval routine that invoked by internal process 
    void posedge_action();
    void negedge_action();

    // error reporting
    void report_error( const char* id, const char* add_msg = 0 ) const;


    void init( const sc_time&, double, const sc_time&, bool );

protected:

    sc_time  m_period;		// the period of this clock
    double   m_duty_cycle;	// the duty cycle (fraction of period)
    sc_time  m_start_time;	// the start time of the first edge
    bool     m_posedge_first;   // true if first edge is positive
    sc_time  m_posedge_time;	// time till next positive edge
    sc_time  m_negedge_time;	// time till next negative edge

    bool     m_cur_val;

    /* these are internal events and never exposed to any external processes */
    sc_event m_next_posedge_event;
    sc_event m_next_negedge_event;

    sc_dt::uint64   m_delta; // delta of last event

    // internal clock signal (for MTI interface)
    sc_signal<bool> * sig;

private:

    // disabled
    sc_clock( const sc_clock& );
    sc_clock& operator = ( const sc_clock& );

    // start 2.1 additions
	
    bool is_clock() const { return true; }

    // end 2.1 additions

    // BEGIN MODELSIM ADDITIONS

    // the following two booleans decide whether clock eval process should
    // be activated or not when internal clock edge events mature.
    // When clock edge process doesn't need to be evaluated, we are skipping:
    //
    // 1. internal clock edge process scheduling and execution. (eval phase)
    // 2. clock signal (prim channel) scheduling and update (update phase)
    // 3. delta notification of external clock event scheduling and triggering.

    bool   m_mti_turn_off_posedge_action;
    bool   m_mti_turn_off_negedge_action;

    sc_method_handle m_mti_posedge_process;
    sc_method_handle m_mti_negedge_process;

    inline int num_of_posedge_event_static_trigs() const {
        return
            sig->value_changed_event().mti_get_num_static_triggers() +
            sig->posedge_event().mti_get_num_static_triggers();
    }

    inline int num_of_negedge_event_static_trigs() const {
        return 
            sig->value_changed_event().mti_get_num_static_triggers() +
            sig->negedge_event().mti_get_num_static_triggers();
    }

    inline int num_of_posedge_event_dynamic_trigs() const {
        return
            sig->value_changed_event().mti_get_num_dynamic_triggers() +
            sig->posedge_event().mti_get_num_dynamic_triggers();
    }

    inline int num_of_negedge_event_dynamic_trigs() const {
        return 
            sig->value_changed_event().mti_get_num_dynamic_triggers() +
            sig->negedge_event().mti_get_num_dynamic_triggers();
    }

public:	

    // binding to internal clock signal (MTI)
    virtual bool mti_bind_sub_interface() { return true; }
    virtual sc_interface* mti_get_sub_interface() { return sig; }

    //
    // Control an HDL signal. The value change of this signal is propagated
    // to the HDL signal, but not vice versa.
    // This function returns true if the foreign object is found and
    // the type is compatible.
    //
    bool control_foreign_signal(const char* hdl_name)
    {
        m_mti_turn_off_posedge_action = false;
        m_mti_turn_off_negedge_action = false;
        return sig->control_foreign_signal(hdl_name); 
    }

    //
    // It is an error for sc_clock to observe a HDL signal.
    //
    bool observe_foreign_signal(const char* hdl_name)
    {
        SC_REPORT_ERROR(MSG_IDX_SC_CLOCK_OBSERVE_ERROR, name());
        return false;
    }


    // END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// interface methods

// get the default event

inline
const sc_event&
sc_clock::default_event() const
{
    return sig->default_event();
}


// get the value changed event

inline
const sc_event&
sc_clock::value_changed_event() const
{
    return sig->value_changed_event();
}

// get the positive edge event

inline
const sc_event&
sc_clock::posedge_event() const
{
    return sig->posedge_event();
}

// get the negative edge event

inline
const sc_event&
sc_clock::negedge_event() const
{
    return sig->negedge_event();
}


// read the current value

inline
const bool&
sc_clock::read() const
{
    return m_cur_val;
}


// was there a value changed event?

inline
bool
sc_clock::event() const
{
    return ( sc_delta_count() == m_delta + 1 );
}

// was there a positive edge event?

inline
bool
sc_clock::posedge() const
{
    return ( event() && m_cur_val );
}

// was there a negative edge event?

inline
bool
sc_clock::negedge() const
{
    return ( event() && ! m_cur_val );
}


// processes

inline
void
sc_clock::posedge_action()
{
    // BEGIN MODELSIM MODIFICATIONS
    m_cur_val = true;
    m_next_posedge_event.notify_clock_event( m_period );
    m_delta = sc_delta_count();
    // update the internal clock 'signal'
    sig->write(1);
    // END MODELSIM MODIFICATIONS
}

inline
void
sc_clock::negedge_action()
{
    // BEGIN MODELSIM MODIFICATIONS
    m_cur_val = false;
    m_next_negedge_event.notify_clock_event( m_period );
    m_delta = sc_delta_count();
    // update the internal clock 'signal'
    sig->write(0);
    // END MODELSIM MODIFICATIONS
}


// ----------------------------------------------------------------------------

// for backward compatibility with 1.0

inline
void
sc_start( sc_clock& clock, const sc_time& duration )
{
    clock.start( duration );
}

inline
void
sc_start( sc_clock& clock, double v, sc_time_unit tu )
{
    clock.start( v, tu );
}

inline
void
sc_start( sc_clock& clock, double duration = -1 )
{
    clock.start( duration );
}

} // namespace sc_core


#endif

// Taf!
