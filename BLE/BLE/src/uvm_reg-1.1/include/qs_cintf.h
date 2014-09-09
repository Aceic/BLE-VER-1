/*
 *  qs_cintf.h
 *
 * This file contains the constant definitions, structure definitions,
 * and routine declarations used by 'e' C interface.
 *
 */

#ifndef QS_CINTF_H_
#define QS_CINTF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Ensure that size-critical types are defined on all OS platforms. */
#if defined (_MSC_VER)
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int8 uint8_t;
typedef signed __int64 int64_t;
typedef signed __int32 int32_t;
typedef signed __int16 int16_t;
typedef signed __int8 int8_t;
#elif defined(__MINGW32__)
#include <stdint.h>
#elif defined(__linux)
#include <inttypes.h>
#else
#include <sys/types.h>
#endif

#if defined(_MSC_VER)
# define LLOSTR "%I64o"
# define LLDSTR "%I64d"
# define LLUSTR "%I64u"
# define LLXSTR "%I64x"
#elif defined(__64BIT__)
# define LLOSTR "%lo"
# define LLDSTR "%ld"
# define LLUSTR "%lu"
# define LLXSTR "%lx"
#else
# define LLOSTR "%llo"
# define LLDSTR "%lld"
# define LLUSTR "%llu"
# define LLXSTR "%llx"
#endif

#include "qs_cintf_prv.h"

/*********************************************************************************
 * public user C macros 
 *********************************************************************************/

/* macro for 'e' types */

#define QS_TYPE(type)      qs_##type
#define QS_STRUCT(type)    qs_struct_handle
#define QS_LIST(type)      qs_list_handle
#define QS_BITVEC(nwords)  qs_bitvec_##nwords
#define QS_ENUM(type, val) QS_ENUM_PRV(type, val)
#define QS_QUALIFIED_NAME(pkg, type) pkg##____##type

/* dispatch export call */

#define QS_CALL_EXPORT(methodName, this, structName, params) \
    QS_CALL_EXPORT_PRV(methodName, this, structName, params, __FILE__, __LINE__) 

/* macro for struct handle */

#define QS_STRUCT_NEW(type)                   \
    QS_STRUCT_NEW_PRV(type, __FILE__, __LINE__)

#define QS_STRUCT_GET_FIELD(struct, field_type, field_name)      \
    QS_STRUCT_GET_FIELD_PRV(struct, field_type, #field_name, __FILE__, __LINE__) 

#define QS_STRUCT_SET_FIELD(struct, field_type, field_name, val)             \
    QS_STRUCT_SET_FIELD_PRV(struct, field_type, #field_name, val, __FILE__, __LINE__)

#define QS_SYS()              \
    QS_SYS_PRV(__FILE__, __LINE__)

/* macro for list handle */

#define QS_LIST_NEW(elemtype)      \
    QS_LIST_NEW_PRV(elemtype, __FILE__, __LINE__)

#define QS_LIST_SIZE(list)          \
    QS_LIST_SIZE_PRV(list, __FILE__, __LINE__)

#define QS_LIST_RESIZE(list, size)  \
    QS_LIST_RESIZE_PRV(list, size, __FILE__, __LINE__) 

#define QS_LIST_GET(list, index, elemtype) \
    (elemtype) QS_LIST_GET_PRV(list, index, elemtype, __FILE__, __LINE__)

#define QS_LIST_SET(list, index, elemtype, value)     \
    QS_LIST_SET_PRV(list, index, elemtype, value, __FILE__, __LINE__)

#define QS_LIST_ADD(list, elemtype, value)            \
    QS_LIST_ADD_PRV(list, elemtype, value, __FILE__, __LINE__)

#define QS_BITVEC_GET_WORD(vec, word_index) \
    QS_BITVEC_GET_WORD_PRV(vec, word_index, __FILE__, __LINE__)

#define QS_BITVEC_SET_WORD(vec, word_index, value) \
    QS_BITVEC_SET_WORD_PRV(vec, word_index, value, __FILE__, __LINE__)

#define QS_BITVEC_WORDNO(b) \
    QS_BITVEC_WORDNO_PRV(b, __FILE__, __LINE__)

#define QS_BITVEC_BITNO(b) \
    QS_BITVEC_BITNO_PRV(b, __FILE__, __LINE__)

#define QS_ALLOC_AUTO_FREE(size) \
    QS_ALLOC_AUTO_FREE_PRV(size, __FILE__, __LINE__)

/* No macro needed for string */

#ifdef __cplusplus
}
#endif

#endif
