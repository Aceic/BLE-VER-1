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

  sc_master_slave_b.h -- interface for abstract and refined master/slave ports.

  Original Author: Dirk Vermeersch, Coware, Inc.
		   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_master_slave_b.h#1 $

 *****************************************************************************/
#ifndef SC_MASTER_SLAVE_B_H
#define SC_MASTER_SLAVE_B_H

#include "sc_master_slave_if.h"
#include "sc_link_mp_if.h"

//-----------------------------------------------------------------
// base class for all master/slave ports
class sc_master_slave_b
{ 
public:
  virtual const char *	port_typename( ) { return 0; }

  virtual const char*   get_name( ) = 0;

  // BEGIN MODELSIM ADDITION
  virtual ~sc_master_slave_b() {}
  // END MODELSIM ADDITION
};

#endif  // SC_MASTER_SLAVE_B_H
