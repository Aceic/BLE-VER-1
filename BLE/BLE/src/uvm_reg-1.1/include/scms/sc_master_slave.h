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

  sc_master_slave.h -- Implementation of Master/Slave ports
  
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_master_slave.h#1 $

 *****************************************************************************/
#ifndef SC_MASTER_SLAVE_H
#define SC_MASTER_SLAVE_H
  					  
#include "sc_report.h"
#include "sc_refined.h"
#include "sc_abstract.h"
#include "sc_indexed.h"

//----------------------------------------------------------------
// definition of the master/slave ports

template< class T=int, class PROTOCOL = sc_abstract<T> >
class sc_master : public PROTOCOL::sc_master_protocol 
{
public:
  typedef typename PROTOCOL::sc_master_protocol	protocol;
  typedef sc_master< T, PROTOCOL >			this_type;
  // constructor 
  sc_master( const char * nm =0 ) : protocol( )
  { }
  void operator( ) ( typename protocol::if_type& link )	{ protocol::bind( link ); }
  void operator( ) ( ) 				{ protocol::operator( ) ( ); }
  void operator( ) ( this_type& other )		{ protocol::bind( other ); }
};

//--------------------------------------------------------------------
template< class T=int, class PROTOCOL = sc_abstract<T> >
class sc_inmaster : public PROTOCOL::sc_inm_protocol
{
public:
  typedef typename PROTOCOL::sc_inm_protocol	protocol;
  typedef sc_inmaster< T, PROTOCOL >		this_type;
  // constructor 
  sc_inmaster( const char * nm =0 ) : protocol( )
  { }
  const T& read( ) 		{ return protocol::read( ); }
  operator const T& ( ) 	{ return this->read( ); }
};

//--------------------------------------------------------------------
template< class T=int, class PROTOCOL = sc_abstract<T> >
class sc_outmaster : public PROTOCOL::sc_outm_protocol
{
public:
  typedef typename PROTOCOL::sc_outm_protocol	protocol;
  typedef sc_outmaster< T, PROTOCOL >		this_type;
  // constructor 
  sc_outmaster( const char * nm =0 ) : protocol( )
  { }
  void write( const T& data )	{ protocol::write( data ); }

  this_type& operator = ( const T& data ) {
    this->write( data );
    return *this;
  }
};

//--------------------------------------------------------------------
template< class T=int, class PROTOCOL = sc_abstract<T> >
class sc_inoutmaster : public PROTOCOL::sc_inoutm_protocol
{
public:
  typedef typename PROTOCOL::sc_inoutm_protocol protocol;
  typedef sc_inoutmaster< T, PROTOCOL >		this_type;
  // constructor 
  sc_inoutmaster( const char * nm =0 ) : protocol( )
  { }
  const T& read( ) 		{ return protocol::read( ); }
  operator const T& ( ) 	{ return read( ); }
  void write( const T& data )	{ protocol::write( data ); }
  
  this_type& operator = ( const T& data ) {
    write( data );
    return *this;
  }
};

///////////////////////////////////////////////////////////////////////
// definition of the slave ports
template < class T=int, class PROTOCOL = sc_abstract<T> >
class sc_slave: public virtual PROTOCOL::sc_slave_protocol
{
public:
  typedef typename PROTOCOL::sc_slave_protocol	protocol;
  typedef sc_slave< T, PROTOCOL >			this_type;
  // constructor 
  sc_slave( const char * nm =0 ) : protocol( )
  { }
};

//--------------------------------------------------------------------
template< class T=int, class PROTOCOL = sc_abstract<T> >
class sc_inslave : public PROTOCOL::sc_ins_protocol
{
public:
  typedef typename PROTOCOL::sc_ins_protocol	protocol;
  typedef sc_inslave< T, PROTOCOL >		this_type;
  // constructor 
  sc_inslave( const char * nm =0 ) : protocol( )
  { }
  const T& read( ) 		{ return protocol::read( ); }
  operator const T& ( ) 	{ return read( ); }
};

//--------------------------------------------------------------------
template< class T=int, class PROTOCOL = sc_abstract<T> >
class sc_outslave : public PROTOCOL::sc_outs_protocol
{
public:
  typedef typename PROTOCOL::sc_outs_protocol	protocol;
  typedef sc_outslave< T, PROTOCOL >		this_type;
  // constructor 
  sc_outslave( const char * nm =0 ) : protocol( )
  { }
  void write(const T& data)	{ protocol::write( data ); }
  
  this_type& operator = ( const T& data ) {
    write( data );
    return *this;
  }
};

//--------------------------------------------------------------------
template< class T=int, class PROTOCOL = sc_abstract<T> >
class sc_inoutslave : public PROTOCOL::sc_inouts_protocol
{
public:
  typedef typename PROTOCOL::sc_inouts_protocol	protocol;
  typedef sc_inoutslave< T, PROTOCOL >		this_type;
  // constructor 
  sc_inoutslave( const char * nm =0 ) : protocol( )
  { }
  const T& read( ) 		{ return protocol::read( ); }
  operator const T& ( ) 	{ return read( ); }
  void write(const T& data)	{ protocol::write( data ); }
  
  this_type& operator = ( const T& data ) {
    write( data );
    return *this;
  }
  bool input( ) { return protocol::input( ); }
};


//--------------------------------------------------------------------
const char*
get_ms_lib_version( );

#endif

