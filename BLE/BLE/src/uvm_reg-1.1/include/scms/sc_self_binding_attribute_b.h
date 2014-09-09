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

  sc_self_binding_attribute_b.h -- self binding_attribute interface
  
  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_self_binding_attribute_b.h#1 $

 *****************************************************************************/
#ifndef SC_SELF_BINDING_ATTRIBUTE_B_H
#define SC_SELF_BINDING_ATTRIBUTE_B_H

#include "sc_self_binding_b.h"
			       
class sc_self_binding_attribute_b : public sc_self_binding_b
{
public:
  virtual void bind( void* ) = 0;
  virtual void*	make_attribute( ) = 0;
};

#endif // SC_SELF_BINDING_ATTRIBUTE_B_H

