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

  sc_link_mp.h -- implementation of multi-point link

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_link_mp.h#1 $

 *****************************************************************************/
#ifndef SC_LINK_MP_H
#define SC_LINK_MP_H


#include "sc_report.h"
#include "sc_string.h"
#include "sc_abstract_if.h"
#include "sc_indexed_if.h"
#include "sc_refined_if.h"
#include "sc_ms_ids.h"

class sc_link_mp_b : public virtual sc_link_mp_if 
{
public:
};
//----------------------------------------------------------------
template<class T = int>
class sc_link_mp :
  public virtual sc_link_mp_b,
  public virtual sc_abstract_master_if<T>,
  public virtual sc_abstract_inmaster_if<T>,
  public virtual sc_abstract_outmaster_if<T>,
  public virtual sc_abstract_inoutmaster_if<T>,
  public virtual sc_abstract_slave_if<T>,
  public virtual sc_abstract_inslave_if<T>,
  public virtual sc_abstract_outslave_if<T>,
  public virtual sc_abstract_inoutslave_if<T>,

  public virtual sc_indexed_master_if<T>,
  public virtual sc_indexed_inmaster_if<T>,
  public virtual sc_indexed_outmaster_if<T>,
  public virtual sc_indexed_inoutmaster_if<T>,
  public virtual sc_indexed_slave_if<T>,
  public virtual sc_indexed_inslave_if<T>,
  public virtual sc_indexed_outslave_if<T>,
  public virtual sc_indexed_inoutslave_if<T>,

  public virtual sc_refined_if<T>,
  public virtual sc_prim_channel
{
public:
  sc_link_mp( const char* nm = 0 ) : 
    sc_prim_channel( sc_gen_unique_name( nm ? nm : "link" ) ),
    validated_( false )
  { 
    set_input( false );
  }
  
  void		run( sc_slave_proc_list& list) {
    int size = list.size( );
    sc_slave_handle* l_slaves_static = list.raw_data( );
    sc_slave_handle h;

    for(int i = 0; i < size ; i++) {
      h = l_slaves_static[i];
      execute_slave_process( h );
    }
  }

public:
  // abstract interface implementation

  void		run_slaves( ) 		{  run( slave_procs ); }
  void		run_inslaves( ) 	{  run( inslave_procs ); }
  void		run_outslaves( )        {  run( outslave_procs ); }
  void		run_inoutslaves( )      {  run( inoutslave_procs ); }


  virtual void register_port( sc_port_base& port, const char* port_name);
  
  bool is_validated( ) 	{ return validated_; }
  void validate( );
  void add_port( sc_master_slave_b* port ); // { port_list_.push_back( port ); }

  void	add_slave_proc( sc_slave_proc_list& l );
  void	add_inslave_proc( sc_slave_proc_list& l );
  void	add_outslave_proc( sc_slave_proc_list& l );
  void	add_inoutslave_proc( sc_slave_proc_list& l );

  virtual void end_of_elaboration( ) {
      validate( );
  }

private:
  const T& 	read( ) { return value; }
  void		write( const T& data ) {
    if ( write_count > 0 ) {
        char msg[BUFSIZ];
        const char* link_name = sc_object::name( );
        sprintf( msg, ": %s", link_name );
        SC_REPORT_ERROR(SC_ID_MS_MULTIPLE_WRITE_TO_LINK_, msg );
    }
    write_count++;
    value = data;
  }
  void		free_write( const T& data ) { value = data; } 

private:
  void	add_to_list( sc_slave_proc_list& , sc_slave_proc_list& );
  bool	can_bind( sc_master_slave_b*, sc_master_slave_b *);

private:
  bool		get_input( )		{ return is_input; }
  void 		set_input( bool b )	{ is_input = b; }

private:
  T 	value;
  int	write_count;	// for error checking, only one slave can write
  bool	is_input;	// are we currently reading or writing
			// inout slaves can use this information to
			// respond in the read or write mode.

// indexed port implementation
  virtual void		set_address( uint64 i )	{ last_address = i; }
  virtual uint64	get_address( ) const	{ return last_address; }
protected:
  uint64	last_address;

private:
  // implementation of abstract M/S read write functions
  virtual void operator ( ) ( ) {  run_slaves( ); }  
  virtual const T& read_inmaster( ) 
  {
    set_input( false );
    reset_write_count( );
    run_outslaves( );
    run_inoutslaves( );
    return read( );
  }
  
  virtual void write_outmaster(const T& data )
  {
    set_input( true );
    free_write( data );
    run_inslaves( );
    run_inoutslaves( );
  } 

  virtual void write_inoutmaster(const T& data ) 
  {
    set_input( true );
    free_write( data );
    run_inslaves( );
    run_inoutslaves( );
  }
  
  
  virtual const T& read_inoutmaster( )  
  {
    reset_write_count( );
    set_input( false );
    run_outslaves( );
    run_inoutslaves( );
    return read( );
  }
  
  void		reset_write_count( )	{ write_count = 0; }
public:
  virtual bool	input( ) { return get_input( ); }

private:
  sc_master_slave_list  port_list_;
  bool			validated_;


  // the list of all slave processess attached to this link.
  sc_slave_proc_list	slave_procs;
  sc_slave_proc_list	inslave_procs;
  sc_slave_proc_list	outslave_procs;
  sc_slave_proc_list	inoutslave_procs;

  sc_master_slave_list	master_list;
  sc_master_slave_list	inmaster_list;
  sc_master_slave_list	outmaster_list;
  sc_master_slave_list	inoutmaster_list;

  sc_master_slave_list	slave_list;
  sc_master_slave_list	inslave_list;
  sc_master_slave_list	outslave_list;
  sc_master_slave_list	inoutslave_list;


  // implementation for refined protocol
private:
  typedef sc_phash<int, sc_interface* > refined_signals;
  typedef sc_phash<int, void* >		attribute_data;

  refined_signals	data_signals;
  refined_signals	address_signals;
  refined_signals	control_signals;
  attribute_data	attribute_data_list;
  
public:
  virtual sc_interface* get_signal(int id, sc_terminal_type ttype );
  virtual void add_signal( sc_interface* intf, sc_terminal_type ttype, int id );
  virtual void add_attribute_data( void * data, int id );
  virtual void * get_attribute_data( int id );

// BEGIN MODELSIM ADDITIONS
  MTI_SC_PORT_ENABLE_DEBUG
// END MODELSIM ADDITIONS
};  // sc_link_mp


////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------

//----------------------------------------------------------------------
template<class T>
void sc_link_mp<T>::register_port( sc_port_base& port, 
                                   const char* port_name) 
{
  sc_master_slave_b* ms_port 
    =  dynamic_cast< sc_master_slave_b* >( &port );
  
  if ( !ms_port ) {
    cout << "sc_link_mp.h::register_port : Error " 
      << port_name << " is not an master/slave port " << endl;
  }

  add_port( ms_port );
  // validate( );
} // register_port

//----------------------------------------------------------------------
template<class T>
void 
sc_link_mp<T>::validate( ) 
{
  if ( is_validated( ) ) {  
    return;  
  }

  if (port_list_.size( ) <= 0) {
    SC_REPORT_ERROR(SC_ID_MS_PORT_LIST_SIZE_, 0);
  }
  sc_master_slave_list::iterator it	= port_list_.begin( );
  sc_master_slave_list::iterator last	= port_list_.end( );

  sc_master_slave_b *port = *it; // save the first one
  bool valid = true; 

  for ( it++ ; ( it != last) && valid ; it++ ) {
    valid = can_bind( port, *it );
  }
  validated_ = true;
} // validate

//----------------------------------------------------------------------
// can_bind is called to check if two ports connected to the same link
// are compatible.  ** This is not used for checking compatiblity between
// port-to-port binding**
template<class T>
bool 
sc_link_mp<T>::can_bind( sc_master_slave_b* port1, 
                         sc_master_slave_b* port2 )
{
  const char * this_name 	= port1->port_typename( );
  const char * other_name 	= port2->port_typename( );
  bool match = strcmp( this_name, other_name)==0;

  if ( ! match ) {
    char msg[BUFSIZ];
    const char* link_name = sc_object::name( );
    const char* port_name = port1->get_name( ); 
    sprintf( msg,  
	     "`%s\' and `%s\'  : Cannot bind port `%s\' to link `%s\'",
	     port1->port_typename( ), 
	     port2->port_typename( ), 
	     port_name, 
	     link_name );
    SC_REPORT_ERROR( SC_ID_MS_INCOMPATIBLE_PORT_PROTOCOL_ON_LINKMP_, msg );
  }
  return true;
}

////////////////////////////////////////////////////////////////////
// add slave procs to the appropriate list
template<class T> 
void sc_link_mp<T>::add_slave_proc( sc_slave_proc_list& l ) {
  add_to_list( slave_procs, l );
}
template<class T> 
void sc_link_mp<T>::add_inslave_proc( sc_slave_proc_list& l )  {
  add_to_list( inslave_procs, l );
}

template<class T> 
void sc_link_mp<T>::add_outslave_proc( sc_slave_proc_list& l ) {
  add_to_list( outslave_procs, l );
}

template<class T> 
void sc_link_mp<T>::add_inoutslave_proc( sc_slave_proc_list& l )  {
  add_to_list( inoutslave_procs, l );
}


////////////////////////////////////////////////////////////////////
template<class T> 
void sc_link_mp<T>::add_to_list( sc_slave_proc_list& list1, 
			   sc_slave_proc_list& list2 ) 
{
  for ( sc_slave_proc_list::iterator 
	it 	= list2.begin( ),
	last 	= list2.end( );
       last != it;
       it++ ) {
    list1.push_back( *it );
  }
}



////////////////////////////////////////////////////////////////////
template<class T> 
sc_interface* sc_link_mp<T>::get_signal(int id, sc_terminal_type ttype ) {
  sc_interface* sig = 0;

  switch( ttype ) {
  case TT_DATA:
    data_signals.lookup( id, &sig );
    break;
  
  case TT_ADDRESS:
    address_signals.lookup( id, &sig );
    break;
  
  case TT_CONTROL:
    control_signals.lookup( id, &sig );
    break;
  } // switch

  return sig;
}

template<class T> 
void sc_link_mp<T>::add_signal( sc_interface* intf, sc_terminal_type ttype, int id ) {
  switch( ttype ) {
  case TT_DATA:
    data_signals.insert( id, intf );
    break;
  
  case TT_ADDRESS:
    address_signals.insert( id, intf );
    break;
  
  case TT_CONTROL:
    control_signals.insert( id, intf );
    break;
  } // switch
}

template<class T> 
void sc_link_mp<T>::add_attribute_data( void * data, int id ) {
  attribute_data_list.insert( id, data );
}

template<class T> 
void* sc_link_mp<T>::get_attribute_data( int id ) {
  void *d;
  attribute_data_list.lookup( id, &d );
  return d;
}

template<class T>
void sc_link_mp<T>::add_port( sc_master_slave_b* port  ) {

    // add port to the general list
    port_list_.push_back( port ); 

    // add the master port to the correct list
    if ( dynamic_cast< sc_port< sc_abstract_master_if<T> > * > ( port ) ) {
        master_list.push_back( port ); 
    } else if ( dynamic_cast< sc_port< sc_abstract_inmaster_if<T> > * > ( port ) ) {
        inmaster_list.push_back( port );
    } else if ( dynamic_cast< sc_port< sc_abstract_outmaster_if<T> > * > ( port ) ) {
        outmaster_list.push_back( port );
    } else if ( dynamic_cast< sc_port< sc_abstract_inoutmaster_if<T> > * > ( port ) ) {
        inoutmaster_list.push_back( port );
    } else if ( dynamic_cast< sc_port< sc_abstract_slave_if<T> > * > ( port ) ) {
        slave_list.push_back( port ); 
    } else if ( dynamic_cast< sc_port< sc_abstract_inslave_if<T> > * > ( port ) ) {
        inslave_list.push_back( port );
    } else if ( dynamic_cast< sc_port< sc_abstract_outslave_if<T> > * > ( port ) ) {
        outslave_list.push_back( port );
    } else if ( dynamic_cast< sc_port< sc_abstract_inoutslave_if<T> > * > ( port ) ) {
        inoutslave_list.push_back( port );
    }
}


#endif  // SC_LINK_MP_H
