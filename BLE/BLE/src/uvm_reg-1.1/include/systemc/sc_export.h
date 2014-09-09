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

  sc_export.h -- Base classes of all export classes.

  Original Author: Andy Goodrich, Forte Design Systems
                   Bishnupriya Bhattacharya, Cadence Design Systems

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_export.h#1 $

 *****************************************************************************/

#ifndef SC_EXPORT_H
#define SC_EXPORT_H
#include <typeinfo>

#include "sc_communication_ids.h"
#include "sc_interface.h"
#include "sc_object.h"
#include "ptrarray.h"

namespace sc_core {

// BEGIN MODELSIM ADDITIONS

extern mtiObjectHandleT mti_get_current_region_id();

// END MODELSIM ADDITIONS

//=============================================================================
//  CLASS : sc_export_base
//
//  Abstract base class for class sc_export<IF>.
//=============================================================================

class sc_export_base : public sc_object
{
    friend class sc_export_registry;
public:

    // typedefs
    
    typedef sc_export_base this_type;

public:

    virtual       sc_interface* get_interface() = 0;
    virtual       const sc_interface* get_interface() const = 0;

protected:
    
    // constructors

    sc_export_base();
    sc_export_base(const char* name);

    // destructor

    virtual ~sc_export_base();

protected:

    // called when construction is done 
    virtual void before_end_of_elaboration();

    // called when elaboration is done (does nothing by default)
    virtual void end_of_elaboration();

    // called before simulation starts (does nothing by default)
    virtual void start_of_simulation();

    // called after simulation ends (does nothing)
    virtual void end_of_simulation();

    virtual const char* if_typename() const = 0;

private:

    // disabled
    sc_export_base(const this_type&);
    this_type& operator = (const this_type& );

	// BEGIN MODELSIM ADDITIONS

protected:

	virtual void mti_create_export_alias()
	{}

	// END MODELSIM ADDITIONS

};

//=============================================================================
//  CLASS : sc_export
//
//  Generic export class for other export classes. This
//  class provides a binding point for access to an interface.
//=============================================================================
template<class IF>
class sc_export : public sc_export_base
{
    typedef sc_export<IF> this_type;

public: // constructors:
    sc_export() : sc_export_base()
    {
	m_interface_p = 0;

	// BEGIN MODELSIM ADDITIONS
	mti_init_export();
	// END MODELSIM ADDITIONS
    }

    explicit sc_export( const char* name_ ) : sc_export_base(name_)
    {
	m_interface_p = 0;

	// BEGIN MODELSIM ADDITIONS
	mti_init_export();
	// END MODELSIM ADDITIONS
    }

public: // destructor:
    virtual ~sc_export() 
    {
    }

public: // interface access:

    virtual sc_interface* get_interface() 
    {
	return m_interface_p;
    }

    virtual const sc_interface* get_interface() const
    {
        return m_interface_p;
    }

	const IF* operator -> () const {
		if ( m_interface_p == 0 )
		{
			SC_REPORT_ERROR(SC_ID_SC_EXPORT_HAS_NO_INTERFACE_,name());
		}
		return m_interface_p;
	}

    IF* operator -> () {
        if ( m_interface_p == 0 )
        {
            SC_REPORT_ERROR(SC_ID_SC_EXPORT_HAS_NO_INTERFACE_,name());
        }
        return m_interface_p;
    }

    operator IF& ()
    {
	if ( m_interface_p == 0 )
	{
	    SC_REPORT_ERROR(SC_ID_SC_EXPORT_HAS_NO_INTERFACE_,name());
	}
	return *m_interface_p;
    }

public: // binding:
    void bind( IF& interface_ )
    {
		if ( m_interface_p ) {
			SC_REPORT_ERROR(SC_ID_SC_EXPORT_ALREADY_BOUND_,name());
		} else {
			m_interface_p = &interface_;
			// BEGIN MODELSIM ADDITION
			mti_change_binding();
			// END MODELSIM ADDITION
		}
    }

    void operator () ( IF& interface_ )
    {
		if ( m_interface_p ) {
			SC_REPORT_ERROR(SC_ID_SC_EXPORT_ALREADY_BOUND_,name());
		} else {
			m_interface_p = &interface_;
			// BEGIN MODELSIM ADDITION
			mti_change_binding();
			// END MODELSIM ADDITION
		}
    }

public: // identification:
    virtual const char* kind() const { return "sc_export"; }

protected:
  const char* if_typename() const {
    return typeid( IF ).name();
  }

private: // disabled
    sc_export( const this_type& );
    this_type& operator = ( const this_type& );

protected: // data fields:
    IF* m_interface_p;		// Interface this port provides.

	// BEGIN MODELSIM ADDITIONS

private:

	mtiObjectHandleT m_mti_channel_id; // prim channel this export is bound to
	mtiObjectHandleT m_mti_region_id;  // module region id
	mtiObjectHandleT m_mti_export_id;
	int              m_mti_alias_created;

protected:

	virtual void mti_create_export_alias() {
		if (m_interface_p != NULL) {
			m_mti_channel_id = m_interface_p->mti_get_channel_id(this);
			m_mti_export_id = mti_CreateScSignalAlias(
								(char*)basename(),
								(char*)mti_name(),
								this,
								m_mti_region_id,
								m_mti_channel_id,
								MTI_INTERNAL,
								1);
			m_mti_alias_created = 1;
		}
	}

private:

	void mti_init_export() {
		m_mti_channel_id = 0;
		m_mti_region_id = 0;
		m_mti_export_id = 0;
		m_mti_alias_created = 0;
		mti_record_parent_region_id();
	}

	void mti_record_parent_region_id()
	{ 
		m_mti_region_id = mti_get_current_region_id();
	}

	void mti_change_binding()
	{
		if (m_mti_alias_created)
			SC_REPORT_WARNING(SC_ID_SC_EXPORT_BINDING_CHANGED, name());
	}

	// END MODELSIM ADDITIONS
};

// ----------------------------------------------------------------------------
//  CLASS : sc_export_registry
//
//  Registry for all exports.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

class sc_export_registry
{
    friend class sc_simcontext;

public:

    void insert( sc_export_base* );
    void remove( sc_export_base* );

    int size() const
        { return m_export_vec.size(); }

private:

    // constructor
    explicit sc_export_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_export_registry();

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

private:

    sc_simcontext*               m_simc;

    // BEGIN MODELSIM MODIFICATIONS
    ptrarray<sc_export_base*>    m_export_vec;
    // END MODELSIM MODIFICATIONS

private:

    // disabled
    sc_export_registry();
    sc_export_registry( const sc_export_registry& );
    sc_export_registry& operator = ( const sc_export_registry& );

	// BEGIN MODELSIM ADDITIONS

public:
	bool mti_is_valid_export(sc_export_base* ex) const;

	// END MODELSIM ADDITIONS
};

} // namespace sc_core

#endif

// Taf!
