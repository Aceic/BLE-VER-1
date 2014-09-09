#ifndef WLFARCHIVEMAP_h
#define WLFARCHIVEMAP_h

#include "tcl.h"
#include "wlfCommon.h"

int wlfArchiveMapIsArchiveMapped(
    WlfCmnFileId pFile,
    int32_t      an);

int32_t wlfArchiveMapLookupChannelNumber(
    WlfCmnFileId pFile,
    int32_t      an);

void wlfArchiveMapAddEntry(
    WlfCmnFileId pFile,
    int32_t      an,
    int32_t      cn,
    void         (*pCBFunc)(int32_t an, int32_t cn, void* pCBData),
    void*        pCBData);

int wlfArchiveMapNewEntriesToString(
    WlfCmnFileId pFile,
    Tcl_DString* tstr);

void wlfArchiveMapTraverse(
    WlfCmnFileId pFile,
    void         (*pCBFunc)(int32_t an, int32_t cn, void* pCBData),
    void*        pCBData);

#endif   /* #ifndef WLFARCHIVEMAP_h */
