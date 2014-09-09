//  -*- C++ -*- <this line is for emacs to recognize it as C++ code>
/*****************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  scv_introspection.h -- 
  The public interface for the introspection facility.
 
  You can use this facility without the other SCV facilities
  by using #include "scv_introspection.h" and
  -D_SCV_INTROSPECTION_ONLY on your compilation line.
 

  Original Authors (Cadence Design Systems, Inc):
  Norris Ip, Dean Shea, John Rose, Jasvinder Singh, William Paulsen,
  John Pierce, Rachida Kebichi, Ted Elkind, David Bailey, Samir Agrawal
  2002-09-23

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scv/scv_introspection.h#1 $

 *****************************************************************************/

#ifndef SCV_INTROSPECTION_H
#define SCV_INTROSPECTION_H

#include "scv_object_if.h"

// ----------------------------------------
// configuration
//
// some of these to be moved to scv_config.h
// ----------------------------------------

// compiler configuration
#if defined( __HP_aCC )
#ifndef _INCLUDE_LONGLONG
#define _INCLUDE_LONGLONG
#endif
#endif

// utilities
#include <assert.h>
#include <list>
#include <string>


#if defined(__SUNPRO_CC)
using std::list;
using std::pair;
#include <string>
using std::string;
#endif

// specific stuff for randomization extensions
template<typename T> class scv_extensions ;
class scv_constraint_base;
class scv_extensions_if;
class scv_smart_ptr_if;
class _scv_constraint_data;
class scv_expression;

#ifndef _SCV_INTROSPECTION_ONLY
#include "scv_report.h"
#include "scv_bag.h"
#include "scv_random.h"
#else
template<typename T> class scv_bag {};
class scv_random {};
enum scv_severity {
  SCV_INFO = 0,	// informative only
  SCV_WARNING,	// indicates potentially incorrect condition
  SCV_ERROR,	// indicates a definite problem
  SCV_FATAL	// indicates a problem from which we cannot recover
};
class _scv_message_desc {
friend class _scv_message;
private:
  _scv_message_desc(string tag, string format, scv_severity severity, unsigned actions)
    : _tag(tag), _format(format), _severity(severity), _actions(actions) {}
  const char *get_tag() const { return _tag.c_str(); }
  const char *get_format() const { return _format.c_str(); }
  scv_severity get_severity() const { return _severity; }
  unsigned get_actions() const { return _actions; }
  string _tag;
  string _format;
  scv_severity _severity;
  unsigned _actions;
};
class _scv_message {
public:
  enum severity_level { INFO, WARNING, ERROR, FATAL };

  enum response_level {
    NONE_SPECIFIED, ENABLE_MESSAGE, SUPPRESS_MESSAGE
  };

  enum stack_print {
    NO_STACK, SHORT_STACK, LONG_STACK
  };

// Message types are actually pointers to descriptors
#define _SCV_DEFERR(code, number, string, severity, printStack) \
  static _scv_message_desc *code##_base; \
  static _scv_message_desc **code;
#include "scv_messages.h"
#undef _SCV_DEFERR

  // Used internally by SystemC Verification Standard to report exceptions
  static void message(_scv_message_desc **desc_pp, ... ) { scv_out << "exception received" << endl; }
};
#endif

// specific stuff for scv_smart_ptr 
#include "scv_shared_ptr.h"

// ----------------------------------------
// test sc_uint<N> without SystemC
// ----------------------------------------
#define TEST_NEST_TEMPLATE
template<int N>
class test_uint {
  int i;
public:
  test_uint(int i = 0) : i(i) {}
  test_uint(const test_uint& j) : i(j) {}
  test_uint& operator=(const test_uint& j) { i = j.i; return *this; }
  test_uint& operator=(int j) { i = j; return *this; }
  operator int() const { return i; } 
  int to_int64() const { return i; }
  int to_uint64() const { return i; }
};

// ----------------------------------------
// core introspection infrastructure
// ----------------------------------------

// overall interface for the extensions 
class scv_extensions_if;

#define _SCV_INTROSPECTION_BASE scv_object_if

// ----------------------------------------
// utilities supporting all extensions
// ----------------------------------------
class scv_extension_util_if : public _SCV_INTROSPECTION_BASE {
public:
  // scv_object_if's interface is also available 
  //
  // const char *get_name() const;
  // const char *kind() const;
  // void print();
  // void show();
  //
  virtual bool has_valid_extensions() const = 0;
  virtual bool is_dynamic() const = 0;
  virtual std::string get_short_name() const = 0;
  virtual void set_name(const char *) = 0; // error if performed on fields/array-elts 
};

#undef _SCV_INTROSPECTION_BASE
#define _SCV_INTROSPECTION_BASE scv_extension_util_if

// ----------------------------------------
// static extension to extract type information
// ----------------------------------------
class scv_extension_type_if : public _SCV_INTROSPECTION_BASE {
public:
  virtual const char *get_type_name() const = 0;

  enum data_type {
    BOOLEAN,                       // bool
    ENUMERATION,                   // enum
    INTEGER,                       // char, short, int, mtiLongT, long long
    UNSIGNED,                      // unsigned { char, short, int, mtiLongT, long long }
    FLOATING_POINT_NUMBER,         // float, double 
	BIT,                           // sc_bit
    BIT_VECTOR,                    // sc_bv
	LOGIC,                         // sc_logic
    LOGIC_VECTOR,                  // sc_lv
	SC_SIGNED,				   	   // sc_int, sc_bigint
	SC_UNSIGNED,				   // sc_uint, sc_biguint
    FIXED_POINT_INTEGER,           // sc_fixed
    UNSIGNED_FIXED_POINT_INTEGER,  // sc_ufixed
    RECORD,                        // struct/class
    POINTER,                       // T*
    ARRAY,                         // T[N]
// BEGIN MODELSIM ADDITIONS [MDE-SCV]
    VECTOR,                        // vector<T>
// END MODELSIM ADDITIONS [MDE-SCV]
    STRING                         // string, std::string
  };
  virtual data_type get_type() const = 0;

  bool is_bool() const { return get_type() == BOOLEAN; }

  bool is_enum() const { return get_type() == ENUMERATION; }
  virtual int get_enum_size() const = 0;
  virtual void get_enum_details(list<const char *>&, list<int>&) const = 0;
  virtual const char *get_enum_string(int) const = 0;

  bool is_integer() const { return (get_type() == INTEGER || get_type() == SC_SIGNED); }
  bool is_unsigned() const { return (get_type() == UNSIGNED || get_type() == SC_UNSIGNED); }
  bool is_bit_vector() const { return (get_type() == BIT_VECTOR || get_type() == BIT); }
  bool is_logic_vector() const { return (get_type() == LOGIC_VECTOR || get_type() == LOGIC); }
  bool is_fixed() const { return get_type() == FIXED_POINT_INTEGER; }
  bool is_unsigned_fixed() const { return get_type() == UNSIGNED_FIXED_POINT_INTEGER; }
  bool is_floating_point_number() const { return get_type() == FLOATING_POINT_NUMBER; } 

  bool is_record() const { return get_type() == RECORD; }
  virtual int get_num_fields() const = 0;
  virtual scv_extensions_if *get_field(unsigned) = 0; 
  virtual const scv_extensions_if *get_field(unsigned) const = 0; 

  bool is_pointer() const { return get_type() == POINTER; }
  virtual scv_extensions_if *get_pointer() = 0;
  virtual const scv_extensions_if *get_pointer() const = 0;

  bool is_array() const { return get_type() == ARRAY; }
  virtual int get_array_size() const = 0; 
  virtual scv_extensions_if *get_array_elt(int) = 0; 
  virtual const scv_extensions_if *get_array_elt(int) const = 0;

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  bool is_vector() const { return get_type() == VECTOR; }
  virtual scv_extensions_if *get_vector_size() = 0;
  virtual const scv_extensions_if *get_vector_size() const = 0;
  virtual void resize_vector(unsigned old_size, unsigned new_size) = 0;
// END MODELSIM ADDITIONS [MDE-SCV]

  bool is_string() const { return get_type() == STRING; }

  virtual int get_bitwidth() const = 0;

  // return non-null if this is a field in a record
  // or an element in an array. 
  virtual scv_extensions_if *get_parent() = 0;
  virtual const scv_extensions_if *get_parent() const = 0;

  // ... more to be added.
};

#undef _SCV_INTROSPECTION_BASE
#define _SCV_INTROSPECTION_BASE scv_extension_type_if

// ----------------------------------------
// static extension to read and write to an object, its field, and its array element
// ----------------------------------------
class scv_extension_rw_if : public _SCV_INTROSPECTION_BASE {
public:
  virtual void assign(bool) = 0;
  virtual void assign(char) = 0;
  virtual void assign(unsigned char) = 0;
  virtual void assign(short) = 0;
  virtual void assign(unsigned short) = 0;
  virtual void assign(int) = 0;
  virtual void assign(unsigned) = 0;
  virtual void assign(mtiLongT) = 0;
  virtual void assign(mtiUlongT) = 0;
  virtual void assign(long long) = 0;
  virtual void assign(unsigned long long) = 0;
  virtual void assign(float) = 0;
  virtual void assign(double) = 0;
  virtual void assign(const string&) = 0;
  virtual void assign(const char *) = 0;

  virtual bool get_bool() const = 0;
  virtual long long get_integer() const = 0;
  virtual unsigned long long get_unsigned() const = 0;
  virtual double get_double() const = 0;
  virtual std::string get_string() const = 0;

#ifdef SYSTEMC_H
  virtual void assign(const sc_bv_base& v) = 0;
  virtual void get_value(sc_bv_base& v) const = 0;
  virtual void assign(const sc_lv_base& v) = 0;
  virtual void get_value(sc_lv_base& v) const = 0;
#endif
};

#undef _SCV_INTROSPECTION_BASE
#define _SCV_INTROSPECTION_BASE scv_extension_rw_if

class _scv_dynamic_data;

// ----------------------------------------
// dynamic extension to perform randomization
// ----------------------------------------
class scv_extension_rand_if : public _SCV_INTROSPECTION_BASE {
public:
  enum mode_t {
    RANDOM,
    SCAN,
    RANDOM_AVOID_DUPLICATE,
    DISTRIBUTION
  };
public: 
  virtual void next() = 0; 
  virtual void disable_randomization() = 0;
  virtual void enable_randomization() = 0;
  virtual bool is_randomization_enabled() = 0;
  virtual void use_constraint(scv_extensions_if*) = 0;
  virtual void set_random(scv_shared_ptr<scv_random> gen) = 0;
  virtual scv_shared_ptr<scv_random> get_random(void) = 0;
  virtual scv_expression form_expression() const = 0;

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  void add_hard_constraint(
    const char *name,
    scv_expression expr,
    const char *file, int line );

  void add_soft_constraint(
    const char *name,
    scv_expression expr,
    const char *file, int line );

  scv_smart_ptr_if* _smart_ptr;
// END MODELSIM ADDITIONS [MDE-SCV]

public: // implementation (private)
  virtual _scv_constraint_data *get_constraint_data() =0;
  virtual void get_generator() =0;
  virtual void set_constraint(scv_constraint_base*) = 0;
  virtual void set_constraint(bool mode=true) = 0;
  virtual void set_extension(scv_extensions_if *e = NULL) = 0;
  virtual void set_distribution_from(scv_extensions_if*) = 0;
  virtual _scv_dynamic_data *get_dynamic_data() = 0;
  virtual void updated() = 0;
  virtual void uninitialize() = 0;
  virtual void initialize() const = 0;
  virtual bool is_initialized() const = 0;
};

#undef _SCV_INTROSPECTION_BASE
#define _SCV_INTROSPECTION_BASE scv_extension_rand_if

// ----------------------------------------
// dynamic extension to perform value change callback
// ----------------------------------------
class scv_extension_callbacks_if : public _SCV_INTROSPECTION_BASE {
public:
  enum callback_reason { VALUE_CHANGE, DELETE };
  typedef int callback_h;

public:
  callback_h register_cb(void (*f)(scv_extensions_if&, callback_reason)) {
    return _register_cb(new callback_t(f));
  }
  template<typename arg_t>
  callback_h register_cb(void (*f)(scv_extensions_if&, callback_reason, arg_t), arg_t arg) {
    return _register_cb(new callback_arg_t<arg_t>(f,arg));
  }
  virtual void remove_cb(callback_h) = 0;

public: // internal
  class callback_base {
    int *_children;
    int _id;
  public:  
    virtual void execute(scv_extensions_if*,callback_reason) = 0;
    virtual callback_base *duplicate() const = 0;
    callback_base() : _children(NULL), _id(0) {} 
    virtual ~callback_base() { if (_children) delete[] _children; }
    int get_id() { return _id; }
    void set_id(int i) { _id = i; }
    int *get_children() { return _children; }
    void set_children(int *p) { _children = p; }
  };
  class callback_t : public callback_base {
  public:
    typedef void (*fcn_t)(scv_extensions_if&, callback_reason);
    fcn_t _fcn;
    callback_t(fcn_t f) : _fcn(f) {}
    virtual void execute(scv_extensions_if*d, callback_reason r) { (*_fcn)(*d,r); }
    virtual callback_base *duplicate() const { return new callback_t(_fcn); }
  };
  template<typename arg_t>
  class callback_arg_t : public callback_base {
  public:
    typedef void (*fcn_t)(scv_extensions_if&, callback_reason, arg_t);
    fcn_t _fcn;
    arg_t _arg;
    callback_arg_t(fcn_t f, arg_t arg) : _fcn(f), _arg(arg) {}
    virtual void execute(scv_extensions_if*d, callback_reason r) { (*_fcn)(*d,r,_arg); }
    virtual callback_base *duplicate() const { return new callback_arg_t(_fcn, _arg); }
  };
  virtual callback_h _register_cb(callback_base *) = 0;
};

#undef _SCV_INTROSPECTION_BASE
#define _SCV_INTROSPECTION_BASE scv_extension_callbacks_if

// ----------------------------------------
// a thin coordination layer to collect all the extensions
// -- to be updated when a new extension is made available
// ----------------------------------------

// interface for the overall extensions
class scv_extensions_if : public _SCV_INTROSPECTION_BASE {
public:
  static int get_debug();
  static void set_debug(int);
// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  scv_extensions_if():is_shared_vector_elem(0), max_vector_size(0), auxiliary_data(NULL) {}

  void set_aux_data(void *new_aux_data) { auxiliary_data = new_aux_data; }
  void *get_aux_data() { return auxiliary_data; }

public:
  int is_shared_vector_elem; // selectively deleting data when destructors are called
                         // while re-allocating memory for resizing STL vectors.
protected:
  unsigned max_vector_size; // For use by vector<T>'s only.
  void *auxiliary_data; // User attachable opaque auxiliary data (uninterpreted)
// END MODELSIM ADDITIONS [MDE-SCV]
};

#undef _SCV_INTROSPECTION_BASE
#define _SCV_INTROSPECTION_BASE scv_extensions_if


#ifndef _SCV_INTROSPECTION_ONLY
#include "scv_expression.h"
#else
class scv_expression_core_base {};
template<typename T>
class scv_expression_core : public scv_expression_core_base { 
public:
  scv_expression_core(scv_extensions_if*) {}
};
class scv_expression {
public:
  scv_expression() {}
  scv_expression(scv_expression_core_base*) {}
};
#endif


// implementation details
#include "_scv_ext_comp.h"

// to be used as base class of your composite type
template<typename T>
class scv_extensions_base : public _SCV_INTROSPECTION_BASE {
public:
  virtual ~scv_extensions_base() {}
};

// to be used as base class of your enum type
template<typename T>
class scv_enum_base : public _SCV_INTROSPECTION_BASE_ENUM {
public:
  scv_enum_base() {}
  virtual ~scv_enum_base() {
// BEGIN MODELSIM MODIFICATIONS [MDE-SCV]
    if (!this->is_shared_vector_elem && this->_has_dynamic_data() && this->_get_dynamic_data()->dist_)
      delete _get_distribution();
// END MODELSIM MODIFICATIONS [MDE-SCV]
  }
  virtual list<const char *>& _get_names() const { static list<const char *> _names; return _names; }
  virtual list<int>& _get_values() const { static list<int> _values; return _values; }
  virtual int get_bitwidth() const { return 8*sizeof(T); }
  T read() const { return (T) _SCV_INTROSPECTION_BASE_ENUM::read(); }
  void write(const T rhs) { _SCV_INTROSPECTION_BASE_ENUM::write((int) rhs); }
  void _set_instance(T *p) { _SCV_INTROSPECTION_BASE_ENUM::_set_instance((int*)p); }
  void _set_as_field(_scv_extension_util_record *parent,
		     T *p, const string& name) {
    _SCV_INTROSPECTION_BASE_ENUM::_set_as_field(parent,(int*)p,name);
  }
  T *_get_instance() const { return (T*)_instance; }
  T *get_instance() {
    _scv_message::message(_scv_message::INTROSPECTION_GET_INSTANCE_USAGE);
    return (T *) _instance;
  }
  const T *get_instance() const { return (T*)_instance; }
  virtual _scv_distribution<T> *_get_distribution() {         
    return (_scv_distribution<T> *) _get_dynamic_data()->dist_;
  }                                                                    
  void _set_distribution(_scv_distribution<T>* d) {
    _scv_distribution<T> * dist = _get_distribution();
    _scv_constraint_data * cdata = get_constraint_data();
    if (!dist) { 
      this->_get_dynamic_data()->dist_ = new _scv_distribution<T>; 
      dist = _get_distribution();
    } else {
      dist->reset_distribution();
    }
    if (d->dist_) {
      dist->set_mode(*d->dist_, cdata, this);
    } else if (d->dist_r_) {
      dist->set_mode(*d->dist_r_, cdata, this);
    } else {
      _scv_message::message(_scv_message::INTERNAL_ERROR, "_set_distribution(enum)");
    }
  }
  void set_distribution_from(scv_extensions_if* e) {
    _scv_distribution<T> *dist = (_scv_distribution<T>*)
      e->get_dynamic_data()->dist_;
    _set_distribution(dist);
  }
  void set_mode(scv_bag<pair<T, T> >& d){               
    _reset_keep_only_distribution();                                   
    if (!_get_distribution())                                          
      _get_dynamic_data()->dist_ = new _scv_distribution<T>;   
    _get_distribution()->set_mode(d,this);                             
  }                                                                    
  void set_mode(scv_bag<T>& d) { 
    _reset_keep_only_distribution(); 
    if (!_get_distribution())       
      _get_dynamic_data()->dist_ = new _scv_distribution<T>;   
    _get_distribution()->set_mode(d,this);                             
  }                                                                    
  void set_mode(scv_extensions_if::mode_t t) {                          
    if (!_get_distribution()) 
      _get_dynamic_data()->dist_ = new _scv_distribution<T>;  
    if (!check_mode(t, this, get_name(), _get_distribution()))   
      return;
    else 
      get_constraint_data()->set_ext_mode(t, 0, 0); 
  }
  virtual void _reset_bag_distribution() {                             
    if (_get_distribution()) 
      _get_distribution()->reset_distribution();  
  }
  virtual void generate_value_() {                                     
    if (!_get_distribution()) 
      _get_dynamic_data()->dist_ = new _scv_distribution<T>; 
    _get_distribution()->generate_value_(this,get_constraint_data()); 
    return;                                                            
  }                                                                    
  void keep_only(const T& value)  {                            
    keep_only(value, value);  
  }                                                                    
  void keep_only(const T& lb, const T& ub) {           
    _reset_bag_distribution(); 
    _scv_keep_range(this, lb, ub, false); 
  }                                                                    
  void keep_only(const list<T>& vlist) {                       
    _reset_bag_distribution(); 
    _scv_keep_range_list_enum(this, vlist, false);
  }                                                                    
  void keep_out(const T& value){                               
    _reset_bag_distribution();
    keep_out(value, value); 
  }                                                                    
  void keep_out(const T& lb, const T& ub) {            
    _reset_bag_distribution();  
    _scv_keep_range(this, lb, ub, true); 
  }                                                                    
  void keep_out(const list<T>& vlist) {                        
    _reset_bag_distribution(); 
    _scv_keep_range_list_enum(this, vlist, true);  
  }                                                                    
};

// to be specialized for user-specified datatype
template<typename T>
class scv_extensions : public scv_extensions_base<T> {
public:
  scv_extensions() {
    // this class should never be instantiated because
    // only specializations of this template is instantiated
    _scv_message::message(_scv_message::INTROSPECTION_INVALID_EXTENSIONS);
  }
};



#define _SCV_PAREN_OPERATOR(typename)  \
  scv_expression operator()() {                                     \
    return scv_expression(new scv_expression_core(this));\
  }                                                                \



// supporting macros
#define SCV_EXTENSIONS(type_name)                                        \
  template<>                                                             \
  class scv_extensions<type_name> : public scv_extensions_base<type_name>\

#define SCV_EXTENSIONS_CTOR(type_name)                                   \
  virtual const char *get_type_name() const {                            \
    static const char *s = strdup(#type_name);                           \
    return s;                                                            \
  }                                                                      \
  scv_extensions() { this->_set_instance(NULL); }                              \
  scv_extensions(const scv_extensions& rhs) {                            \
    this->_set_instance(NULL);                                                 \
    this->_set_instance((type_name*)rhs._get_instance());                      \
  }                                                                      \
  virtual ~scv_extensions() {}                                           \
  scv_extensions& operator=(const scv_extensions& rhs) {                 \
    write(*rhs._get_instance()); return *this;                           \
  }                                                                      \
  scv_extensions& operator=(const type_name& rhs) {                      \
    write(rhs); return *this;                                            \
  }                                                                      \
  operator const type_name&() const { return *(type_name*)this->_get_instance(); } \
  _SCV_PAREN_OPERATOR(typename);                                         \
/* BEGIN MODELSIM ADDITIONS [MDE-SCV] */                                 \
   const type_name *get_instance() const {                               \
      return (type_name*)this->_get_instance(); }                        \
  type_name *get_instance() {                                            \
      return (type_name*)this->_get_instance(); }                        \
/* END MODELSIM ADDITIONS [MDE-SCV] */                                   \
  virtual void _set_instance_core_wrap(void *p) { _set_instance_core((type_name*)p); } \
  void _set_instance_core(type_name *_scv_object_with_introspection) \

#define SCV_FIELD(field_name)                                            \
  string field_name ## _name = #field_name;                       \
  field_name._set_as_field(this,_scv_object_with_introspection?(&_scv_object_with_introspection->field_name):0,field_name ## _name);   \

#define SCV_EXTENSIONS_BASE_CLASS(class_name) \
  scv_extensions<class_name>::_set_instance_core(_scv_object_with_introspection); \

#define SCV_ENUM_EXTENSIONS(type_name)                                   \
  template<>                                                             \
  class scv_extensions<type_name> : public scv_enum_base<type_name>      \

#define SCV_ENUM_CTOR(type_name)                                         \
  virtual const char *get_type_name() const {                           \
    static const char *s = strdup(#type_name);                           \
    return s;                                                            \
  }                                                                      \
  scv_extensions() { static bool dummy = _init(); if (0) cout << dummy;} \
  scv_extensions(const scv_extensions& rhs) {                            \
    _set_instance(rhs._get_instance());                                  \
  }                                                                      \
  virtual ~scv_extensions() {}                                           \
  scv_extensions& operator=(const scv_extensions& rhs) {                 \
    write(*rhs._get_instance()); return *this;                           \
  }                                                                      \
  scv_extensions& operator=(type_name rhs) {                             \
    write(rhs); return *this;                                            \
  }                                                                      \
  operator type_name() const { return *(type_name*)_get_instance(); }    \
  _SCV_PAREN_OPERATOR(typename);                                         \
  bool _init() { __init(); return true; }                                \
  void __init()                                                          \

#define SCV_ENUM(element_name)                                           \
  _set_enum((int)element_name,#element_name);                            \


// convenient functions for extensions 
#if !defined( __HP_aCC )
template<typename T>
ostream& operator<<(ostream& os, const scv_extensions<T>& data) {
  os << *data._get_instance(); return os;
}
#endif

// implementation details
#include "_scv_introspection.h"

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
extern scv_smart_ptr_if* extn_cntxt;
// END MODELSIM ADDITIONS [MDE-SCV]

// ----------------------------------------
// various access interface to access an extension object
//
// scv_get_const_extensions is there to get around HP ambituous
// overloaded function call problem.
// ----------------------------------------
template <typename T>
scv_extensions<T> scv_get_extensions(T& d);

template <typename T>
const scv_extensions<T> scv_get_const_extensions(const T& d);

// ----------------------------------------
// the scv_smart_ptr template
// ----------------------------------------
class scv_smart_ptr_if : public scv_object_if {
public:
  virtual scv_extensions_if *get_extensions_ptr() = 0;
  virtual const scv_extensions_if *get_extensions_ptr() const = 0;
};

template <typename T>
class scv_smart_ptr : public scv_smart_ptr_if {
public:
  scv_extensions<T>& operator*() { return *tmp_; }
  const scv_extensions<T>& operator*() const { return *tmp_; }
// BEGIN MODELSIM MODIFICATIONS [MDE-SCV]
  scv_extensions<T> *operator->() { extn_cntxt = this; return tmp_; }
  const scv_extensions<T> *operator->() const { extn_cntxt = this; return tmp_; }
// BEGIN MODELSIM MODIFICATIONS [MDE-SCV]
  scv_expression operator()() { return (*tmp_)(); }
  const T& read() const { tmp_->initialize(); return *data_; }
  void write(const T& rhs) { *tmp_ = rhs; }

public:
  scv_smart_ptr();
  scv_smart_ptr(const string& name);
  scv_smart_ptr(const char *name);
  scv_smart_ptr(T *data);
  scv_smart_ptr(T *data, const string& name);
  scv_smart_ptr(T *data, const char *name);
  scv_smart_ptr(const scv_smart_ptr<T>& rhs); 
  scv_smart_ptr(
    scv_shared_ptr<T> data,
    scv_shared_ptr< scv_extensions<T> > ext,
    scv_extensions<T> *tmp
  );
  virtual ~scv_smart_ptr();

public:
// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  scv_shared_ptr<T > _get_data_() const { return data_; }
  scv_shared_ptr<scv_extensions<T > > _get_ext_() const { return ext_; }
  scv_extensions<T > *_get_tmp_() const { return tmp_; }
// END MODELSIM ADDITIONS [MDE-SCV]

  template<typename T2>
  operator scv_smart_ptr<T2> () { return scv_smart_ptr<T2>(data_,ext_,tmp_); }

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  template<typename T2>
  scv_smart_ptr(const scv_smart_ptr<T2>& src)
    : data_(src._get_data_()), ext_(src._get_ext_()), tmp_(src._get_tmp_()) {}
// END MODELSIM ADDITIONS [MDE-SCV]

public:
  virtual scv_extensions_if *get_extensions_ptr() { return tmp_; }
  virtual const scv_extensions_if *get_extensions_ptr() const { return tmp_; }

public:
  scv_smart_ptr& operator=(const scv_smart_ptr& rhs);

public:
  virtual const char *get_name() const { return tmp_->get_name(); }
  virtual const char *kind() const { return tmp_->kind(); }

  virtual void print(ostream& o=scv_out, int details=0, int indent=0) const {
    tmp_->print(o,details,indent);
  }
  virtual void show(int details=0, int indent=0) const {
    tmp_->show(details,indent);
  }

public: // private
  const T *get_value() const { return &*data_; }
  T *get_value() { return &*data_; }

private:
  scv_smart_ptr(scv_shared_ptr<T> data);
  scv_shared_ptr<T> data_;
  scv_shared_ptr< scv_extensions<T> > ext_;
  scv_extensions<T> *tmp_;

  friend class scv_constraint_base;
  void init();
};

// implementation details
#include "_scv_smart_ptr.h"

#endif
