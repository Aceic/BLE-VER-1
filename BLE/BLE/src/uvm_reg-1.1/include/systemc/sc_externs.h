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

  sc_externs.h -- Declaration of `sc_main' and other global variables.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_externs.h#1 $

 *****************************************************************************/

#ifndef SC_EXTERNS_H
#define SC_EXTERNS_H



extern "C" int sc_main( int argc, char* argv[] );

namespace sc_core {

// BEGIN MODELSIM MODIFICATIONS

int sc_argc();
const char* const* sc_argv();

// END MODELSIM MODIFICATIONS

} // namespace sc_core

#endif
