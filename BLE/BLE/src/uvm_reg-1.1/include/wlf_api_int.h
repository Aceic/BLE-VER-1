#ifndef WLFAPIINT_H
#define WLFAPIINT_H

/******************************************************************************
*
*   WLF API Extensions for Internal Clients 
*
******************************************************************************/

#include "wlf_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Type Definitions
 */

/* Function pointer types, used for elaboration callbacks */
typedef void (*wlfMkRegionFp)  (void *user_data, char *region_name, char *lib, 
							    char *primary, char *sec, int acc_type);
typedef void (*wlfEndRegionFp) (void *user_data, int acc_type);
typedef void (*wlfMkSigFp)     (void *load_info_vp, char *name, int **an_ptr, 
						        int acc_type, void *td_vp, int port_mode);

typedef struct WlfElabCb_t {
    void           *userData;
    wlfMkRegionFp   mkRegionFp;
    wlfEndRegionFp  endRegionFp;
    wlfMkSigFp      mkSigFp;
} WlfElabCb;


typedef void*        WlfWrtTxDatabaseId;
typedef unsigned int WlfWrtTxId;

#define WLF_TX_INVALID_HANDLE 0


/*-----------------------------------------------------------------------------
 TD CREATION

 This is a pilot WLF2-style API for internal partners.  All routines follow the
 noun-verb-adjective naming convention.  Some general types have specific
 variation depending on what source language is intended.  These routines are
 broken out specifically, and the name of the routine reflects the language.

-----------------------------------------------------------------------------*/

/*
 * Use these symbols when calling any wlfTypeCreate routine that has a
 * wlfTypeKind parameter.
 * 
 * Don't write code that depends on the value of any symbol in this
 * enumeration.  The values and order of these symbols can be changed at any
 * time.  They are NOT to be considered fixed in place.
 */
enum {
    /* C-language: */
    WLF_TYPE_C_ENUM,
    WLF_TYPE_C_BOOL,
    WLF_TYPE_C_UCHAR,
    WLF_TYPE_C_CHAR,
    WLF_TYPE_C_USHORT,
    WLF_TYPE_C_SHORT,
    WLF_TYPE_C_UINT,
    WLF_TYPE_C_INT,
    WLF_TYPE_C_ULONG,
    WLF_TYPE_C_LONG,
    WLF_TYPE_C_ULONGLONG,
    WLF_TYPE_C_LONGLONG,
    WLF_TYPE_C_FLOAT,
    WLF_TYPE_C_DOUBLE,
    WLF_TYPE_C_LONG_DOUBLE,
    WLF_TYPE_C_POINTER,
    WLF_TYPE_C_ARRAY,
    WLF_TYPE_C_STRUCT,
    WLF_TYPE_C_CLASS,
    WLF_TYPE_C_UNION,

    /* SystemC atomic types: */
    WLF_TYPE_SC_BIT,
    WLF_TYPE_SC_LOGIC,

    /* SystemC vector types: */
    WLF_TYPE_SC_BV,
    WLF_TYPE_SC_LV,
    WLF_TYPE_SC_UINT,
    WLF_TYPE_SC_INT,
    WLF_TYPE_SC_BIGUINT,
    WLF_TYPE_SC_BIGINT,
    WLF_TYPE_SC_UNSIGNED,
    WLF_TYPE_SC_SIGNED

};

extern WlfTypeId wlfTypeCreateCArray(
    WlfWrtFileId   wlfFile,
    WlfTypeId      elementType,
    int            numElements);

extern WlfTypeId wlfTypeCreateCBool(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCChar(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCComposite(
    WlfWrtFileId  wlfFile,
    int           wlfTypeKind,
    unsigned int  numMembers,
    char**        memberNames,
    WlfTypeId*    memberTypes);

extern WlfTypeId wlfTypeCreateCDouble(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCEnum(
    WlfWrtFileId wlfFile,
    int     numEnums,
    char**  enumSymbols,
    int*    enumValues);

extern WlfTypeId wlfTypeCreateCInt(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCLong(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCLongDouble(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCLongLong(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCShort(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCString(
    WlfWrtFileId   wlfFile);

extern WlfTypeId wlfTypeCreateCUChar(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCFloat(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCUInt(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCULong(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCULongLong(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateCUShort(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateEnum(
    WlfWrtFileId wlfFile,
    unsigned     num,
    char**       enums);

extern WlfTypeId wlfTypeCreateNativeC(
    WlfWrtFileId   wlfFile,
    int            wlfTypeKind);

extern WlfTypeId wlfTypeCreateScalar(
    WlfWrtFileId wlfFile,
    int          left,
    int          right);

extern WlfTypeId wlfTypeCreateSCBigInt(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCBigUInt(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCBit(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateSCBV(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCInt(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCLogic(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateSCLV(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCScalar(
    WlfWrtFileId wlfFile,
    int          wlfTypeKind);

extern WlfTypeId wlfTypeCreateSCSigned(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCUInt(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCUnsigned(
    WlfWrtFileId   wlfFile,
    int            length);
    
extern WlfTypeId wlfTypeCreateSCVector(
    WlfWrtFileId   wlfFile,
    int            length,
    int            wlfTypeKind);

extern WlfTypeId wlfTypeCreateCString(
    WlfWrtFileId wlfFile);

extern WlfTypeId wlfTypeCreateDynamicArray(
    WlfWrtFileId   wlfFile,
    WlfTypeId      elementType);

/*-----------------------------------------------------------------------------
 TD access routines:
-----------------------------------------------------------------------------*/
extern WlfTypeId wlfTypeGetArrayElementType(
    WlfTypeId arrayTD);

extern int wlfTypeGetSize(
    WlfTypeId td);

extern int wlfTypeGetFieldOffset(
    WlfTypeId td,
    int       fieldNum);
/*-----------------------------------------------------------------------------
 WLF-Drive API

 This is a pilot API for internal partners.
-----------------------------------------------------------------------------*/

/* 
 * Function Declarations
 */

/* Open WLF file in "View Lite" mode.  Instead of contructing a WLF context 
 * tree, the set of callbacks specified in elabCb will called when 
 * processing the WLF table of contents. 
 */
extern WlfFileId wlfFileOpenLite(
    char               *fname, 
    char               *lname, 
	WlfElabCb          *elabCb
);


#define WLF_FILEFLAG_OPEN_AS_MONITOR 		0x0001

extern WlfFileId wlfFileOpenSpecial(
    char               *fname, 
    char               *lname, 
    unsigned int        flags
);


/*  Enable WLF file reader cache, and optionally specify the cache size.  
 *  If cache_size is zero, a default cache size is selected.
 *  NOTE: 'cache_size' should be set to the size in megabytes.
 */
extern int wlfFileEnableCache(
    WlfFileId           fileId, 
    int                 cache_size
);


/* Do setup required for "fast" read (see below).  This must be done once. 
 */
extern int wlfReadDataOverRangeFastInit(
    WlfFileId           fileId, 
	WlfPackId           packId
);


/* Perform "fast" read over the specified data range.  This read resumes 
 * where it left off in the previous scan, rather than re-reading from the
 * start of the buffer.
 */
extern int wlfReadDataOverRangeFast(
    WlfPackId           packId,
    WlfTime64           startTime,
    int                 startDelta,        
    WlfTime64           endTime, 
    int                 endDelta, 
    WlfTimeAdvanceCB    timeAdvCB,
    WlfDeltaAdvanceCB   deltaAdvCB,
    void               *cb_data 
);



/*-----------------------------------------------------------------------------
 DONT-CARE RECORDING

 This is an API for creating don't care signals to be used with wave-window
 display.  wlfWrtCreateDontCare() creates a (global) shadow for a symbol that
 will be used in all wave-window display.  wlfWrtCreateAssertionDontCare()
 creates a shadow that is used only when the user expands an assertion to show
 the child signals.  A global don't-care shadow will be used in assertion-child
 display if an assertion-specific don't-care does not exist.
-----------------------------------------------------------------------------*/
extern WlfSymbolId  wlfWrtCreateDontCare(WlfSymbolId child);
extern WlfSymbolId  wlfWrtCreateAssertionDontCare(WlfSymbolId assertion, WlfSymbolId child);


/*-----------------------------------------------------------------------------
 META DATA SUPPORT
-----------------------------------------------------------------------------*/
#ifndef WLFMETADATA_H
typedef struct WlfMetaHandle_s {char c;} *WlfMetaHandle;
#endif

extern WlfMetaHandle wlfFileGetMetaDatabase(void* fileId);
extern int  wlfFileIsRead(void* fileId);


/*-----------------------------------------------------------------------------
 TRANSACTION RECORDING

 This is a pilot API for internal partners.
-----------------------------------------------------------------------------*/

/* 
 * This is NOT part of the API.  It is for debug when testing a WLF
 * application: 
 */
extern void wlfWrtTxPrint(char* msg);


extern WlfWrtTxDatabaseId wlfWrtTxDatabaseOpen(
    WlfWrtFileId wlfFile);

extern void wlfWrtTxDatabaseClose(
    WlfWrtTxDatabaseId hDB);

extern WlfSymbolId wlfWrtTxStreamCreate(
    WlfWrtTxDatabaseId hDB,
    WlfSymbolId        region,
    const char*        streamName,
    const char*        streamKind,
    int                concurrency,
    int                interestLevel);

extern WlfTypeId wlfWrtTxStreamGetType(
    WlfSymbolId streamId);

extern const char* wlfWrtTxStreamGetKind(
    WlfSymbolId streamId);

extern int wlfWrtTxStreamGetNumANs(
    WlfSymbolId streamId);

extern int* wlfWrtTxStreamGetANs(
    WlfSymbolId streamId);

extern int wlfWrtTxStreamGetInterestLevel(
    WlfSymbolId streamId);

extern WlfWrtTxId  wlfWrtTxBegin(
    WlfSymbolId streamId,
    const char* name,
    WlfTime64    beginTime,
    WlfWrtTxId  parentTx);

extern WlfWrtTxId  wlfWrtTxBeginAndEnd(
    WlfSymbolId streamId,
    const char* name,
    WlfTime64   beginTime,
    WlfTime64   endTime,
    WlfWrtTxId  parentTx);

extern void  wlfWrtTxDelete(
    WlfWrtTxDatabaseId hDB,
    WlfWrtTxId         txId);

extern void  wlfWrtTxEnd(
    WlfWrtTxDatabaseId hDB,
    WlfWrtTxId         txId,
    WlfTime64          endTime);

extern void  wlfWrtTxFree(
    WlfWrtTxDatabaseId hDB,
    WlfWrtTxId         txId);

extern void wlfWrtTxAddAttribute(
    WlfWrtTxDatabaseId hDB,
    WlfWrtTxId   txId,
    WlfTypeId    type,
    void*        value,
    const char*  name,
    int          options);

extern void wlfWrtTxAddRelation(
    WlfWrtTxDatabaseId hDB,
    WlfWrtTxId         srcTxId,
    WlfWrtTxId         tgtTxId,
    const char*        name);

extern int wlfWrtTxStreamGetId(
    WlfSymbolId streamId);

extern int wlfWrtTxStringGetId(
    WlfWrtTxDatabaseId hDB,
    const char* pString);

typedef void (*WlfWrtTxLogStateCB)(WlfSymbolId streamId, int isLogged, void* cbData);

void wlfWrtTxStreamAddLogStateCB(
    WlfSymbolId         streamId,
    WlfWrtTxLogStateCB  cbFunc,
    void*               cbData);

int wlfWrtTxStreamGetLogState(
    WlfSymbolId streamId);

void wlfWrtTxStreamSetLogState(
    WlfSymbolId streamId,
    int         isLogged);

void wlfWrtTxLinkStreamToObject(
    WlfSymbolId sourceStreamId,
    char*       targetPath,
    char*       linkName);

void wlfWrtTxLinkObjectToStream(
    char*       sourcePath,
    WlfSymbolId targetStreamId,
    char*       linkName);

#ifdef __cplusplus
}
#endif

#endif /* WLFAPIINT_H */
