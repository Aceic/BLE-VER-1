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

  sc_object.h -- Abstract base class of all SystemC `simulation' objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems
                               5 September 2003
  Description of Modification: - Made creation of attributes structure      
                                 conditional on its being used. This eliminates
                                 100 bytes of storage for each normal sc_object.

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_object.h#1 $

 *****************************************************************************/

#ifndef SC_OBJECT_H
#define SC_OBJECT_H


#include "sc_iostream.h"
#include "sc_attribute.h"

// BEGIN MODELSIM ADDITIONS
#include "scmti.h"
// END MODELSIM ADDITIONS


namespace sc_core {

class sc_trace_file;
class sc_simcontext;


// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
//  Abstract base class of all SystemC `simulation' objects.
// ----------------------------------------------------------------------------

class sc_object 
{
    friend class sc_object_manager;
	friend class sc_module_dynalloc_list;
	friend class sc_process_b;

public:

    const char* name() const;
    const char* basename() const;

    virtual void print(::std::ostream& os = ::std::cout) const;

    // dump() is more detailed than print()
    virtual void dump(::std::ostream& os = ::std::cout) const;

    virtual void trace( sc_trace_file* tf ) const;

    virtual const char* kind() const { return "sc_object"; }

    sc_simcontext* simcontext() const
        { return m_simc; }

    // add attribute
    bool add_attribute( sc_attr_base& );

    // get attribute by name
          sc_attr_base* get_attribute( const std::string& name_ );
    const sc_attr_base* get_attribute( const std::string& name_ ) const;

    // remove attribute by name
    sc_attr_base* remove_attribute( const std::string& name_ );

    // remove all attributes
    void remove_all_attributes();

    // get the number of attributes
    int num_attributes() const;

    // get the attribute collection
          sc_attr_cltn& attr_cltn();
    const sc_attr_cltn& attr_cltn() const;

    virtual const std::vector<sc_object*>& get_child_objects() const
        { return *(new std::vector<sc_object*>); }

    sc_object* get_parent() const { return m_parent; } 
    sc_object* get_parent_object() const { return m_parent; }

protected:

    sc_object();
    sc_object(const char* nm);
    virtual ~sc_object();

private:

    void sc_object_init(const char* nm);

private:

    /* Each simulation object is associated with a simulation context */ 
    sc_simcontext*         m_simc;
    char*                  m_name;
    mutable sc_attr_cltn*  m_attr_cltn_p;
    sc_object*             m_parent;

	// BEGIN MODELSIM ADDITIONS

public:

    virtual int mti_is_region() { return 0; }

	virtual int mti_bind_sc_obj_name(char* m_mti_name_);

	void mti_set_typename(char* m_mti_typename_);

	bool mti_is_typename_checked()
		{ return m_mti_is_typename_checked; }

	const char* mti_name() const
		{ return m_mti_name; }

	char* mti_typename()
		{ m_mti_is_typename_checked = true; return m_mti_typename; }

	virtual void  mti_set_use_default_name(bool status)
		{ m_mti_is_use_default_name = status; }

	virtual bool  mti_is_use_default_name()
		{ return m_mti_is_use_default_name; }

 private:
	/* fix systemc default name (i.e. "port_0", "signal_0", etc.) to be the c instance 
	 * name for meaningful user report. This default name (m_name) is created at 
	 * sc_object construction time. At the end of elaboration the c instance name 
	 * is bound to m_mti_name, which we will use for the default name.
	 */

	void mti_fix_default_name();

private:
	/* factor out m_name related initialization for supporting mti_fix_default_port_name() */
	void mti_init_name(const char* name);

	/* factor out m_name deletion for supporting mti_fix_default_port_name() */
	void mti_delete_name();

	char* m_mti_name;
	char* m_mti_typename;         

	/* indicate whether sc_object has been instantiated with a name */
	bool  m_mti_is_use_default_name;

	/* indicate is this object already checked for typename
	 * to prevent the infinite loop for children having pointer
	 * to its parent */
	bool  m_mti_is_typename_checked;


public:
	static bool m_mti_is_strip_sc_main;

	// END MODELSIM ADDITIONS
};


// ----------------------------------------------------------------------------

extern const char SC_HIERARCHY_CHAR;
extern bool sc_enable_name_checking;

inline 
sc_object* sc_get_parent( const sc_object* obj_p ) 
{ 
	return obj_p->get_parent_object(); 
}

} // namespace sc_core

#endif // SC_OBJECT_H
