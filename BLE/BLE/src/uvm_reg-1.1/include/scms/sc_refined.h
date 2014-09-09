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

  sc_refined.h -- implementation of refined protocols 
  		     
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_refined.h#1 $

 *****************************************************************************/
#ifndef SC_REFINED_H
#define SC_REFINED_H

#include "sc_refined_port_impl.h"
#include "sc_terminal.h"

//------------------------------------------------------------------
// various macros required for the master/slave implementation
#define SC_MASTER_P 		struct sc_master_protocol:	public virtual sc_refined_master_port_impl<datatype>
#define SC_INMASTER_P 		struct sc_inm_protocol:		public virtual sc_refined_inmaster_port_impl<datatype>
#define SC_OUTMASTER_P 		struct sc_outm_protocol:	public virtual sc_refined_outmaster_port_impl<datatype>
#define SC_INOUTMASTER_P	struct sc_inoutm_protocol:	public virtual sc_refined_inoutmaster_port_impl<datatype>

#define SC_SLAVE_P 		struct sc_slave_protocol:	public virtual sc_refined_slave_port_impl<datatype>
#define SC_INSLAVE_P		struct sc_ins_protocol:		public virtual sc_refined_inslave_port_impl<datatype>
#define SC_OUTSLAVE_P 		struct sc_outs_protocol:	public virtual sc_refined_outslave_port_impl<datatype>
#define SC_INOUTSLAVE_P 	struct sc_inouts_protocol:	public virtual sc_refined_inoutslave_port_impl<datatype>

//------------------------------------------------------------------
template <class datatype>
SC_PROTOCOL(sc_enableHandshake) {
  public :  
  SC_OUTMASTER_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> en;
  };
  SC_INMASTER_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> en;
  };
  SC_INOUTMASTER_P {
    sc_terminal_in<datatype,TT_DATA> din;
    sc_terminal_out<datatype,TT_DATA> dout;
    sc_terminal_out<bool,TT_CONTROL> en;
    sc_terminal_out<bool,TT_CONTROL> nRW;
  };
  SC_OUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> en;
  };
  SC_INSLAVE_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> en;
  };
  SC_INOUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> din;
    sc_terminal_in<datatype,TT_DATA> dout;
    sc_terminal_in<bool,TT_CONTROL> en;
    sc_terminal_in<bool,TT_CONTROL> nRW;
  };
};


template <class datatype>
SC_PROTOCOL(sc_fullHandshake) {
  public:
  SC_OUTMASTER_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> ack;
    sc_terminal_out<bool,TT_CONTROL> req;
  };
  SC_INMASTER_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> ack;
    sc_terminal_out<bool,TT_CONTROL> req;
  };
  SC_INOUTMASTER_P {
    sc_terminal_in<datatype,TT_DATA> din;
    sc_terminal_out<datatype,TT_DATA> dout;
    sc_terminal_in<bool,TT_CONTROL> ack;
    sc_terminal_out<bool,TT_CONTROL> req;
    sc_terminal_out<bool,TT_CONTROL> nRW;
 };
  SC_OUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> ack;
    sc_terminal_in<bool,TT_CONTROL> req;
  };
  SC_INSLAVE_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> ack;
    sc_terminal_in<bool,TT_CONTROL> req;
  };
  SC_INOUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> din;
    sc_terminal_in<datatype,TT_DATA> dout;
    sc_terminal_out<bool,TT_CONTROL> ack;
    sc_terminal_in<bool,TT_CONTROL> req;
    sc_terminal_in<bool,TT_CONTROL> nRW;
 };

};


template <class datatype>
SC_PROTOCOL(sc_noHandshake) {
  public:
  SC_OUTMASTER_P {
    sc_terminal_out<datatype,TT_DATA> d;
  };
  SC_INMASTER_P {
    sc_terminal_in<datatype,TT_DATA> d;
  };
  SC_INOUTMASTER_P {
    sc_terminal_inout<datatype,TT_DATA> d;
 };
  SC_OUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> d;
  };
  SC_INSLAVE_P {
    sc_terminal_in<datatype,TT_DATA> d;
  };
  SC_INOUTSLAVE_P {
    sc_terminal_inout<datatype,TT_DATA> d;
 };

};


template<class datatype,int size,int adrwidth>
SC_PROTOCOL(sc_memenHandshake) {
  public :
  SC_OUTMASTER_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> en;
    sc_terminal_out<int,TT_ADDRESS> A;
  };
  SC_INMASTER_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> en;
    sc_terminal_out<int,TT_ADDRESS> A;
  };
  SC_INOUTMASTER_P {
    sc_terminal_in<datatype,TT_DATA> din;
    sc_terminal_out<datatype,TT_DATA> dout;
    sc_terminal_out<bool,TT_CONTROL> en;
    sc_terminal_out<bool,TT_CONTROL> nRW;
    sc_terminal_out<int,TT_ADDRESS> A;
 };
  SC_OUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> en;
    sc_terminal_in<int,TT_ADDRESS> A;
  };
  SC_INSLAVE_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> en;
    sc_terminal_in<int,TT_ADDRESS> A;
  };
  SC_INOUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> din;
    sc_terminal_in<datatype,TT_DATA> dout;
    sc_terminal_in<bool,TT_CONTROL> en;
    sc_terminal_in<bool,TT_CONTROL> nRW;
    sc_terminal_in<int,TT_ADDRESS> A;
 };
};

template<class datatype,int size,int adrwidth>
SC_PROTOCOL(sc_memfullHandshake) {
  public :
  SC_OUTMASTER_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> req;
    sc_terminal_in<bool,TT_CONTROL> ack;
    sc_terminal_out<int,TT_ADDRESS> A;
  };
  SC_INMASTER_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_out<bool,TT_CONTROL> req;
    sc_terminal_in<bool,TT_CONTROL> ack;
    sc_terminal_out<int,TT_ADDRESS> A;
  };
  SC_INOUTMASTER_P {
    sc_terminal_in<datatype,TT_DATA> din;
    sc_terminal_out<datatype,TT_DATA> dout;
    sc_terminal_out<bool,TT_CONTROL> req;
    sc_terminal_in<bool,TT_CONTROL> ack;
    sc_terminal_out<bool,TT_CONTROL> nRW;
    sc_terminal_out<int,TT_ADDRESS> A;
 };
  SC_OUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> req;
    sc_terminal_out<bool,TT_CONTROL> ack;
    sc_terminal_out<int,TT_ADDRESS> A;
  };
  SC_INSLAVE_P {
    sc_terminal_in<datatype,TT_DATA> d;
    sc_terminal_in<bool,TT_CONTROL> req;
    sc_terminal_out<bool,TT_CONTROL> ack;
    sc_terminal_in<int,TT_ADDRESS> A;
  };
  SC_INOUTSLAVE_P {
    sc_terminal_out<datatype,TT_DATA> din;
    sc_terminal_in<datatype,TT_DATA> dout;
    sc_terminal_in<bool,TT_CONTROL> req;
    sc_terminal_out<bool,TT_CONTROL> ack;
    sc_terminal_in<bool,TT_CONTROL> nRW;
    sc_terminal_in<int,TT_ADDRESS> A;
 };
};

#endif // SC_REFINED_H
