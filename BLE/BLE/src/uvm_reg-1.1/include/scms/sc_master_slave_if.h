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

  sc_master_slave_if.h -- Provides a simulation context for use with multiple
                       simulations.

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_master_slave_if.h#1 $

 *****************************************************************************/
#ifndef SC_MASTER_SLAVE_IF_H
#define SC_MASTER_SLAVE_IF_H

#include <typeinfo>
#include "systemc.h"

#define		ERROR_MSG_BUF_SIZE	1024
// sc_terminal_type : the type of bus protocol terminals
// identifies the purpose of the terminal
// terminals will also have an id ( or assigned an id automatically )
// terminals with the same id/type on the same sc_link_mp will be connected
enum sc_terminal_type 	{TT_DATA, TT_ADDRESS, TT_CONTROL};

class sc_slave_process;
typedef sc_slave_process* sc_slave_handle;

class sc_link_mp_if;
typedef sc_pvector<sc_slave_handle>  sc_slave_proc_list;

void execute_slave_process( sc_slave_handle handle );

sc_slave_handle register_slave_process( const char* name,
                                         SC_ENTRY_FUNC entry_fn,
                                         sc_module* module );

//forward declaratin
class sc_master_slave_b;

#define declare_slave_process(handle, name, module_tag, func, port)         \
    sc_slave_handle handle;                                                 \
    {                                                                       \
        SC_DECL_HELPER_STRUCT( module_tag, func );                          \
        handle = register_slave_process( name,           		    \
                     SC_MAKE_FUNC_PTR( module_tag, func ), this );          \
	port.make_sensitive_slave( handle ); 				    \
    }
      
#define SC_SLAVE(func, port)                                                \
    declare_slave_process( func ## _handle,                                 \
                            #func,                                          \
                            SC_CURRENT_USER_MODULE,                         \
                            func, port )                                    \


//-----------------------------------------------------------------
// base class for all master/slave interfaces
class sc_master_slave_if: public virtual sc_interface  {
public:
  virtual ~sc_master_slave_if( ) { }
};

//-----------------------------------------------------------------
class SBprotocol
{
public:
  virtual ~SBprotocol( ) { }
};

#define SC_PROTOCOL(name)  struct name : public SBprotocol

//-----------------------------------------------------------------
// read interface for the master/slave
template<class T>
class sc_master_slave_read_if 
{
public:
   virtual const T& read( ) =0 ;
};

// write interface master/slave
template<class T>
class sc_master_slave_write_if 
{
public:
   virtual void write( const T& ) = 0;
};

template<class T>
class sc_master_slave_read_write_if :
  public virtual sc_master_slave_read_if<T>,
  public virtual sc_master_slave_write_if<T>
{

};
#endif // SC_MASTER_SLAVE_IF_H
