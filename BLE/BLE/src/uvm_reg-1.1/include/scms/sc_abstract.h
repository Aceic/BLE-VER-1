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

  sc_abstract.h -- Implements the abstract protocols

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_abstract.h#1 $

 *****************************************************************************/
#ifndef SC_ABSTRACT_H
#define SC_ABSTRACT_H
  
#include "sc_abstract_port_impl.h"

//------------------------------------------------------------------
// various macros required for the master/slave implementation
#define SC_MASTER_RPC		struct sc_master_protocol: 	public virtual sc_abstract_master_port_impl<datatype>
#define SC_INMASTER_RPC 	struct sc_inm_protocol:		public virtual sc_abstract_inmaster_port_impl<datatype>
#define SC_OUTMASTER_RPC 	struct sc_outm_protocol:	public virtual sc_abstract_outmaster_port_impl<datatype>
#define SC_INOUTMASTER_RPC 	struct sc_inoutm_protocol:	public virtual sc_abstract_inoutmaster_port_impl<datatype>

#define SC_SLAVE_RPC		struct sc_slave_protocol:	public virtual sc_abstract_slave_port_impl<datatype>
#define SC_INSLAVE_RPC		struct sc_ins_protocol:		public virtual sc_abstract_inslave_port_impl<datatype>
#define SC_OUTSLAVE_RPC 	struct sc_outs_protocol:	public virtual sc_abstract_outslave_port_impl<datatype>
#define SC_INOUTSLAVE_RPC 	struct sc_inouts_protocol:	public virtual sc_abstract_inoutslave_port_impl<datatype>

//------------------------------------------------------------------
// implementation of abstract protocol

template <class datatype=int>
SC_PROTOCOL( sc_abstract )
{
public:
  SC_MASTER_RPC 	{ };
  SC_INMASTER_RPC 	{ };
  SC_OUTMASTER_RPC 	{ };
  SC_INOUTMASTER_RPC    { };

  SC_SLAVE_RPC          { };
  SC_INSLAVE_RPC        { };
  SC_OUTSLAVE_RPC       { };
  SC_INOUTSLAVE_RPC     { };
};

#endif  // SC_ABSTRACT_H

