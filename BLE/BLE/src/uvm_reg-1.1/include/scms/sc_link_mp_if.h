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

  sc_link_mp_if.h -- interface for multipoint link which connects 
		     master/slave ports
		     
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_link_mp_if.h#1 $

 *****************************************************************************/
#ifndef SC_LINK_MP_IF_H                      
#define SC_LINK_MP_IF_H

#include "sc_master_slave_if.h"

typedef sc_pvector< sc_master_slave_b* > sc_master_slave_list;

// sc_link_mp_if: 
//	provides the capability to register/retrieve signals used
//	to hookup terminals of an abstract port
class sc_link_mp_if : public virtual sc_interface
{
public:
  virtual void	add_port	( sc_master_slave_b* )	=0;
  
  // validate this link.  This function is not automatically called by
  // the kernel but should be called by at least one port connected to this
  // link.  Typically this call must be made from the 'end_of_elaboration'
  // function of the port
  virtual void	validate( ) = 0;  
  virtual bool	is_validated( ) = 0;

  // destructor 
  virtual 	~sc_link_mp_if( )	{ }
};

#endif

