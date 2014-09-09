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

  sc_signal.h -- The sc_signal<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  Modification log appears at the end of the file
 *****************************************************************************/

/* $Id: //dvt/mti/rel/10.0b/src/systemc/sc_signal.h#1 $ */

#ifndef SC_SIGNAL_H
#define SC_SIGNAL_H


#include "sc_port.h"
#include "sc_prim_channel.h"
#include "sc_signal_ifs.h"
#include "sc_string.h"
#include "sc_event.h"
#include "sc_process.h"
#include "sc_reset.h"
#include "sc_simcontext.h"
#include "sc_logic.h"
#include "sc_trace.h"
#include <typeinfo>

namespace sc_core {


// to avoid code bloat in sc_signal<T>

extern void sc_deprecated_get_data_ref();
extern void sc_deprecated_get_new_value();
extern void sc_deprecated_trace();

extern
void
sc_signal_invalid_writer(
    sc_object* target, sc_object* first_writer, sc_object* second_writer );


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<T>
//
//  The sc_signal<T> primitive channel class.
// ----------------------------------------------------------------------------

template <class T>
class sc_signal
: public sc_prim_channel,  // modelsim modification
						   // Need to be the first to ensure the address
						   // of the class is in the registery for name
						   // binding to work correctly.
  public sc_signal_inout_if<T>
{
  public: // constructors and destructor:

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
	  m_change_event_p( 0 ), m_cur_val( T() ), 
	  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( T() ), m_output( 0 ), 
	  m_writer( 0 ) 
	{
		// BEGIN MODELSIM ADDITIONS
		sc_object::mti_set_use_default_name(true);
		// END MODELSIM ADDITIONS
	}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
	  m_change_event_p( 0 ), m_cur_val( T() ), 
	  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( T() ), m_output( 0 ), 
	  m_writer( 0 ) 
	{}


    virtual ~sc_signal()
	{
	    if ( !m_change_event_p ) delete m_change_event_p;
	}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ 
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_change_event_p ) {
			m_change_event_p = new sc_event; 
			m_change_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ 
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_change_event_p ) {
			m_change_event_p = new sc_event; 
			m_change_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_change_event_p; 
	}


    // read the current value
    virtual const T& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const T& get_data_ref() const
        { sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta + 1); }

    // write the new value
    virtual void write( const T& );


    // other methods

    operator const T& () const
	{ return read(); }


    sc_signal<T>& operator = ( const T& a )
	{ write( a ); return *this; }

    sc_signal<T>& operator = ( const sc_signal<T>& a )
	{ write( a.read() ); return *this; }


    const T& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
	{ 
	    sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_trace( tf, read(), name() ); 
#	    endif
	}


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_signal"; }

protected:

    virtual void update();

protected:

    mutable sc_event*  m_change_event_p;
    T                  m_cur_val;
    sc_dt::uint64      m_delta;   // delta of last event
    T                  m_new_val;
    sc_port_base*      m_output; // used for static design rule checking
    sc_object*         m_writer; // used for dynamic design rule checking

private:

    // disabled
    sc_signal( const sc_signal<T>& );

protected:
	// BEGIN MODELSIM ADDITIONS
	
    virtual mtiObjectHandleT mti_get_channel_id( sc_object* );

    virtual void* mti_get_channel_value(int is_driving_value);

    virtual void mti_set_channel_value(
        void* value,
        int is_driving_value);

	virtual void mti_get_construction_params(
		int* elem_num_bits,
		int* elem_num_integer_bits,
		int* fifo_num_elem,
		int* hasConstructionParams,
		int* hasElementConstructionParams);

	virtual mtiTypeIdT mti_get_channel_type();

public:
    //
    // Control an HDL signal. The value change of this signal is propagated
    // to the HDL signal, but not vice versa.
    // This function returns true if the foreign object is found and
    // the type is compatible.
    //
    bool control_foreign_signal(const char* hdl_name);

    //
    // Observe an HDL signal. The value change of the HDL signal is progagated
    // to the signal, but not vice versa.
    // This function returns true if the foreign object is found and
    // the type is compatible.
    //
    bool observe_foreign_signal(const char* hdl_name);


private:
	T 	m_mti_value;

	// END MODELSIM ADDITIONS

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


template <class T>
inline
void
sc_signal<T>::register_port( sc_port_base& port_, const char* if_typename_ )
{
    if ( sc_get_curr_simcontext()->write_check() )
    {
	std::string nm( if_typename_ );
	if( nm == typeid( sc_signal_inout_if<T> ).name() ) {
	    // an out or inout port; only one can be connected
	    if( m_output != 0) {
		sc_signal_invalid_writer( this, m_output, &port_ );
	    }
	    m_output = &port_;
	}
    }
}


// write the new value

template <class T>
inline
void
sc_signal<T>::write( const T& value_ )
{
	// BEGIN MODELSIM MODIFICATIONS
    if ( sc_get_curr_simcontext()->write_check() )
    {
		sc_object* writer = sc_get_curr_simcontext()->get_current_writer();
		if( m_writer == 0 ) {
		m_writer = writer;
		} else if( m_writer != writer ) {
		sc_signal_invalid_writer( this, m_writer, writer );
		}
	}
	// END MODELSIM MODIFICATIONS

    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


template <class T>
inline
void
sc_signal<T>::print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

template <class T>
inline
void
sc_signal<T>::dump( ::std::ostream& os ) const
{
    os << "     name = " << name() << ::std::endl;
    os << "    value = " << m_cur_val << ::std::endl;
    os << "new value = " << m_new_val << ::std::endl;
}


template <class T>
inline
void
sc_signal<T>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_cur_val = m_new_val;
	if ( m_change_event_p ) m_change_event_p->notify_next_delta();
	m_delta = sc_delta_count();
    }
}

class sc_reset;

// ----------------------------------------------------------------------------
//  CLASS : sc_signal<bool>
//
//  Specialization of sc_signal<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class sc_signal<bool>
: public sc_prim_channel,  // modelsim modification
						   // Need to be the first to ensure the address
						   // of the class is in the registery for name
						   // binding to work correctly.
  public sc_signal_inout_if<bool>
{
public: // constructors and destructor:

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
		  m_change_event_p( 0 ),
          m_cur_val( false ),
          m_delta( ~sc_dt::UINT64_ONE ),
		  m_negedge_event_p( 0 ),
          m_new_val( false ),
          m_output( 0 ),
		  m_posedge_event_p( 0 ),
          m_reset_p( 0 ),
          m_writer( 0 ),
		  m_mti_value( false ),
		  m_mti_is_clock( false )
	{
		// BEGIN MODELSIM ADDITIONS
		sc_object::mti_set_use_default_name(true);
		// END MODELSIM ADDITIONS
	}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
		  m_change_event_p( 0 ),
          m_cur_val( false ),
          m_delta( ~sc_dt::UINT64_ONE ),
		  m_negedge_event_p( 0 ),
          m_new_val( false ),
          m_output( 0 ),
		  m_posedge_event_p( 0 ),
          m_reset_p( 0 ),
          m_writer( 0 ),
		  m_mti_value( false ),
		  m_mti_is_clock( false )
	{}

    virtual ~sc_signal()
	{
		if ( !m_change_event_p )  delete m_change_event_p;
		if ( !m_negedge_event_p ) delete m_negedge_event_p;
		if ( !m_posedge_event_p ) delete m_posedge_event_p;
		if ( m_reset_p )          delete m_reset_p;
	}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_change_event_p ) {
			m_change_event_p = new sc_event; 
			m_change_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_change_event_p ) {
			m_change_event_p = new sc_event; 
			m_change_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_change_event_p; 
	}

    // get the positive edge event
    virtual const sc_event& posedge_event() const
	{
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_posedge_event_p ) {
			m_posedge_event_p = new sc_event; 
			m_posedge_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_posedge_event_p; 
	}

    // get the negative edge event
    virtual const sc_event& negedge_event() const
	{
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_negedge_event_p ) {
			m_negedge_event_p = new sc_event; 
			m_negedge_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_negedge_event_p; 
	}


    // read the current value
    virtual const bool& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const bool& get_data_ref() const
        { sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there a value changed event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta + 1); }

    // was there a positive edge event?
    virtual bool posedge() const
	{ return ( event() && m_cur_val ); }

    // was there a negative edge event?
    virtual bool negedge() const
	{ return ( event() && ! m_cur_val ); }

    // reset creation

    virtual sc_reset* is_reset() const;

    // write the new value
    virtual void write( const bool& );

    // other methods

    operator const bool& () const
	{ return read(); }


    sc_signal<bool>& operator = ( const bool& a )
	{ write( a ); return *this; }

    sc_signal<bool>& operator = ( const sc_signal<bool>& a )
	{ write( a.read() ); return *this; }


    const bool& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
	{
	    sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_trace( tf, read(), name() ); 
#           endif
	}


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_signal"; }

protected:

    virtual void update();

	// START MODELSIM MODIFICATIONS

    bool is_clock() const { return m_mti_is_clock; }

	// END MODELSIM MODIFICATIONS


protected:
    mutable sc_event* m_change_event_p;  // value change event if present.
    bool              m_cur_val;         // current value of object.
    sc_dt::uint64     m_delta;           // delta of last event
    mutable sc_event* m_negedge_event_p; // negative edge event if present.
    bool              m_new_val;         // next value of object.
    sc_port_base*     m_output;          // used for static design rule checking
    mutable sc_event* m_posedge_event_p; // positive edge event if present.
    mutable sc_reset* m_reset_p;         // reset mechanism if present.
    sc_object*        m_writer;          // process writing this object's value.

private:

    // disabled
    sc_signal( const sc_signal<bool>& );

public:
	// BEGIN MODELSIM ADDITIONS
	
	//
	// This is a special constructor added only for sc_signal<bool>
	// that takes the initial value of the signal as a parameter.
	// This is used by the signal inside sc_clock which needs to start
	// off with an initial value of 0 or 1 without generating a value
	// change event.  The clock signal can start with a value 1 if it
	// has a negedge first or a value 0 if it has a posedge first.
	//
	sc_signal( const char* name_, bool initial_value_ )
		: sc_prim_channel( name_ ),
		  m_change_event_p( 0 ),
          m_cur_val( initial_value_ ),
          m_delta( ~sc_dt::UINT64_ONE ),
		  m_negedge_event_p( 0 ),
          m_new_val( false ),
          m_output( 0 ),
		  m_posedge_event_p( 0 ),
          m_reset_p( 0 ),
          m_writer( 0 ),
		  m_mti_value( false),
		  m_mti_is_clock( false )
	{}

	void mti_set_is_clock(bool is_clock) { m_mti_is_clock = is_clock; }

    //
    // Control an HDL signal. The value change of this signal is propagated
    // to the HDL signal, but not vice versa.
    // This function returns true if the foreign object is found and
    // the type is compatible.
    //
    bool control_foreign_signal(const char* hdl_name);

    //
    // Observe an HDL signal. The value change of the HDL signal is progagated
    // to the signal, but not vice versa.
    // This function returns true if the foreign object is found and
    // the type is compatible.
    //
    bool observe_foreign_signal(const char* hdl_name);

protected:

    virtual mtiObjectHandleT mti_get_channel_id( sc_object* );

    virtual void* mti_get_channel_value(int is_driving_value);

    virtual void mti_set_channel_value(
        void* value,
        int is_driving_value);

	virtual void mti_get_construction_params(
		int* elem_num_bits,
		int* elem_num_integer_bits,
		int* fifo_num_elem,
		int* hasConstructionParams,
		int* hasElementConstructionParams);

	virtual mtiTypeIdT mti_get_channel_type();

private:
	bool m_mti_value;
	bool m_mti_is_clock;

	// END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_signal<bool>::register_port( sc_port_base& port_, const char* if_typename_ )
{
    if ( sc_get_curr_simcontext()->write_check() )
    {
	std::string nm( if_typename_ );
	if( nm == typeid( sc_signal_inout_if<bool> ).name() ) {
	    // an out or inout port; only one can be connected
	    if( m_output != 0 ) {
		sc_signal_invalid_writer( this, m_output, &port_ );
	    }
	    m_output = &port_;
	}
    }
}


// write the new value

inline
void
sc_signal<bool>::write( const bool& value_ )
{
	// BEGIN MODELSIM MODIFICATIONS
	
	//
	// Don't bother checking for multiple drivers on the signal inside
	// sc_clock since it will always have multiple drivers from the posedge
	// and negedge processes.
	//
	if (!m_mti_is_clock && sc_get_curr_simcontext()->write_check())
    {
		sc_object* writer = sc_get_curr_simcontext()->get_current_writer();
		if( m_writer == 0 ) {
		m_writer = writer;
		} else if( m_writer != writer ) {
		sc_signal_invalid_writer( this, m_writer, writer );
		}
	}
	// END MODELSIM MODIFICATIONS

    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


inline
void
sc_signal<bool>::print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

inline
void
sc_signal<bool>::dump( ::std::ostream& os ) const
{
    os << "     name = " << name() << ::std::endl;
    os << "    value = " << m_cur_val << ::std::endl;
    os << "new value = " << m_new_val << ::std::endl;
}


inline
void
sc_signal<bool>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
        // order of execution below is important, the notify_processes() call
        // must come after the update of m_cur_val for things to work properly!
        m_cur_val = m_new_val;
	if ( m_reset_p ) m_reset_p->notify_processes();

        if ( m_change_event_p ) m_change_event_p->notify_next_delta();
        if( m_cur_val ) {
            if ( m_posedge_event_p ) m_posedge_event_p->notify_next_delta();
        } else {
            if ( m_negedge_event_p ) m_negedge_event_p->notify_next_delta();
        }
        m_delta = sc_delta_count();
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<sc_dt::sc_logic>
//
//  Specialization of sc_signal<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

template <>
class sc_signal<sc_dt::sc_logic>
: public sc_prim_channel,  // modelsim modification
						   // Need to be the first to ensure the address
						   // of the class is in the registery for name
						   // binding to work correctly.
  public sc_signal_inout_if<sc_dt::sc_logic>
{
public:

    // constructors

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
	  m_change_event_p( 0 ),
	  m_cur_val(),
	  m_delta( ~sc_dt::UINT64_ONE ),
	  m_negedge_event_p( 0 ),
	  m_new_val(),
	  m_output( 0 ),
	  m_posedge_event_p( 0 ),
	  m_writer( 0 )
	{
		// BEGIN MODELSIM ADDITIONS
		sc_object::mti_set_use_default_name(true);
		// END MODELSIM ADDITIONS
	}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
	  m_change_event_p( 0 ),
	  m_cur_val(),
	  m_delta( ~sc_dt::UINT64_ONE ),
	  m_negedge_event_p( 0 ),
	  m_new_val(),
	  m_output( 0 ),
	  m_posedge_event_p( 0 ),
	  m_writer( 0 )
	{}

    // destructor (does nothing)

    virtual ~sc_signal()
	{}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ 
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_change_event_p ) {
			m_change_event_p = new sc_event; 
			m_change_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ 
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_change_event_p ) {
			m_change_event_p = new sc_event; 
			m_change_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_change_event_p; 
	}

    // get the positive edge event
    virtual const sc_event& posedge_event() const
	{ 
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_posedge_event_p ) {
			m_posedge_event_p = new sc_event; 
			m_posedge_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_posedge_event_p; 
	}

    // get the negative edge event
    virtual const sc_event& negedge_event() const
	{ 
		// BEGIN MODELSIM MODIFICATIONS
	    if ( !m_negedge_event_p ) {
			m_negedge_event_p = new sc_event; 
			m_negedge_event_p->mti_set_is_prim_channel_event(true);
		}
		// END MODELSIM MODIFICATIONS
	    return *m_negedge_event_p; 
	}


    // read the current value
    virtual const sc_dt::sc_logic& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const sc_dt::sc_logic& get_data_ref() const
        { sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta + 1); }

    // was there a positive edge event?
    virtual bool posedge() const
	{ return ( event() && m_cur_val == sc_dt::SC_LOGIC_1 ); }

    // was there a negative edge event?
    virtual bool negedge() const
	{ return ( event() && m_cur_val == sc_dt::SC_LOGIC_0 ); }


    // write the new value
    virtual void write( const sc_dt::sc_logic& );


    // other methods

    operator const sc_dt::sc_logic& () const
	{ return read(); }


    sc_signal<sc_dt::sc_logic>& operator = ( const sc_dt::sc_logic& a )
	{ write( a ); return *this; }

    sc_signal<sc_dt::sc_logic>& operator = ( const sc_signal<sc_dt::sc_logic>& a )
	{ write( a.read() ); return *this; }


    const sc_dt::sc_logic& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
	{
	    sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_trace( tf, read(), name() ); 
#           endif
	}

    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_signal"; }

protected:

    virtual void update();

protected:

    mutable sc_event* m_change_event_p;  // value change event if present.
    sc_dt::sc_logic   m_cur_val;         // current value of object.
    sc_dt::uint64     m_delta;           // delta of last event
    mutable sc_event* m_negedge_event_p; // negative edge event if present.
    sc_dt::sc_logic   m_new_val;         // next value of object.
    sc_port_base*     m_output;          // used for static design rule checking
    mutable sc_event* m_posedge_event_p; // positive edge event if present.
    sc_object*        m_writer;          // process writing this object's value.


private:

    // disabled
    sc_signal( const sc_signal<sc_dt::sc_logic>& );

protected:
	// BEGIN MODELSIM ADDITIONS

    virtual mtiObjectHandleT mti_get_channel_id( sc_object* );

    virtual void* mti_get_channel_value(int is_driving_value);

    virtual void mti_set_channel_value(
        void* value,
        int is_driving_value);

	virtual void mti_get_construction_params(
		int* elem_num_bits,
		int* elem_num_integer_bits,
		int* fifo_num_elem,
		int* hasConstructionParams,
		int* hasElementConstructionParams);

	virtual mtiTypeIdT mti_get_channel_type();

public:
    //
    // Control an HDL signal. The value change of this signal is propagated
    // to the HDL signal, but not vice versa.
    // This function returns true if the foreign object is found and
    // the type is compatible.
    //
    bool control_foreign_signal(const char* hdl_name);

    //
    // Observe an HDL signal. The value change of the HDL signal is progagated
    // to the signal, but not vice versa.
    // This function returns true if the foreign object is found and
    // the type is compatible.
    //
    bool observe_foreign_signal(const char* hdl_name);


private:
	sc_dt::sc_logic m_mti_value;

	// END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_signal<sc_dt::sc_logic>::register_port( sc_port_base& port_,
				    const char* if_typename_ )
{
    if ( sc_get_curr_simcontext()->write_check() )
    {
	std::string nm( if_typename_ );
	if( nm == typeid( sc_signal_inout_if<sc_dt::sc_logic> ).name() ) {
	    // an out or inout port; only one can be connected
	    if( m_output != 0 ) {
		sc_signal_invalid_writer( this, m_output, &port_ );
	    }
	    m_output = &port_;
	}
    }
}


// write the new value

inline
void
sc_signal<sc_dt::sc_logic>::write( const sc_dt::sc_logic& value_ )
{
	// BEGIN MODELSIM MODIFICATIONS
    if ( sc_get_curr_simcontext()->write_check() )
    {
		sc_object* writer = sc_get_curr_simcontext()->get_current_writer();
		if( m_writer == 0 ) {
		m_writer = writer;
		} else if( m_writer != writer ) {
		sc_signal_invalid_writer( this, m_writer, writer );
		}
	}
	// BEGIN MODELSIM MODIFICATIONS

    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


inline
void
sc_signal<sc_dt::sc_logic>::print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

inline
void
sc_signal<sc_dt::sc_logic>::dump( ::std::ostream& os ) const
{
    os << "     name = " << name() << ::std::endl;
    os << "    value = " << m_cur_val << ::std::endl;
    os << "new value = " << m_new_val << ::std::endl;
}


inline
void
sc_signal<sc_dt::sc_logic>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_cur_val = m_new_val;
	if ( m_change_event_p ) m_change_event_p->notify_next_delta();
	if( m_posedge_event_p && (m_cur_val == sc_dt::SC_LOGIC_1) ) {
	    m_posedge_event_p->notify_next_delta();
	} else if( m_negedge_event_p && (m_cur_val == sc_dt::SC_LOGIC_0) ) {
	    m_negedge_event_p->notify_next_delta();
	}
	m_delta = sc_delta_count();
    }
}


// ----------------------------------------------------------------------------

template <class T>
inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_signal<T>& a )
{
    return ( os << a.read() );
}

} // namespace sc_core


// BEGIN MODELSIM ADDITIONS

#include "sc_signal_mti.h"

// END MODELSIM ADDITIONS



#endif

// Taf!
