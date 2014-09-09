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

  sc_attribute.h -- self binding attribute
  
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_attribute.h#1 $

 *****************************************************************************/

#ifndef SC_ATTRIBUTE_H
#define SC_ATTRIBUTE_H

#include "sc_self_binding_attribute_b.h"
  
/////////////////////////////////////////////////////////////////////
template<class T, int N>
class sc_self_binding_attribute : public sc_self_binding_attribute_b
{
public:
  // get the last registered self binding port
  virtual sc_self_binding_port_b* get_registered_self_binding_port( ) {
    return current_port_protocol;
  }
  int		get_index( ) { return index; }
  void*		make_attribute( ) { return new T( ); }

  // constructor
  sc_self_binding_attribute( ) {  
    data = 0;  // initialize
    if ( N<0 ) {
      index = get_registered_self_binding_port( )->get_attribute_index( );
    } else  {
      index = N;
    }
	if (index == -1 ) {
      SC_REPORT_ERROR(SC_ID_MS_NEGATIVE_ATTRIBUTE_INDEX_, msg );
	}
  }

  void bind( void* d ) {
    data = d;
  }

protected:
  void do_write( const T& d ) {
    *static_cast< T* >( data ) = d;
  }

  const T& do_read( ) {
    return *( static_cast< T* >( data ) );
  }

private:
  int		index;
  void*		data;
};

/////////////////////////////////////////////////////////////////////
template<class T, int N=-1>
class sc_attribute_in : public sc_self_binding_attribute< T, N >
{
public:
  sc_attribute_in( ) {
    get_registered_self_binding_port( )->add_attribute_b( this, get_index( ) );
  }

  const T& read( ) { return do_read( ); }
  operator const T& ( ) {  return read( ); }
};

//------------------------------------------------------------------
template<class T, int N=-1>
class sc_attribute_out : public sc_self_binding_attribute< T, N >
{
public:
  sc_attribute_out( ) {
    get_registered_self_binding_port( )->add_attribute_b( this, get_index( ) );
  }

  sc_attribute_out<T,N>& write( const T& data ) { 
    do_write( data ); 
    return *this;
  }
  sc_attribute_out<T,N>& operator = ( const T& data ) {
    return write( data );
  }
};

//------------------------------------------------------------------
template<class T, int N=-1>
class sc_attribute_inout : public sc_self_binding_attribute< T, N >
{
public:
  sc_attribute_inout( ) {
    get_registered_self_binding_port( )->add_attribute_b( this, get_index( ) );
  }

  const T& read( ) { return do_read( ); }
  operator const T& ( ) {  return read( ); }

  sc_attribute_inout<T,N>& write( const T& data ) { 
    do_write( data ); 
    return *this;
  }
  sc_attribute_inout<T,N>& operator = ( const T& data ) {
    return write( data );
  }
};

#endif // SC_ATTRIBUTE_H
