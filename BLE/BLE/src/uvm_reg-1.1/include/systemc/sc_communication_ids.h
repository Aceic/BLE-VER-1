/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_communication_ids.h -- Report ids for the communication code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_communication_ids.h#1 $

 *****************************************************************************/

#ifndef SC_COMMUNICATION_IDS_H
#define SC_COMMUNICATION_IDS_H


#include "sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (communication)
//
//  Report ids in the range of 100-199.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
	namespace sc_core { extern const char id[]; }

namespace sc_core {
	extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
} // namespace sc_core 
#endif

SC_DEFINE_MESSAGE( SC_ID_PORT_OUTSIDE_MODULE_, MSG_IDX_PORT_OUTSIDE_MODULE,
			"port specified outside of module" )
SC_DEFINE_MESSAGE( SC_ID_CLOCK_PERIOD_ZERO_, MSG_IDX_CLOCK_PERIOD_ZERO,
			"sc_clock period is zero" )              
SC_DEFINE_MESSAGE( SC_ID_CLOCK_HIGH_TIME_ZERO_, MSG_IDX_CLOCK_HIGH_TIME_ZERO,
			"sc_clock high time is zero" )    
SC_DEFINE_MESSAGE( SC_ID_CLOCK_LOW_TIME_ZERO_, MSG_IDX_CLOCK_LOW_TIME_ZERO,
			"sc_clock low time is zero" )     
SC_DEFINE_MESSAGE( SC_ID_MORE_THAN_ONE_FIFO_READER_, MSG_IDX_MORE_THAN_ONE_FIFO_READER,
			"sc_fifo<T> cannot have more than one reader" )
SC_DEFINE_MESSAGE( SC_ID_MORE_THAN_ONE_FIFO_WRITER_, MSG_IDX_MORE_THAN_ONE_FIFO_WRITER,
			"sc_fifo<T> cannot have more than one writer" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_FIFO_SIZE_, MSG_IDX_INVALID_FIFO_SIZE,
			"sc_fifo<T> must have a size of at least 1" )
SC_DEFINE_MESSAGE( SC_ID_BIND_IF_TO_PORT_, MSG_IDX_BIND_IF_TO_PORT,
			"bind interface to port failed" ) 
SC_DEFINE_MESSAGE( SC_ID_BIND_PORT_TO_PORT_, MSG_IDX_BIND_PORT_TO_PORT,
			"bind parent port to port failed" )
SC_DEFINE_MESSAGE( SC_ID_COMPLETE_BINDING_, MSG_IDX_COMPLETE_BINDING,
			"complete binding failed" )
SC_DEFINE_MESSAGE( SC_ID_UNBOUND_PORT, MSG_IDX_UNBOUND_PORT,
			"port not bound to any channel" )
SC_DEFINE_MESSAGE( SC_ID_INSERT_PORT_, MSG_IDX_INSERT_PORT,
			"insert port failed" )
SC_DEFINE_MESSAGE( SC_ID_REMOVE_PORT_, MSG_IDX_REMOVE_PORT,
			"remove port failed" )
SC_DEFINE_MESSAGE( SC_ID_GET_IF_, MSG_IDX_GET_IF,
			"get interface failed" )
SC_DEFINE_MESSAGE( SC_ID_INSERT_PRIM_CHANNEL_, MSG_IDX_INSERT_PRIM_CHANNEL,
			"insert primitive channel failed" )
SC_DEFINE_MESSAGE( SC_ID_REMOVE_PRIM_CHANNEL_, MSG_IDX_REMOVE_PRIM_CHANNEL, 
			"remove primitive channel failed" ) 
SC_DEFINE_MESSAGE( SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_, MSG_IDX_MORE_THAN_ONE_SIGNAL_DRIVER,
			"sc_signal<T> cannot have more than one driver" )
SC_DEFINE_MESSAGE( SC_ID_NO_DEFAULT_EVENT_,    MSG_IDX_NO_DEFAULT_EVENT,
			"channel doesn't have a default event" )
SC_DEFINE_MESSAGE( SC_ID_RESOLVED_PORT_NOT_BOUND_, MSG_IDX_RESOLVED_PORT_NOT_BOUND,
			"resolved port not bound to resolved signal" )
SC_DEFINE_MESSAGE( SC_ID_FIND_EVENT_, MSG_IDX_FIND_EVENT,
			"find event failed" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_SEMAPHORE_VALUE_,  MSG_IDX_INVALID_SEMAPHORE_VALUE,
			"sc_semaphore requires an initial value >= 0" )
SC_DEFINE_MESSAGE( SC_ID_SC_EXPORT_HAS_NO_INTERFACE_,  MSG_IDX_SC_EXPORT_HAS_NO_INTERFACE,
			"sc_export instance has no interface" )
SC_DEFINE_MESSAGE( SC_ID_SC_EXPORT_AFTER_START_,  MSG_IDX_SC_EXPORT_AFTER_START,
			"insert sc_export failed, sc_export instance after simulation has started" )
SC_DEFINE_MESSAGE( SC_ID_SC_EXPORT_ALREADY_REGISTERED_,  MSG_IDX_SC_EXPORT_ALREADY_REGISTERED,
			"insert sc_export failed, sc_export already inserted" )
SC_DEFINE_MESSAGE( SC_ID_SC_EXPORT_NOT_REGISTERED_,  MSG_IDX_SC_EXPORT_NOT_REGISTERED,
			"remove sc_export failed, sc_export not registered" )
SC_DEFINE_MESSAGE( SC_ID_SC_EXPORT_NOT_BOUND_AFTER_CONSTRUCTION_,  MSG_IDX_SC_EXPORT_NOT_BOUND_AFTER_CONSTRUCTION,
			"sc_export instance not bound to interface at end of construction" )
SC_DEFINE_MESSAGE( SC_ID_ATTEMPT_TO_WRITE_TO_CLOCK_,  MSG_IDX_ATTEMPT_TO_WRITE_TO_CLOCK,
			"attempt to write the value of an sc_clock instance" )
SC_DEFINE_MESSAGE( SC_ID_SC_EXPORT_ALREADY_BOUND_, MSG_IDX_SC_EXPORT_ALREADY_BOUND, "sc_export instance already bound" )

// BEGIN MODELSIM ADDITIONS

SC_DEFINE_MESSAGE( SC_ID_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL_,  MSG_IDX_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL, "MSG_IDX_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL")
SC_DEFINE_MESSAGE( SC_ID_TLM_FIFO_RESIZE_NOT_SUPPORTED_,  MSG_IDX_TLM_FIFO_RESIZE_NOT_SUPPORTED, "MSG_IDX_TLM_FIFO_RESIZE_NOT_SUPPORTED")
SC_DEFINE_MESSAGE( SC_ID_SC_EXPORT_BINDING_CHANGED,  MSG_IDX_SC_EXPORT_BINDING_CHANGED, "MSG_IDX_SC_EXPORT_BINDING_CHANGED")
SC_DEFINE_MESSAGE( SC_ID_MIXLANG_SC_OUT_READ,  MSG_IDX_MIXLANG_SC_OUT_READ, "MSG_IDX_MIXLANG_SC_OUT_READ")
SC_DEFINE_MESSAGE( SC_ID_CUSTOM_DEBUG_NOT_ALLOWED,  MSG_IDX_CUSTOM_DEBUG_NOT_ALLOWED, "MSG_IDX_CUSTOM_DEBUG_NOT_ALLOWED")
// END MODELSIM ADDITIONS

#endif

// Taf!
