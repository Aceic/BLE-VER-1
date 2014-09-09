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

  sc_indexed_port_if.h -- interface for indexed master/slave ports.

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_indexed_port_impl.h#1 $

 *****************************************************************************/
#ifndef SC_INDEXED_PORT_IF_H
#define SC_INDEXED_PORT_IF_H
  
#include "sc_report.h"
#include "sc_ms_ids.h"
#include "sc_abstract_port_impl.h"
#include "sc_indexed_if.h"

// forward declarations
template<class T, uint64 N> class sc_indexed_master_port_impl;
template<class T, uint64 N> class sc_indexed_inmaster_port_impl;
template<class T, uint64 N> class sc_indexed_outmaster_port_impl;
template<class T, uint64 N> class sc_indexed_inoutmaster_port_impl;

template<class T, uint64 N> class sc_indexed_slave_port_impl;
template<class T, uint64 N> class sc_indexed_inslave_port_impl;
template<class T, uint64 N> class sc_indexed_outslave_port_impl;
template<class T, uint64 N> class sc_indexed_inoutslave_port_impl;

//---------------------------------------------------------------------
// common base for all indexed master ports
template<uint64 N >
class sc_indexed_master_base : public virtual sc_abstract_master_base
{
public:
  const char*	port_typename( ) { return "sc_indexed_master_slave"; }
  virtual uint64 get_address( ) = 0;
  virtual void set_address( uint64 i ) = 0;

protected:
  virtual const char* get_name( ) = 0;
  void check_address( uint64 i ) {
    if ( i >= N ) {
      char msg[BUFSIZ];
      sprintf( msg, "`%s\'", get_name( ) );
      SC_REPORT_WARNING(SC_ID_MS_INDEX_OUT_OF_RANGE_, msg );
    }
  }

};

//---------------------------------------------------------------------
// common base for all indexed slave ports
template<uint64 N>
class sc_indexed_slave_base : public virtual sc_abstract_slave_base
{
public:
  virtual ~sc_indexed_slave_base( ) { }
  uint64 get_address( ) { 
    // return get_ms_interface( )->get_address( );
  	return N;
  }
  const char*	port_typename( ) { return "sc_indexed_master_slave"; }
protected:
  virtual const char* get_name( ) = 0;
  void check_address( uint64 i ) {
    if ( i >= N ) {
      char msg[BUFSIZ];
      sprintf( msg, "index out of range: port `%s\'", get_name( ) );
      SC_REPORT_WARNING(SC_ID_MS_INDEX_OUT_OF_RANGE_, msg );
    }
  }

};

////////////////////////////////////////////////////////////////////
// master port interface
//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_master_port_impl 
: public virtual sc_indexed_master_base<N>,
  public virtual sc_core::sc_port< sc_indexed_master_if<T> > 
{
public:
  typedef sc_indexed_master_base<N> base_type;
  typedef sc_indexed_master_port_impl<T,N>	this_type;
  typedef sc_indexed_master_if<T> if_type;

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
    get_ms_interface( )->operator( ) ( ); 
  }

  this_type& operator [ ] (uint64 i ) {  // used by indexed ports
    set_address( i );
    return *this;
  }

public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator -> ( );
  }

protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};


//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_inmaster_port_impl 
: public virtual sc_indexed_master_base<N>,
  public virtual sc_master_slave_read_if< T >,
  public virtual sc_core::sc_port< sc_indexed_inmaster_if<T> >
{
public:
  typedef sc_indexed_master_base<N > base_type;
  typedef sc_indexed_inmaster_port_impl<T,N>		this_type;
  typedef sc_indexed_inmaster_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_indexed_inoutmaster_port_impl<T,N>& other) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )		{ bind( other); }
  void operator( ) ( sc_indexed_inoutmaster_port_impl<T,N>& other) { bind( other ); }
  
  const T& read( ) { 
    if_type* link = get_ms_interface( );
    return link->read_inmaster( );
  }
  
  operator const T& ( ) {
      return read( );
  };
  
  this_type& operator [ ] (uint64 i ) {  // used by indexed ports
    set_address( i );
    return *this;
  }


public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};

//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_outmaster_port_impl 
: public virtual sc_indexed_master_base<N >,
  public virtual sc_master_slave_write_if< T >,
  public virtual sc_core::sc_port< sc_indexed_outmaster_if<T> >
{
public:
  typedef sc_indexed_master_base<N > base_type;
  typedef sc_indexed_outmaster_port_impl<T,N>		this_type;
  typedef sc_indexed_outmaster_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_indexed_inoutmaster_port_impl<T,N>& other) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )		{ bind( other); }
  void operator( ) ( sc_indexed_inoutmaster_port_impl<T,N>& other) { bind( other ); }

  
  void write( const T& data )	{ 
    if_type* link = get_ms_interface( );
    link->write_outmaster( data );
  }

  this_type& operator [ ] ( uint64 i ) {  // used by indexed ports
    set_address( i );
    return *this;
  }
  
  this_type& operator = ( const T& data ) {
    write( data );
    return *this;
  }


public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};

//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_inoutmaster_port_impl 
: public virtual sc_indexed_master_base<N >,
  public virtual sc_master_slave_read_write_if< T >,
  public virtual sc_core::sc_port< sc_indexed_inoutmaster_if<T> >
{
public:
  typedef sc_indexed_master_base<N > base_type;
  typedef sc_indexed_inoutmaster_port_impl<T,N>		this_type;
  typedef sc_indexed_inoutmaster_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  
  
  const T& read( ) { 
    if_type* link = get_ms_interface( );
    return link->read_inoutmaster( );
  }

  void write( const T& data )	{ 
    if_type* link = get_ms_interface( );
    link->write_inoutmaster( data );
  }

  operator const T& ( ) {
      return read( );
  };
  
  this_type& operator [ ] (uint64 i ) {  // used by indexed ports
    set_address( i );
    return *this;
  }
  
  this_type& operator = ( const T& data ) {
    write( data );
    return *this;
  }


public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};


////////////////////////////////////////////////////////////////////
// slave port interface
//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_slave_port_impl 
: public virtual sc_indexed_slave_base<N >,
  public virtual sc_core::sc_port< sc_indexed_slave_if<T> >
{
public:
  typedef sc_indexed_slave_port_impl<T,N>		this_type;
  typedef sc_indexed_slave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  
  virtual void end_of_elaboration( ) {
    if_type* alink = get_ms_interface( );
    alink->add_slave_proc ( this->m_slave_procs );
  }


public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};


//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_inslave_port_impl : 
  public virtual sc_indexed_slave_base<N >,
  public virtual sc_master_slave_read_if< T >,
  public virtual sc_core::sc_port< sc_indexed_inslave_if<T> >
{
public:
  typedef sc_indexed_inslave_port_impl<T,N>		this_type;
  typedef sc_indexed_inslave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_indexed_inoutslave_port_impl<T,N>& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  void operator( ) ( sc_indexed_outslave_port_impl<T,N>& other ) { bind( other ); }
  
  
  virtual void end_of_elaboration( ) {
    if_type* alink  = get_ms_interface(  );
    alink->add_inslave_proc ( this->m_slave_procs );
  }
  const T& read( ) {
    if_type* link = get_ms_interface( );
    return link->read( );
  }
  operator const T& ( )		{ return read( ); };


public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};

//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_outslave_port_impl 
: public sc_indexed_slave_base<N >,
  public virtual sc_master_slave_write_if< T >,
  public virtual sc_core::sc_port< sc_indexed_outslave_if<T> >
{
public:
  typedef sc_indexed_outslave_port_impl<T,N>		this_type;
  typedef sc_indexed_outslave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  virtual void bind( sc_indexed_inoutslave_port_impl<T,N>& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  void operator( ) ( sc_indexed_inoutslave_port_impl<T,N>& other ) { bind( other ); }

  virtual void end_of_elaboration( ) {
    if_type* alink  = get_ms_interface( );
    alink->add_outslave_proc ( this->m_slave_procs );
  }
  void write( const T& data )	{
    if_type* link = get_ms_interface( );
    link->write( data );
  }


public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};

//------------------------------------------------------------------
template<class T, uint64 N> 
class sc_indexed_inoutslave_port_impl 
: public virtual sc_indexed_slave_base<N >,
  public virtual sc_master_slave_read_write_if< T >,
  public virtual sc_core::sc_port< sc_indexed_inoutslave_if<T> >
{
public:
  typedef sc_indexed_inoutslave_port_impl<T,N>		this_type;
  typedef sc_indexed_inoutslave_if<T> if_type;
  
  void bind( if_type& link ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_interface&>( link ) );
  }
  void bind( this_type& other ) {
	  sc_core::sc_port_base::bind( static_cast<sc_core::sc_port_base&>( other ) );
  }
  
  void operator( ) ( if_type& link )		{ bind( link); }
  void operator( ) ( this_type& other )	{ bind( other); }
  
  virtual void end_of_elaboration( ) {
    if_type* alink  = get_ms_interface( );
    alink->add_inoutslave_proc( this->m_slave_procs );
  }
  const T& read( ) {
    if_type* link = get_ms_interface( );
    return link->read( );
  }
  void write( const T& data )	{
    if_type* link = get_ms_interface( );
    link->write( data );
  }
  operator const T& ( )		{ return read( ); };
  
  bool input( ) 		{ return get_ms_interface( )->input( ); }


public:
  uint64 get_address( ) { 
    return get_ms_interface( )->get_address( );
  }
  virtual void set_address( uint64 i ) {
    this->check_address( i );
    get_ms_interface( )->set_address( i );
  }

public:
  if_type* get_ms_interface( ) {
    return (*this).operator->( );
  }
protected:
  virtual const char* get_name( ) {
    return this->name( );
  }
};


#endif  // SC_INDEXED_IF_H
