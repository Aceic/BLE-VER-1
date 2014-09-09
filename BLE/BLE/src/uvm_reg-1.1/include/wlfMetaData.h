#ifndef WLFMETADATA_H
#define WLFMETADATA_H

#include "tcl.h"

#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
 APPLICATION SYMBOLS
 
 I've put application-specific symbols here at the top, just so they are easy
 to find: 

----------------------------------------------------------------------------- */

/*---  Power Aware Highlighting (PAH)  ---*/

#define PAH_BIAS_LINK_NAME              "MTI_PAH_BIAS_LINK"
#define PAH_CRPT_LINK_NAME              "MTI_PAH_CORRUPTION_LINK"
#define PAH_ISO_LINK_NAME               "MTI_PAH_ISOLATION_LINK"
#define PAH_ISO_GROUP_LINK_NAME         "MTI_PAH_PORT_ISO_GROUP_LINK"
#define PAH_ISO_DETAIL_LINK_NAME        "MTI_PAH_PORT_ISO_DETAIL_LINK"
#define PAH_ISO_DETAIL_REC_NAME         "MTI_PAH_PORT_ISO_DETAIL_REC"



/*-----------------------------------------------------------------------------
 META API & SYMBOLS
----------------------------------------------------------------------------- */

/* Error codes are standard for Meta: */
typedef enum {
    /* General: */
    WLF_META_OK = 0,
    WLF_META_UNKNOWN_ERROR,   /* Unknown error code! */
    WLF_META_INTERNAL_ERROR,  /* Should never happen, should CHECK() in debug build */

    /* Resources: */
    WLF_META_ALLOCATION_ERROR,  /* failed to allocate memory */
    WLF_META_NO_LOG_FILE,
    WLF_META_NO_DATABASE,       /* No database found in file: */

    /* Parameter and value validation: */
    WLF_META_UNKNOWN_FILE_KIND,  /* file kind is not recognized */
    WLF_META_WRONG_FILE_KIND,    /* file kind is not recognized */
    WLF_META_NULL_HANDLE,     /* object handle is NULL */
    WLF_META_UNKNOWN_OBJECT,  /* object kind is not recognized */
    WLF_META_WRONG_OBJECT,    /* got the wrong kind of object */
    WLF_META_NULL_ALLOCATOR,  /* NULL pointer to allocation function */
    WLF_META_BAD_NAME,        /* string is NULL or empty */
    WLF_META_UNKNOWN_VALUE_KIND,  /* value kind is not recognized */
    WLF_META_UNKNOWN_LINK_KIND,  /* link kind is not recognized */
    WLF_META_BAD_COUNT,       /* number of field values <= 0 */
    WLF_META_NULL_VALUE_POINTER,  /* pointer to values is NULL */
    WLF_META_BAD_SYMBOL,
    WLF_META_MIXED_LOG_FILES,     /* objects or symbols are from multiple files */

    /* Searching: */
    WLF_META_NOT_FOUND,
    WLF_META_BAD_SN,

    /* Meta source files: */
    WLF_META_FILE_OPEN_FAILURE,
    WLF_META_FILE_READ_FAILURE,
    WLF_META_UNEXPECTED_RECORD,
    WLF_META_UNEXPECTED_FIELD,
    WLF_META_UNEXPECTED_END,
    WLF_META_UNEXPECTED_LINK,
    WLF_META_UNEXPECTED_TOKEN,
    WLF_META_UNKNOWN_TOKEN,
    WLF_META_RECORD_COUNT_MISMATCH,
    WLF_META_RECORD_NAME_MISMATCH,
    WLF_META_FIELD_COUNT_MISMATCH,
    WLF_META_FIELD_NAME_MISMATCH,
    WLF_META_VALUE_KIND_MISMATCH,
    WLF_META_VALUE_MISMATCH,
    WLF_META_VALUE_NUMBER_MISMATCH,
    WLF_META_LINK_NAME_MISMATCH,

    /* Used solely for error checking */
    WLF_META_LAST_ERROR
} WlfMetaError_t;


typedef enum {
    /* Used for error checking only: */
    WLF_META_LINK_KIND_NONE = 0,
   
    WLF_META_LINK_KIND_RECORD_TO_SYMBOLS,
    WLF_META_LINK_KIND_SYMBOL_TO_SYMBOLS,

    /* Used for error checking only: */
    WLF_META_LINK_KIND_LAST
} WlfMetaLinkKind_t, *pWlfMetaLinkKind_t;

#define wlfMetaLinkKindIsValid(k) \
    ((WLF_META_LINK_KIND_NONE < (k)) && ((k) < WLF_META_LINK_KIND_LAST))


typedef enum {
    /* Used for error checking only: */
    WLF_META_OBJECT_NONE = 0,

    /* These are the valid kinds and must remain a contiguous set: */
    WLF_META_OBJECT_DATABASE,
    WLF_META_OBJECT_RECORD,
    WLF_META_OBJECT_FIELD,
    WLF_META_OBJECT_ITERATOR,
    WLF_META_OBJECT_LINK,
    WLF_META_OBJECT_SYMBOL,

    /* Not a true object, but we need this to support recognition of strings over RPC: */
    WLF_META_OBJECT_STRING,

    /* Used for error checking only: */
    WLF_META_OBJECT_LAST
} WlfMetaObjectKind_t, *pWlfMetaObjectKind_t;

#define wlfMetaObjKindIsValid(k) \
    ((WLF_META_OBJECT_NONE < (k)) && ((k) < WLF_META_OBJECT_LAST))

typedef enum {
    WLF_META_VALUE_KIND_NONE = 0,
    WLF_META_VALUE_KIND_INTEGER,
    WLF_META_VALUE_KIND_REAL,
    WLF_META_VALUE_KIND_PATH,
    WLF_META_VALUE_KIND_STRING,
    WLF_META_VALUE_KIND_SYMBOL,
    WLF_META_VALUE_KIND_TIME,
    WLF_META_VALUE_KIND_LAST
} WlfMetaValueKind_t, *pWlfMetaValueKind_t;

#define wlfMetaValueKindIsValid(k)  ((WLF_META_VALUE_KIND_NONE < (k)) && ((k) < WLF_META_VALUE_KIND_LAST))


#ifndef WLFAPIINT_H
/*
 * All meta routines are based on these opaque handles: */
typedef struct WlfMetaHandle_s {char c;} *WlfMetaHandle;
#endif

/* 
 * Memory allocation is handled by function pointers passed to the engine when
 * it is created.  These functions will be used exclusively.
 */
typedef void* (*WlfMetaAlloc_t) (size_t numBytes);
typedef void (*WlfMetaFree_t) (void* mem);
typedef char* (*WlfMetaStrSave_t) (const char* str);



/*---------------- Database Routines: -----------------*/

extern WlfMetaError_t wlfMetaDatabaseBuildFromFile(
    const char*    pFileName,
    WlfMetaHandle  hDB);

extern WlfMetaError_t wlfMetaDatabaseCheckAgainstFile(
    const char*    pFileName,
    WlfMetaHandle  hDB);

extern void wlfMetaDatabaseClearAdditions(
    WlfMetaHandle hDatabase);

extern WlfMetaHandle wlfMetaDatabaseCreate(
    void*             pWlfFile,
    WlfMetaAlloc_t    pAlloc,
    WlfMetaStrSave_t  pStrSave,
    WlfMetaFree_t     pFree);

extern void wlfMetaDatabaseDestroy(
    WlfMetaHandle hDatabase);

extern void wlfMetaDatabaseFlush(
    WlfMetaHandle hDatabase);

extern WlfMetaAlloc_t wlfMetaDatabaseGetAlloc(
    WlfMetaHandle hDatabase);

extern void* wlfMetaDatabaseGetFile(
    WlfMetaHandle hDatabase);

extern WlfMetaFree_t wlfMetaDatabaseGetFree(
    WlfMetaHandle hDatabase);

extern WlfMetaHandle wlfMetaDatabaseGetLinkBySN(
    WlfMetaHandle hDatabase,
    int           SN);

extern WlfMetaHandle wlfMetaDatabaseGetLinks(
    WlfMetaHandle hDatabase);

extern WlfMetaHandle wlfMetaDatabaseGetNamedLinks(
    WlfMetaHandle hDatabase, 
    const char*   pRecordName); 

WlfMetaHandle wlfMetaDatabaseGetNamedLinksForMetaSymbol(
    const char*   pLinkName,
    WlfMetaHandle hSymbol);

extern WlfMetaHandle wlfMetaDatabaseGetNamedLinksForSymbol(
    WlfMetaHandle hDatabase, 
    const char*   pRecordName,
    void*         pSymbol); 

extern WlfMetaHandle wlfMetaDatabaseGetRecordBySN(
    WlfMetaHandle hDatabase,
    int           SN);
    
extern WlfMetaHandle wlfMetaDatabaseGetRecords(
    WlfMetaHandle hDatabase);

extern WlfMetaHandle wlfMetaDatabaseGetNamedRecords(
    WlfMetaHandle hDatabase, 
    const char*   pRecordName); 

extern WlfMetaHandle wlfMetaDatabaseGetNamedRecordsForSymbol(
    WlfMetaHandle hDatabase, 
    const char*   pRecordName,
    void*         pSymbol); 

extern WlfMetaStrSave_t wlfMetaDatabaseGetStrSave(
    WlfMetaHandle hDatabase);

extern int wlfMetaDatabaseHasAdditions(
    WlfMetaHandle hDatabase);

extern int wlfMetaDatabaseHasObjects(
    WlfMetaHandle hDatabase);

extern const char* wlfMetaDatabaseLookupString(
    WlfMetaHandle hDatabase,
    int32_t       SN);

extern WlfMetaError_t wlfMetaDatabaseRegisterRemoteString(
    WlfMetaHandle hDatabase,
    const char*   pString,
    int32_t       SN);

extern WlfMetaError_t wlfMetaDatabaseTellToString(
    WlfMetaHandle hDatabase,
    Tcl_DString*  pDstr);

extern void wlfMetaDatabaseUpdateComplete(
    WlfMetaHandle hDatabase);

extern int wlfMetaDatabaseUpdateNeeded(
    WlfMetaHandle hDatabase);

/*---------------- Error Routines: -----------------*/
extern WlfMetaError_t _wlfMetaErrorSet(
    WlfMetaError_t code,
    char*          fileName,
    int            lineNum);


#define wlfMetaErrorClear()    _wlfMetaErrorSet((WLF_META_OK), __FILE__, __LINE__ )
#define wlfMetaErrorSet(code)  _wlfMetaErrorSet((code), __FILE__, __LINE__ )


extern WlfMetaError_t wlfMetaErrorGetLastCode();

const char* wlfMetaErrorGetLastFile();

int wlfMetaErrorGetLastLine();

extern const char* wlfMetaErrorGetString(
    WlfMetaError_t code);
    
/*---------------- Field Routines: -----------------*/
extern WlfMetaHandle wlfMetaFieldCreate(
    WlfMetaHandle       hRecord,
    const char*         pName,
    WlfMetaValueKind_t  kind,
    int                 numValues,
    void*               pValues);

extern const char* wlfMetaFieldGetName(
    WlfMetaHandle  hField);

extern int wlfMetaFieldGetNumValues(
    WlfMetaHandle  hField);

extern WlfMetaValueKind_t wlfMetaFieldGetValueKind(
    WlfMetaHandle  hField);

extern void* wlfMetaFieldGetValues(
    WlfMetaHandle  hField);


/*---------------- Handle Routines: -----------------*/
extern WlfMetaHandle wlfMetaHandleGetDatabase(
    WlfMetaHandle hObject);

extern int wlfMetaHandleIsKind(
    WlfMetaHandle    hObject,
    WlfMetaObjectKind_t expected);

extern WlfMetaObjectKind_t wlfMetaHandleKind(
    WlfMetaHandle hObject);


/*---------------- Iteration Routines: -----------------*/
extern void wlfMetaIteratorDestroy(
    WlfMetaHandle hIterator);

extern WlfMetaHandle wlfMetaIteratorGetFirst(
    WlfMetaHandle hIterator);

extern WlfMetaHandle wlfMetaIteratorGetNext(
    WlfMetaHandle hIterator);

extern int wlfMetaIteratorNumItems(
    WlfMetaHandle hIterator);


/*---------------- Link Routines: -----------------*/
extern WlfMetaLinkKind_t wlfMetaLinkGetKind(
    WlfMetaHandle  hLink);

extern const char* wlfMetaLinkGetName(
    WlfMetaHandle  hLink);

extern int wlfMetaLinkGetSN(
    WlfMetaHandle  hLink);

extern WlfMetaHandle wlfMetaLinkGetSrcRecord(
    WlfMetaHandle  hLink);

extern WlfMetaHandle wlfMetaLinkGetSrcSymbol(
    WlfMetaHandle  hLink);

extern int wlfMetaLinkGetNumTgtSymbols(
    WlfMetaHandle  hLink);

extern WlfMetaHandle* wlfMetaLinkGetTgtSymbols(
    WlfMetaHandle  hLink);

extern void wlfMetaLinkRecordToSymbolsFull(
    WlfMetaHandle   hDB,
    const char*     pName,
    int             symbolsAreHOID,
    WlfMetaHandle   hRecord,
    int             numTgtSymbols,
    WlfMetaHandle*  phTgtSymbols);

#define wlfMetaLinkRecordToSymbols(db,nm,rec,num,tgts) \
    wlfMetaLinkRecordToSymbolsFull(db,nm,0,rec,num,tgts)


extern void wlfMetaLinkRecordSNToSymbolsFull(
    WlfMetaHandle hDB,
    const char*   pName,
    int           symbolsAreHOID,
    int           recSN,
    int           numTgtSymbols,
    void**        pTgtSymbols);

#define wlfMetaLinkRecordSNToSymbols(db,nm,sn,num,tgts) \
    wlfMetaLinkRecordSNToSymbolsFull(db,nm,0,sn,num,tgts)


void wlfMetaLinkSymbolToSymbolsFull(
    WlfMetaHandle   hDB,
    const char*     pName,
    int             symbolsAreHOID,
    WlfMetaHandle   hSrcSymbol,
    int             numTgtSymbols,
    WlfMetaHandle*  phTgtSymbols);

#define wlfMetaLinkSymbolToSymbols(db,nm,src,num,tgts) \
    wlfMetaLinkSymbolToSymbolsFull(db,nm,0,src,num,tgts)


/*---------------- Record Routines: -----------------*/
extern void wlfMetaRecordClose(
    WlfMetaHandle hRecord);

extern WlfMetaHandle wlfMetaRecordCreate(
    WlfMetaHandle hDatabase, 
    const char*   pName);

extern WlfMetaHandle wlfMetaRecordCreateFieldIterator(
    WlfMetaHandle  hRecord);

extern WlfMetaHandle wlfMetaRecordGetFields(
    WlfMetaHandle hRecord);

extern int wlfMetaRecordGetSN(
    WlfMetaHandle  hRecord);

extern const char* wlfMetaRecordGetName(
    WlfMetaHandle  hRecord);

extern WlfMetaHandle wlfMetaRecordGetNamedFields(
    WlfMetaHandle hRecord,
    const char*   pFieldName);

/*---------------- Symbol Routines: -----------------*/
extern WlfMetaHandle wlfMetaSymbolCreate(
    void*        pUserSymbol,
    const char*  pUserExpr);

extern WlfMetaHandle wlfMetaSymbolCreateFromPath(
    void*        pWlfFile,
    const char*  pUserPath);

extern WlfMetaHandle wlfMetaSymbolFind(
    void*        pUserSymbol,
    const char*  pUserExpr);

extern void* wlfMetaSymbolGetBase(
    WlfMetaHandle hSymbol);

extern const char* wlfMetaSymbolGetExpr(
    WlfMetaHandle hSymbol);

extern int32_t wlfMetaSymbolGetExprSN(
    WlfMetaHandle hSymbol);

extern void* wlfMetaSymbolGetResolved(
    WlfMetaHandle hSymbol);


/*---------------- Helper Routines: -----------------*/
extern const char* wlfMetaLinkKindToString(
    WlfMetaLinkKind_t linkKind);

extern const char* wlfMetaValueKindToString(
    WlfMetaValueKind_t valueKind);

extern const char* wlfMetaSymbolToPath(
    void*             pWlfFile,
    void*             pSymbol);

extern WlfMetaHandle wlfMetaFileGetDatabase(
    void* pLogFile);


#ifdef __cplusplus
}
#endif

#endif   /* #ifndef WLFMETADATA_H */
