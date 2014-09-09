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

  sc_wait_cthread.h -- Wait() and related functions for SC_CTHREADs.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/******************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_wait_cthread.h#1 $

******************************************************************************/

#ifndef SC_WAIT_CTHREAD_H
#define SC_WAIT_CTHREAD_H


#include "sc_simcontext.h"
#include "sc_logic.h"
#include "sc_signal_ifs.h"


namespace sc_core 
{

// for SC_CTHREADs

extern
void
halt( sc_simcontext* = sc_get_curr_simcontext() );


extern
void
wait( int,
      sc_simcontext* = sc_get_curr_simcontext() );


extern
void
at_posedge( const sc_signal_in_if<bool>&,
	    sc_simcontext* = sc_get_curr_simcontext() );

extern
void
at_posedge( const sc_signal_in_if<sc_dt::sc_logic>&,
	    sc_simcontext* = sc_get_curr_simcontext() );

extern
void
at_negedge( const sc_signal_in_if<bool>&,
	    sc_simcontext* = sc_get_curr_simcontext() );

extern
void
at_negedge( const sc_signal_in_if<sc_dt::sc_logic>&,
	    sc_simcontext* = sc_get_curr_simcontext() );



class sc_watch
{
public:

    sc_cthread_handle cthread_h;

    sc_watch( sc_simcontext* simc )
    {
        sc_curr_proc_handle cpi = simc->get_curr_proc_info();
        assert( SC_CTHREAD_PROC_ == cpi->kind );
        cthread_h = RCAST<sc_cthread_handle>( cpi->process_handle );
    }

    ~sc_watch()
    {
    }
};

} // namespace sc_core

#endif
