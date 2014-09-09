/* $Id: //dvt/mti/rel/10.0b/src/systemc/sc_signal_ports_mti.h#1 $ */

#include "sc_lv.h"

extern bool mti_is_creating_dummy_signal;
extern bool mti_is_resolved_port;

using sc_dt::sc_lv;
using sc_dt::sc_logic;

template <class T>
inline void sc_in<T>::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());
    mti_is_creating_dummy_signal = true;
    m_mti_dummy_signal = new sc_signal<T>(sig_name);
    mti_is_creating_dummy_signal = false;
    sc_port_base::bind(*m_mti_dummy_signal);
}

template <class T>
inline int sc_in<T>::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}


inline void sc_in<bool>::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());

	mti_is_creating_dummy_signal = true;
	m_mti_dummy_signal = new sc_signal<bool>(sig_name);
	mti_is_creating_dummy_signal = false;
	sc_port_base::bind(*m_mti_dummy_signal);
}

inline int sc_in<bool>::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}

inline void sc_in<sc_logic>::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());

	mti_is_creating_dummy_signal = true;
    if (mti_is_resolved_port)
        m_mti_dummy_signal = new sc_signal_resolved(sig_name);
    else
        m_mti_dummy_signal = new sc_signal<sc_logic>(sig_name);
	mti_is_creating_dummy_signal = false;
	sc_port_base::bind(*m_mti_dummy_signal);
}

inline int sc_in<sc_logic>::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}

// ----------------------------------------------------------------------------
//  CLASS : sc_in<sc_lv<W> >
//
//  The sc_signal<sc_lv<W>> input port class.
// ----------------------------------------------------------------------------

template <int W>
class sc_in<sc_lv<W> >
: public sc_port<sc_signal_in_if<sc_lv<W> >,1,SC_ZERO_OR_MORE_BOUND>
{
public:

    // typedefs

    typedef sc_lv<W>                      				  data_type;

    typedef sc_signal_in_if<data_type>    				  if_type;
    typedef sc_port<if_type,1,SC_ZERO_OR_MORE_BOUND> 	  base_type;
    typedef sc_in<data_type>              				  this_type;

    typedef if_type                       				  in_if_type;
    typedef base_type                     				  in_port_type;
    typedef sc_signal_inout_if<data_type> 				  inout_if_type;
    typedef sc_port<inout_if_type,1,SC_ZERO_OR_MORE_BOUND> inout_port_type;

public:

    // constructors

    sc_in()
	: base_type(), 
	  m_traces( 0 ),
	  m_change_finder_p(0)
	{
		m_mti_dummy_signal = NULL;
        if (this->mti_is_boundary_top())
            mti_initialize_boundary_port();
	}

    explicit sc_in( const char* name_ )
	: base_type( name_ ), 
	  m_traces( 0 ),
	  m_change_finder_p(0)
	{
		m_mti_dummy_signal = NULL;
        if (this->mti_is_boundary_top())
            mti_initialize_boundary_port();
	}

    explicit sc_in( const in_if_type& interface_ )
	: base_type( CCAST<in_if_type&>( interface_ ) ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p(0)
    {}

    sc_in( const char* name_, const in_if_type& interface_ )
	: base_type( name_, CCAST<in_if_type&>( interface_ ) ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p(0)
	{}

    explicit sc_in( in_port_type& parent_ )
	: base_type( parent_ ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p(0)
	{}

    sc_in( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p(0)
	{}

    explicit sc_in( inout_port_type& parent_ )
	: base_type(), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL), 
	  m_change_finder_p(0)
	{ sc_port_base::bind( parent_ ); }

    sc_in( const char* name_, inout_port_type& parent_ )
	: base_type( name_ ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL), 
	  m_change_finder_p(0)
	{ sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
	: base_type( parent_ ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL), 
	  m_change_finder_p(0)
	{}

    sc_in( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL), 
	  m_change_finder_p(0)
	{}


    // destructor

    virtual ~sc_in() 
    { 
        remove_traces();   
		if ( m_change_finder_p ) delete m_change_finder_p;
        if( m_mti_dummy_signal != NULL) {
            delete m_mti_dummy_signal;
            m_mti_dummy_signal = NULL;
        }
    }


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
		 if ( !m_change_finder_p )
        {
            m_change_finder_p = new sc_event_finder_t<in_if_type>(
                *this, &in_if_type::value_changed_event );
        }
        return *m_change_finder_p;
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();

    virtual const char* kind() const
        { return "sc_in"; }


    void add_trace( sc_trace_file*, const std::string& ) const;

    // called by sc_trace
    void add_trace_internal( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:
  mutable sc_event_finder* m_change_finder_p;

private:

    // disabled
    sc_in( const this_type& );
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif

	// BEGIN MODELSIM ADDITIONS
	
public:
    virtual int mti_bind_sc_obj_name(char* m_mti_name_);
	void         mti_initialize_boundary_port();

private:
    sc_signal<sc_lv<W> >* m_mti_dummy_signal;

	// END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// called when elaboration is done

template <int W>
inline void sc_in<sc_lv<W> >::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < (int)m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( this->get_interface() );

		if (iface)
			sc_trace( p->tf, iface->read(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <int W>
inline void sc_in<sc_lv<W> >::add_trace_internal( sc_trace_file* tf_,
			       const std::string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}


template <int W>
inline void sc_in<sc_lv<W> >::add_trace( sc_trace_file* tf_,
			       const std::string& name_ ) const
{
    sc_deprecated_add_trace();
    add_trace_internal(tf_, name_);
}

template <int W>
inline void sc_in<sc_lv<W> >::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = (int)m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// called by pbind (for internal use only)

template <int W>
inline int sc_in<sc_lv<W> >::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

template <int W>
inline int sc_in<sc_lv<W> >::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}

template <int W>
inline void sc_in<sc_lv<W> >::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());
	mti_is_creating_dummy_signal = true;
    if (mti_is_resolved_port)
        m_mti_dummy_signal = new sc_signal_rv<W>(sig_name);
    else
        m_mti_dummy_signal = new sc_signal<sc_lv<W> >(sig_name);
	mti_is_creating_dummy_signal = false;
	sc_port_base::bind(*m_mti_dummy_signal);
}

template <int W>
inline int sc_in<sc_lv<W> >::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}



template <class T>
inline void sc_inout<T>::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());
    mti_is_creating_dummy_signal = true;
    m_mti_dummy_signal = new sc_signal<T>(sig_name);
    mti_is_creating_dummy_signal = false;
    sc_port_base::bind(*m_mti_dummy_signal);
}

template <class T>
inline int sc_inout<T>::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}


inline void sc_inout<bool>::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());

	mti_is_creating_dummy_signal = true;
	m_mti_dummy_signal = new sc_signal<bool>(sig_name);
	mti_is_creating_dummy_signal = false;
	sc_port_base::bind(*m_mti_dummy_signal);
}


inline int sc_inout<bool>::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}


inline void sc_inout<sc_logic>::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());

	mti_is_creating_dummy_signal = true;
    if (mti_is_resolved_port)
        m_mti_dummy_signal = new sc_signal_resolved(sig_name);
    else
        m_mti_dummy_signal = new sc_signal<sc_logic>(sig_name);
	mti_is_creating_dummy_signal = false;
	sc_port_base::bind(*m_mti_dummy_signal);
}


inline int sc_inout<sc_logic>::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<sc_lv<W> >
//
//  The sc_signal<sc_lv<W> > input/output port class.
// ----------------------------------------------------------------------------

template <int W>
class sc_inout<sc_lv<W> >
: public sc_port<sc_signal_inout_if<sc_lv<W> >,1,SC_ZERO_OR_MORE_BOUND>
{
public:

    // typedefs

    typedef sc_lv<W>                      			   data_type;

    typedef sc_signal_inout_if<data_type> 			   if_type;
    typedef sc_port<if_type,1,SC_ZERO_OR_MORE_BOUND>    base_type;
    typedef sc_inout<data_type>           			   this_type;

    typedef sc_signal_in_if<data_type>    			   in_if_type;
    typedef sc_port<in_if_type,1,SC_ZERO_OR_MORE_BOUND> in_port_type;
    typedef if_type                       			   inout_if_type;
    typedef base_type                     			   inout_port_type;

public:

    // constructors

    sc_inout()
	: base_type(), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_change_finder_p( 0 )
	{
		m_mti_dummy_signal = NULL;
        if (this->mti_is_boundary_top())
            mti_initialize_boundary_port();
	}

    explicit sc_inout( const char* name_ )
	: base_type( name_ ), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_change_finder_p( 0 )
	{
		m_mti_dummy_signal = NULL;
        if (this->mti_is_boundary_top())
            mti_initialize_boundary_port();
	}

    explicit sc_inout( inout_if_type& interface_ )
	: base_type( interface_ ), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL), 
	  m_change_finder_p( 0 )
	{}

    sc_inout( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ ), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p( 0 )
	{}

    explicit sc_inout( inout_port_type& parent_ )
	: base_type( parent_ ), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p( 0 )
	{}

    sc_inout( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ ), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p( 0 )
	{}

    sc_inout( this_type& parent_ )
	: base_type( parent_ ), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p( 0 )
	{}

    sc_inout( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), 
	  m_init_val( 0 ), 
	  m_traces( 0 ), 
	  m_mti_dummy_signal(NULL),
	  m_change_finder_p( 0 )
	{}


    // destructor

    virtual ~sc_inout();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // write the new value

    void write( const data_type& value_ )
	{ (*this)->write( value_ ); }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
		if ( !m_change_finder_p )
        {
            m_change_finder_p = new sc_event_finder_t<in_if_type>(
                *this, &in_if_type::value_changed_event );
        }
        return *m_change_finder_p;
    }


    virtual const char* kind() const
        { return "sc_inout"; }

protected:

    data_type* m_init_val;

public:

    void add_trace( sc_trace_file*, const std::string& ) const;

    // called by sc_trace
    void add_trace_internal( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

private:
    mutable sc_event_finder* m_change_finder_p;

private:

    // disabled
    sc_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif

	// BEGIN MODELSIM ADDITIONS
	
public:
    virtual int mti_bind_sc_obj_name(char* m_mti_name_);
	void         mti_initialize_boundary_port();

private:
    sc_signal<sc_lv<W> >* m_mti_dummy_signal;

	// END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// destructor

template <int W>
inline sc_inout<sc_lv<W> >::~sc_inout()
{
	if ( m_change_finder_p ) delete m_change_finder_p;
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    if( m_mti_dummy_signal != NULL) {
        delete m_mti_dummy_signal;
        m_mti_dummy_signal = NULL;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

template <int W>
inline void sc_inout<sc_lv<W> >::initialize( const data_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( this->get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new data_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

template <int W>
inline void sc_inout<sc_lv<W> >::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < (int)m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( this->get_interface() );

		if (iface)
			sc_trace( p->tf, iface->read(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <int W>
inline void sc_inout<sc_lv<W> >::add_trace_internal( sc_trace_file* tf_,
			       const std::string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}


template <int W>
inline void sc_inout<sc_lv<W> >::add_trace( sc_trace_file* tf_,
			       const std::string& name_ ) const
{
    sc_deprecated_add_trace();
    add_trace_internal(tf_, name_);
}


template <int W>
inline void sc_inout<sc_lv<W> >::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = (int)m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


template <int W>
inline void sc_inout<sc_lv<W> >::mti_initialize_boundary_port()
{
    //
    // Create a sc_signal channel and bind it with this port.
    // A fixed string (MTI_INTERNAL_CHANNEL_NAME_PREFIX) is prefixed
    // with the port name to create a signal name different 
    // than the port name.  
    //
    char sig_name[BUFSIZ];
    sprintf(sig_name, "%s%s", MTI_INTERNAL_CHANNEL_NAME_PREFIX, this->basename());

	mti_is_creating_dummy_signal = true;
    if (mti_is_resolved_port)
        m_mti_dummy_signal = new sc_signal_rv<W>(sig_name);
    else
        m_mti_dummy_signal = new sc_signal<sc_lv<W> >(sig_name);
	mti_is_creating_dummy_signal = false;
	sc_port_base::bind(*m_mti_dummy_signal);
}


template <int W>
inline int sc_inout<sc_lv<W> >::mti_bind_sc_obj_name(char* m_mti_name_)
{
	int status = 0;
    //
    // The dummy signal created for a boundary port is
    // saved in m_mti_dummy_signal. The dummy signal needs to
    // be bound with the declared port name so that
    // kernel can see it in case the explicit port name is
    // not the same as the declared port name.
    //
	if (m_mti_dummy_signal != NULL) {
		if (m_mti_dummy_signal->mti_name() != NULL) {
			std::string msg;
			msg = m_mti_name_;
			msg += " referring to object ";
			msg += this->name();
			SC_REPORT_WARNING(SC_ID_DUPLICATE_NAME_BIND_WARNING_, msg.c_str());
			status = 1;
		} else {
			if (sc_object::mti_is_use_default_name())
				m_mti_dummy_signal->mti_set_use_default_name(true);
			status = m_mti_dummy_signal->mti_bind_sc_obj_name(m_mti_name_);
		}         
	}

	if (status == 0)
		status = sc_object::mti_bind_sc_obj_name(m_mti_name_);
	return status;
}
