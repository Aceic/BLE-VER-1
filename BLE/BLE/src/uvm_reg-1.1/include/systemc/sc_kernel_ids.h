/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  sc_kernel_ids.h -- Report ids for the kernel code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_kernel_ids.h#1 $

 *****************************************************************************/

#ifndef SC_KERNEL_IDS_H
#define SC_KERNEL_IDS_H


#include "sc_report.h"

// BEGIN MODELSIM ADDITIONS
#include "scmti_msg.h"
// END MODELSIM ADDITIONS


// ----------------------------------------------------------------------------
//  Report ids (kernel)
//
//  Report ids in the range of 500-599.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
	namespace sc_core { extern const char id[]; }

namespace sc_core {
	extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
} // namespace sc_core

#endif

// START MODELSIM MODIFICATIONS

SC_DEFINE_MESSAGE(SC_ID_NO_BOOL_RETURNED_            , MSG_IDX_NO_BOOL_RETURNED, 
	"operator does not return boolean")
SC_DEFINE_MESSAGE(SC_ID_NO_INT_RETURNED_             , MSG_IDX_NO_INT_RETURNED,
	"operator does not return int")
SC_DEFINE_MESSAGE(SC_ID_NO_SC_LOGIC_RETURNED_        , MSG_IDX_NO_SC_LOGIC_RETURNED,
	"operator does not return sc_logic")
SC_DEFINE_MESSAGE(SC_ID_OPERAND_NOT_SC_LOGIC_        , MSG_IDX_OPERAND_NOT_SC_LOGIC,
	"operand is not sc_logic")
SC_DEFINE_MESSAGE(SC_ID_OPERAND_NOT_BOOL_            , MSG_IDX_OPERAND_NOT_BOOL,
	"operand is not bool")
SC_DEFINE_MESSAGE(SC_ID_OBJECT_EXISTS_               , MSG_IDX_OBJECT_EXISTS,
	"object already exists")       
SC_DEFINE_MESSAGE(SC_ID_ILLEGAL_CHARACTERS_          , MSG_IDX_ILLEGAL_CHARACTERS,
	"illegal characters" )
SC_DEFINE_MESSAGE(SC_ID_VC6_PROCESS_HELPER_          , MSG_IDX_VC6_PROCESS_HELPER,
	"internal error: sc_vc6_process_helper" )
SC_DEFINE_MESSAGE(SC_ID_VC6_MAX_PROCESSES_EXCEEDED_  , MSG_IDX_VC6_MAX_PROCESSES_EXCEEDED,
	"maximum number of processes per module exceeded (VC6)" )
SC_DEFINE_MESSAGE(SC_ID_END_MODULE_NOT_CALLED_       , MSG_IDX_END_MODULE_NOT_CALLED,
	"module construction not properly completed: did "
	"you forget to add a sc_module_name parameter to "
	"your module constructor?" )
SC_DEFINE_MESSAGE(SC_ID_HIER_NAME_INCORRECT_         , MSG_IDX_HIER_NAME_INCORRECT,
	"hierarchical name as shown may be incorrect due to previous errors" )
SC_DEFINE_MESSAGE(SC_ID_SET_STACK_SIZE_              , MSG_IDX_SET_STACK_SIZE,
	"set_stack_size() is only allowed for SC_THREADs and SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_SC_MODULE_NAME_USE_          , MSG_IDX_SC_MODULE_NAME_USE,
	"incorrect use of sc_module_name" )
SC_DEFINE_MESSAGE(SC_ID_SC_MODULE_NAME_REQUIRED_     , MSG_IDX_SC_MODULE_NAME_REQUIRED,
	"an sc_module_name parameter for your constructor is required" )
SC_DEFINE_MESSAGE(SC_ID_SET_TIME_RESOLUTION_         , MSG_IDX_SET_TIME_RESOLUTION_ERROR,
	"set time resolution failed" )
SC_DEFINE_MESSAGE(SC_ID_SET_DEFAULT_TIME_UNIT_       , MSG_IDX_SET_DEFAULT_TIME_UNIT,
	"set default time unit failed" )
SC_DEFINE_MESSAGE(SC_ID_DEFAULT_TIME_UNIT_CHANGED_   , MSG_IDX_DEFAULT_TIME_UNIT_CHANGED,
	"default time unit changed to time resolution" )
SC_DEFINE_MESSAGE(SC_ID_WAIT_N_NOT_ALLOWED_          , MSG_IDX_WAIT_N_NOT_ALLOWED,
	"wait(n) is only allowed in SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_WAIT_NOT_ALLOWED_            , MSG_IDX_WAIT_NOT_ALLOWED,
	"wait() is only allowed in SC_THREADs and SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_NEXT_TRIGGER_NOT_ALLOWED_    , MSG_IDX_NEXT_TRIGGER_NOT_ALLOWED,
	"next_trigger() is only allowed in SC_METHODs" )
SC_DEFINE_MESSAGE(SC_ID_IMMEDIATE_NOTIFICATION_      , MSG_IDX_IMMEDIATE_NOTIFICATION,
	"immediate notification is not allowed during the update phase" )
SC_DEFINE_MESSAGE(SC_ID_HALT_NOT_ALLOWED_            , MSG_IDX_HALT_NOT_ALLOWED,
	"halt() is only allowed in SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_WATCHING_NOT_ALLOWED_        , MSG_IDX_WATCHING_NOT_ALLOWED,
	"watching() has been deprecated, use reset_signal_is()")
SC_DEFINE_MESSAGE(SC_ID_DONT_INITIALIZE_             , MSG_IDX_DONT_INITIALIZE,
	"dont_initialize() has no effect for SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_WAIT_N_INVALID_              , MSG_IDX_WAIT_N_INVALID,
	"wait(n) is only valid for n > 0" )
SC_DEFINE_MESSAGE(SC_ID_MAKE_SENSITIVE_              , MSG_IDX_MAKE_SENSITIVE,
	"make sensitive failed" )
SC_DEFINE_MESSAGE(SC_ID_MAKE_SENSITIVE_POS_          , MSG_IDX_MAKE_SENSITIVE_POS,
	"make sensitive pos failed" )
SC_DEFINE_MESSAGE(SC_ID_MAKE_SENSITIVE_NEG_          , MSG_IDX_MAKE_SENSITIVE_NEG,
	"make sensitive neg failed" )
SC_DEFINE_MESSAGE(SC_ID_INSERT_MODULE_               , MSG_IDX_INSERT_MODULE,
	"insert module failed" )
SC_DEFINE_MESSAGE(SC_ID_REMOVE_MODULE_               , MSG_IDX_REMOVE_MODULE,
	"remove module failed" )
SC_DEFINE_MESSAGE(SC_ID_NOTIFY_DELAYED_              , MSG_IDX_NOTIFY_DELAYED,
	"notify_delayed() cannot be called on events "
	"that have pending notifications" )
SC_DEFINE_MESSAGE(SC_ID_GEN_UNIQUE_NAME_             , MSG_IDX_GEN_UNIQUE_NAME,
	"cannot generate unique name from null string" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_NAME_STACK_EMPTY_     , MSG_IDX_MODULE_NAME_STACK_EMPTY,
	"module name stack is empty: did you forget to "
	"add a sc_module_name parameter to your module "
	"constructor?" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_METHOD_AFTER_START_   , MSG_IDX_MODULE_METHOD_AFTER_START,
	"call to SC_METHOD in sc_module while simulation running" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_THREAD_AFTER_START_   , MSG_IDX_MODULE_THREAD_AFTER_START,
	"call to SC_THREAD in sc_module while simulation running" )
SC_DEFINE_MESSAGE(SC_ID_MODULE_CTHREAD_AFTER_START_   , MSG_IDX_MODULE_CTHREAD_AFTER_START,
	"call to SC_CTHREAD in sc_module while simulation running" )

SC_DEFINE_MESSAGE(SC_ID_SIMULATION_STOP_CALLED_TWICE_ , MSG_IDX_SIMULATION_STOP_CALLED_TWICE,
	"sc_stop has already been called" ) 

SC_DEFINE_MESSAGE(SC_ID_STOP_MODE_AFTER_START_        , MSG_IDX_STOP_MODE_AFTER_START,
	"attempt to set sc_stop mode  after start will be ignored" ) 
SC_DEFINE_MESSAGE(SC_ID_METHOD_TERMINATION_EVENT_     , MSG_IDX_METHOD_TERMINATION_EVENT,
	"Attempt to get terminated event for a method process" ) 
SC_DEFINE_MESSAGE(SC_ID_JOIN_ON_METHOD_HANDLE_        , MSG_IDX_JOIN_ON_METHOD_HANDLE,
	"Attempt to register method process with sc_join object" ) 
SC_DEFINE_MESSAGE(SC_ID_EVENT_ON_NULL_PROCESS_        , MSG_IDX_EVENT_ON_NULL_PROCESS,
	"Attempt to get an event for non-existent process" )
SC_DEFINE_MESSAGE(SC_ID_MULTIPLE_RESETS_              , MSG_IDX_MULTIPLE_RESETS,
	"Attempt to set more than one signal as reset for a process" )
SC_DEFINE_MESSAGE(SC_ID_UNKNOWN_PROCESS_TYPE_         , MSG_IDX_UNKNOWN_PROCESS_TYPE,
	"Unknown process type" )
SC_DEFINE_MESSAGE(SC_ID_RESET_SIGNAL_IS_NOT_ALLOWED_  , MSG_IDX_RESET_SIGNAL_IS_NOT_ALLOWED,
	"reset_signal_is() is only allowed for SC_CTHREADs" )
SC_DEFINE_MESSAGE(SC_ID_NEGATIVE_SIMULATION_TIME_, MSG_IDX_NEGATIVE_SIMULATION_TIME,
	"negative simulation interval specified in sc_start call")


// END MODELSIM MODIFICATIONS

// BEGIN MODELSIM ADDITIONS

SC_DEFINE_MESSAGE(SC_ID_DESTRUCT_IN_ELAB_, MSG_IDX_DESTRUCT_IN_ELAB, "MSG_IDX_DESTRUCT_IN_ELAB")
SC_DEFINE_MESSAGE(SC_ID_START_NOT_SUPPORTED_, MSG_IDX_START_NOT_SUPPORTED, "MSG_IDX_START_NOT_SUPPORTED")
SC_DEFINE_MESSAGE(SC_ID_CYCLE_NOT_SUPPORTED_, MSG_IDX_CYCLE_NOT_SUPPORTED, "MSG_IDX_CYCLE_NOT_SUPPORTED")
SC_DEFINE_MESSAGE(SC_ID_PARAM_WITH_VOPT_, MSG_IDX_SC_PARAM_WITH_VOPT, "MSG_IDX_SC_PARAM_WITH_VOPT")
SC_DEFINE_MESSAGE(SC_ID_GET_TYPE_PARAM_FAILED_, MSG_IDX_GET_TYPE_PARAM_FAILED, "MSG_IDX_GET_TYPE_PARAM_FAILED")
SC_DEFINE_MESSAGE(SC_ID_HDL_LOAD_FAILED_, MSG_IDX_HDL_LOAD_FAILED, "MSG_IDX_HDL_LOAD_FAILED")
SC_DEFINE_MESSAGE(SC_ID_DUPLICATE_NAME_BIND_WARNING_, MSG_IDX_DUPLICATE_NAME_BIND_WARNING, "MSG_IDX_DUPLICATE_NAME_BIND_WARNING")
SC_DEFINE_MESSAGE(SC_ID_INTERNAL_FATAL_, MSG_IDX_INTERNAL_FATAL, "MSG_IDX_INTERNAL_FATAL")
SC_DEFINE_MESSAGE(SC_ID_SIMULATION_STOP_, MSG_IDX_SIMULATION_STOP, "MSG_IDX_SIMULATION_STOP")
SC_DEFINE_MESSAGE(SC_ID_USER_CALL_DISALLOWED_, MSG_IDX_USER_CALL_DISALLOWED, "MSG_IDX_USER_CALL_DISALLOWED")
SC_DEFINE_MESSAGE(SC_ID_SET_TIME_RESOLUTION_WARN_, MSG_IDX_SET_TIME_RESOLUTION_WARN, "MSG_IDX_SET_TIME_RESOLUTION_WARN")
SC_DEFINE_MESSAGE(SC_ID_SC_START_WRONG_CONTEXT_, MSG_IDX_SC_START_WRONG_CONTEXT, "MSG_IDX_SC_START_WRONG_CONTEXT")
SC_DEFINE_MESSAGE(SC_ID_SC_PROCESS_IN_ILLEGAL_CONTEXT_, MSG_IDX_SC_PROCESS_IN_ILLEGAL_CONTEXT, "MSG_IDX_SC_PROCESS_IN_ILLEGAL_CONTEXT")
SC_DEFINE_MESSAGE(SC_ID_SC_PROCESS_IN_FOREIGN_MODULE_, MSG_IDX_SC_PROCESS_IN_FOREIGN_MODULE, "MSG_IDX_SC_PROCESS_IN_FOREIGN_MODULE")
SC_DEFINE_MESSAGE(SC_ID_MSG_IDX_SIGSPY_IN_ILLEGAL_CONTEXT_, MSG_IDX_SIGSPY_IN_ILLEGAL_CONTEXT, "MSG_IDX_SIGSPY_IN_ILLEGAL_CONTEXT")
SC_DEFINE_MESSAGE(SC_ID_MSG_IDX_SIGSPY_IN_ILLEGAL_PARENT_, MSG_IDX_SIGSPY_IN_ILLEGAL_PARENT, "MSG_IDX_SIGSPY_IN_ILLEGAL_PARENT")
SC_DEFINE_MESSAGE(SC_ID_MSG_IDX_PENDING_DELTA_ACTIVITY_NOT_SUPPORTED_, MSG_IDX_PENDING_DELTA_ACTIVITY_NOT_SUPPORTED, "MSG_IDX_PENDING_DELTA__ACTIVITY_NOT_SUPPORTED")
SC_DEFINE_MESSAGE(SC_ID_DUPLICATE_FOREIGN_ELAB, MSG_IDX_DUPLICATE_FOREIGN_ELAB,
	"Attempt to elaborate a foreign module that is already elaborated.")
SC_DEFINE_MESSAGE(SC_ID_MISSING_FOREIGN_ELAB, MSG_IDX_MISSING_FOREIGN_ELAB,
	"")

// END MODELSIM ADDITIONS

#endif

// Taf!
