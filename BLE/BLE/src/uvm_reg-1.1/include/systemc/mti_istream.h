/*****************************************************************************
 *
 * MTI istream class for cin support
 *
 * Copyright 1991-2011 Mentor Graphics Corporation
 *
 * All Rights Reserved.
 *
 * THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
 * MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
 * $Revision: #1 $
 * $Id: //dvt/mti/rel/10.0b/src/systemc/mti_istream.h#1 $
 *
 *****************************************************************************/

#ifndef MTI_ISTREAM_H_
#define MTI_ISTREAM_H_

#include <iostream>
#include <sstream>
#include <string>

#define mti_istream_ns std
#include "mti.h"

namespace mti_istream_ns {

class mti_istream : public istringstream
{
 public:
  mti_istream();
  ~mti_istream()  {};

public:
  //get called when restarting the simulation
  void Restart();

public:
  // declare operator>> functions

#define DECL_ISTREAM_OPERATOR_IN(type, varname) \
        mti_istream& operator>> (type varname)

  DECL_ISTREAM_OPERATOR_IN(bool&, val);
  DECL_ISTREAM_OPERATOR_IN(short&, val);
  DECL_ISTREAM_OPERATOR_IN(unsigned short&, val);
  DECL_ISTREAM_OPERATOR_IN(int&, val);
  DECL_ISTREAM_OPERATOR_IN(unsigned int&, val);
  DECL_ISTREAM_OPERATOR_IN(mtiLongT&, val);
  DECL_ISTREAM_OPERATOR_IN(mtiUlongT&, val);
  DECL_ISTREAM_OPERATOR_IN(float&, val);
  DECL_ISTREAM_OPERATOR_IN(double&, val);
  DECL_ISTREAM_OPERATOR_IN(long double&, val);

  DECL_ISTREAM_OPERATOR_IN(char*, val);
  DECL_ISTREAM_OPERATOR_IN(unsigned char*, val);
  DECL_ISTREAM_OPERATOR_IN(signed char*, val);
  DECL_ISTREAM_OPERATOR_IN(char&, val);
  DECL_ISTREAM_OPERATOR_IN(unsigned char&, val);
  DECL_ISTREAM_OPERATOR_IN(signed char&, val);

#if (__GNUC__ > 2 ) ||  defined(__hpux)
  DECL_ISTREAM_OPERATOR_IN(void*&, val);
#ifdef _GLIBCPP_USE_LONG_LONG
  DECL_ISTREAM_OPERATOR_IN(long long&, val);
  DECL_ISTREAM_OPERATOR_IN(unsigned long long&, val);
#endif
#endif

#undef DECL_ISTREAM_OPERATOR_IN

public:
  //other supported interfaces
  int get();
  mti_istream& get(char& c);
  mti_istream& get(char* s, streamsize n);
  mti_istream& get(char* s, streamsize n, char delim);

  mti_istream& getline(char* s, streamsize n);
  mti_istream& getline(char* s, streamsize n, char delim);

  mti_istream& read(char* s, streamsize n);
 
#if (__GNUC__ > 2 ) ||  defined(__hpux)
  streamsize   readsome(char* s, streamsize n); 
#endif

  //istream manipulator support

  mti_istream& operator>> (istream& ( *pf )(istream&));
  mti_istream& operator>> (ios& ( *pf )(ios&));

#if (__GNUC__ > 2 ) ||  defined(__hpux)
  mti_istream& operator>> (ios_base& ( *pf )(ios_base&));
#endif

public:
  //these are unsupported interfaces of cin.
  mti_istream&  putback(char c); 
  mti_istream&  unget();

private:
  mti_istream(const mti_istream& src);
  mti_istream& operator=(const mti_istream& rhs);

private:
  void         InitState();
  void         SetBuffer();
  void         MaybeSetBuffer();

  streampos    mEnd;
};

// drop-in replacement for cin at preprocessing phase

extern mti_istream mti_cin;

}; //namespace mti_istream_ns

using mti_istream_ns::mti_cin;
#endif
