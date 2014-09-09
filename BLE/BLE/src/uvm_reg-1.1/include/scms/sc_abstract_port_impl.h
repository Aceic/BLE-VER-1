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

  sc_abstract_port_impl.h -- implementation for abstract master/slave ports.

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_abstract_port_impl.h#1 $

 *****************************************************************************/
#ifndef SC_ABSTRACT_PORT_IMPL_H
#define SC_ABSTRACT_PORT_IMPL_H
  
#include "sc_master_slave_b.h"
#include "sc_link_mp.h"
#include "sc_abstract_if.h"


// forward declarations
template<class T> class sc_abstract_master_port_impl;
template<class T> class sc_abstract_inmaster_port_impl;
template<class T> class sc_abstract_outmaster_port_impl;
template<class T> class sc_abstract_inoutmaster_port_impl;

template<class T> class sc_abstract_slave_port_impl;
template<class T> class sc_abstract_inslave_port_impl;
template<class T> class sc_abstract_outslave_port_impl;
template<class T> class sc_abstract_inoutslave_port_impl;

//---------------------------------------------------------------------
// common base for all abstract master/slave ports
class sc_abstract_master_slave_base : public virtual sc_master_slave_b
{
public:
  virtual const char* port_typename( ) { return "sc_abstract_master_slave"; }
};


//---------------------------------------------------------------------
// common base for all abstract master ports
class sc_abstract_master_base : public virtual sc_abstract_master_slave_base
{
public:
};

//---------------------------------------------------------------------
// common base for all abstract slave ports
class sc_abstract_slave_base : public virtual sc_abstract_master_slave_base
{
public:
  void		make_sensitive_slave( sc_slave_handle handle ) {
    m_slave_procs.push_back( handle );
  }
protected:
  sc_slave_proc_list	m_slave_procs;
};

////////////////////////////////////////////////////////////////////
// master port interface
//------------------------------------------------------------------
template<class T> 
class sc_abstract_master_port_impl
: public virtual sc_abstract_master_base,
  public virtual sc_core::sc_port< sc_abstract_master_if<T> >
{
public:
  typedef sc_abstract_master_port_impl<T>		this_type;
  typedef sc_abstract_master_if<T>		if_type;

  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )		{ bind( other); }
  
  // run the slaves
  void operator ( ) ( ) { 
    // get_ms_interface( )->run_slaves( ); 
    if_type* link = get_ms_interface( );
    link->operator( ) ( ); 
  }
  
// helper routines to get to the interface quickly
public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};

//------------------------------------------------------------------
template<class T> 
class sc_abstract_inmaster_port_impl
: public virtual sc_abstract_master_base,
  public virtual sc_master_slave_read_if< T >,
  public virtual sc_core::sc_port< sc_abstract_inmaster_if<T> >
{
public:
  typedef sc_abstract_inmaster_port_impl<T>		this_type;
  typedef sc_abstract_inmaster_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_abstract_inoutmaster_port_impl<T>& other) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )	{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  void operator( ) ( sc_abstract_inoutmaster_port_impl<T>& other) { bind( other ); }
  
  const T& read( ) { 
    if_type* link = get_ms_interface( );
    return link->read_inmaster( );
  }

// helper routines to get to the interface quickly
public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};

//------------------------------------------------------------------
template<class T> 
class sc_abstract_outmaster_port_impl 
: public virtual sc_abstract_master_base,
  public virtual sc_master_slave_write_if< T >,
  public virtual sc_core::sc_port< sc_abstract_outmaster_if<T> > 
{
public:
  typedef sc_abstract_outmaster_port_impl<T>		this_type;
  typedef sc_abstract_outmaster_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_abstract_inoutmaster_port_impl<T>& other) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )		{ bind( other); }
  void operator( ) ( sc_abstract_inoutmaster_port_impl<T>& other) { bind( other ); }

  
  void write( const T& data )	{ 
    if_type* link = get_ms_interface( );
    link->write_outmaster( data );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};

//------------------------------------------------------------------
template<class T> 
class sc_abstract_inoutmaster_port_impl 
: public virtual sc_abstract_master_base,
  public virtual sc_master_slave_read_write_if< T >,
  public virtual sc_core::sc_port< sc_abstract_inoutmaster_if<T> >
{
public:
  typedef sc_abstract_inoutmaster_port_impl<T>		this_type;
  typedef sc_abstract_inoutmaster_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  void operator( ) ( if_type& link )	{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  
  
  const T& read( ) { 
    if_type* link = get_ms_interface( );
    return link->read_inoutmaster( );
  }

  void write( const T& data )	{ 
    if_type* link = get_ms_interface( );
    link->write_inoutmaster( data );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};


////////////////////////////////////////////////////////////////////
// slave port interface
//------------------------------------------------------------------
template<class T> 
class sc_abstract_slave_port_impl : 
  public virtual sc_abstract_slave_base,
  public virtual sc_core::sc_port< sc_abstract_slave_if<T> >
{
public:
  typedef sc_abstract_slave_port_impl<T>		this_type;
  typedef sc_abstract_slave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  void operator( ) ( if_type& link )	{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  
  virtual void end_of_elaboration( ) {
    if_type* alink = get_ms_interface( );
    alink->add_slave_proc ( m_slave_procs );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};

//------------------------------------------------------------------
template<class T> class sc_abstract_inslave_port_impl : 
  public virtual sc_abstract_slave_base,
  public virtual sc_master_slave_read_if< T >,
  public virtual sc_core::sc_port< sc_abstract_inslave_if<T> >
{
public:
  typedef sc_abstract_inslave_port_impl<T>		this_type;
  typedef sc_abstract_inslave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_abstract_inoutslave_port_impl<T>& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )	{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  void operator( ) ( sc_abstract_outslave_port_impl<T>& other ) { bind( other ); }
  
  
  virtual void end_of_elaboration( ) {
    if_type* alink  = get_ms_interface(  );
    alink->add_inslave_proc ( m_slave_procs );
  }
  const T& read( ) {
    if_type* link = get_ms_interface( );
    return link->read( );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};

//------------------------------------------------------------------
template<class T> class sc_abstract_outslave_port_impl : 
  public sc_abstract_slave_base,
  public virtual sc_master_slave_write_if< T >,
  public virtual sc_core::sc_port< sc_abstract_outslave_if<T> > 
{
public:
  typedef sc_abstract_outslave_port_impl<T>		this_type;
  typedef sc_abstract_outslave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_abstract_inoutslave_port_impl<T>& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )	{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  void operator( ) ( sc_abstract_inoutslave_port_impl<T>& other ) { bind( other ); }

  virtual void end_of_elaboration( ) {
    if_type* alink  = get_ms_interface( );
    alink->add_outslave_proc ( m_slave_procs );
  }
  void write( const T& data ) {
    if_type* link = get_ms_interface( );
    link->write( data );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};

//------------------------------------------------------------------
template<class T> class sc_abstract_inoutslave_port_impl : 
  public virtual sc_abstract_slave_base,
  public virtual sc_master_slave_read_write_if< T >,
  public virtual sc_core::sc_port<sc_abstract_inoutslave_if<T> > 
{
public:
  typedef sc_abstract_inoutslave_port_impl<T>		this_type;
  typedef sc_abstract_inoutslave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )		{ bind( link);  }
  void operator( ) ( this_type& other )		{ bind( other); }
  
  virtual void end_of_elaboration( ) {
    if_type* alink  = get_ms_interface( );
    alink->add_inoutslave_proc ( m_slave_procs );
  }
  const T& read( ) {
    if_type* link = get_ms_interface( );
    return link->read( );
  }
  void write( const T& data )	{
    if_type* link = get_ms_interface( );
    link->write( data );
  }
  bool input( )		{ return get_ms_interface( )->input( ); }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
  virtual const char* get_name( ) { return this->name( ); }
};

#endif  // SC_ABSTRACT_IMPL_H
