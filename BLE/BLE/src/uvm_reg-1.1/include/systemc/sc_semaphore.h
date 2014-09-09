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

  sc_semaphore.h -- The sc_semaphore primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_semaphore.h#1 $

 *****************************************************************************/

#ifndef SC_SEMAPHORE_H
#define SC_SEMAPHORE_H


#include "sc_event.h"
#include "sc_prim_channel.h"
#include "sc_semaphore_if.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_semaphore
//
//  The sc_semaphore primitive channel class.
// ----------------------------------------------------------------------------

// START MODELSIM MODIFICATIONS

class sc_semaphore
: public sc_prim_channel,  // Need to be the first to ensure the address
						   // of the class is in the registery for name
						   // binding to work correctly.
  public sc_semaphore_if

// END MODELSIM MODIFICATIONS
{
public:

    // constructors

    explicit sc_semaphore( int init_value_ );
    sc_semaphore( const char* name_, int init_value_ );


    // interface methods

    // lock (take) the semaphore, block if not available
    virtual int wait();

    // lock (take) the semaphore, return -1 if not available
    virtual int trywait();

    // unlock (give) the semaphore
    virtual int post();

    // get the value of the semaphore
    virtual int get_value() const
	{ return m_value; }

    virtual const char* kind() const
        { return "sc_semaphore"; }

protected:

    // support methods

    bool in_use() const
	{ return ( m_value <= 0 ); }


    // error reporting
    void report_error( const char* id, const char* add_msg = 0 ) const;

protected:

    int      m_value;  // current value of the semaphore
    sc_event m_free;   // event to block on when m_value is negative

private:

    // disabled
    sc_semaphore( const sc_semaphore& );
    sc_semaphore& operator = ( const sc_semaphore& );

protected:
	// BEGIN MODELSIM ADDITIONS

	virtual mtiObjectHandleT mti_get_channel_id( sc_object* );
	virtual void end_of_elaboration();

	// END MODELSIM ADDITIONS
};

} // namespace sc_core

#endif

// Taf!
