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

  _scv_introspection.h -- The main implementation for the introspection
  facility.

  Original Authors (Cadence Design Systems, Inc):
  Norris Ip, Dean Shea, John Rose, Jasvinder Singh, William Paulsen,
  John Pierce, Rachida Kebichi, Ted Elkind, David Bailey
  2002-09-23

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scv/_scv_introspection.h#1 $

 *****************************************************************************/

// ----------------------------------------
// final definition of the extension classes for builtin C/C++ types
// ----------------------------------------

#if !( defined( __HP_aCC ) || defined(__linux__) )
#define _SCV_OSTREAM(typename) \
 friend ostream& operator<<(ostream& os,                            \
			     const scv_extensions< typename >& e) { \
    const_cast<scv_extensions< typename >& >(e).initialize(); \
    os << *e._get_instance(); return os; \
  }
#else
#define _SCV_OSTREAM(typename)
#endif

template<>
class scv_extensions<bool>
  : public scv_extensions_base<bool> {
public:
  scv_extensions<bool>& operator=(bool b)
    { *_get_instance() = b; trigger_value_change_cb(); return *this; }
  operator bool() const {
    const_cast<scv_extensions<bool> * >(this)->initialize();
    return *_get_instance();
  }
  _SCV_OSTREAM(bool)
  _SCV_PAREN_OPERATOR(typename)
};

#define _SCV_INTEGER_INTERFACE(typename) \
public:                                                             \
  scv_extensions< typename >& operator=(const scv_extensions<typename>& i) { \
    *_get_instance() = *(i._get_instance()); trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator=(typename i) { \
    *_get_instance() = i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator+=(typename i) { \
    *_get_instance() += i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator-=(typename i) { \
    *_get_instance() -= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator*=(typename i) { \
    *_get_instance() *= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator/=(typename i) { \
    *_get_instance() /= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator%=(typename i) { \
    *_get_instance() %= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator&=(typename i) { \
    *_get_instance() &= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator|=(typename i) { \
    *_get_instance() |= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator^=(typename i) { \
    *_get_instance() ^= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator<<=(typename i) { \
    *_get_instance() <<= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator>>=(typename i) { \
    *_get_instance() >>= i; trigger_value_change_cb(); return *this; \
  } \
  scv_extensions< typename >& operator++() { \
    ++*_get_instance(); trigger_value_change_cb(); return *this; \
  } \
  typename operator++(int) { \
    typename tmp = *_get_instance(); \
    ++*_get_instance(); trigger_value_change_cb(); return tmp; \
  } \
  scv_extensions< typename >& operator--() { \
    --*_get_instance(); trigger_value_change_cb(); return *this; \
  } \
  typename operator--(int) { \
    typename tmp = *_get_instance(); \
    --*_get_instance(); trigger_value_change_cb(); return tmp; \
  } \
  operator typename() const { \
    const_cast<scv_extensions< typename > * >(this)->initialize(); \
    return *_get_instance(); \
  } \
  _SCV_OSTREAM(typename) \
  _SCV_PAREN_OPERATOR(typename) \

// for all C/C++ builtin integer types
#define _SCV_TAG_FINAL_COMPONENT(typename) \
template<> \
class scv_extensions< typename > \
  : public scv_extensions_base< typename > { \
  _SCV_INTEGER_INTERFACE(typename) \
};

_SCV_TAG_FINAL_COMPONENT(char)
_SCV_TAG_FINAL_COMPONENT(unsigned char)
_SCV_TAG_FINAL_COMPONENT(short)
_SCV_TAG_FINAL_COMPONENT(unsigned short)
_SCV_TAG_FINAL_COMPONENT(int)
_SCV_TAG_FINAL_COMPONENT(unsigned int)
_SCV_TAG_FINAL_COMPONENT(mtiLongT)
_SCV_TAG_FINAL_COMPONENT(mtiUlongT)
_SCV_TAG_FINAL_COMPONENT(long long)
_SCV_TAG_FINAL_COMPONENT(unsigned long long)

#undef _SCV_TAG_FINAL_COMPONENT

// for all C/C++ builtin floating point type types
#define _SCV_TAG_FINAL_COMPONENT(typename) \
template<> \
class scv_extensions< typename > \
  : public scv_extensions_base< typename > { \
public:                                                            \
  scv_extensions< typename >& operator=(const scv_extensions<typename>& i) { \
    *_get_instance() = *(i._get_instance()); trigger_value_change_cb(); return *this; \
  }                                                                 \
  scv_extensions< typename >& operator=(typename i) { \
    *_get_instance() = i; trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator+=(typename i) { \
    *_get_instance() += i; trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator-=(typename i) { \
    *_get_instance() -= i; trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator*=(typename i) { \
    *_get_instance() *= i; trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator/=(typename i) { \
    *_get_instance() /= i; trigger_value_change_cb(); return *this; \
  }                                                                \
  operator typename() const { \
    const_cast<scv_extensions< typename > * >(this)->initialize(); \
    return *_get_instance(); \
  }                                                                \
  _SCV_OSTREAM(typename); \
  _SCV_PAREN_OPERATOR(typename); \
}; \

_SCV_TAG_FINAL_COMPONENT(float);
_SCV_TAG_FINAL_COMPONENT(double);

template<>
class scv_extensions<string>
  : public scv_extensions_base<string> {
public:
  scv_extensions< string >& operator=(const scv_extensions< string >& i) { \
    *_get_instance() = *(i._get_instance()); trigger_value_change_cb(); return *this; \
  }                                                                 \
  scv_extensions<string>& operator=(const string& s) {
    *_get_instance() = s; trigger_value_change_cb(); return *this; 
  }
  scv_extensions<string>& operator=(const char * s) {
    *_get_instance() = s; trigger_value_change_cb(); return *this; 
  }
  _SCV_OSTREAM(string);
};


#undef _SCV_TAG_FINAL_COMPONENT

#undef _SCV_INTEGER_INTERFACE

#define _SCV_INTEGER_INTERFACE(typename) \
public:                                                            \
  scv_extensions< typename >& operator=(const scv_extensions< typename >& i) { \
    *this->_get_instance() = *(i._get_instance()); this->trigger_value_change_cb(); return *this; \
  }                                                                 \
  scv_extensions< typename >& operator=(typename i) { \
    *this->_get_instance() = i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator+=(typename i) { \
    *this->_get_instance() += i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator-=(typename i) { \
    *this->_get_instance() -= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator*=(typename i) { \
    *this->_get_instance() *= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator/=(typename i) { \
    *this->_get_instance() /= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator%=(typename i) { \
    *this->_get_instance() %= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator&=(typename i) { \
    *this->_get_instance() &= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator|=(typename i) { \
    *this->_get_instance() |= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator^=(typename i) { \
    *this->_get_instance() ^= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator<<=(typename i) { \
    *this->_get_instance() <<= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator>>=(typename i) { \
    *this->_get_instance() >>= i; this->trigger_value_change_cb(); return *this; \
  }                                                                \
  scv_extensions< typename >& operator++() { \
    ++*this->_get_instance(); this->trigger_value_change_cb(); return *this; \
  }                                                                \
  typename operator++(int) { \
    typename tmp = *this->_get_instance(); \
    ++*this->_get_instance(); this->trigger_value_change_cb(); return tmp; \
  }                                                                \
  scv_extensions< typename >& operator--() { \
    --*this->_get_instance(); this->trigger_value_change_cb(); return *this; \
  }                                                                \
  typename operator--(int) { \
    typename tmp = *this->_get_instance(); \
    --*this->_get_instance(); this->trigger_value_change_cb(); return tmp; \
  }                                                                \
  operator typename() const { \
    const_cast<scv_extensions< typename > * >(this)->initialize(); \
    return *this->_get_instance(); \
  }                                                                \
  _SCV_OSTREAM(typename); \
  _SCV_PAREN_OPERATOR(typename); \


// for all SystemC templated types
#define _SCV_TAG_FINAL_COMPONENT(typename) \
template<int N> \
class scv_extensions< typename > \
  : public scv_extensions_base< typename > { \
  _SCV_INTEGER_INTERFACE(typename); \
};

#ifdef TEST_NEST_TEMPLATE
_SCV_TAG_FINAL_COMPONENT(test_uint<N> );
#endif

#ifdef SYSTEMC_H

// _SCV_TAG_FINAL_COMPONENT(sc_fixed);
// _SCV_TAG_FINAL_COMPONENT(sc_ufixed);

#undef _SCV_TAG_FINAL_COMPONENT

#define _SCV_IMPL { *this->_get_instance() = v; this->trigger_value_change_cb(); return *this; }
#define _SCV_IMPL1 { *this->_get_instance() = *(v._get_instance()); this->trigger_value_change_cb(); return *this; }
#define _SCV_IMPL2(op) { this->initialize(); *this->_get_instance() op v; this->trigger_value_change_cb(); return *this; }
#define _SCV_IMPL3(op) { u.initialize(); v.initialize(); return *u._get_instance() op *v._get_instance(); }
#define _SCV_IMPL4(op) { u.initialize(); return *u._get_instance() op v; }
#define _SCV_IMPL5(op) { v.initialize(); return u op *v._get_instance(); }
#define _SCV_MAP(return_type,method) return_type method() const { this->initialize(); return this->_get_instance()->method(); }

#if SC_VERSION > 2000000
using namespace sc_dt;
#endif

#define _SCV_BASE_ASSIGN(src_type) \
  return_type& operator=(src_type i) \
    { *this->_get_instance() = i; this->trigger_value_change_cb(); return *this; } \

#define _SCV_SIGNED_SELFOP(op,src_type) \
  return_type& operator op (src_type i) \
    { this->initialize(); *this->_get_instance() += i; this->trigger_value_change_cb(); return *this; } \

#define _SCV_SIGNED_SELFOPS(op) \
  _SCV_SIGNED_SELFOP(op,const sc_signed&) \
  _SCV_SIGNED_SELFOP(op,const sc_unsigned&) \
  _SCV_SIGNED_SELFOP(op,int64) \
  _SCV_SIGNED_SELFOP(op,uint64) \
  _SCV_SIGNED_SELFOP(op,mtiLongT) \
  _SCV_SIGNED_SELFOP(op,mtiUlongT) \
  _SCV_SIGNED_SELFOP(op,int) \
  _SCV_SIGNED_SELFOP(op,unsigned int) \
  _SCV_SIGNED_SELFOP(op,const sc_int_base&) \
  _SCV_SIGNED_SELFOP(op,const sc_uint_base&) \

#ifdef SC_INCLUDE_FX
# define _SCV_INT_FX_ASSIGN() \
    _SCV_BASE_ASSIGN(const sc_fxval&) \
    _SCV_BASE_ASSIGN(const sc_fxval_fast&) \
    _SCV_BASE_ASSIGN(const sc_fxnum&) \
    _SCV_BASE_ASSIGN(const sc_fxnum_fast&)
#else
# define _SCV_INT_FX_ASSIGN()
#endif

#ifdef SC_DT_DEPRECATED
# define _SCV_INT_DEPRECATED(typename) \
    _SCV_MAP(int,to_signed) \
    _SCV_MAP(unsigned,to_unsigned)
#else
# define _SCV_INT_DEPRECATED(typename)
#endif

#define _SCV_SIGNED_INTERFACE(typename) \
public: \
  operator const typename&() const { this->initialize(); return *this->_get_instance(); } \
  typedef scv_extensions< typename > return_type; \
  return_type& operator=(const return_type& i) \
    { *this->_get_instance() = *(i._get_instance()); this->trigger_value_change_cb(); return *this; } \
  _SCV_BASE_ASSIGN(const sc_signed&) \
  _SCV_BASE_ASSIGN(const sc_signed_subref&) \
  _SCV_BASE_ASSIGN(const sc_unsigned&) \
  _SCV_BASE_ASSIGN(const sc_unsigned_subref&) \
  _SCV_BASE_ASSIGN(const char*) \
  _SCV_BASE_ASSIGN(int64) \
  _SCV_BASE_ASSIGN(uint64) \
  _SCV_BASE_ASSIGN(mtiLongT) \
  _SCV_BASE_ASSIGN(mtiUlongT) \
  _SCV_BASE_ASSIGN(int) \
  _SCV_BASE_ASSIGN(unsigned int) \
  _SCV_BASE_ASSIGN(double) \
  _SCV_BASE_ASSIGN(const sc_int_base&) \
  _SCV_BASE_ASSIGN(const sc_uint_base&) \
  _SCV_BASE_ASSIGN(const sc_bv_base&) \
  _SCV_BASE_ASSIGN(const sc_lv_base&) \
  _SCV_INT_FX_ASSIGN() \
  return_type& operator++() \
    { ++*this->_get_instance(); this->trigger_value_change_cb(); return *this; } \
  typename operator++(int) { \
    typename tmp = *this->_get_instance(); \
    ++*this->_get_instance(); this->trigger_value_change_cb(); return tmp; \
  } \
  return_type& operator--() \
    { --*this->_get_instance(); this->trigger_value_change_cb(); return *this; } \
  typename operator--(int) { \
    typename tmp = *this->_get_instance(); \
    --*this->_get_instance(); this->trigger_value_change_cb(); return tmp; \
  } \
  _SCV_MAP(int,to_int) \
  _SCV_MAP(unsigned int,to_uint) \
  _SCV_MAP(mtiLongT,to_long) \
  _SCV_MAP(mtiUlongT,to_ulong) \
  _SCV_MAP(int64,to_int64) \
  _SCV_MAP(uint64,to_uint64) \
  _SCV_MAP(double,to_double) \
  _SCV_INT_DEPRECATED(typename) \
  const std::string to_string(sc_numrep numrep=SC_DEC) const \
    { this->initialize(); return this->_get_instance()->to_string(numrep); } \
  const std::string to_string(sc_numrep numrep, bool w_prefix) const \
    { this->initialize(); return this->_get_instance()->to_string(numrep,w_prefix); } \
  void scan( istream& is = cin ) \
    { this->_get_instance()->scan(is); this->trigger_value_change_cb(); } \
  void dump( ostream& os = cout ) const \
    { this->initialize(); this->_get_instance()->dump(os); } \
  _SCV_MAP(int,length) \
  _SCV_MAP(bool,iszero) \
  _SCV_MAP(bool,sign) \
  bool test(int i) const \
    { this->initialize(); return this->_get_instance()->test(i); } \
  void set(int i) \
    { this->initialize(); this->_get_instance()->set(i); this->trigger_value_change_cb(); } \
  void clear(int i) \
    { this->initialize(); this->_get_instance()->clear(i); this->trigger_value_change_cb(); } \
  void set(int i, bool v) \
    { this->initialize(); this->_get_instance()->set(i,v); this->trigger_value_change_cb(); } \
  void invert(int i) \
    { this->initialize(); this->_get_instance()->invert(i); this->trigger_value_change_cb(); } \
  void reverse() \
    { this->initialize(); this->_get_instance()->reverse(); this->trigger_value_change_cb(); } \
  void get_packed_rep(sc_digit *buf) const \
    { this->initialize(); this->_get_instance()->get_packed_rep(buf); } \
  void set_packed_rep(sc_digit *buf) \
    { this->initialize(); this->_get_instance()->set_packed_rep(buf); this->trigger_value_change_cb(); } \
  _SCV_SIGNED_SELFOPS(+=) \
  _SCV_SIGNED_SELFOPS(-=) \
  _SCV_SIGNED_SELFOPS(*=) \
  _SCV_SIGNED_SELFOPS(/=) \
  _SCV_SIGNED_SELFOPS(%=) \
  _SCV_SIGNED_SELFOPS(&=) \
  _SCV_SIGNED_SELFOPS(|=) \
  _SCV_SIGNED_SELFOPS(^=) \
  _SCV_SIGNED_SELFOPS(<<=) \
  _SCV_SIGNED_SELFOPS(>>=) \
  _SCV_OSTREAM(typename); \
  _SCV_PAREN_OPERATOR(typename); \

#define _SCV_TAG_FINAL_COMPONENT(typename) \
template<> \
class scv_extensions< typename > \
  : public scv_extensions_base< typename > { \
  _SCV_SIGNED_INTERFACE(typename) \
};

_SCV_TAG_FINAL_COMPONENT(sc_signed)
_SCV_TAG_FINAL_COMPONENT(sc_unsigned)

#undef _SCV_TAG_FINAL_COMPONENT

#define _SCV_INT_BASE_SELFOPS(op) \
  _SCV_SIGNED_SELFOP(op,int64) \
  _SCV_SIGNED_SELFOP(op,uint64) \
  _SCV_SIGNED_SELFOP(op,mtiLongT) \
  _SCV_SIGNED_SELFOP(op,mtiUlongT) \
  _SCV_SIGNED_SELFOP(op,int) \
  _SCV_SIGNED_SELFOP(op,unsigned int) \
  _SCV_SIGNED_SELFOP(op,const sc_int_base&) \
  _SCV_SIGNED_SELFOP(op,const sc_uint_base&) \

#define _SCV_INT_BASE_INTERFACE(typename) \
public: \
  operator const typename&() const { this->initialize(); return *this->_get_instance(); } \
  typedef scv_extensions< typename > return_type; \
  void invalid_length() const { this->initialize(); this->invalid_length(); } \
  void invalid_index(int i) const { this->initialize(); this->invalid_index(i); } \
  void invalid_range(int l, int r) const { this->initialize(); this->invalid_range(l,r); } \
  void check_length() const { this->initialize(); this->check_length(); } \
  void check_index(int i) const { this->initialize(); this->check_index(i); } \
  void check_range(int l, int r) const { this->initialize(); this->check_range(l,r); } \
  void extend_sign() { this->initialize(); this->extend_sign(); } \
  return_type& operator=(const return_type& i) \
    { *this->_get_instance() = *(i._get_instance()); this->trigger_value_change_cb(); return *this; } \
  _SCV_BASE_ASSIGN(int_type) \
  _SCV_BASE_ASSIGN(const sc_int_base&) \
  _SCV_BASE_ASSIGN(const sc_int_subref&) \
  _SCV_BASE_ASSIGN(const sc_signed&) \
  _SCV_BASE_ASSIGN(const sc_unsigned&) \
  _SCV_INT_FX_ASSIGN() \
  _SCV_BASE_ASSIGN(const sc_bv_base&) \
  _SCV_BASE_ASSIGN(const sc_lv_base&) \
  _SCV_BASE_ASSIGN(const char*) \
  _SCV_BASE_ASSIGN(mtiUlongT) \
  _SCV_BASE_ASSIGN(mtiLongT) \
  _SCV_BASE_ASSIGN(unsigned int) \
  _SCV_BASE_ASSIGN(int) \
  _SCV_BASE_ASSIGN(uint64) \
  _SCV_BASE_ASSIGN(double) \
  _SCV_SIGNED_SELFOP(+=,int_type) \
  _SCV_SIGNED_SELFOP(-=,int_type) \
  _SCV_SIGNED_SELFOP(*=,int_type) \
  _SCV_SIGNED_SELFOP(/=,int_type) \
  _SCV_SIGNED_SELFOP(%=,int_type) \
  _SCV_SIGNED_SELFOP(&=,int_type) \
  _SCV_SIGNED_SELFOP(|=,int_type) \
  _SCV_SIGNED_SELFOP(^=,int_type) \
  _SCV_SIGNED_SELFOP(<<=,int_type) \
  _SCV_SIGNED_SELFOP(>>=,int_type) \
  return_type& operator++() \
    { ++*this->_get_instance(); this->trigger_value_change_cb(); return *this; } \
  typename operator++(int) { \
    typename tmp = *this->_get_instance(); \
    ++*this->_get_instance(); this->trigger_value_change_cb(); return tmp; \
  } \
  return_type& operator--() \
    { --*this->_get_instance(); this->trigger_value_change_cb(); return *this; } \
  typename operator--(int) { \
    typename tmp = *this->_get_instance(); \
    --*this->_get_instance(); this->trigger_value_change_cb(); return tmp; \
  } \
  bool test(int i) const \
    { this->initialize(); return this->_get_instance()->test(i); } \
  void set(int i) \
    { this->initialize(); this->_get_instance()->set(i); this->trigger_value_change_cb(); } \
  void set(int i, bool v) \
    { this->initialize(); this->_get_instance()->set(i,v); this->trigger_value_change_cb(); } \
  _SCV_MAP(int,length) \
  _SCV_MAP(bool,and_reduce) \
  _SCV_MAP(bool,nand_reduce) \
  _SCV_MAP(bool,or_reduce) \
  _SCV_MAP(bool,nor_reduce) \
  _SCV_MAP(bool,xor_reduce) \
  _SCV_MAP(bool,xnor_reduce) \
  operator int_type() const { return this->value(); } \
  _SCV_MAP(int_type,value) \
  _SCV_MAP(int,to_int) \
  _SCV_MAP(unsigned int,to_uint) \
  _SCV_MAP(mtiLongT,to_long) \
  _SCV_MAP(mtiUlongT,to_ulong) \
  _SCV_MAP(int64,to_int64) \
  _SCV_MAP(uint64,to_uint64) \
  _SCV_MAP(double,to_double) \
  const std::string to_string(sc_numrep numrep=SC_DEC) const \
    { this->initialize(); return this->_get_instance()->to_string(numrep); } \
  const std::string to_string(sc_numrep numrep, bool w_prefix) const \
    { this->initialize(); return this->_get_instance()->to_string(numrep,w_prefix); } \
  void scan( istream& is = cin ) \
    { this->_get_instance()->scan(is); this->trigger_value_change_cb(); } \
  _SCV_OSTREAM(typename); \
  _SCV_PAREN_OPERATOR(typename); \

#define _SCV_TAG_FINAL_COMPONENT(typename) \
template<> \
class scv_extensions< typename > \
  : public scv_extensions_base< typename > { \
  _SCV_INT_BASE_INTERFACE(typename) \
};

_SCV_TAG_FINAL_COMPONENT(sc_int_base)
_SCV_TAG_FINAL_COMPONENT(sc_uint_base)

#undef _SCV_TAG_FINAL_COMPONENT

#define _SCV_BIT_BASE_INTERFACE(typename) \
public: \
  operator const typename&() const { this->initialize(); return *this->_get_instance(); } \
  typedef scv_extensions< typename > return_type; \
  return_type& operator=(const return_type& i) \
    { *this->_get_instance() = *(i._get_instance()); this->trigger_value_change_cb(); return *this; } \
  _SCV_BASE_ASSIGN(const typename&) \
  template <class X> \
  _SCV_BASE_ASSIGN(const sc_proxy<X>&) \
  _SCV_BASE_ASSIGN(const char*) \
  _SCV_BASE_ASSIGN(const bool*) \
  _SCV_BASE_ASSIGN(const sc_logic*) \
  _SCV_BASE_ASSIGN(const sc_unsigned&) \
  _SCV_BASE_ASSIGN(const sc_signed&) \
  _SCV_BASE_ASSIGN(const sc_uint_base&) \
  _SCV_BASE_ASSIGN(const sc_int_base&) \
  _SCV_BASE_ASSIGN(mtiUlongT) \
  _SCV_BASE_ASSIGN(mtiLongT) \
  _SCV_BASE_ASSIGN(unsigned int) \
  _SCV_BASE_ASSIGN(int) \
  _SCV_BASE_ASSIGN(uint64) \
  _SCV_BASE_ASSIGN(int64) \
  _SCV_MAP(int,length) \
  _SCV_MAP(int,size) \
  sc_logic_value_t get_bit(int i) const \
    { this->initialize(); return this->_get_instance()->get_bit(i); } \
  void set_bit(int i, sc_logic_value_t v) \
    { this->initialize(); this->_get_instance()->set_bit(i,v); this->trigger_value_change_cb(); } \
  mtiUlongT get_word(int i) const \
    { this->initialize(); return this->_get_instance()->get_word(i); } \
  void set_word(int i, mtiUlongT w) \
    { this->initialize(); this->_get_instance()->set_word(i,w); this->trigger_value_change_cb(); } \
  mtiUlongT get_cword(int i) const \
    { this->initialize(); return this->_get_instance()->get_cword(i); } \
  void set_cword(int i, mtiUlongT w) \
    { this->initialize(); this->_get_instance()->set_cword(i,w); this->trigger_value_change_cb(); } \
  void clean_tail() \
    { this->initialize(); this->_get_instance()->clean_tail(); this->trigger_value_change_cb(); } \
  _SCV_MAP(bool,is_01) \
  _SCV_OSTREAM(typename); \
  _SCV_PAREN_OPERATOR(typename); \

#define _SCV_TAG_FINAL_COMPONENT(typename) \
template<> \
class scv_extensions< typename > \
  : public scv_extensions_base< typename > { \
  _SCV_BIT_BASE_INTERFACE(typename) \
};

_SCV_TAG_FINAL_COMPONENT(sc_lv_base)
_SCV_TAG_FINAL_COMPONENT(sc_bv_base)

#undef _SCV_TAG_FINAL_COMPONENT


// sc_uint and sc_int are exactly the same as 
template<int W>
class scv_extensions< sc_uint<W> >
  : public scv_extensions_base< sc_uint<W> > {
public:
  _SCV_PAREN_OPERATOR(sc_uint<W>);
public:
  typedef scv_extensions< sc_uint<W> > return_type;

  return_type& operator=(const return_type& v) _SCV_IMPL1
  // from class sc_uint
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator = (uint_type                    v) _SCV_IMPL
#endif
  return_type& operator = (const sc_uint_base&          v) _SCV_IMPL
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator = (const sc_uint_subref&        v) _SCV_IMPL
#endif
//for 2.0.1beta, remove sc_uint_concat usage
#if SC_VERSION < 2000001
#ifndef _SCV_INTROSPECTION_ONLY
  template<class T1, class T2>
    return_type& operator = (const sc_uint_concat<T1,T2>& v) _SCV_IMPL
#endif
#endif
  return_type& operator = (const sc_signed&             v) _SCV_IMPL
  return_type& operator = (const sc_unsigned&           v) _SCV_IMPL
#ifdef SC_INCLUDE_FX
  return_type& operator = ( const sc_fxval&             v) _SCV_IMPL
  return_type& operator = ( const sc_fxval_fast&        v) _SCV_IMPL
  return_type& operator = ( const sc_fxnum&             v) _SCV_IMPL
  return_type& operator = ( const sc_fxnum_fast&        v) _SCV_IMPL
#endif
  return_type& operator = ( const sc_bv_base&           v) _SCV_IMPL
  return_type& operator = ( const sc_lv_base&           v) _SCV_IMPL 
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator += (uint_type v) _SCV_IMPL2(+=)
  return_type& operator -= (uint_type v) _SCV_IMPL2(-=)
  return_type& operator *= (uint_type v) _SCV_IMPL2(*=)
  return_type& operator /= (uint_type v) _SCV_IMPL2(/=) 
  return_type& operator %= (uint_type v) _SCV_IMPL2(%=)
  return_type& operator &= (uint_type v) _SCV_IMPL2(&=)
  return_type& operator |= (uint_type v) _SCV_IMPL2(|=)
  return_type& operator ^= (uint_type v) _SCV_IMPL2(^=)
  return_type& operator <<= (uint_type v) _SCV_IMPL2(<<=)
  return_type& operator >>= (uint_type v) _SCV_IMPL2(>>=)
#endif

  return_type& operator ++ () // prefix
  { this->initialize(); ++*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const return_type operator ++ (int) // postfix
  { this->initialize(); sc_uint<W> tmp = *this->_get_instance()++; this->trigger_value_change_cb(); return tmp; }
  return_type& operator -- () // prefix
  { this->initialize(); --*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const return_type operator -- (int) // postfix 
  { this->initialize(); sc_uint<W> tmp = *this->_get_instance()--; this->trigger_value_change_cb(); return tmp; }

  // from class sc_uint_base
#ifndef _SCV_INTROSPECTION_ONLY
  operator uint_type() const { this->initialize(); return this->_get_instance()->operator uint_type(); }
#endif

  _SCV_MAP(int,bitwidth);
  _SCV_MAP(int,length);
  _SCV_MAP(unsigned int,to_uint);
  _SCV_MAP(int,to_int);
  _SCV_MAP(uint64,to_uint64);
  _SCV_MAP(int64,to_int64);
#ifndef _32BIT_
  _SCV_MAP(mtiLongT,long_low);
  _SCV_MAP(mtiLongT,long_high);
#endif
  bool test(int i) const { this->initialize(); return this->_get_instance()->test(i); }
  void set(int i) { this->initialize(); this->_get_instance()->set(i); this->trigger_value_change_cb(); }
  void set(int i, bool v) { this->initialize(); this->_get_instance()->set(i,v); this->trigger_value_change_cb(); }
  // sc_uint_bitref operator [] (int i)
  bool operator [] (int i) const { this->initialize(); return this->_get_instance()->operator [](i); }
  //  sc_uint_subref range(int left, int right); 
#ifndef _SCV_INTROSPECTION_ONLY
  uint_type range(int left, int right) const { this->initialize(); return this->_get_instance()->range(left,right); }
#endif

  // operator ==, !=, <, <=, >, >= should be handled by uint_type();
  // operator +, -, etc. as well.

  //  void print( ostream& os ) const { this->initialize(); this->_get_instance()->print(os); }
};

template<int W>
class scv_extensions< sc_int<W> >
  : public scv_extensions_base< sc_int<W> > {
public:
    _SCV_PAREN_OPERATOR(sc_int<W>);
public:
  typedef scv_extensions< sc_int<W> > return_type;

  return_type& operator=(const return_type& v) _SCV_IMPL1
  // from class sc_int
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator = (int_type                     v) _SCV_IMPL
#endif
  return_type& operator = (const sc_int_base&           v) _SCV_IMPL
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator = (const sc_int_subref&         v) _SCV_IMPL
#endif
//for 2.0.1beta, remove sc_int_concat usage
#if SC_VERSION < 2000001
#ifndef _SCV_INTROSPECTION_ONLY
  template<class T1, class T2>
  return_type& operator = (const sc_int_concat<T1,T2>&  v) _SCV_IMPL
#endif
#endif
  return_type& operator = (const sc_signed&             v) _SCV_IMPL
  return_type& operator = (const sc_unsigned&           v) _SCV_IMPL
#ifdef SC_INCLUDE_FX
  return_type& operator = ( const sc_fxval&             v) _SCV_IMPL
  return_type& operator = ( const sc_fxval_fast&        v) _SCV_IMPL
  return_type& operator = ( const sc_fxnum&             v) _SCV_IMPL
  return_type& operator = ( const sc_fxnum_fast&        v) _SCV_IMPL
#endif
  return_type& operator = ( const sc_bv_base&           v) _SCV_IMPL
  return_type& operator = ( const sc_lv_base&           v) _SCV_IMPL 
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator += (int_type v) _SCV_IMPL2(+=)
  return_type& operator -= (int_type v) _SCV_IMPL2(-=)
  return_type& operator *= (int_type v) _SCV_IMPL2(*=)
  return_type& operator /= (int_type v) _SCV_IMPL2(/=) 
  return_type& operator %= (int_type v) _SCV_IMPL2(%=)
  return_type& operator &= (int_type v) _SCV_IMPL2(&=)
  return_type& operator |= (int_type v) _SCV_IMPL2(|=)
  return_type& operator ^= (int_type v) _SCV_IMPL2(^=)
  return_type& operator <<= (int_type v) _SCV_IMPL2(<<=)
  return_type& operator >>= (int_type v) _SCV_IMPL2(>>=)
#endif
  
  return_type& operator ++ () // prefix
  { this->initialize(); ++*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const return_type operator ++ (int) // postfix
  { this->initialize(); sc_int<W> tmp = *this->_get_instance()++; this->trigger_value_change_cb(); return tmp; }
  return_type& operator -- () // prefix
  { this->initialize(); --*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const return_type operator -- (int) // postfix 
  { this->initialize(); sc_int<W> tmp = *this->_get_instance()--; this->trigger_value_change_cb(); return tmp; }

  // from class sc_int_base
#ifndef _SCV_INTROSPECTION_ONLY
  operator int_type() const { this->initialize(); return this->_get_instance()->operator int_type(); }
#endif

  _SCV_MAP(int,bitwidth);
  _SCV_MAP(int,length);
  _SCV_MAP(unsigned int,to_uint);
  _SCV_MAP(int,to_int);
  _SCV_MAP(uint64,to_uint64);
  _SCV_MAP(int64,to_int64);
#ifndef _32BIT_
  _SCV_MAP(mtiLongT,long_low);
  _SCV_MAP(mtiLongT,long_high);
#endif
  bool test(int i) const { this->initialize(); return this->_get_instance()->test(i); }
  void set(int i) { this->initialize(); this->_get_instance()->set(i); this->trigger_value_change_cb(); }
  void set(int i, bool v) { this->initialize(); this->_get_instance()->set(i,v); this->trigger_value_change_cb(); }
  // sc_int_bitref operator [] (int i)
  bool operator [] (int i) const { this->initialize(); return this->_get_instance()->operator [](i); }
  //  sc_int_subref range(int left, int right); 
#ifndef _SCV_INTROSPECTION_ONLY
  int_type range(int left, int right) const { this->initialize(); return this->_get_instance()->range(left,right); }
#endif

  // operator ==, !=, <, <=, >, >= should be handled by int_type();
  // operator +, -, etc. as well.

  //  void print( ostream& os ) const { this->initialize(); this->_get_instance()->print(os); }
};

// sc_biguint and sc_bigint are exactly the same.
// need to add &=, etc.
template<int W>
class scv_extensions< sc_biguint<W> >
  : public scv_extensions_base< sc_biguint<W> > {
public:
    _SCV_PAREN_OPERATOR(sc_biguint<W>);
public:
  typedef scv_extensions< sc_biguint<W> > return_type;

  return_type& operator=(const return_type& v) _SCV_IMPL1
  return_type& operator=(const sc_biguint<W>&      v) _SCV_IMPL
  return_type& operator=(const sc_unsigned&        v) _SCV_IMPL
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator=(const sc_unsigned_subref& v) _SCV_IMPL
#endif
  return_type& operator=(const sc_signed&          v) _SCV_IMPL
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator=(const sc_signed_subref&   v) _SCV_IMPL
#endif
  return_type& operator=(const char*               v) _SCV_IMPL 
  return_type& operator=(int64                     v) _SCV_IMPL
  return_type& operator=(uint64                    v) _SCV_IMPL
  return_type& operator=(mtiLongT v) _SCV_IMPL
  return_type& operator=(mtiUlongT v) _SCV_IMPL
  return_type& operator=(int                       v) _SCV_IMPL 
  return_type& operator=(unsigned int              v) _SCV_IMPL 
  return_type& operator=(double                    v) _SCV_IMPL
  return_type& operator=( const sc_bv_base&        v) _SCV_IMPL
  return_type& operator=( const sc_lv_base&        v) _SCV_IMPL
  return_type& operator=( const sc_int_base&       v) _SCV_IMPL
  return_type& operator=( const sc_uint_base&      v) _SCV_IMPL
#ifdef SC_INCLUDE_FX
  return_type& operator = ( const sc_fxval& v ) _SCV_IMPL
  return_type& operator = ( const sc_fxval_fast& v ) _SCV_IMPL
  return_type& operator = ( const sc_fxnum& v ) _SCV_IMPL
  return_type& operator = ( const sc_fxnum_fast& v ) _SCV_IMPL
#endif
  return_type& operator += (const sc_signed&    v) _SCV_IMPL2(+=) 
  return_type& operator += (const sc_unsigned&  v) _SCV_IMPL2(+=) 
  return_type& operator += (int64               v) _SCV_IMPL2(+=) 
  return_type& operator += (uint64              v) _SCV_IMPL2(+=) 
  return_type& operator += (mtiLongT v) _SCV_IMPL2(+=) 
  return_type& operator += (mtiUlongT v) _SCV_IMPL2(+=) 
  return_type& operator += (int                 v) _SCV_IMPL2(+=) 
  return_type& operator += (unsigned int        v) _SCV_IMPL2(+=) 
  return_type& operator += (const sc_int_base&  v) _SCV_IMPL2(+=)
  return_type& operator += (const sc_uint_base& v) _SCV_IMPL2(+=)

  return_type& operator -= (const sc_signed&    v) _SCV_IMPL2(-=) 
  return_type& operator -= (const sc_unsigned&  v) _SCV_IMPL2(-=) 
  return_type& operator -= (int64               v) _SCV_IMPL2(-=) 
  return_type& operator -= (uint64              v) _SCV_IMPL2(-=) 
  return_type& operator -= (mtiLongT v) _SCV_IMPL2(-=) 
  return_type& operator -= (mtiUlongT v) _SCV_IMPL2(-=) 
  return_type& operator -= (int                 v) _SCV_IMPL2(-=) 
  return_type& operator -= (unsigned int        v) _SCV_IMPL2(-=) 
  return_type& operator -= (const sc_int_base&  v) _SCV_IMPL2(-=)
  return_type& operator -= (const sc_uint_base& v) _SCV_IMPL2(-=)

  return_type& operator *= (const sc_signed&    v) _SCV_IMPL2(*=) 
  return_type& operator *= (const sc_unsigned&  v) _SCV_IMPL2(*=) 
  return_type& operator *= (int64               v) _SCV_IMPL2(*=) 
  return_type& operator *= (uint64              v) _SCV_IMPL2(*=) 
  return_type& operator *= (mtiLongT v) _SCV_IMPL2(*=) 
  return_type& operator *= (mtiUlongT v) _SCV_IMPL2(*=) 
  return_type& operator *= (int                 v) _SCV_IMPL2(*=) 
  return_type& operator *= (unsigned int        v) _SCV_IMPL2(*=) 
  return_type& operator *= (const sc_int_base&  v) _SCV_IMPL2(*=)
  return_type& operator *= (const sc_uint_base& v) _SCV_IMPL2(*=)

  return_type& operator /= (const sc_signed&    v) _SCV_IMPL2(/=) 
  return_type& operator /= (const sc_unsigned&  v) _SCV_IMPL2(/=) 
  return_type& operator /= (int64               v) _SCV_IMPL2(/=) 
  return_type& operator /= (uint64              v) _SCV_IMPL2(/=) 
  return_type& operator /= (mtiLongT v) _SCV_IMPL2(/=) 
  return_type& operator /= (mtiUlongT v) _SCV_IMPL2(/=) 
  return_type& operator /= (int                 v) _SCV_IMPL2(/=) 
  return_type& operator /= (unsigned int        v) _SCV_IMPL2(/=) 
  return_type& operator /= (const sc_int_base&  v) _SCV_IMPL2(/=)
  return_type& operator /= (const sc_uint_base& v) _SCV_IMPL2(/=)

  return_type& operator %= (const sc_signed&    v) _SCV_IMPL2(%=) 
  return_type& operator %= (const sc_unsigned&  v) _SCV_IMPL2(%=) 
  return_type& operator %= (int64               v) _SCV_IMPL2(%=) 
  return_type& operator %= (uint64              v) _SCV_IMPL2(%=) 
  return_type& operator %= (mtiLongT v) _SCV_IMPL2(%=) 
  return_type& operator %= (mtiUlongT v) _SCV_IMPL2(%=) 
  return_type& operator %= (int                 v) _SCV_IMPL2(%=) 
  return_type& operator %= (unsigned int        v) _SCV_IMPL2(%=) 
  return_type& operator %= (const sc_int_base&  v) _SCV_IMPL2(%=)
  return_type& operator %= (const sc_uint_base& v) _SCV_IMPL2(%=)

  return_type& operator &= (const sc_signed&    v) _SCV_IMPL2(&=) 
  return_type& operator &= (const sc_unsigned&  v) _SCV_IMPL2(&=) 
  return_type& operator &= (int64               v) _SCV_IMPL2(&=) 
  return_type& operator &= (uint64              v) _SCV_IMPL2(&=) 
  return_type& operator &= (mtiLongT v) _SCV_IMPL2(&=) 
  return_type& operator &= (mtiUlongT v) _SCV_IMPL2(&=) 
  return_type& operator &= (int                 v) _SCV_IMPL2(&=) 
  return_type& operator &= (unsigned int        v) _SCV_IMPL2(&=) 
  return_type& operator &= (const sc_int_base&  v) _SCV_IMPL2(&=)
  return_type& operator &= (const sc_uint_base& v) _SCV_IMPL2(&=)

  return_type& operator |= (const sc_signed&    v) _SCV_IMPL2(|=) 
  return_type& operator |= (const sc_unsigned&  v) _SCV_IMPL2(|=) 
  return_type& operator |= (int64               v) _SCV_IMPL2(|=) 
  return_type& operator |= (uint64              v) _SCV_IMPL2(|=) 
  return_type& operator |= (mtiLongT v) _SCV_IMPL2(|=) 
  return_type& operator |= (mtiUlongT v) _SCV_IMPL2(|=) 
  return_type& operator |= (int                 v) _SCV_IMPL2(|=) 
  return_type& operator |= (unsigned int        v) _SCV_IMPL2(|=) 
  return_type& operator |= (const sc_int_base&  v) _SCV_IMPL2(|=)
  return_type& operator |= (const sc_uint_base& v) _SCV_IMPL2(|=)

  return_type& operator ^= (const sc_signed&    v) _SCV_IMPL2(^=) 
  return_type& operator ^= (const sc_unsigned&  v) _SCV_IMPL2(^=) 
  return_type& operator ^= (int64               v) _SCV_IMPL2(^=) 
  return_type& operator ^= (uint64              v) _SCV_IMPL2(^=) 
  return_type& operator ^= (mtiLongT v) _SCV_IMPL2(^=) 
  return_type& operator ^= (mtiUlongT v) _SCV_IMPL2(^=) 
  return_type& operator ^= (int                 v) _SCV_IMPL2(^=) 
  return_type& operator ^= (unsigned int        v) _SCV_IMPL2(^=) 
  return_type& operator ^= (const sc_int_base&  v) _SCV_IMPL2(^=)
  return_type& operator ^= (const sc_uint_base& v) _SCV_IMPL2(^=)

  return_type& operator <<= (const sc_signed&    v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (const sc_unsigned&  v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (int64               v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (uint64              v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (mtiLongT v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (mtiUlongT v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (int                 v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (unsigned int        v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (const sc_int_base&  v) _SCV_IMPL2(<<=)
  return_type& operator <<= (const sc_uint_base& v) _SCV_IMPL2(<<=)

  return_type& operator >>= (const sc_signed&    v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (const sc_unsigned&  v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (int64               v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (uint64              v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (mtiLongT v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (mtiUlongT v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (int                 v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (unsigned int        v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (const sc_int_base&  v) _SCV_IMPL2(>>=)
  return_type& operator >>= (const sc_uint_base& v) _SCV_IMPL2(>>=)

  return_type& operator ++ ()
  { this->initialize(); ++*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const sc_unsigned operator ++ (int)
  { this->initialize(); sc_biguint<W> tmp = *this->_get_instance()++; this->trigger_value_change_cb(); return tmp; }
  return_type& operator -- ()
  { this->initialize(); --*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const sc_unsigned operator -- (int)
  { this->initialize(); sc_biguint<W> tmp = *this->_get_instance()--; this->trigger_value_change_cb(); return tmp; }
  //  sc_unsigned_bitref operator [] (int i)
  const bool operator [] (int i) const
  { this->initialize(); return this->_get_instance()->operator [](i); }
  const sc_unsigned range(int i, int j) const
  { this->initialize(); return this->_get_instance()->range(i,j); }
  //  sc_unsigned_subref operator () (int i, int j) 
  const sc_unsigned operator () (int i, int j) const
  { this->initialize(); return this->_get_instance()->operator ()(i,j); }

  std::string to_string(sc_numrep base = SC_DEC, bool formatted = false) const
  { this->initialize(); return this->_get_instance()->to_string(base,formatted); }
  std::string to_string(int base, bool formatted = false) const
  { this->initialize(); return this->_get_instance()->to_string(base,formatted); }

  _SCV_MAP(int64,to_int64);  
  _SCV_MAP(uint64,to_uint64);  
  _SCV_MAP(mtiLongT,to_long);  
  _SCV_MAP(mtiUlongT,to_ulong);
  _SCV_MAP(mtiUlongT,to_unsigned_long);
  _SCV_MAP(int,to_int);
  _SCV_MAP(int,to_signed);
  _SCV_MAP(unsigned int,to_uint);
  _SCV_MAP(unsigned int,to_unsigned);
  _SCV_MAP(unsigned int,to_unsigned_int);
  _SCV_MAP(double,to_double);
  //  void print() const { this->initialize(); this->_get_instance()->print(); }
  //  void print(ostream &os) const { this->initialize(); this->_get_instance()->print(os); }
  void dump() const { this->initialize(); this->_get_instance()->dump(); };
  void dump(ostream &os) const { this->initialize(); this->_get_instance()->dump(os); };
  _SCV_MAP(int,length);
  _SCV_MAP(bool,iszero);
  _SCV_MAP(bool,sign);
  bool test(int i) const { this->initialize(); return this->_get_instance()->test(i); }
  void set(int i) { this->initialize(); this->_get_instance()->set(i); this->trigger_value_change_cb(); }
  void clear(int i) { this->initialize(); this->_get_instance()->clear(i); this->trigger_value_change_cb(); }
  void set(int i, bool v) { this->initialize(); this->_get_instance()->set(i,v); this->trigger_value_change_cb(); }
  void invert(int i) { this->initialize(); this->_get_instance()->invert(i); this->trigger_value_change_cb(); }
  void reverse() { this->initialize(); this->_get_instance()->reverse(); this->trigger_value_change_cb(); }
  void get_packed_rep(mtiUlongT *buf) const { this->initialize(); this->_get_instance()->get_packet_ref(buf); }
  void set_packed_rep(mtiUlongT *buf) { this->_get_instance()->get_packet_ref(buf); this->trigger_value_change_cb(); }

  operator const sc_unsigned&() const { this->initialize(); return *this->_get_instance(); }
};

template<int W>
class scv_extensions< sc_bigint<W> >
  : public scv_extensions_base< sc_bigint<W> > {
public:
    _SCV_PAREN_OPERATOR(sc_bigint<W>);
public:
  typedef scv_extensions< sc_bigint<W> > return_type;

  return_type& operator=(const return_type& v) _SCV_IMPL1
  return_type& operator=(const sc_bigint<W>&       v) _SCV_IMPL
  return_type& operator=(const sc_unsigned&        v) _SCV_IMPL
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator=(const sc_unsigned_subref& v) _SCV_IMPL
#endif
  return_type& operator=(const sc_signed&          v) _SCV_IMPL
#ifndef _SCV_INTROSPECTION_ONLY
  return_type& operator=(const sc_signed_subref&   v) _SCV_IMPL
#endif
  return_type& operator=(const char*               v) _SCV_IMPL 
  return_type& operator=(int64                     v) _SCV_IMPL
  return_type& operator=(uint64                    v) _SCV_IMPL
  return_type& operator=(mtiLongT v) _SCV_IMPL
  return_type& operator=(mtiUlongT v) _SCV_IMPL
  return_type& operator=(int                       v) _SCV_IMPL 
  return_type& operator=(unsigned int              v) _SCV_IMPL 
  return_type& operator=(double                    v) _SCV_IMPL
  return_type& operator=( const sc_bv_base&        v) _SCV_IMPL
  return_type& operator=( const sc_lv_base&        v) _SCV_IMPL
  return_type& operator=( const sc_int_base&       v) _SCV_IMPL
  return_type& operator=( const sc_uint_base&      v) _SCV_IMPL
#ifdef SC_INCLUDE_FX
  return_type& operator = ( const sc_fxval& v ) _SCV_IMPL
  return_type& operator = ( const sc_fxval_fast& v ) _SCV_IMPL
  return_type& operator = ( const sc_fxnum& v ) _SCV_IMPL
  return_type& operator = ( const sc_fxnum_fast& v ) _SCV_IMPL
#endif

  return_type& operator += (const sc_signed&    v) _SCV_IMPL2(+=) 
  return_type& operator += (const sc_unsigned&  v) _SCV_IMPL2(+=) 
  return_type& operator += (int64               v) _SCV_IMPL2(+=) 
  return_type& operator += (uint64              v) _SCV_IMPL2(+=) 
  return_type& operator += (mtiLongT v) _SCV_IMPL2(+=) 
  return_type& operator += (mtiUlongT v) _SCV_IMPL2(+=) 
  return_type& operator += (int                 v) _SCV_IMPL2(+=) 
  return_type& operator += (unsigned int        v) _SCV_IMPL2(+=) 
  return_type& operator += (const sc_int_base&  v) _SCV_IMPL2(+=)
  return_type& operator += (const sc_uint_base& v) _SCV_IMPL2(+=)

  return_type& operator -= (const sc_signed&    v) _SCV_IMPL2(-=) 
  return_type& operator -= (const sc_unsigned&  v) _SCV_IMPL2(-=) 
  return_type& operator -= (int64               v) _SCV_IMPL2(-=) 
  return_type& operator -= (uint64              v) _SCV_IMPL2(-=) 
  return_type& operator -= (mtiLongT v) _SCV_IMPL2(-=) 
  return_type& operator -= (mtiUlongT v) _SCV_IMPL2(-=) 
  return_type& operator -= (int                 v) _SCV_IMPL2(-=) 
  return_type& operator -= (unsigned int        v) _SCV_IMPL2(-=) 
  return_type& operator -= (const sc_int_base&  v) _SCV_IMPL2(-=)
  return_type& operator -= (const sc_uint_base& v) _SCV_IMPL2(-=)

  return_type& operator *= (const sc_signed&    v) _SCV_IMPL2(*=) 
  return_type& operator *= (const sc_unsigned&  v) _SCV_IMPL2(*=) 
  return_type& operator *= (int64               v) _SCV_IMPL2(*=) 
  return_type& operator *= (uint64              v) _SCV_IMPL2(*=) 
  return_type& operator *= (mtiLongT v) _SCV_IMPL2(*=) 
  return_type& operator *= (mtiUlongT v) _SCV_IMPL2(*=) 
  return_type& operator *= (int                 v) _SCV_IMPL2(*=) 
  return_type& operator *= (unsigned int        v) _SCV_IMPL2(*=) 
  return_type& operator *= (const sc_int_base&  v) _SCV_IMPL2(*=)
  return_type& operator *= (const sc_uint_base& v) _SCV_IMPL2(*=)

  return_type& operator /= (const sc_signed&    v) _SCV_IMPL2(/=) 
  return_type& operator /= (const sc_unsigned&  v) _SCV_IMPL2(/=) 
  return_type& operator /= (int64               v) _SCV_IMPL2(/=) 
  return_type& operator /= (uint64              v) _SCV_IMPL2(/=) 
  return_type& operator /= (mtiLongT v) _SCV_IMPL2(/=) 
  return_type& operator /= (mtiUlongT v) _SCV_IMPL2(/=) 
  return_type& operator /= (int                 v) _SCV_IMPL2(/=) 
  return_type& operator /= (unsigned int        v) _SCV_IMPL2(/=) 
  return_type& operator /= (const sc_int_base&  v) _SCV_IMPL2(/=)
  return_type& operator /= (const sc_uint_base& v) _SCV_IMPL2(/=)

  return_type& operator %= (const sc_signed&    v) _SCV_IMPL2(%=) 
  return_type& operator %= (const sc_unsigned&  v) _SCV_IMPL2(%=) 
  return_type& operator %= (int64               v) _SCV_IMPL2(%=) 
  return_type& operator %= (uint64              v) _SCV_IMPL2(%=) 
  return_type& operator %= (mtiLongT v) _SCV_IMPL2(%=) 
  return_type& operator %= (mtiUlongT v) _SCV_IMPL2(%=) 
  return_type& operator %= (int                 v) _SCV_IMPL2(%=) 
  return_type& operator %= (unsigned int        v) _SCV_IMPL2(%=) 
  return_type& operator %= (const sc_int_base&  v) _SCV_IMPL2(%=)
  return_type& operator %= (const sc_uint_base& v) _SCV_IMPL2(%=)

  return_type& operator &= (const sc_signed&    v) _SCV_IMPL2(&=) 
  return_type& operator &= (const sc_unsigned&  v) _SCV_IMPL2(&=) 
  return_type& operator &= (int64               v) _SCV_IMPL2(&=) 
  return_type& operator &= (uint64              v) _SCV_IMPL2(&=) 
  return_type& operator &= (mtiLongT v) _SCV_IMPL2(&=) 
  return_type& operator &= (mtiUlongT v) _SCV_IMPL2(&=) 
  return_type& operator &= (int                 v) _SCV_IMPL2(&=) 
  return_type& operator &= (unsigned int        v) _SCV_IMPL2(&=) 
  return_type& operator &= (const sc_int_base&  v) _SCV_IMPL2(&=)
  return_type& operator &= (const sc_uint_base& v) _SCV_IMPL2(&=)

  return_type& operator |= (const sc_signed&    v) _SCV_IMPL2(|=) 
  return_type& operator |= (const sc_unsigned&  v) _SCV_IMPL2(|=) 
  return_type& operator |= (int64               v) _SCV_IMPL2(|=) 
  return_type& operator |= (uint64              v) _SCV_IMPL2(|=) 
  return_type& operator |= (mtiLongT v) _SCV_IMPL2(|=) 
  return_type& operator |= (mtiUlongT v) _SCV_IMPL2(|=) 
  return_type& operator |= (int                 v) _SCV_IMPL2(|=) 
  return_type& operator |= (unsigned int        v) _SCV_IMPL2(|=) 
  return_type& operator |= (const sc_int_base&  v) _SCV_IMPL2(|=)
  return_type& operator |= (const sc_uint_base& v) _SCV_IMPL2(|=)

  return_type& operator ^= (const sc_signed&    v) _SCV_IMPL2(^=) 
  return_type& operator ^= (const sc_unsigned&  v) _SCV_IMPL2(^=) 
  return_type& operator ^= (int64               v) _SCV_IMPL2(^=) 
  return_type& operator ^= (uint64              v) _SCV_IMPL2(^=) 
  return_type& operator ^= (mtiLongT v) _SCV_IMPL2(^=) 
  return_type& operator ^= (mtiUlongT v) _SCV_IMPL2(^=) 
  return_type& operator ^= (int                 v) _SCV_IMPL2(^=) 
  return_type& operator ^= (unsigned int        v) _SCV_IMPL2(^=) 
  return_type& operator ^= (const sc_int_base&  v) _SCV_IMPL2(^=)
  return_type& operator ^= (const sc_uint_base& v) _SCV_IMPL2(^=)

  return_type& operator <<= (const sc_signed&    v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (const sc_unsigned&  v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (int64               v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (uint64              v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (mtiLongT v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (mtiUlongT v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (int                 v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (unsigned int        v) _SCV_IMPL2(<<=) 
  return_type& operator <<= (const sc_int_base&  v) _SCV_IMPL2(<<=)
  return_type& operator <<= (const sc_uint_base& v) _SCV_IMPL2(<<=)

  return_type& operator >>= (const sc_signed&    v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (const sc_unsigned&  v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (int64               v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (uint64              v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (mtiLongT v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (mtiUlongT v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (int                 v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (unsigned int        v) _SCV_IMPL2(>>=) 
  return_type& operator >>= (const sc_int_base&  v) _SCV_IMPL2(>>=)
  return_type& operator >>= (const sc_uint_base& v) _SCV_IMPL2(>>=)

  return_type& operator ++ ()
  { this->initialize(); ++*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const sc_unsigned operator ++ (int)
  { this->initialize(); sc_bigint<W> tmp = *this->_get_instance()++; this->trigger_value_change_cb(); return tmp; }
  return_type& operator -- ()
  { this->initialize(); --*this->_get_instance(); this->trigger_value_change_cb(); return *this; }
  const sc_unsigned operator -- (int)
  { this->initialize(); sc_bigint<W> tmp = *this->_get_instance()--; this->trigger_value_change_cb(); return tmp; }
  //  sc_unsigned_bitref operator [] (int i)
  const bool operator [] (int i) const
  { this->initialize(); return this->_get_instance()->operator [](i); }
  const sc_unsigned range(int i, int j) const
  { this->initialize(); return this->_get_instance()->range(i,j); }
  //  sc_unsigned_subref operator () (int i, int j) 
  const sc_unsigned operator () (int i, int j) const
  { this->initialize(); return this->_get_instance()->operator ()(i,j); }

  std::string to_string(sc_numrep base = SC_DEC, bool formatted = false) const
  { this->initialize(); return this->_get_instance()->to_string(base,formatted); }
  std::string to_string(int base, bool formatted = false) const
  { this->initialize(); return this->_get_instance()->to_string(base,formatted); }

  _SCV_MAP(int64,to_int64);  
  _SCV_MAP(uint64,to_uint64);  
  _SCV_MAP(mtiLongT,to_long);  
  _SCV_MAP(mtiUlongT,to_ulong);
  _SCV_MAP(mtiUlongT,to_unsigned_long);
  _SCV_MAP(int,to_int);
  _SCV_MAP(int,to_signed);
  _SCV_MAP(unsigned int,to_uint);
  _SCV_MAP(unsigned int,to_unsigned);
  _SCV_MAP(unsigned int,to_unsigned_int);
  _SCV_MAP(double,to_double);
  //  void print() const { this->initialize(); this->_get_instance()->print(); }
  //  void print(ostream &os) const { this->initialize(); this->_get_instance()->print(os); }
  void dump() const { this->initialize(); this->_get_instance()->dump(); };
  void dump(ostream &os) const { this->initialize(); this->_get_instance()->dump(os); };
  _SCV_MAP(int,length);
  _SCV_MAP(bool,iszero);
  _SCV_MAP(bool,sign);
  bool test(int i) const { this->initialize(); return this->_get_instance()->test(i); }
  void set(int i) { this->initialize(); this->_get_instance()->set(i); this->trigger_value_change_cb(); }
  void clear(int i) { this->initialize(); this->_get_instance()->clear(i); this->trigger_value_change_cb(); }
  void set(int i, bool v) { this->initialize(); this->_get_instance()->set(i,v); this->trigger_value_change_cb(); }
  void invert(int i) { this->initialize(); this->_get_instance()->invert(i); this->trigger_value_change_cb(); }
  void reverse() { this->initialize(); this->_get_instance()->reverse(); this->trigger_value_change_cb(); }
  void get_packed_rep(mtiUlongT *buf) const { this->initialize(); this->_get_instance()->get_packet_ref(buf); }
  void set_packed_rep(mtiUlongT *buf) { this->_get_instance()->get_packet_ref(buf); this->trigger_value_change_cb(); }

  operator const sc_signed&() const { this->initialize(); return *this->_get_instance(); }
};

template<>
class scv_extensions< sc_bit >
  : public scv_extensions_base< sc_bit > {
public:
    _SCV_PAREN_OPERATOR(sc_bit);
public:
  typedef scv_extensions< sc_bit > return_type;

  return_type& operator=(const return_type& v) _SCV_IMPL1 
  return_type& operator = ( const sc_bit& v ) _SCV_IMPL
  return_type& operator = ( int v ) _SCV_IMPL
  return_type& operator = ( bool v ) _SCV_IMPL
  return_type& operator = ( char v ) _SCV_IMPL
  //for 2.0.1beta, assignment from sc_logic to bit is not available
#if SC_VERSION < 2000001
  return_type& operator = ( const sc_logic& v ) _SCV_IMPL
#endif
  return_type& operator &= ( const sc_bit& v ) _SCV_IMPL2(&=)
  return_type& operator &= ( int v ) _SCV_IMPL2(&=)
  return_type& operator &= ( bool v ) _SCV_IMPL2(&=)
  return_type& operator &= ( char v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const sc_bit& v ) _SCV_IMPL2(|=)
  return_type& operator |= ( int v ) _SCV_IMPL2(|=)
  return_type& operator |= ( bool v ) _SCV_IMPL2(|=)
  return_type& operator |= ( char v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const sc_bit& v ) _SCV_IMPL2(^=)
  return_type& operator ^= ( int v ) _SCV_IMPL2(^=)
  return_type& operator ^= ( bool v ) _SCV_IMPL2(^=)
  return_type& operator ^= ( char v ) _SCV_IMPL2(^=)
  _SCV_MAP(bool,to_bool);
  _SCV_MAP(char,to_char);
  //  void print( ostream& os) const { this->initialize(); return this->_get_instance()->print(os); }
  operator const sc_bit&() const { this->initialize(); return *this->_get_instance(); }
};

template<>
class scv_extensions< sc_logic >
  : public scv_extensions_base< sc_logic > {
public:
    _SCV_PAREN_OPERATOR(sc_logic);
public:
  typedef scv_extensions< sc_logic > return_type;

  return_type& operator=(const return_type& v) _SCV_IMPL1
  //for 2.0.1beta, sc_logic::Log_enum is changed to sc_dt::sc_logic_value_t
#if SC_VERSION < 2000001
  return_type& operator = ( sc_logic::Log_enum v ) _SCV_IMPL
#else
  return_type& operator = ( sc_dt::sc_logic_value_t v ) _SCV_IMPL
#endif
  return_type& operator = ( const sc_logic& v ) _SCV_IMPL
  return_type& operator = ( char v ) _SCV_IMPL
  //for 2.0.1beta, no sc_logic::operator=(mtiLongT)
#if SC_VERSION < 2000001
  return_type& operator = (mtiLongT v ) _SCV_IMPL
#endif
  return_type& operator = ( int v ) _SCV_IMPL
  return_type& operator = ( bool v ) _SCV_IMPL
  return_type& operator &= ( const sc_logic& v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const sc_logic& v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const sc_logic& v ) _SCV_IMPL2(^=)
  //for 2.0.1beta, sc_logic::negate() is removed
#if SC_VERSION < 2000001
  return_type& negate() { this->initialize(); this->_get_instance()->negate(); this->trigger_value_change_cb(); return *this; }
#endif
  bool operator == ( const sc_logic& r ) const { this->initialize(); return *this->_get_instance() == r; }
  bool operator == ( char r ) const { this->initialize(); return *this->_get_instance() == r; }
  bool operator != ( const sc_logic& r ) const { this->initialize(); return *this->_get_instance() != r; }
  bool operator != ( char r ) const { this->initialize(); return *this->_get_instance() != r; }
  //for 2.0.1beta, sc_logic::operator! is removed
#if SC_VERSION < 2000001
  sc_logic operator ! () { this->initialize(); return !*this->_get_instance(); }
#endif
  _SCV_MAP(char,to_char);
  //for 2.0.1beta, sc_logic::to_long() is removed
#if SC_VERSION < 2000001
  _SCV_MAP(mtiLongT,to_long);
#endif
  _SCV_MAP(bool,is_01);
  //for 2.0.1beta, check_01 is changed to invalid_01 and is now private
#if SC_VERSION < 2000001
  _SCV_MAP(void,check_01);
#endif
  _SCV_MAP(bool,to_bool);
  //  void print( ostream& os ) const { this->initialize(); this->_get_instance()->print(os); }

  operator const sc_logic&() const { this->initialize(); return *this->_get_instance(); }
};

template<int W>
class scv_extensions< sc_bv<W> >
  : public scv_extensions_base< sc_bv<W> > {
public:
    _SCV_PAREN_OPERATOR(sc_bv<W>);
  operator const sc_bv<W>&() const { this->initialize(); return *this->_get_instance(); }
public:
  typedef scv_extensions< sc_bv<W> > return_type;

  sc_bv_base* clone() { return this->_get_instance()->clone(); /* don't clone randomization status */ }
  return_type& operator=(const return_type& v) _SCV_IMPL1
  //  template<class T> return_type& operator=(const sc_proxy<T>& v) _SCV_IMPL
  return_type& operator=(const sc_bv<W> & v) _SCV_IMPL
  return_type& operator=( const char* v) _SCV_IMPL
  return_type& operator=( const bool* v) _SCV_IMPL
  return_type& operator=( const sc_unsigned& v) _SCV_IMPL
  return_type& operator=( const sc_signed& v) _SCV_IMPL
  return_type& operator=( const sc_uint_base& v) _SCV_IMPL
  return_type& operator=( const sc_int_base& v) _SCV_IMPL
  return_type& operator=(mtiLongT v) _SCV_IMPL
  return_type& operator=(mtiUlongT v) _SCV_IMPL
  return_type& operator=( int v) _SCV_IMPL
  return_type& operator=( unsigned v) _SCV_IMPL
  return_type& operator=( char v) _SCV_IMPL
  return_type& operator=( const sc_bit& v) _SCV_IMPL
  return_type& operator=(int64 v) _SCV_IMPL
  return_type& operator=(uint64 v) _SCV_IMPL
  return_type& operator=( const sc_int<W>& v) _SCV_IMPL
  return_type& operator=( const sc_uint<W>& v) _SCV_IMPL

  void resize(mtiUlongT new_size) 
  { this->initialize(); this->_get_instance()->resize(new_size); this->trigger_value_change_cb(); }

  // from sc_bv_base
  mtiLongT get_bit(unsigned n) const { this->initialize(); return this->_get_instance()->get_bit(n); }
  void set_bit(unsigned bit_number, mtiLongT value) 
  { this->initialize(); this->_get_instance()->set_bit(bit_number,value); this->trigger_value_change_cb(); }
  mtiUlongT get_word(unsigned i) const { this->initialize(); return this->_get_instance()->get_word(i); }
  void set_word(unsigned i, mtiUlongT w)
  { this->initialize(); this->_get_instance()->set_word(i,w); this->trigger_value_change_cb(); }
  mtiUlongT get_cword(unsigned i) const { this->initialize(); return this->_get_instance()->get_cword(i); }
  void set_cword(unsigned i, mtiUlongT w)
  { this->initialize(); this->_get_instance()->set_cword(i,w); this->trigger_value_change_cb(); }
  _SCV_MAP(int,length);

  return_type& operator &= ( const sc_unsigned& v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const sc_unsigned& v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const sc_unsigned& v ) _SCV_IMPL2(^=)

  return_type& operator &= ( const sc_signed& v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const sc_signed& v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const sc_signed& v ) _SCV_IMPL2(^=)

  return_type& operator &= ( unsigned int v ) _SCV_IMPL2(&=)
  return_type& operator |= ( unsigned int v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( unsigned int v ) _SCV_IMPL2(^=)

  return_type& operator &= ( int v ) _SCV_IMPL2(&=)
  return_type& operator |= ( int v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( int v ) _SCV_IMPL2(^=)

  return_type& operator &= (mtiUlongT v ) _SCV_IMPL2(&=)
  return_type& operator |= (mtiUlongT v ) _SCV_IMPL2(|=)
  return_type& operator ^= (mtiUlongT v ) _SCV_IMPL2(^=)

  return_type& operator &= (mtiLongT v ) _SCV_IMPL2(&=) 
  return_type& operator |= (mtiLongT v ) _SCV_IMPL2(|=)
  return_type& operator ^= (mtiLongT v ) _SCV_IMPL2(^=)

  return_type& operator &= ( const char* v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const char* v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const char* v ) _SCV_IMPL2(^=)

  sc_bv_base operator & ( const char* s ) const 
  { this->initialize(); return *this->_get_instance() & s; }
  sc_bv_base operator | ( const char* s ) const
  { this->initialize(); return *this->_get_instance() | s; }
  sc_bv_base operator ^ ( const char* s ) const
  { this->initialize(); return *this->_get_instance() ^ s; }

  friend return_type operator & ( const char* s, const return_type& b )
  { b.initialize(); return *b._get_instance() & s; }
  friend return_type operator | ( const char* s, const return_type& b )
  { b.initialize(); return *b._get_instance() | s; }
  friend return_type operator ^ ( const char* s, const return_type& b )
  { b.initialize(); return *b._get_instance() ^ s; }

  void set(mtiUlongT v=0) { this->initialize(); this->_get_instance()->set(v); this->trigger_value_change_cb(); }

};

template<int W>
class scv_extensions< sc_lv<W> >
  : public scv_extensions_base< sc_lv<W> > {
public:
    _SCV_PAREN_OPERATOR(sc_lv<W>);
  operator const sc_lv<W>&() const { this->initialize(); return *this->_get_instance(); }
public:
  typedef scv_extensions< sc_lv<W> > return_type;

  sc_bv_base* clone() { return this->_get_instance()->clone(); /* don't clone randomization status */ }
  return_type& operator=(const return_type& v) _SCV_IMPL1 
  //  template<class T> return_type& operator=(const sc_proxy<T>& v) _SCV_IMPL
  return_type& operator=(const sc_lv<W>& v) _SCV_IMPL
  return_type& operator=( const char* v) _SCV_IMPL
  return_type& operator=( const bool* v) _SCV_IMPL
  return_type& operator=( const sc_logic* v) _SCV_IMPL // this is the only difference from sc_bv.
  return_type& operator=( const sc_unsigned& v) _SCV_IMPL
  return_type& operator=( const sc_signed& v) _SCV_IMPL
  return_type& operator=( const sc_uint_base& v) _SCV_IMPL
  return_type& operator=( const sc_int_base& v) _SCV_IMPL
  return_type& operator=(mtiLongT v) _SCV_IMPL
  return_type& operator=(mtiUlongT v) _SCV_IMPL
  return_type& operator=( int v) _SCV_IMPL
  return_type& operator=( unsigned v) _SCV_IMPL
  return_type& operator=( char v) _SCV_IMPL
  return_type& operator=( const sc_bit& v) _SCV_IMPL
  return_type& operator=(int64 v) _SCV_IMPL
  return_type& operator=(uint64 v) _SCV_IMPL
  return_type& operator=( const sc_int<W>& v) _SCV_IMPL
  return_type& operator=( const sc_uint<W>& v) _SCV_IMPL

  void resize(mtiUlongT new_size) 
  { this->initialize(); this->_get_instance()->resize(new_size); this->trigger_value_change_cb(); }

  // from sc_bv_base
  mtiLongT get_bit(unsigned n) const { this->initialize(); return this->_get_instance()->get_bit(n); }
  void set_bit(unsigned bit_number, mtiLongT value) 
  { this->initialize(); this->_get_instance()->set_bit(bit_number,value); this->trigger_value_change_cb(); }
  mtiUlongT get_word(unsigned i) const { this->initialize(); return this->_get_instance()->get_word(i); }
  void set_word(unsigned i, mtiUlongT w)
  { this->initialize(); this->_get_instance()->set_word(i,w); this->trigger_value_change_cb(); }
  mtiUlongT get_cword(unsigned i) const { this->initialize(); return this->_get_instance()->get_cword(i); }
  void set_cword(unsigned i, mtiUlongT w)
  { this->initialize(); this->_get_instance()->set_cword(i,w); this->trigger_value_change_cb(); }
  _SCV_MAP(int,length);

  return_type& operator &= ( const sc_unsigned& v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const sc_unsigned& v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const sc_unsigned& v ) _SCV_IMPL2(^=)

  return_type& operator &= ( const sc_signed& v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const sc_signed& v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const sc_signed& v ) _SCV_IMPL2(^=)

  return_type& operator &= ( unsigned int v ) _SCV_IMPL2(&=)
  return_type& operator |= ( unsigned int v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( unsigned int v ) _SCV_IMPL2(^=)

  return_type& operator &= ( int v ) _SCV_IMPL2(&=)
  return_type& operator |= ( int v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( int v ) _SCV_IMPL2(^=)

  return_type& operator &= (mtiUlongT v ) _SCV_IMPL2(&=)
  return_type& operator |= (mtiUlongT v ) _SCV_IMPL2(|=)
  return_type& operator ^= (mtiUlongT v ) _SCV_IMPL2(^=)

  return_type& operator &= (mtiLongT v ) _SCV_IMPL2(&=) 
  return_type& operator |= (mtiLongT v ) _SCV_IMPL2(|=)
  return_type& operator ^= (mtiLongT v ) _SCV_IMPL2(^=)

  return_type& operator &= ( const char* v ) _SCV_IMPL2(&=)
  return_type& operator |= ( const char* v ) _SCV_IMPL2(|=)
  return_type& operator ^= ( const char* v ) _SCV_IMPL2(^=)

  sc_bv_base operator & ( const char* s ) const 
  { this->initialize(); return *this->_get_instance() & s; }
  sc_bv_base operator | ( const char* s ) const
  { this->initialize(); return *this->_get_instance() | s; }
  sc_bv_base operator ^ ( const char* s ) const
  { this->initialize(); return *this->_get_instance() ^ s; }

  friend return_type operator & ( const char* s, const return_type& b )
  { b.initialize(); return *b._get_instance() & s; }
  friend return_type operator | ( const char* s, const return_type& b )
  { b.initialize(); return *b._get_instance() | s; }
  friend return_type operator ^ ( const char* s, const return_type& b )
  { b.initialize(); return *b._get_instance() ^ s; }

  bool is_01() { this->initialize(); return this->_get_instance()->is_01(); } // this should have been "const"

};

#endif // SystemC

#undef _SCV_INTEGER_INTERFACE

// ----------------------------------------
// special extension class to handle getting an extension from an extension
// ----------------------------------------
template<typename T>
class scv_extensions< scv_extensions<T> > : public scv_extensions<T> {
public:
  scv_extensions< scv_extensions<T> > () {}
  scv_extensions< scv_extensions<T> > (const scv_extensions<T>& rhs) : scv_extensions<T>(rhs) {}
  virtual ~scv_extensions() {}
  scv_extensions& operator=(const scv_extensions<T>& rhs) {
    return scv_extensions<T>::operator=(rhs);
  }
  scv_extensions& operator=(const T& rhs) {
    return scv_extensions<T>::operator=(rhs);
  }
  operator const T&() const { return *scv_extensions<T>::_get_instance(); }
  scv_expression operator()() { return scv_extensions<T>::form_expression(); }

  virtual void _set_instance(T *i) {
    scv_extensions<T>::_set_instance(i);
  }  
  virtual void _set_instance(scv_extensions<T> *i) {
    scv_extensions<T>::_set_instance(i->_get_instance());
  }  
};

// ----------------------------------------
// specialization for array
// ----------------------------------------
template<typename T, int N>
class scv_extensions<T[N]> : public _SCV_INTROSPECTION_BASE2 {

  typedef T my_type[N];
public:
// ----------------------------------------
// implementation of the specialization for array
// (added cast of N to "int" since some compilers automatically
// regard it as unsigned even though I have declard it as int)
// ----------------------------------------
  scv_extensions();
  virtual ~scv_extensions() {};

public:
  scv_extensions<T>& operator[](int i);
  const scv_extensions<T>& operator[](int i) const;

public:
  virtual void _set_instance_core_wrap(void *p);

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  virtual scv_extensions_if *get_vector_size() { return &vector_size; }
  virtual const scv_extensions_if *get_vector_size() const {
      return &vector_size; }
// END MODELSIM ADDITIONS [MDE-SCV]

public:
  scv_extensions& operator=(const scv_extensions& rhs);
  scv_extensions& operator=(const T * rhs);

private:
  scv_extensions<T> _array[N];

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
public:
  // This is the vector_size parameter that allows for
  // support of randomly sized "fixed max" static arrays.
  scv_extensions<unsigned> vector_size;
// END MODELSIM ADDITIONS [MDE-SCV]
};

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
// ----------------------------------------
// specialization for vector
// ----------------------------------------
template<typename T>
class scv_extensions< vector<T> > : public _SCV_INTROSPECTION_BASE3 {

  typedef vector<T> my_type;
public:
  scv_extensions();
  virtual ~scv_extensions() {};

public:
  scv_extensions<T>& operator[](int i);
  const scv_extensions<T>& operator[](int i) const;

  void clear() { this->get_instance()->clear(); }
  void resize(unsigned new_size) { resize_vector(_vector.size(), new_size); }
  unsigned size() const { return this->get_instance()->size(); }

  typename vector<T >::const_iterator begin() const
    { return this->get_instance()->begin(); }
  typename vector<T >::const_iterator end() const
    { return this->get_instance()->end(); }
  typename vector<T >::const_reverse_iterator rbegin() const
    { return this->get_instance()->rbegin(); }
  typename vector<T >::const_reverse_iterator rend() const
    { return this->get_instance()->rend(); }

  void push_back(const T& value) {
    this->get_instance()->push_back(value);
    resize_vector(_vector.size(), _vector.size()+1);
  }
  void pop_back() {
    this->get_instance()->pop_back();
    resize_vector(_vector.size(), _vector.size()-1);
  }

public:
  virtual void _set_instance_core_wrap(void *p);

  virtual scv_extensions_if *get_vector_size() {
    return &vector_size;
  }
  virtual const scv_extensions_if *get_vector_size() const {
    return &vector_size;
  }

  virtual void resize_vector(unsigned old_size, unsigned new_size);

public:
  scv_extensions& operator=(const scv_extensions& rhs);
  scv_extensions& operator=(const vector<T> &rhs);

private:
  vector< scv_extensions<T> > _vector;
  scv_extensions<T> _vector_elem;

public:
  // This is the vector_size parameter that allows for
  // support of randomly sized vectors.
  scv_extensions<unsigned> vector_size;
};
// END MODELSIM ADDITIONS [MDE-SCV]

// ----------------------------------------
// specialization for pointers
// ----------------------------------------
template<typename T> class scv_smart_ptr; 

template<typename T>
class scv_extensions<T*> : public _SCV_INTROSPECTION_BASE1 {
public:
  // (can only be used with pointer to a single object)
  // (cannot only be used with pointer to an array)
  scv_extensions() {}
  virtual ~scv_extensions() {}

public:
  scv_extensions<T>& operator*();
  const scv_extensions<T>& operator*() const;
  scv_extensions<T> * operator->();
  const scv_extensions<T> * operator->() const;

public:
  scv_extensions<T*>& operator=(const scv_extensions<T*>& rhs);
  scv_extensions<T*>& operator=(scv_extensions<T> * rhs);
  scv_extensions<T*>& operator=(const scv_smart_ptr<T>& rhs);
  scv_extensions<T*>& operator=(T * rhs);
  scv_extensions<T*>& operator=(int);

public:
  virtual void _set_instance_core_wrap(void *p);
  const char* get_type_name() const {
    static const char * s = _scv_ext_util_get_name("%s*", scv_extensions<T>().get_type_name());
    return s;
  }

private:
  mutable bool _own_typed_ptr;
  mutable scv_extensions<T> * _typed_ptr;
  const scv_extensions<T> * _get_ptr() const;
  const scv_extensions<T> * _set_ptr() const;
};

// ----------------------------------------
// implementation of the specialization for array
// (added cast of N to "int" since some compilers automatically
// regard it as unsigned even though I have declard it as int)
// ----------------------------------------
template<typename T, int N>
scv_extensions<T[N]>::scv_extensions() {

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  // By default, set vector_size to array_size
  vector_size._set_instance(new unsigned(N));
  vector_size._set_dynamic();
  vector_size.set_name("vector_size");
  vector_size.disable_randomization();
// END MODELSIM ADDITIONS [MDE-SCV]

  string tmp;
  _scv_extension_util ** a = new _scv_extension_util*[N];
  for (unsigned i=0; i<N; ++i) { 
    a[i] = &_array[i];
    tmp = "[" + _scv_ext_util_get_string(i) + "]";
    _array[i]._set_parent(this,tmp);
  }
  this->_set_up_array(a);
}

template<typename T, int N>
scv_extensions<T>& scv_extensions<T[N]>::operator[](int i) {
  if (i<0 || i>=(int)N) {
    _scv_message::message(_scv_message::INTROSPECTION_INVALID_INDEX,i,"array",this->get_name());
    return _array[0];
  }
  return _array[i];
}

template<typename T, int N>
const scv_extensions<T>& scv_extensions<T[N]>::operator[](int i) const {
  if (i<0 || i>=(int)N) {
    _scv_message::message(_scv_message::INTROSPECTION_INVALID_INDEX,i,"array",this->get_name());
    return _array[0];
  }
  return _array[i];
}

template<typename T, int N>
void scv_extensions<T[N]>::_set_instance_core_wrap(void *p) {
  if ( p ) {
    my_type *tp = (my_type*)p;
    for (int i=0; i<(int)N; ++i) _array[i]._set_instance(&(*tp)[i]);
  }
}

/*
template<typename T, int N>
scv_extensions<T[N]>& scv_extensions<T[N]>::operator=(const scv_extensions& rhs) {
  for (int i=0; i<(int)N; ++i) { _array[i] = rhs[i]; }
  this->trigger_value_change_cb();
}
*/

template<typename T, int N>
scv_extensions<T[N]>& scv_extensions<T[N]>::operator=(const T * rhs) {
  for (int i=0; i<(int)N; ++i) { _array[i] = rhs[i]; }
  this->trigger_value_change_cb();
  return *this;
}

// BEGIN MODELSIM ADDITIONS [MDE-SCV]
// ----------------------------------------
// implementation of the specialization for vector
// ----------------------------------------
template<typename T>
scv_extensions< vector<T> >::scv_extensions() {
  const unsigned VECTOR_DEFAULT_SIZE = 1024;

  vector_size._set_instance(new unsigned(VECTOR_DEFAULT_SIZE));
  vector_size._set_dynamic();
  vector_size.set_name("vector_size");
  vector_size.disable_randomization();

  _vector_elem._set_dynamic();
  _vector_elem.set_name("<anonymous>");

  this->max_vector_size = 0;

  resize(VECTOR_DEFAULT_SIZE);
}

template<typename T>
void scv_extensions< vector<T> >::resize_vector(unsigned old_size, unsigned new_size) {
  // flag to see if memory re-allocation is required for re-sizing
  int realloc_vec = ((old_size != 0) && (new_size > _vector.capacity()));

  // check if the vector is being resized the first time
  int first_time_resize = (old_size == 1024)
                       && !this->_get_array_elt(1)->is_shared_vector_elem
                       && vector_size.is_randomization_enabled();

  bool is_randomization_enabled;
  bool is_dynamic;
  bool is_initialized;
  bool has_dynamic_data;
  bool has_constraint_data;
// BEGIN MODELSIM ADDITIONS [MDE-SCV]
  // fix gcc uninitialized warning with -Wall -Werror 
  is_initialized = has_constraint_data = 0;
// END MODELSIM ADDITIONS [MDE-SCV]

  if (vector_size.is_randomization_enabled()) {
    if (old_size != 0
      && this->_get_array_elt(0)->_has_dynamic_data()
      && this->_get_array_elt(0)->_get_dynamic_data()->cdata_->is_complex_constraint()) {
      _scv_message::message(_scv_message::CMPLX_CONSTRAINT_ON_VECTOR_ELEMENT);
      return;
    }
  }
 
  if (!new_size)
    new_size = _vector.capacity();

  if (old_size) {
    is_randomization_enabled = this->_get_array_elt(0)->is_randomization_enabled();
    is_dynamic = this->_get_array_elt(0)->is_dynamic();
    is_initialized = this->_get_array_elt(0)->is_initialized();
    has_dynamic_data = this->_get_array_elt(0)->_has_dynamic_data();
    has_constraint_data = has_dynamic_data && this->_get_array_elt(0)->_get_dynamic_data()->cdata_;

    if (has_constraint_data)
      _vector_elem._copy_dynamic_data(this->_get_array_elt(0), 1);
  }

  if (realloc_vec || first_time_resize) {
    // Instead of going the STL way to resize a vector, we're deleting
    // the whole vector and creating it again from scratch to avoid
    // potential memory issues while copying objects.
    this->_get_array_elt(0)->is_shared_vector_elem = 1;
    _vector.clear();
    this->_clear_elts_vector();
    this->_get_array_elt(0)->is_shared_vector_elem = 0;

    vector_size.assign(new_size);
    vector_size._set_dynamic();
    this->max_vector_size = 0;
    old_size = 0;
  }

  // resizing the vector
  _vector.resize(new_size);

  if (new_size > this->max_vector_size) {
    string tmp;
    vector<_scv_extension_util *> a;

    a.resize(new_size);

    for (unsigned i=0; i<new_size; ++i) {
      a[i] = &_vector[i];
      tmp = "[" + _scv_ext_util_get_string(i) + "]";
      _vector[i]._set_parent(this,tmp);
    }

    this->_set_up_vector(a);
  }

  // Now propagate the effects of resize to the various lower layers of the
  // vector<T> extension.
  scv_extension_callbacks< vector<T> >::resize_vector(old_size, new_size);

  vector_size.assign(new_size);

  if (new_size > this->max_vector_size)
    this->max_vector_size = new_size;

  if (old_size || realloc_vec || first_time_resize) {
    if (realloc_vec || first_time_resize)
      this->_set_dynamic();

    for (unsigned i=old_size; i<new_size; ++i) {
      // Apply initialization setting to new elements ...
      if (is_initialized)
        this->_get_array_elt(i)->initialize();
      else
        this->_get_array_elt(i)->uninitialize();

      if (has_constraint_data)
        this->_get_array_elt(i)->_copy_dynamic_data(&_vector_elem, i != 0);
    }
  }
}

template<typename T>
scv_extensions<T>& scv_extensions< vector<T> >::operator[](int i) {
  if (i<0 || i>= (int)_vector.size()) {
    _scv_message::message(_scv_message::INTROSPECTION_INVALID_INDEX,i,"vector",this->get_name());
    return _vector[0];
  }
  return _vector[i];
}

template<typename T>
const scv_extensions<T>& scv_extensions< vector<T> >::operator[](int i) const {
  if (i<0 || i>=(int)_vector.size()) {
    _scv_message::message(_scv_message::INTROSPECTION_INVALID_INDEX,i,"vector",this->get_name());
    return _vector[0];
  }
  return _vector[i];
}

template<typename T>
void scv_extensions< vector<T> >::_set_instance_core_wrap(void *p) {
  if (p) {
    my_type *tp = (my_type*)p;
    tp->resize(_vector.size());
    for (unsigned i=0; i<_vector.size(); ++i) _vector[i]._set_instance(&(*tp)[i]);
  }
}

template<typename T>
scv_extensions< vector<T> >& scv_extensions< vector<T> >::operator=(const vector<T> &rhs) {
  unsigned N = _vector.size() < rhs.size() ? _vector.size() : rhs.size();
  for (int i=0; i<(int)N; ++i) { _vector[i] = rhs[i]; }
  this->trigger_value_change_cb();
  return *this;
}
// END MODELSIM ADDITIONS [MDE-SCV]

// ----------------------------------------
// implementation of the specialization for pointers
// ----------------------------------------
template<typename T> const scv_extensions<T> * 
scv_extensions<T*>::_get_ptr() const {
  if (*(this->_instance)) {
    if (!this->_ptr) { 
      this->_own_typed_ptr = true;
      this->_typed_ptr = new scv_extensions<T>();
      this->_ptr = this->_typed_ptr;
      this->_typed_ptr->_set_instance(*this->_instance);
    }
  } else {
    if (this->_ptr) {
      if (this->_own_typed_ptr) delete this->_typed_ptr;
      this->_typed_ptr = NULL;
      this->_ptr = NULL;
    }
  }
  return this->_typed_ptr;
}

template<typename T> const scv_extensions<T> * 
scv_extensions<T*>::_set_ptr() const {
  if (*this->_get_instance()) {
    if (!this->_ptr) {
      this->_own_typed_ptr = true;
      this->_typed_ptr = new scv_extensions<T>();
      this->_ptr = this->_typed_ptr;
    }
    this->_typed_ptr->_set_instance(*this->_get_instance());
  } else {
    if (this->_ptr) {
      if (this->_own_typed_ptr) delete this->_typed_ptr;
      this->_typed_ptr = NULL;
      this->_ptr = NULL;
    }
  }
  return this->_typed_ptr;
}

template<typename T> scv_extensions<T>& 
scv_extensions<T*>::operator*() { 
  const scv_extensions<T> * ptr = _get_ptr();
  if (!ptr) {
    static scv_extensions<T> e;
    _scv_message::message(_scv_message::INTROSPECTION_NULL_POINTER,this->get_name());
    return e;
  }
  return *(scv_extensions<T>*)ptr;
}

template<typename T> const scv_extensions<T>& 
scv_extensions<T*>::operator*() const {
  const scv_extensions<T> * ptr = _get_ptr();
  if (!ptr) {
    static scv_extensions<T> e;
    _scv_message::message(_scv_message::INTROSPECTION_NULL_POINTER,this->get_name());
    return &e;
  }
  return *ptr;
}

template<typename T> scv_extensions<T> * 
scv_extensions<T*>::operator->() { 
  return (scv_extensions<T>*)_get_ptr();
}

template<typename T> const scv_extensions<T> * 
scv_extensions<T*>::operator->()  const { 
  return (scv_extensions<T>*)_get_ptr();
}

template<typename T> scv_extensions<T*>& 
scv_extensions<T*>::operator=(const scv_extensions<T*>& rhs) {
  *this->_get_instance() = *rhs._instance;
  if (rhs._ptr->is_dynamic()) {
    // share the same extension until the object disapear
    // (in practise (but slow), probably need to register
    // a deletion callback.
    this->_own_typed_ptr = false;
    this->_ptr = rhs._ptr;
    this->_typed_ptr = rhs._typed_ptr;
  }
  _set_ptr();
  this->trigger_value_change_cb();
  return *this;
}

template<typename T> scv_extensions<T*>& 
scv_extensions<T*>::operator=(scv_extensions<T> * rhs) {
  *this->_get_instance() = rhs->_instance;
  if (rhs->is_dynamic()) {
    // share the same extension until the object disapear
    // (in practise (but slow), probably need to register
    // a deletion callback.
    this->_own_typed_ptr = false;
    this->_ptr = rhs;
    this->_typed_ptr = rhs;
  }
  _set_ptr();
  this->trigger_value_change_cb();
  return *this;
}

/* this is in _scv_smart_ptr.h
template<typename T> scv_extensions<T*>& 
scv_extensions<T*>::operator=(const scv_smart_ptr<T>& rhs) { ... }
*/

template<typename T> scv_extensions<T*>&  
scv_extensions<T*>::operator=(T * rhs) {
  *this->_get_instance() = rhs;
  _set_ptr();
  this->trigger_value_change_cb();
  return *this;
}

template<typename T> scv_extensions<T*>&  
scv_extensions<T*>::operator=(int rhs) {
  *this->_get_instance() = (T*) rhs;
  _set_ptr();
  this->trigger_value_change_cb();
  return *this;
}

template<typename T> void
scv_extensions<T*>::_set_instance_core_wrap(void *) {
  _set_ptr();
}

