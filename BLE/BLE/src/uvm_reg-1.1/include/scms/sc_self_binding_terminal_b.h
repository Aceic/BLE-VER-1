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

  sc_self_binding_terminal_b.h -- interface for terminals of master/slave ports.

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_self_binding_terminal_b.h#1 $

 *****************************************************************************/
#ifndef SC_SELF_BINDING_TERMINAL_B_H
#define SC_SELF_BINDING_TERMINAL_B_H

#include "sc_self_binding_b.h"
#include "sc_self_binding_port_b.h"

//---------------------------------------------------------------
// implementation of self binding terminals and its interface
class sc_self_binding_terminal_b : public sc_self_binding_b {
public:
  virtual sc_terminal_type	get_terminal_type( ) = 0;
  virtual void			bind( ::sc_core::sc_interface* signal ) = 0;
  virtual void			bind( sc_self_binding_terminal_b* ) = 0;
  virtual ::sc_core::sc_interface*		make_signal( ) = 0;

  // BEGIN MODELSIM ADDITION
  virtual ~sc_self_binding_terminal_b() {}
  // END MODELSIM ADDITION
}; // sc_self_binding_terminal_b

#endif  // SC_TERMINAL_B_H

