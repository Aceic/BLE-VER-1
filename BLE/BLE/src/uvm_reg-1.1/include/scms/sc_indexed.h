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

  sc_indexed.h -- Implements the indexed protocol
  
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_indexed.h#1 $

 *****************************************************************************/
#ifndef SC_INDEXED_H
#define SC_INDEXED_H
  
#include "sc_indexed_port_impl.h"

//------------------------------------------------------------------
// various macros required for the master/slave implementation

//------------------------------------------------------------------
// implementation of indexed protocol

template <class datatype, uint64 N>
SC_PROTOCOL( sc_indexed )
{
public:
struct sc_master_protocol: 	
  public virtual sc_indexed_master_port_impl<datatype, N>
{  };


struct sc_inm_protocol:		
  public virtual sc_indexed_inmaster_port_impl<datatype, N>
{  };

struct sc_outm_protocol:	
    public virtual sc_indexed_outmaster_port_impl<datatype, N>
{  };

struct sc_inoutm_protocol:	
      public virtual sc_indexed_inoutmaster_port_impl<datatype, N>
{  };


struct sc_slave_protocol:	
	public virtual sc_indexed_slave_port_impl<datatype, N>
{  };

struct sc_ins_protocol:		
	  public virtual sc_indexed_inslave_port_impl<datatype, N>
{  };

struct sc_outs_protocol:	
	    public virtual sc_indexed_outslave_port_impl<datatype, N>
{  };

struct sc_inouts_protocol:	
	      public virtual sc_indexed_inoutslave_port_impl<datatype, N>
{  };

};

#endif  // SC_INDEXED_H

