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

  sc_fx_ids.h -- Report ids for the datatypes/fx code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_fx_ids.h#1 $

 *****************************************************************************/

#ifndef SC_FX_IDS_H
#define SC_FX_IDS_H


#include "sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/fx)
//
//  Report ids in the range of 300-399.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
	namespace sc_core {extern const char id[]; }

namespace sc_core {
	extern const char SC_ID_REGISTER_ID_FAILED_[]; // in sc_report_handler.cpp
}
#endif

// START MODELSIM MODIFICATIONS

SC_DEFINE_MESSAGE( SC_ID_INVALID_WL_, MSG_IDX_INVALID_WL,
	"total wordlength <= 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_N_BITS_, MSG_IDX_INVALID_N_BITS,
	"number of bits < 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_DIV_WL_, MSG_IDX_INVALID_DIV_WL,
	"division wordlength <= 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_CTE_WL_, MSG_IDX_INVALID_CTE_WL,
	"constant wordlength <= 0 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_MAX_WL_, MSG_IDX_INVALID_MAX_WL,
	"maximum wordlength <= 0 and != -1 is not valid" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_FX_VALUE_, MSG_IDX_INVALID_FX_VALUE,
    "invalid fixed-point value" )
SC_DEFINE_MESSAGE( SC_ID_INVALID_O_MODE_, MSG_IDX_INVALID_O_MODE,
	"invalid overflow mode" )
SC_DEFINE_MESSAGE( SC_ID_OUT_OF_RANGE_, MSG_IDX_OUT_OF_RANGE,
	"index out of range" )
SC_DEFINE_MESSAGE( SC_ID_CONTEXT_BEGIN_FAILED_, MSG_IDX_CONTEXT_BEGIN_FAILED,
	"context begin failed" )  
SC_DEFINE_MESSAGE( SC_ID_CONTEXT_END_FAILED_, MSG_IDX_CONTEXT_END_FAILED, 
	"context end failed" )
SC_DEFINE_MESSAGE( SC_ID_WRAP_SM_NOT_DEFINED_, MSG_IDX_WRAP_SM_NOT_DEFINED, 
	"SC_WRAP_SM not defined for unsigned numbers" )

// END MODELSIM MODIFICATIONS


#endif

// Taf!
