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

  sc_abstract_if.h -- interface for abstract protocols

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_abstract_if.h#1 $

 *****************************************************************************/
#ifndef SC_ABSTRACT_IF_H
#define SC_ABSTRACT_IF_H

#include "sc_link_mp_if.h"
			   
// link mp interface for abstract ports
class sc_abstract_if : public virtual sc_link_mp_if {
public:
  virtual void	reset_write_count( ) = 0;
};

//-------------------------------------------------------------------
template<class T> 
class sc_abstract_master_if : public virtual sc_abstract_if
{
public:
  virtual void operator ( ) ( ) = 0;
};

template<class T> 
class sc_abstract_inmaster_if :  public virtual sc_abstract_if
{
public:
  virtual const T& read_inmaster( ) = 0;
};

template<class T> 
class sc_abstract_outmaster_if : public virtual sc_abstract_if
{
public:
  virtual void write_outmaster(const T& data ) = 0;
};

template<class T> 
class sc_abstract_inoutmaster_if :  public virtual sc_abstract_if
{
public:
  virtual void write_inoutmaster(const T& data )  = 0;
  virtual const T& read_inoutmaster( )  = 0;
  virtual bool input( ) = 0; 
};


//-------------------------------------------------------------------
template<class T> 
class sc_abstract_slave_if : public virtual sc_abstract_if
{
public:
  virtual void add_slave_proc( sc_slave_proc_list& ) = 0;
};

template<class T> 
class sc_abstract_inslave_if 
  : public virtual sc_abstract_if, 
    public sc_master_slave_read_if<T>
{
public:
  virtual void add_inslave_proc( sc_slave_proc_list& ) = 0;
};

template<class T> 
class sc_abstract_outslave_if 
  : public virtual sc_abstract_if,
    public sc_master_slave_write_if<T>
{
public:
  virtual void add_outslave_proc( sc_slave_proc_list& ) = 0;
};

template<class T> 
class sc_abstract_inoutslave_if 
  : public virtual sc_abstract_if,
    public sc_master_slave_read_if<T>,
    public sc_master_slave_write_if<T>
{
public:
  virtual bool input( ) = 0;
  virtual void add_inoutslave_proc( sc_slave_proc_list& ) = 0;
};

#endif  // SC_ABSTRACT_IF_H
