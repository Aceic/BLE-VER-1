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

  sc_indexed_if.h -- implementation of indexed port

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICAT,NION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_indexed_if.h#1 $

 *****************************************************************************/
#ifndef SC_INDEXED_IF_H
#define SC_INDEXED_IF_H

#include "sc_abstract_if.h"

template< class T >
class sc_indexed_if : public virtual sc_abstract_if
{
public:
  virtual void		set_address( uint64 i )	=0;
  virtual uint64	get_address( ) const =0;
};

//-------------------------------------------------------------------
template<class T> 
class sc_indexed_master_if 
: public virtual sc_indexed_if< T >,
  public virtual sc_abstract_master_if<T>
{
};

template<class T> 
class sc_indexed_inmaster_if 
:  public virtual sc_indexed_if< T >,
   public virtual sc_abstract_inmaster_if<T>
{
};

template<class T> 
class sc_indexed_outmaster_if 
:  public virtual sc_indexed_if< T >,
   public virtual sc_abstract_outmaster_if<T>
{
};

template<class T> 
class sc_indexed_inoutmaster_if  
:  public virtual sc_indexed_if< T >,
   public virtual sc_abstract_inoutmaster_if<T>
{
};


//-------------------------------------------------------------------
template<class T> 
class sc_indexed_slave_if 
:  public virtual sc_indexed_if< T >,
   public virtual sc_abstract_slave_if<T>
{
};

template<class T> 
class sc_indexed_inslave_if 
:  public virtual sc_indexed_if< T >,
   public virtual sc_abstract_inslave_if<T>
{
};

template<class T> 
class sc_indexed_outslave_if 
:  public virtual sc_indexed_if< T >,
   public virtual sc_abstract_outslave_if<T>
{
};

template<class T> 
class sc_indexed_inoutslave_if 
:  public virtual sc_indexed_if< T >,
   public virtual sc_abstract_inoutslave_if<T>
{
};


#endif // SC_INDEXED_IF_H
