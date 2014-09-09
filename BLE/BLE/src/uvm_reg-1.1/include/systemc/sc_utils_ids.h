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

  sc_utils_ids.h -- Report ids for the utils code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  Alex Riesen, Synopsys, Inc., 2003-02-02
  ported to SystemC 2.1 exception reporting.
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_utils_ids.h#1 $

 *****************************************************************************/

#ifndef SC_UTILS_IDS_H
#define SC_UTILS_IDS_H

// ----------------------------------------------------------------------------
//  Report ids (utils)
//
//  Report ids in the range of 800-899.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
	namespace sc_core { extern const char id[]; }

namespace sc_core {
	extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
} // namespace sc_core
#endif

// START MODELSIM MODIFICATIONS
SC_DEFINE_MESSAGE(SC_ID_STRING_TOO_LONG_,
		  MSG_IDX_STRING_TOO_LONG, "string is too long")
SC_DEFINE_MESSAGE(SC_ID_FRONT_ON_EMPTY_LIST_,
		  MSG_IDX_FRONT_ON_EMPTY_LIST, "attempt to take front() on an empty list")
SC_DEFINE_MESSAGE(SC_ID_BACK_ON_EMPTY_LIST_,
		  MSG_IDX_BACK_ON_EMPTY_LIST, "attempt to take back() on an empty list")
SC_DEFINE_MESSAGE(SC_ID_IEEE_1666_DEPRECATION_,
		  MSG_IDX_IEEE_1666_DEPRECATION, "/IEEE_Std_1666/deprecated" )
// END MODELSIM MODIFICATIONS

#endif

// Taf!
