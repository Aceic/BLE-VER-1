/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Note: this config file is modified from the SCV generated config files. */
/* This version attempts to provide a single file for all MTI supported    */
/* platforms. */
/* $Id: //dvt/mti/rel/10.0b/src/scv/scv/scv_config.h#1 $ */

#define SCV_VERSION "01.00-b004"

/* Detect if this is a HP aCC compile */
#ifdef __HP_aCC
#define HPACC
#endif

/* Define if we're on Linux */
/* #undef ARCH_IS_X86 */

/* the full canonical system: cpu, vendor, os */
/* define TARGET "sparc-sun-solaris2.8" */

/* found Dl_info in dlfcn.h */
/* #undef HAVE_DL_INFO */

/* A string that contains the shared library extension (e.g. .so or .sl) */
#ifdef HPACC
#define SHLIB_EXT ".sl"
#else
#define SHLIB_EXT ".so"
#endif

/* Define if we're on the HP-UX 11 */
#ifdef HPACC
/*
 * These are not required when using RW Standard C++ library 2.2.1.
 * They apply only to version 1.2.1.  Version 2.2.1 is used
 * by including libstd_v2 & libCsup_v2 while linking.  To make
 * version 2.2.1 multi-thread safe RWSTD_MULTI_THREAD & _REENTRANT
 * are defined from the command line by sccom since they are required for
 * compiling the SystemC source code as well.
 */
/* #define RWSTD_MULTI_THREAD */
/* #define RW_MULTI_THREAD */
/* #define __HPACC_THREAD_SAFE_RB_TREE */
/* #define HP_THREAD_SAFE */
/* #define _THREAD_SAFE */
#define _POSIX_C_SOURCE 199506L
#define hp_hpux11
#else
/*
 * _REENTRANT is defined on the command line for HP by sccom
 * to make code compiled with RW Standard C++ library 2.2.1
 * thread safe.  This library is used by including libstd_v2 and
 * libCsup_v2.  If defined here it clashes with the definition
 * from the command line.  __REENTRANT is used on other platforms 
 * as well.
 */
#ifndef _REENTRANT
#define _REENTRANT
#endif
#endif

/* #undef _USE_FMTFLAGS_TYPE */
#define _USE_FRIEND_FOR_EQUAL
#define _USE_EXPLICIT_NEQ 
/* #undef _USE_HASH_MAP */
/* #undef _USE_PADDING_ON_STRUCTS */
/* #undef _USE_UINT64_WRITE */

#ifdef HPACC
/* DOUG: Don't want this, since we use -AA. */
/* #define SCV_USE_IOSTREAM_H */
#undef _USE_FRIEND_FOR_EQUAL
#endif

#define SC_VERSION 2000001

/* Define if you have the <a.out.h> header file.  */
#ifdef HPACC
#define HAVE_A_OUT_H 1
#endif
/* #undef HAVE_A_OUT_H */

/* Define if you have the <bfd.h> header file.  */
/* #undef HAVE_BFD_H */

/* Define if you have the <dlfcn.h> header file.  */
#define HAVE_DLFCN_H 1

/* Define if you have the <libgen.h> header file.  */
/* #undef HAVE_LIBGEN_H */

/* Define if you have the <stddef.h> header file.  */
/* #undef HAVE_STDDEF_H */

/* Define if you have the <sys/frame.h> header file.  */
/* #undef HAVE_SYS_FRAME_H */

/* Define if you have the <sys/link.h> header file.  */
/* #undef HAVE_SYS_LINK_H */

/* Define if you have the <sys/reg.h> header file.  */
/* #undef HAVE_SYS_REG_H */

/* Define if you have the <sys/regset.h> header file.  */
/* #undef HAVE_SYS_REGSET_H */

/* Define if you have the <ucontext.h> header file.  */
/* #undef HAVE_UCONTEXT_H */

/* Define if you have the <unistd.h> header file.  */
/* #undef HAVE_UNISTD_H */

/* Define if you have the bfd library (-lbfd).  */
/* #undef HAVE_LIBBFD */

/* Define if you have the demangle library (-ldemangle).  */
/* #undef HAVE_LIBDEMANGLE */

#ifdef HPACC
/* Define if you have the dld library (-ldld).  */
#define HAVE_LIBDLD 1
#else
/* Define if you have the dl library (-ldl).  */
#define HAVE_LIBDL 1
#endif

/* Define if you have the elf library (-lelf).  */
/* #undef HAVE_LIBELF */

/* Define if you have the iberty library (-liberty).  */
#ifndef HPACC
#define HAVE_LIBIBERTY 1
#endif

#ifdef __cplusplus

/* cc on HPUX seems to define this automatically so avoid redefinition warnings by */
/* conditionally defining it. */
#ifdef HPACC
#ifndef _HPUX_SOURCE
#define _HPUX_SOURCE
#endif
#endif

#if defined (__SUNPRO_CC)
#include<list>
#include<map>
#include<string>
#endif

/* HP-UX's aCC doesn't have std defined until it sees a std lib header file */
/* (unlike g++ and SunWorks CC.) */
/* So define it here empty.  Namespaces are extensible, so this is harmless. */
namespace std {}
using namespace std;

#ifdef DELETE
/* #undef DELETE */
#endif
#ifdef ERROR
/* #undef ERROR */
#endif

#endif /* __cplusplus */

#if defined (__SUNPRO_CC)
  using std::list;
  using std::pair;
  using std::map;
  using std::less;
  using std::string;
  using std::equal_to;
#endif

#include "mti.h"

#if defined(_MSC_VER)
# define _USE_FMTFLAGS_TYPE
# define _USE_FRIEND_FOR_EQUAL
# define _USE_EXPLICIT_NEQ
# define SC_VERSION 2000001
# define HAVE_STDDEF_H
#endif
#undef PACKAGE
