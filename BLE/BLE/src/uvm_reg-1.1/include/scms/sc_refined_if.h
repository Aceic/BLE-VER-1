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

  sc_refined_if.h -- implementation of indexed protocols 
  		     
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_refined_if.h#1 $

 *****************************************************************************/
#ifndef SC_REFINED_IF_H
#define SC_REFINED_IF_H

#include "sc_link_mp_if.h"

// link mp interface for refined ports
template<class T>
class sc_refined_if : public virtual sc_link_mp_if {
public:
  sc_refined_if( ) {  }
  virtual ~sc_refined_if( ) {  }
  virtual sc_interface* get_signal(int id, sc_terminal_type ttype ) = 0;
  virtual void add_signal( sc_interface* intf, sc_terminal_type ttype, int id ) = 0;
  virtual void add_attribute_data( void * data, int id ) = 0;
  virtual void * get_attribute_data( int id ) = 0;
};  // sc_refined_if

#endif // SC_INDEXED_H
