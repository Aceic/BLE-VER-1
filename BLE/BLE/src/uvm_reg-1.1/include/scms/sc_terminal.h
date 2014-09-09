/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_terminal.h -- terminals of master/slave ports.

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_terminal.h#1 $

 *****************************************************************************/
#ifndef SC_TERMINAL_H
#define SC_TERMINAL_H

#include "sc_self_binding_terminal_b.h"

//---------------------------------------------------------------
template<class T, class base, sc_terminal_type TTYPE, int N>
class sc_terminal_b : 
  public sc_self_binding_terminal_b,
  public base
{
protected:
  int			index;
  sc_terminal_type	ttype;
public:
  virtual sc_self_binding_port_b* get_registered_self_binding_port( ) {
    return current_port_protocol;
  }
  
  int			get_index( ) { return index; }
  ::sc_core::sc_interface*	make_signal( ) { return new ::sc_core::sc_signal<T>( ); }
  sc_terminal_type 	get_terminal_type( ) { return ttype; }

  virtual void bind( ::sc_core::sc_interface* signal ) {
    sc_signal<T>* sig = dynamic_cast< sc_signal<T>* > ( signal );

	if (sig == NULL) {
    	SC_REPORT_ERROR(SC_ID_MS_NULL_SIG_, 0);
	}
    base::bind( *sig );
  }

  virtual void bind( sc_self_binding_terminal_b* other ) {
    // for the time being we need to assume that the other port
    // has the same dynamic type as this
    base* other_b = dynamic_cast< base* > ( other );
    base::bind( *other_b );
  }

  sc_terminal_b( ) {  // constructor
    ttype = TTYPE;
    if ( N<0 ) {
      switch ( ttype ) {
      case TT_DATA:
	index = get_registered_self_binding_port( )->get_data_index( );
	break;

      case TT_ADDRESS:
	index = get_registered_self_binding_port( )->get_address_index( );
	break;

      case TT_CONTROL:
	index = get_registered_self_binding_port( )->get_control_index( );
	break;
      } // switch
    } else {
      index = N;
    }
	if (index == -1) {
    	SC_REPORT_ERROR(SC_ID_MS_NEGATIVE_TERMINAL_INDEX_, 0);
	}
  }
}; // sc_terminal_b

//---------------------------------------------------------------
template<class T, sc_terminal_type ttype=TT_DATA, int N=-1>
class sc_terminal_in : public sc_terminal_b<T, sc_in<T>, ttype, N>
{
public:
  sc_terminal_in( ) {
    this->get_registered_self_binding_port( )->add_terminal_b( this, this->get_index( ) );
  }

};

template<class T, sc_terminal_type ttype=TT_DATA, int N=-1>
class sc_terminal_out : public sc_terminal_b<T, sc_out<T>, ttype, N>
{
public:
  typedef sc_terminal_out this_type;

  sc_terminal_out( ) {
    this->get_registered_self_binding_port( )->add_terminal_b( this, this->get_index( ) );
  }
  
  void write(const T& d) { sc_out<T>::write(d); }

  this_type& operator=(const T& d) {
    write( d );
    return *this;
  }
  
  this_type& operator=(sc_signal<T>& sig) {
    write( sig.read( ) );
    return *this;
  }
};

template<class T, sc_terminal_type ttype=TT_DATA, int N=-1>
class sc_terminal_inout : public sc_terminal_b<T, sc_inout<T>, ttype, N>
{
public:
  typedef sc_terminal_inout this_type;
  
  sc_terminal_inout( ) {
    this->get_registered_self_binding_port( )->add_terminal_b( this, this->get_index( ) );
  }
  this_type& write(const T& d) { 
    sc_inout<T>::write(d); 
    return *this;
  }

  this_type& operator=(const T& d)		{ return write( d ); }
  this_type& operator=(sc_signal<T>& sig)	{ return write( sig.read( ) ); }
  this_type& operator=(sc_inout<T>& port)	{ return write( port.read( ) ); }
};

#endif // SC_TERMINAL_H
