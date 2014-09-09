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

  sc_self_binding_port.h -- interface for terminals of master/slave ports.

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_self_binding_port.h#1 $

 *****************************************************************************/
#ifndef SC_SELF_BINDING_PORT_H
#define SC_SELF_BINDING_PORT_H

#include "sc_self_binding_port_b.h"
#include "sc_self_binding_terminal_b.h"
#include "sc_self_binding_attribute_b.h"

class sc_self_binding_port;
extern sc_self_binding_port* current_port_protocol;

typedef sc_phash<mtiLongT, sc_self_binding_terminal_b*>	terminals;
typedef sc_phash<mtiLongT, sc_self_binding_attribute_b*> 	attributes;

class sc_self_binding_port_binder;
//---------------------------------------------------------------
class sc_self_binding_port : public sc_self_binding_port_b
{
  friend class sc_self_binding_port_binder;
public:
  virtual ~sc_self_binding_port( ); 

protected:
  terminals		data_terminals;
  terminals		address_terminals;
  terminals		control_terminals;
  attributes		attribute_list;
  
  // add the terminal to one of the maps
  void	add_terminal( terminals& the_list, 
                      sc_self_binding_terminal_b* intf, 
                      int id );
public:
  virtual void register_self_binding_port( ) { 
    // register yourself with the global variable. not MT safe
    current_port_protocol	=	this;
  }

  // constructor
  sc_self_binding_port( ) {  register_self_binding_port( );  }

  virtual int	get_data_index( )	{ return data_terminals.count( );    }
  virtual int	get_address_index( )	{ return address_terminals.count( ); }
  virtual int	get_control_index( )	{ return control_terminals.count( ); }
  virtual int	get_attribute_index( )	{ return attribute_list.count( );    }

  void add_terminal_b(sc_self_binding_terminal_b* intf, int id );
  void add_attribute_b( sc_self_binding_attribute_b *, int id );
};  // sc_self_binding_port



// bind the terminals of two self binding port
class sc_self_binding_port_binder {
public:
  sc_self_binding_port_binder( sc_self_binding_port&,
                               sc_self_binding_port& );
  void dummy_func( ) { }
private:
  void bind_terminals( terminals&, terminals& );
};


#endif // SELF_BINDING_PORT_B_H 
