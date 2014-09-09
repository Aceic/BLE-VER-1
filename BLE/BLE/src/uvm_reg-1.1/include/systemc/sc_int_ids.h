/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License")
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_int_ids.h -- Report ids for the datatypes/int code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_int_ids.h#1 $

 *****************************************************************************/

#ifndef SC_INT_IDS_H
#define SC_INT_IDS_H


#include "sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/int)
//
//  Report ids in the range of 400-499.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
	namespace sc_core { extern const char id[]; }

namespace sc_core {
	extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
}
#endif

// START MODELSIM MODIFICATIONS

SC_DEFINE_MESSAGE( SC_ID_INIT_FAILED_, MSG_IDX_INIT_FAILED, "initialization failed" )
SC_DEFINE_MESSAGE( SC_ID_ASSIGNMENT_FAILED_, MSG_IDX_ASSIGNMENT_FAILED, "assignment failed" )
SC_DEFINE_MESSAGE( SC_ID_OPERATION_FAILED_, MSG_IDX_OPERATION_FAILED, "operation failed" )
SC_DEFINE_MESSAGE( SC_ID_CONVERSION_FAILED_, MSG_IDX_CONVERSION_FAILED, "conversion failed" )

// END MODELSIM MODIFICATIONS


#endif

// Taf!
