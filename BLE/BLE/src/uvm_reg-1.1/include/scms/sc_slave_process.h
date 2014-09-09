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

  sc_slave_process.h -- slave process definition

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_slave_process.h#1 $

 *****************************************************************************/
#ifndef SC_SLAVE_PROCESS_H
#define SC_SLAVE_PROCESS_H

#include "sc_process.h"
#include "sc_object.h"
#include "sc_module.h"

class sc_slave_process  : public ::sc_core::sc_object
{
public:
  sc_slave_process( const char*   nm,
                    ::sc_core::SC_ENTRY_FUNC fn,
                    ::sc_core::sc_module*    mod ) 
   : 
    ::sc_core::sc_object( nm ),
    entry_fn( fn ),
    module( mod )
  {
  }
  virtual ~sc_slave_process() {  }
    
  static const char* const kind_string;
  virtual const char* kind() const { return kind_string; }
  void execute( ) { 
    #ifndef SC_USE_MEMBER_FUNC_PTR
	(*entry_fn)( module );
    #else
	(module->*entry_fn)(); 
    #endif
  }
    
protected:
  ::sc_core::SC_ENTRY_FUNC entry_fn;
  ::sc_core::sc_module*    module;
}; // sc_slave_process

#endif
