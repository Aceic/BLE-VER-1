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

  sc_refined_port_impl.h -- interface for refined ports
  		     
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_refined_port_impl.h#1 $

 *****************************************************************************/
#ifndef SC_REFINED_PORT_IMPL_H
#define SC_REFINED_PORT_IMPL_H

#include "sc_master_slave_b.h"
#include "sc_link_mp.h"
#include "sc_indexed_if.h"
#include "sc_self_binding_port.h"
#include "sc_terminal.h"

//---------------------------------------------------------------------
// common base for all refined master/slave ports
template<class T, class IF>
class sc_refined_master_slave_base : 
  public virtual sc_master_slave_b,
  public virtual sc_self_binding_port 
{
protected:
  typedef IF					if_type;
  typedef sc_refined_master_slave_base		this_type;

  void register_terminals( terminals& list, IF* intf ) {
    for ( terminals::iterator it( list ) ; !it.empty( ) ; it++) {
      sc_self_binding_terminal_b* the_terminal = it.contents( );
      sc_terminal_type ttype = the_terminal->get_terminal_type( );
      
      int index = the_terminal->get_index( );
      sc_core::sc_interface* sig = intf->get_signal( index, ttype );

      if ( ! sig ) {
	// if signal is not available then create one and add it to the
	// refined interface
	sig = the_terminal->make_signal( );
	intf->add_signal( sig, ttype, index );
      }
      the_terminal->bind( sig );
    } // for
  }

  void register_attributes( attributes& list, IF* intf ) {
    for ( attributes::iterator it( list ) ; !it.empty( ) ; it++) {
      sc_self_binding_attribute_b* attr = it.contents( );
      
      int index = attr->get_index( );
      void* data = intf->get_attribute_data( index );

      if ( ! data ) {
	// if signal is not available then create one and add it to the
	// refined interface
	data = attr->make_attribute( );
	intf->add_attribute_data( data, index );
      }
      attr->bind( data );
    } // for
  }

  // register the terminals with the link
  void do_registration( IF* refined_link ) {
    register_terminals( data_terminals, refined_link );
    register_terminals( address_terminals, refined_link );
    register_terminals( control_terminals, refined_link );
    register_attributes( attribute_list, refined_link );
  }


  void bind( sc_self_binding_port& lhs, sc_self_binding_port& rhs ) {
    sc_self_binding_port_binder binder( lhs, rhs );
    binder.dummy_func( );  // to avoid the unsed variable warning
  }

public:
  const char*	port_typename( ) { return "sc_refined_master_slave"; }

  virtual void validate_interface( ) {
  }

  virtual const char* get_name( ) { return name( ); }
}; // sc_refined_master_slave_base


//---------------------------------------------------------------------
// common base for all refined master ports
template<class T, class IF>
class sc_refined_master_base : public virtual sc_refined_master_slave_base<T, IF >
{
public:
};

//---------------------------------------------------------------------
// common base for all refined slave ports
template<class T, class IF>
class sc_refined_slave_base : public virtual sc_refined_master_slave_base<T, IF >
{
public:
};


// interface for the refined master/slave ports
//------------------------------------------------------------------
template<class T> 
class sc_refined_master_port_impl 
: public virtual sc_refined_master_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_master_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_master_port_impl<T>		this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }	         					     

  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

template<class T> 
class sc_refined_inmaster_port_impl 
: public virtual sc_refined_master_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_master_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_inmaster_port_impl<T>		this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
    base_type::bind( *this, other );
  }	         					     

  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

template<class T> 
class sc_refined_outmaster_port_impl 
: public virtual sc_refined_master_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_master_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_outmaster_port_impl<T>		this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
    base_type::bind( *this, other );
  }	         					     

  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

template<class T> 
class sc_refined_inoutmaster_port_impl 
: public virtual sc_refined_master_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_master_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_inoutmaster_port_impl<T>		this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
    base_type::bind( *this, other );
  }	         					     

  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

//-------------------------------------------------------------------
// refined slave ports
template<class T> 
class sc_refined_slave_port_impl 
: public virtual sc_refined_slave_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_slave_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_slave_port_impl<T>			this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
    base_type::bind( static_cast<base_type&>( other ) );
  }
  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

template<class T> 
class sc_refined_inslave_port_impl 
: public virtual sc_refined_slave_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_slave_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_inslave_port_impl<T>			this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
    base_type::bind( *this, other );
  }	         					     

  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

template<class T> 
class sc_refined_outslave_port_impl 
: public virtual sc_refined_slave_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_slave_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_outslave_port_impl<T>		this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
    base_type::bind( *this, other );
  }	         					     

  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

template<class T> 
class sc_refined_inoutslave_port_impl 
: public virtual sc_refined_slave_base< T, sc_refined_if<T> >,
  public sc_core::sc_port< sc_refined_if<T> >
{		 
public:
  typedef sc_refined_slave_base<T, sc_refined_if<T> >	base_type;
  typedef sc_refined_inoutslave_port_impl<T>		this_type;
  typedef sc_refined_if<T> if_type;
  
  // bind to interface
  virtual void bind( if_type& link ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
    do_registration( get_ms_interface( ) );
  }
  virtual void operator ( ) ( if_type& link ) 	{ bind( link ); }
  
  // bind to port
  virtual void bind( this_type& other ) {
    sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
    base_type::bind( *this, other );
  }	         					     

  virtual void operator ( ) ( this_type& other ) { bind( other ); }
  
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
};

#endif // SC_REFINED_PORT_IMPL_H
