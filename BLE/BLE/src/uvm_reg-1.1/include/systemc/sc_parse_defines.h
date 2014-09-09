/*
 * 07/16/03 I-I
 *
 * gcc 3.3 has added a lot of predefined macros which EDG is not
 * supported yet. To workaround the problem, we have to predefine
 * those macros ourselves until EDG fix it.
 * This file is passed to EDG when using gcc 3.3.
 */


/* $Id: //dvt/mti/rel/10.0b/src/systemc/sc_parse_defines.h#1 $ */

#ifndef SC_PARSE_DEFINES_H
#define SC_PARSE_DEFINES_H

#define __CHAR_BIT__ 8
#define __SCHAR_MAX__ 127
#define __SHRT_MAX__ 32767
#define __INT_MAX__ 2147483647
#define __LONG_MAX__ 2147483647L
#define __LONG_LONG_MAX__ 9223372036854775807LL
#define __FLT_MANT_DIG__ 24
#define __FLT_DIG__ 6
#define __FLT_RADIX__ 2
#define __FLT_MIN_EXP__ (-125)
#define __FLT_MIN_10_EXP__ (-37)
#define __FLT_MAX_EXP__ 128
#define __FLT_MAX_10_EXP__ 38
#define __FLT_DENORM_MIN__ 1.40129846e-45F
#define __FLT_MIN__ 1.17549435e-38F
#define __FLT_MAX__ 3.40282347e+38F
#define __FLT_EPSILON__ 1.19209290e-7F
#define __DBL_MANT_DIG__ 53
#define __DBL_DIG__ 15
#define __DBL_MIN_EXP__ (-1021)
#define __DBL_MIN_10_EXP__ (-307)
#define __DBL_MAX_EXP__ 1024
#define __DBL_MAX_10_EXP__ 308
#define __DBL_DENORM_MIN__ 4.9406564584124654e-324
#define __DBL_MIN__ 2.2250738585072014e-308
#define __DBL_MAX__ 1.7976931348623157e+308
#define __DBL_EPSILON__ 2.2204460492503131e-16
#if defined( linux )
#define __LDBL_MANT_DIG__ 64
#define __LDBL_DIG__ 18
#define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L
#define __LDBL_MIN__ 3.36210314311209350626e-4932L
#define __LDBL_MAX__ 1.18973149535723176502e+4932L
#define __LDBL_EPSILON__ 1.08420217248550443401e-19L
#else
#define __LDBL_MANT_DIG__ 113
#define __LDBL_DIG__ 33
#define __LDBL_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966L
#define __LDBL_MIN__ 3.36210314311209350626267781732175260e-4932L
#define __LDBL_MAX__ 1.18973149535723176508575932662800702e+4932L
#define __LDBL_EPSILON__ 1.92592994438723585305597794258492732e-34L
#endif
#define __LDBL_MIN_EXP__ (-16381)
#define __LDBL_MIN_10_EXP__ (-4931)
#define __LDBL_MAX_EXP__ 16384
#define __LDBL_MAX_10_EXP__ 4932

/* For gcc 3.4 */
#define __FLT_HAS_INFINITY__ 1
#define __FLT_HAS_QUIET_NAN__ 1
#define __DBL_HAS_INFINITY__ 1
#define __DBL_HAS_QUIET_NAN__ 1
#define __LDBL_HAS_INFINITY__ 1
#define __LDBL_HAS_QUIET_NAN__ 1

#endif
