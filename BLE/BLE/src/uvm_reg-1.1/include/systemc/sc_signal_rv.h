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

  sc_signal_rv.h -- The resolved vector signal class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_signal_rv.h#1 $

 *****************************************************************************/

#ifndef SC_SIGNAL_RV_H
#define SC_SIGNAL_RV_H

#include "sc_signal.h"
#include "sc_lv.h"

// BEGIN MODELSIM ADDITIONS
#include "ptrarray.h"
// END MODELSIM ADDITIONS

namespace sc_core {

class sc_process_b;


// ----------------------------------------------------------------------------
//  CLASS sc_lv_resolve<W>
//
//  Resolution function for sc_dt::sc_lv<W>.
// ----------------------------------------------------------------------------

extern const sc_dt::sc_logic_value_t sc_logic_resolution_tbl[4][4];


template <int W>
class sc_lv_resolve
{
public:

    // resolves sc_dt::sc_lv<W> values and returns the resolved value
    static void resolve( sc_dt::sc_lv<W>&, const ptrarray<sc_dt::sc_lv<W>*>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// resolves sc_dt::sc_lv<W> values and returns the resolved value

template <int W>
inline
void
sc_lv_resolve<W>::resolve( sc_dt::sc_lv<W>& result_,
			   const ptrarray<sc_dt::sc_lv<W>*>& values_ )
{
    int sz = values_.size();

	// START MODELSIM MODIFICATIONS

	//
	// A SystemC signal might be updated once at initialization
	// time to propagate values to the HDL.  Only signals at the
	// top of an input chain or the bottom of an output chain are
	// activated.  Return silently when there is nothing to resolve.
	//
	if (sz == 0)
		return;

	// END MODELSIM MODIFICATIONS

    if( sz == 1 ) {
	result_ = *values_[0];
	return;
    }

    for( int j = result_.length() - 1; j >= 0; -- j ) {
	sc_dt::sc_logic_value_t res = (*values_[0])[j].value();
	for( int i = sz - 1; i > 0 && res != 3; -- i ) {
	    res = sc_logic_resolution_tbl[res][(*values_[i])[j].value()];
	}
	result_[j] = res;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal_rv<W>
//
//  The resolved vector signal class.
// ----------------------------------------------------------------------------

template <int W>
class sc_signal_rv
: public sc_signal<sc_dt::sc_lv<W> >
{
public:

    // typedefs

    typedef sc_signal_rv<W>      this_type;
    typedef sc_signal<sc_dt::sc_lv<W> > base_type;
    typedef sc_dt::sc_lv<W>             data_type;

public:

    // constructors

    sc_signal_rv()
        : base_type( sc_gen_unique_name( "signal_rv" ) )
	{
		// BEGIN MODELSIM ADDITIONS
		m_mti_value_pos = -1;
		this->m_mti_is_resolved = true;
		sc_object::mti_set_use_default_name(true);
		// END MODELSIM ADDITIONS
	}

    explicit sc_signal_rv( const char* name_ )
        : base_type( name_ )
	{
		// BEGIN MODELSIM ADDITIONS
		m_mti_value_pos = -1;
		this->m_mti_is_resolved = true;
		// END MODELSIM ADDITIONS
	}


    // destructor
    virtual ~sc_signal_rv();


    // interface methods

    virtual void register_port( sc_port_base&, const char* )
	{}


    // write the new value
    virtual void write( const data_type& );


    // other methods

    this_type& operator = ( const data_type& a )
        { write( a ); return *this; }

    this_type& operator = ( const this_type& a )
        { write( a.read() ); return *this; }

    virtual const char* kind() const
        { return "sc_signal_rv"; }

protected:

    virtual void update();

protected:

    ptrarray<sc_process_b*> m_proc_vec; // processes writing to this signal
    ptrarray<data_type*>    m_val_vec;  // new values written to this signal

private:

    // disabled
    sc_signal_rv( const this_type& );

	// BEGIN MODELSIM ADDITIONS

protected:

	virtual void* mti_get_channel_value(int is_driving_value);

	virtual void mti_set_channel_value(
		void* value,
		int is_driving_value);

private:

	int                       m_mti_value_pos;

	// END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// destructor

template <int W>
inline
sc_signal_rv<W>::~sc_signal_rv()
{
    for( int i = m_val_vec.size() - 1; i >= 0; -- i ) {
	delete m_val_vec[i];
    }
}


// write the new value

template <int W>
inline
void
sc_signal_rv<W>::write( const data_type& value_ )
{
    sc_process_b* cur_proc = sc_get_current_process_b();

    bool value_changed = false;
    bool found = false;
    
    for( int i = m_proc_vec.size() - 1; i >= 0; -- i ) {
	if( cur_proc == m_proc_vec[i] ) {

		// BEGIN MODELSIM ADDITIONS
		
		if (m_mti_value_pos != -1 && i >= m_mti_value_pos)
			i++;

		// END MODELSIM ADDITIONS
		
	    if( value_ != *m_val_vec[i] ) {
		*m_val_vec[i] = value_;
		value_changed = true;
	    }
	    found = true;
	    break;
	}
    }
    
    if( ! found ) {
	m_proc_vec.push_back( cur_proc );
	m_val_vec.push_back( new data_type( value_ ) );
	value_changed = true;
    }
    
    if( value_changed ) {
	
	this->request_update();
    }
}

// START MODELSIM MODIFICATIONS

template <int W>
inline
void
sc_signal_rv<W>::update()
{
	if (!this->m_mti_is_new_val_resolution_done)
		sc_lv_resolve<W>::resolve( this->m_new_val, m_val_vec );

    base_type::update();
}


template <int W>
inline
void*
sc_signal_rv<W>::mti_get_channel_value(int is_driving_value)
{
	if (is_driving_value && !this->m_mti_is_new_val_resolution_done) {
		sc_lv_resolve<W>::resolve( this->m_new_val, m_val_vec );
		this->m_mti_is_new_val_resolution_done = true;
	}

	return base_type::mti_get_channel_value(is_driving_value);
}


template <int W>
inline
void
sc_signal_rv<W>::mti_set_channel_value(
	void* value,
	int is_driving_value)
{
	unsigned char* array_value = (unsigned char*)value;
	data_type sc_value;

	for (int i = W; i > 0 ; i--)
		sc_value[i-1] = (int)array_value[W-i];

	if (is_driving_value) {
		if (mti_access || !this->m_mti_is_new_val_resolution_done) {
			//
			// Resolve the driving value for an inout port.
			// The resolution should be done only once. Its
			// an error to call this function twice without
			// a call to ::update(). In case of EVCD, this may 
			// be called multiple times.
			//
			if (m_mti_value_pos == -1) {
				m_val_vec.push_back( new data_type( sc_value ) );
				m_mti_value_pos = m_val_vec.size() - 1;
			} else {
				*m_val_vec[m_mti_value_pos] = sc_value;
			}

			sc_lv_resolve<W>::resolve( this->m_new_val, m_val_vec );
			this->m_mti_is_new_val_resolution_done = true;
		} else {
			SC_REPORT_ERROR(MSG_IDX_SC_SIG_WRONG_SEMANTICS, sc_object::name());
		}
	} else {
		//
		// Fanout from HDL for an input or an inout port connected
		// to this channel.  For an inout port there is no need
		// to resolve the value.  The value was resolved while
		// going up the inout chain.
		//
		this->m_mti_is_new_val_resolution_done = true;
		base_type::mti_set_channel_value(value, is_driving_value);
	}
}

// END MODELSIM MODIFICATIONS

} // namespace sc_core

#endif

// Taf!
