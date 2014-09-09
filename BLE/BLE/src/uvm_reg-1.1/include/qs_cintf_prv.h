/*
 *  qs_cintf_prv.h
 *
 * This file contains the internal supporting macro and interfaces
 * used by 'e' C interface support. This is viewable by public.
 *
 */

#ifndef QS_CINTF_PRV_H_
#define QS_CINTF_PRV_H_

/* Use to export a symbol from application */
#if !defined(DPI_DLLISPEC)
#if defined (_MSC_VER)
#define DPI_DLLISPEC __declspec(dllimport)
#else
#define DPI_DLLISPEC
#endif
#endif

/* Use to import a symbol into application */
#if !defined(DPI_DLLESPEC)
#if defined (_MSC_VER)
#define DPI_DLLESPEC __declspec(dllexport)
#else
#define DPI_DLLESPEC
#endif
#endif

/* Use to mark a function as external */
#ifndef DPI_EXTERN
#define DPI_EXTERN
#endif

#ifndef DPI_PROTOTYPES
#define DPI_PROTOTYPES
/* object is defined imported by the application */
#define XXTERN DPI_EXTERN DPI_DLLISPEC
/* object is exported by the application */
#define EETERN DPI_EXTERN DPI_DLLESPEC
#endif

/*******************************************************************************
 * Fixed data type mapping for 'e' 
 *********************************************************************************/
XXTERN typedef int32_t      qs_int;
XXTERN typedef uint8_t      qs_bit;
XXTERN typedef uint8_t      qs_bool;
XXTERN typedef uint8_t      qs_byte;
XXTERN typedef uint32_t     qs_uint;
XXTERN typedef uint64_t     qs_time;
XXTERN typedef int32_t      qs_enum;
XXTERN typedef const char*  qs_string;
XXTERN typedef void*        qs_external_pointer;

/* an instance handle to a struct or unit scope */
XXTERN typedef const void*  qs_struct_handle;

/* an instance handle to a list */
XXTERN typedef const void*  qs_list_handle;

/*********************************************************************************
 * bitvector representation with predefined width
*********************************************************************************/
XXTERN typedef struct {
      uint32_t words[1];
} qs_bitvec_1;

XXTERN typedef struct {
      uint32_t words[2];
} qs_bitvec_2;

XXTERN typedef struct {
      uint32_t words[3];
} qs_bitvec_3;

XXTERN typedef struct {
      uint32_t words[4];
} qs_bitvec_4;

XXTERN typedef struct {
      uint32_t words[5];
} qs_bitvec_5;

XXTERN typedef struct {
      uint32_t words[6];
} qs_bitvec_6;

XXTERN typedef struct {
      uint32_t words[7];
} qs_bitvec_7;

XXTERN typedef struct {
      uint32_t words[8];
} qs_bitvec_8;

XXTERN typedef struct {
      uint32_t words[9];
} qs_bitvec_9;

/*********************************************************************************
 * supporting internal macros 
*********************************************************************************/
#define QS_DEFAULT_PKG_NAME "main"
#define QS_EXPORT_TF_PREFIX "qs_export"
#define qs_QS_QUALIFIED_NAME(pkg, type) qs_##pkg##____##type
#define QS_ENUM_PRV(type,val)  type##__##val
#define QS_CALL_EXPORT_PRV(methodName, this, structName, params, fname, lineno) \
    (                                                                           \
      qs_SetExportCallScope(#methodName, this, #structName, fname, lineno),     \
      qs_export_##structName##__##methodName params                             \
    )

#define QS_STRUCT_NEW_PRV(type, fname, lineno)     \
    qs_struct_new(#type, fname, lineno)

#define QS_STRUCT_GET_FIELD_PRV(struct, field_type, name, fname, lineno)      \
    (                                                                         \
        qs_dummy =                                                            \
           qs_struct_get_field_val_by_name(struct, name, fname, lineno),      \
        qs_dummy == 0 ? *(field_type*) qs_alloc_auto_free(sizeof(field_type), fname, lineno) \
                      : *(field_type*) qs_dummy                               \
    )

#define QS_STRUCT_SET_FIELD_PRV(struct, field_type, name, val, fname, lineno) \
    {                                                                         \
         field_type tmp_buffer = val;                                         \
         qs_struct_set_field_val_by_name(struct, name, &tmp_buffer, fname, lineno); \
    }                                                                               \
    qs_no_op()

#define QS_STRUCT_GET_NAME(struct, p_pkgname, p_structname)      \
    QS_STRUCT_GET_NAME_PRV(struct, p_pkgname, p_structname, __FILE__, __LINE__)

#define QS_STRUCT_GET_NAME_PRV(struct, p_pkgname, p_structname, fname, lineno) \
    qs_struct_get_name(struct, p_pkgname, p_structname, fname, lineno)

#define QS_STRUCT_DUMP_FIELDS(struct)      \
    QS_STRUCT_DUMP_FIELDS_PRV(struct, __FILE__, __LINE__)

#define QS_STRUCT_DUMP_FIELDS_PRV(struct, fname, lineno) \
    qs_struct_dump_fields(struct, 0, fname, lineno)

#define QS_SYS_PRV(fname, lineno)  \
    qs_get_sys_handle(fname, lineno)

#define QS_LIST_NEW_PRV(elemtype, fname, lineno)  \
    qs_list_new(#elemtype, fname, lineno)

#define QS_LIST_SIZE_PRV(list, fname, lineno)     \
    qs_list_get_size(list, fname, lineno)

#define QS_LIST_RESIZE_PRV(list, size, fname, lineno)  \
    qs_list_resize(list, size, fname, lineno)

#define QS_LIST_GET_PRV(list, index, elemtype, fname, lineno)  \
    (                                                          \
      qs_dummy = qs_list_get_elem(list, index, fname, lineno), \
      qs_dummy == 0 ? *(elemtype*) qs_alloc_auto_free(sizeof(elemtype), fname, lineno) \
                    : *(elemtype*) qs_dummy                             \
    )

#define QS_LIST_SET_PRV(list, index, elemtype, value, fname, lineno)  \
    {                                                                 \
         elemtype tmp_buffer = value;                                 \
         qs_list_set_elem(list, index, &tmp_buffer, fname, lineno);   \
    }                                                                 \
    qs_no_op()

#define QS_LIST_ADD_PRV(list, elemtype, value, fname, lineno)         \
    {                                                                 \
         elemtype tmp_buffer = value;                                 \
         qs_list_add(list, &tmp_buffer, fname, lineno);     \
    }                                                                 \
    qs_no_op()

#define QS_LIST_GET_NAME(list, p_pkgname, p_elemname)      \
    QS_LIST_GET_NAME_PRV(list, p_pkgname, p_elemname, __FILE__, __LINE__)

#define QS_LIST_GET_NAME_PRV(list, p_pkgname, p_elemname, fname, lineno) \
    qs_list_get_name(list, p_pkgname, p_elemname, fname, lineno)

#define QS_LIST_DUMP_ELEMS(struct)      \
    QS_LIST_DUMP_ELEMS_PRV(struct, __FILE__, __LINE__)

#define QS_LIST_DUMP_ELEMS_PRV(struct, fname, lineno) \
    qs_list_dump_elems(struct, 0, fname, lineno)

#define QS_BITVEC_GET_WORD_PRV(vec, word_index, fname, lineno) \
    (vec)->words[word_index]

#define QS_BITVEC_SET_WORD_PRV(vec, word_index, value, fname, lineno) \
    (vec)->words[word_index] = value

#define QS_BITVEC_WORDNO_PRV(b, fname, lineno)   ((b) >> 5)
#define QS_BITVEC_BITNO_PRV(b, fname, lineno)    ((b) & 0x1f)

#define QS_ALLOC_AUTO_FREE_PRV(size, fname, lineno) \
    qs_alloc_auto_free(size, fname, lineno)

/*********************************************************************************
 * struct/unit handle supporting methods
 *********************************************************************************/
/* auto garbage collected struct handle. init() method get called on new object. */
XXTERN qs_struct_handle
qs_struct_new(
    const char* struct_type,
    const char* fname,
    int         lineno);

XXTERN void*
qs_struct_get_field_val_by_name(
    qs_struct_handle handle,
    const char* field_name,
    const char* fname,
    int         lineno);

XXTERN void
qs_struct_set_field_val_by_name(
    qs_struct_handle handle,
    const char* field_name,
    void*       val_addr,
    const char* fname,
    int         lineno);

XXTERN qs_struct_handle
qs_get_sys_handle(
    const char* fname,
    int         lineno);

/* get the struct/unit name for a given scope handle */
XXTERN void
qs_struct_get_name(
    qs_struct_handle handle,
    const char** p_pkgName,
    const char** p_structName,
    const char*  fname,
    int          lineno);

/* dump user field (debug routine) */
XXTERN void
qs_struct_dump_fields(
    qs_struct_handle handle,
    int              level,
    const char*      fname,
    int              lineno);

/*********************************************************************************
 * list handle supporting methods
 *********************************************************************************/
/* auto garbage collected handle */
XXTERN qs_list_handle
qs_list_new(
    const char* elem_type,
    const char* fname,
    int         lineno);
  
/* get list size */
XXTERN int
qs_list_get_size(
    qs_list_handle handle,
    const char*    fname,
    int            lineno);

/* resize a list */
XXTERN void
qs_list_resize(
    qs_list_handle handle,
    int            new_size,
    const char*    fname,
    int            lineno);

/* get a list element */
XXTERN void*
qs_list_get_elem(
    qs_list_handle handle,
    int            index,
    const char*    fname,
    int            lineno);

/* set a list element */
XXTERN void
qs_list_set_elem(
    qs_list_handle handle,
    int            index,
    void*          value,
    const char*    fname,
    int            lineno);

/* add a new elem to list */
XXTERN void
qs_list_add(
    qs_list_handle list,
    void*          value,
    const char*    fname,
    int            lineno);

/* get the list elem name for a given scope handle */
XXTERN void
qs_list_get_name(
    qs_struct_handle handle,
    const char** p_pkgName,
    const char** p_elemName,
    const char*  fname,
    int          lineno);

/* dump list elements (debug routine) */
XXTERN void
qs_list_dump_elems(
    qs_list_handle handle,
    int            level,
    const char*    fname,
    int            lineno);


/* dynamic allocation with auto free */
XXTERN void*
qs_alloc_auto_free(
    uint32_t     size,
    const char*  fname,
    int          lineno);

/*********************************************************************************
 * export call related interface 
 *********************************************************************************/
/*
 * Set the 'e' export call calling context.
 */
XXTERN void*
qs_SetExportCallScope(
    const char* methodName,
    const void* this_handle,
    const char* structName,
    const char* fname,
    int         lineno);

XXTERN void*
qs_GetExportCallScope();

XXTERN void
qs_no_op();

extern void* qs_dummy;

#undef DPI_EXTERN

#ifdef DPI_PROTOTYPES
#undef DPI_PROTOTYPES
#undef XXTERN
#undef EETERN
#endif


#endif /* QS_CINTF_PRV_H_ */
