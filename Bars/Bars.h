

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Jan 07 10:26:59 2019
 */
/* Compiler settings for Bars.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __Bars_h__
#define __Bars_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __BarInfoMgr_FWD_DEFINED__
#define __BarInfoMgr_FWD_DEFINED__

#ifdef __cplusplus
typedef class BarInfoMgr BarInfoMgr;
#else
typedef struct BarInfoMgr BarInfoMgr;
#endif /* __cplusplus */

#endif 	/* __BarInfoMgr_FWD_DEFINED__ */


#ifndef __HookData_FWD_DEFINED__
#define __HookData_FWD_DEFINED__

#ifdef __cplusplus
typedef class HookData HookData;
#else
typedef struct HookData HookData;
#endif /* __cplusplus */

#endif 	/* __HookData_FWD_DEFINED__ */


#ifndef __BendData_FWD_DEFINED__
#define __BendData_FWD_DEFINED__

#ifdef __cplusplus
typedef class BendData BendData;
#else
typedef struct BendData BendData;
#endif /* __cplusplus */

#endif 	/* __BendData_FWD_DEFINED__ */


#ifndef __BarData_FWD_DEFINED__
#define __BarData_FWD_DEFINED__

#ifdef __cplusplus
typedef class BarData BarData;
#else
typedef struct BarData BarData;
#endif /* __cplusplus */

#endif 	/* __BarData_FWD_DEFINED__ */


#ifndef __BarCollection_FWD_DEFINED__
#define __BarCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class BarCollection BarCollection;
#else
typedef struct BarCollection BarCollection;
#endif /* __cplusplus */

#endif 	/* __BarCollection_FWD_DEFINED__ */


#ifndef __BarRecord_FWD_DEFINED__
#define __BarRecord_FWD_DEFINED__

#ifdef __cplusplus
typedef class BarRecord BarRecord;
#else
typedef struct BarRecord BarRecord;
#endif /* __cplusplus */

#endif 	/* __BarRecord_FWD_DEFINED__ */


#ifndef __Barlist_FWD_DEFINED__
#define __Barlist_FWD_DEFINED__

#ifdef __cplusplus
typedef class Barlist Barlist;
#else
typedef struct Barlist Barlist;
#endif /* __cplusplus */

#endif 	/* __Barlist_FWD_DEFINED__ */


#ifndef __Group_FWD_DEFINED__
#define __Group_FWD_DEFINED__

#ifdef __cplusplus
typedef class Group Group;
#else
typedef struct Group Group;
#endif /* __cplusplus */

#endif 	/* __Group_FWD_DEFINED__ */


#ifndef __GroupCollection_FWD_DEFINED__
#define __GroupCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class GroupCollection GroupCollection;
#else
typedef struct GroupCollection GroupCollection;
#endif /* __cplusplus */

#endif 	/* __GroupCollection_FWD_DEFINED__ */


#ifndef __BarRecordCollection_FWD_DEFINED__
#define __BarRecordCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class BarRecordCollection BarRecordCollection;
#else
typedef struct BarRecordCollection BarRecordCollection;
#endif /* __cplusplus */

#endif 	/* __BarRecordCollection_FWD_DEFINED__ */


#ifndef __StatusMessageCollection_FWD_DEFINED__
#define __StatusMessageCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class StatusMessageCollection StatusMessageCollection;
#else
typedef struct StatusMessageCollection StatusMessageCollection;
#endif /* __cplusplus */

#endif 	/* __StatusMessageCollection_FWD_DEFINED__ */


#ifndef __StatusMessage_FWD_DEFINED__
#define __StatusMessage_FWD_DEFINED__

#ifdef __cplusplus
typedef class StatusMessage StatusMessage;
#else
typedef struct StatusMessage StatusMessage;
#endif /* __cplusplus */

#endif 	/* __StatusMessage_FWD_DEFINED__ */


#ifndef __AddinMgr_FWD_DEFINED__
#define __AddinMgr_FWD_DEFINED__

#ifdef __cplusplus
typedef class AddinMgr AddinMgr;
#else
typedef struct AddinMgr AddinMgr;
#endif /* __cplusplus */

#endif 	/* __AddinMgr_FWD_DEFINED__ */


#ifndef __Type50_FWD_DEFINED__
#define __Type50_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type50 Type50;
#else
typedef struct Type50 Type50;
#endif /* __cplusplus */

#endif 	/* __Type50_FWD_DEFINED__ */


#ifndef __Type51_FWD_DEFINED__
#define __Type51_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type51 Type51;
#else
typedef struct Type51 Type51;
#endif /* __cplusplus */

#endif 	/* __Type51_FWD_DEFINED__ */


#ifndef __Type52_FWD_DEFINED__
#define __Type52_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type52 Type52;
#else
typedef struct Type52 Type52;
#endif /* __cplusplus */

#endif 	/* __Type52_FWD_DEFINED__ */


#ifndef __Type53_FWD_DEFINED__
#define __Type53_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type53 Type53;
#else
typedef struct Type53 Type53;
#endif /* __cplusplus */

#endif 	/* __Type53_FWD_DEFINED__ */


#ifndef __Type54_FWD_DEFINED__
#define __Type54_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type54 Type54;
#else
typedef struct Type54 Type54;
#endif /* __cplusplus */

#endif 	/* __Type54_FWD_DEFINED__ */


#ifndef __Type55_FWD_DEFINED__
#define __Type55_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type55 Type55;
#else
typedef struct Type55 Type55;
#endif /* __cplusplus */

#endif 	/* __Type55_FWD_DEFINED__ */


#ifndef __Type56_FWD_DEFINED__
#define __Type56_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type56 Type56;
#else
typedef struct Type56 Type56;
#endif /* __cplusplus */

#endif 	/* __Type56_FWD_DEFINED__ */


#ifndef __Type57_FWD_DEFINED__
#define __Type57_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type57 Type57;
#else
typedef struct Type57 Type57;
#endif /* __cplusplus */

#endif 	/* __Type57_FWD_DEFINED__ */


#ifndef __Type58_FWD_DEFINED__
#define __Type58_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type58 Type58;
#else
typedef struct Type58 Type58;
#endif /* __cplusplus */

#endif 	/* __Type58_FWD_DEFINED__ */


#ifndef __Type59_FWD_DEFINED__
#define __Type59_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type59 Type59;
#else
typedef struct Type59 Type59;
#endif /* __cplusplus */

#endif 	/* __Type59_FWD_DEFINED__ */


#ifndef __Type60_FWD_DEFINED__
#define __Type60_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type60 Type60;
#else
typedef struct Type60 Type60;
#endif /* __cplusplus */

#endif 	/* __Type60_FWD_DEFINED__ */


#ifndef __Type61_FWD_DEFINED__
#define __Type61_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type61 Type61;
#else
typedef struct Type61 Type61;
#endif /* __cplusplus */

#endif 	/* __Type61_FWD_DEFINED__ */


#ifndef __Type62_FWD_DEFINED__
#define __Type62_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type62 Type62;
#else
typedef struct Type62 Type62;
#endif /* __cplusplus */

#endif 	/* __Type62_FWD_DEFINED__ */


#ifndef __Type63_FWD_DEFINED__
#define __Type63_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type63 Type63;
#else
typedef struct Type63 Type63;
#endif /* __cplusplus */

#endif 	/* __Type63_FWD_DEFINED__ */


#ifndef __Type66_FWD_DEFINED__
#define __Type66_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type66 Type66;
#else
typedef struct Type66 Type66;
#endif /* __cplusplus */

#endif 	/* __Type66_FWD_DEFINED__ */


#ifndef __Type67_FWD_DEFINED__
#define __Type67_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type67 Type67;
#else
typedef struct Type67 Type67;
#endif /* __cplusplus */

#endif 	/* __Type67_FWD_DEFINED__ */


#ifndef __Type68_FWD_DEFINED__
#define __Type68_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type68 Type68;
#else
typedef struct Type68 Type68;
#endif /* __cplusplus */

#endif 	/* __Type68_FWD_DEFINED__ */


#ifndef __Type69_FWD_DEFINED__
#define __Type69_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type69 Type69;
#else
typedef struct Type69 Type69;
#endif /* __cplusplus */

#endif 	/* __Type69_FWD_DEFINED__ */


#ifndef __Type70_FWD_DEFINED__
#define __Type70_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type70 Type70;
#else
typedef struct Type70 Type70;
#endif /* __cplusplus */

#endif 	/* __Type70_FWD_DEFINED__ */


#ifndef __Type71_FWD_DEFINED__
#define __Type71_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type71 Type71;
#else
typedef struct Type71 Type71;
#endif /* __cplusplus */

#endif 	/* __Type71_FWD_DEFINED__ */


#ifndef __Type72_FWD_DEFINED__
#define __Type72_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type72 Type72;
#else
typedef struct Type72 Type72;
#endif /* __cplusplus */

#endif 	/* __Type72_FWD_DEFINED__ */


#ifndef __Type73_FWD_DEFINED__
#define __Type73_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type73 Type73;
#else
typedef struct Type73 Type73;
#endif /* __cplusplus */

#endif 	/* __Type73_FWD_DEFINED__ */


#ifndef __Type74_FWD_DEFINED__
#define __Type74_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type74 Type74;
#else
typedef struct Type74 Type74;
#endif /* __cplusplus */

#endif 	/* __Type74_FWD_DEFINED__ */


#ifndef __Type75_FWD_DEFINED__
#define __Type75_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type75 Type75;
#else
typedef struct Type75 Type75;
#endif /* __cplusplus */

#endif 	/* __Type75_FWD_DEFINED__ */


#ifndef __Type76_FWD_DEFINED__
#define __Type76_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type76 Type76;
#else
typedef struct Type76 Type76;
#endif /* __cplusplus */

#endif 	/* __Type76_FWD_DEFINED__ */


#ifndef __Type77_FWD_DEFINED__
#define __Type77_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type77 Type77;
#else
typedef struct Type77 Type77;
#endif /* __cplusplus */

#endif 	/* __Type77_FWD_DEFINED__ */


#ifndef __Type78_FWD_DEFINED__
#define __Type78_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type78 Type78;
#else
typedef struct Type78 Type78;
#endif /* __cplusplus */

#endif 	/* __Type78_FWD_DEFINED__ */


#ifndef __Type79_FWD_DEFINED__
#define __Type79_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type79 Type79;
#else
typedef struct Type79 Type79;
#endif /* __cplusplus */

#endif 	/* __Type79_FWD_DEFINED__ */


#ifndef __Type80_FWD_DEFINED__
#define __Type80_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type80 Type80;
#else
typedef struct Type80 Type80;
#endif /* __cplusplus */

#endif 	/* __Type80_FWD_DEFINED__ */


#ifndef __Type81_FWD_DEFINED__
#define __Type81_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type81 Type81;
#else
typedef struct Type81 Type81;
#endif /* __cplusplus */

#endif 	/* __Type81_FWD_DEFINED__ */


#ifndef __Type82_FWD_DEFINED__
#define __Type82_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type82 Type82;
#else
typedef struct Type82 Type82;
#endif /* __cplusplus */

#endif 	/* __Type82_FWD_DEFINED__ */


#ifndef __Type83_FWD_DEFINED__
#define __Type83_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type83 Type83;
#else
typedef struct Type83 Type83;
#endif /* __cplusplus */

#endif 	/* __Type83_FWD_DEFINED__ */


#ifndef __Type84_FWD_DEFINED__
#define __Type84_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type84 Type84;
#else
typedef struct Type84 Type84;
#endif /* __cplusplus */

#endif 	/* __Type84_FWD_DEFINED__ */


#ifndef __Type89_FWD_DEFINED__
#define __Type89_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type89 Type89;
#else
typedef struct Type89 Type89;
#endif /* __cplusplus */

#endif 	/* __Type89_FWD_DEFINED__ */


#ifndef __Type90_FWD_DEFINED__
#define __Type90_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type90 Type90;
#else
typedef struct Type90 Type90;
#endif /* __cplusplus */

#endif 	/* __Type90_FWD_DEFINED__ */


#ifndef __Type91_FWD_DEFINED__
#define __Type91_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type91 Type91;
#else
typedef struct Type91 Type91;
#endif /* __cplusplus */

#endif 	/* __Type91_FWD_DEFINED__ */


#ifndef __Type92_FWD_DEFINED__
#define __Type92_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type92 Type92;
#else
typedef struct Type92 Type92;
#endif /* __cplusplus */

#endif 	/* __Type92_FWD_DEFINED__ */


#ifndef __Type93_FWD_DEFINED__
#define __Type93_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type93 Type93;
#else
typedef struct Type93 Type93;
#endif /* __cplusplus */

#endif 	/* __Type93_FWD_DEFINED__ */


#ifndef __Type94_FWD_DEFINED__
#define __Type94_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type94 Type94;
#else
typedef struct Type94 Type94;
#endif /* __cplusplus */

#endif 	/* __Type94_FWD_DEFINED__ */


#ifndef __Type95_FWD_DEFINED__
#define __Type95_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type95 Type95;
#else
typedef struct Type95 Type95;
#endif /* __cplusplus */

#endif 	/* __Type95_FWD_DEFINED__ */


#ifndef __Type96_FWD_DEFINED__
#define __Type96_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type96 Type96;
#else
typedef struct Type96 Type96;
#endif /* __cplusplus */

#endif 	/* __Type96_FWD_DEFINED__ */


#ifndef __Type97_FWD_DEFINED__
#define __Type97_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type97 Type97;
#else
typedef struct Type97 Type97;
#endif /* __cplusplus */

#endif 	/* __Type97_FWD_DEFINED__ */


#ifndef __Type98_FWD_DEFINED__
#define __Type98_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type98 Type98;
#else
typedef struct Type98 Type98;
#endif /* __cplusplus */

#endif 	/* __Type98_FWD_DEFINED__ */


#ifndef __Type99_FWD_DEFINED__
#define __Type99_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type99 Type99;
#else
typedef struct Type99 Type99;
#endif /* __cplusplus */

#endif 	/* __Type99_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "Barlst.h"
#include "AddinMgr.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __BARSLib_LIBRARY_DEFINED__
#define __BARSLib_LIBRARY_DEFINED__

/* library BARSLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BARSLib;

EXTERN_C const CLSID CLSID_BarInfoMgr;

#ifdef __cplusplus

class DECLSPEC_UUID("F1F5C068-12AA-11D3-8932-006097C68A9C")
BarInfoMgr;
#endif

EXTERN_C const CLSID CLSID_HookData;

#ifdef __cplusplus

class DECLSPEC_UUID("F1F5C06A-12AA-11D3-8932-006097C68A9C")
HookData;
#endif

EXTERN_C const CLSID CLSID_BendData;

#ifdef __cplusplus

class DECLSPEC_UUID("1454C8E4-1380-11D3-8933-006097C68A9C")
BendData;
#endif

EXTERN_C const CLSID CLSID_BarData;

#ifdef __cplusplus

class DECLSPEC_UUID("1454C8E6-1380-11D3-8933-006097C68A9C")
BarData;
#endif

EXTERN_C const CLSID CLSID_BarCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("1454C8EA-1380-11D3-8933-006097C68A9C")
BarCollection;
#endif

EXTERN_C const CLSID CLSID_BarRecord;

#ifdef __cplusplus

class DECLSPEC_UUID("BFC2D3A8-143B-11D3-8934-006097C68A9C")
BarRecord;
#endif

EXTERN_C const CLSID CLSID_Barlist;

#ifdef __cplusplus

class DECLSPEC_UUID("BFC2D3AA-143B-11D3-8934-006097C68A9C")
Barlist;
#endif

EXTERN_C const CLSID CLSID_Group;

#ifdef __cplusplus

class DECLSPEC_UUID("BFC2D3AC-143B-11D3-8934-006097C68A9C")
Group;
#endif

EXTERN_C const CLSID CLSID_GroupCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("BFC2D3AE-143B-11D3-8934-006097C68A9C")
GroupCollection;
#endif

EXTERN_C const CLSID CLSID_BarRecordCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("BFC2D3B0-143B-11D3-8934-006097C68A9C")
BarRecordCollection;
#endif

EXTERN_C const CLSID CLSID_StatusMessageCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("9DE5701D-1919-11D3-893C-006097C68A9C")
StatusMessageCollection;
#endif

EXTERN_C const CLSID CLSID_StatusMessage;

#ifdef __cplusplus

class DECLSPEC_UUID("0A69D046-1DB1-11D3-893F-006097C68A9C")
StatusMessage;
#endif

EXTERN_C const CLSID CLSID_AddinMgr;

#ifdef __cplusplus

class DECLSPEC_UUID("12FCFC9B-1F41-11D3-8941-006097C68A9C")
AddinMgr;
#endif

EXTERN_C const CLSID CLSID_Type50;

#ifdef __cplusplus

class DECLSPEC_UUID("2597E762-150F-11d3-8937-006097C68A9C")
Type50;
#endif

EXTERN_C const CLSID CLSID_Type51;

#ifdef __cplusplus

class DECLSPEC_UUID("0B21EEEE-1539-11d3-8937-006097C68A9C")
Type51;
#endif

EXTERN_C const CLSID CLSID_Type52;

#ifdef __cplusplus

class DECLSPEC_UUID("F02C1198-1F86-11d3-8942-006097C68A9C")
Type52;
#endif

EXTERN_C const CLSID CLSID_Type53;

#ifdef __cplusplus

class DECLSPEC_UUID("735049B2-1F88-11d3-8942-006097C68A9C")
Type53;
#endif

EXTERN_C const CLSID CLSID_Type54;

#ifdef __cplusplus

class DECLSPEC_UUID("E5303A00-1F89-11d3-8942-006097C68A9C")
Type54;
#endif

EXTERN_C const CLSID CLSID_Type55;

#ifdef __cplusplus

class DECLSPEC_UUID("3BB5B3A0-2007-11d3-8944-006097C68A9C")
Type55;
#endif

EXTERN_C const CLSID CLSID_Type56;

#ifdef __cplusplus

class DECLSPEC_UUID("994EC4D4-2007-11d3-8944-006097C68A9C")
Type56;
#endif

EXTERN_C const CLSID CLSID_Type57;

#ifdef __cplusplus

class DECLSPEC_UUID("0F985006-2008-11d3-8944-006097C68A9C")
Type57;
#endif

EXTERN_C const CLSID CLSID_Type58;

#ifdef __cplusplus

class DECLSPEC_UUID("ADCEDC5E-2008-11d3-8944-006097C68A9C")
Type58;
#endif

EXTERN_C const CLSID CLSID_Type59;

#ifdef __cplusplus

class DECLSPEC_UUID("C82A1B4A-2008-11d3-8944-006097C68A9C")
Type59;
#endif

EXTERN_C const CLSID CLSID_Type60;

#ifdef __cplusplus

class DECLSPEC_UUID("9ECEEF80-200A-11d3-8944-006097C68A9C")
Type60;
#endif

EXTERN_C const CLSID CLSID_Type61;

#ifdef __cplusplus

class DECLSPEC_UUID("56F9528C-200E-11d3-8944-006097C68A9C")
Type61;
#endif

EXTERN_C const CLSID CLSID_Type62;

#ifdef __cplusplus

class DECLSPEC_UUID("16599174-2013-11d3-8944-006097C68A9C")
Type62;
#endif

EXTERN_C const CLSID CLSID_Type63;

#ifdef __cplusplus

class DECLSPEC_UUID("05A27426-2014-11d3-8944-006097C68A9C")
Type63;
#endif

EXTERN_C const CLSID CLSID_Type66;

#ifdef __cplusplus

class DECLSPEC_UUID("11E8EBE8-2019-11d3-8944-006097C68A9C")
Type66;
#endif

EXTERN_C const CLSID CLSID_Type67;

#ifdef __cplusplus

class DECLSPEC_UUID("8920C5DC-2014-11d3-8944-006097C68A9C")
Type67;
#endif

EXTERN_C const CLSID CLSID_Type68;

#ifdef __cplusplus

class DECLSPEC_UUID("A6226848-2267-11d3-8945-006097C68A9C")
Type68;
#endif

EXTERN_C const CLSID CLSID_Type69;

#ifdef __cplusplus

class DECLSPEC_UUID("46B52712-226A-11d3-8945-006097C68A9C")
Type69;
#endif

EXTERN_C const CLSID CLSID_Type70;

#ifdef __cplusplus

class DECLSPEC_UUID("97275C84-226E-11d3-8945-006097C68A9C")
Type70;
#endif

EXTERN_C const CLSID CLSID_Type71;

#ifdef __cplusplus

class DECLSPEC_UUID("CB9B5150-2278-11d3-8945-006097C68A9C")
Type71;
#endif

EXTERN_C const CLSID CLSID_Type72;

#ifdef __cplusplus

class DECLSPEC_UUID("37C70690-2273-11d3-8945-006097C68A9C")
Type72;
#endif

EXTERN_C const CLSID CLSID_Type73;

#ifdef __cplusplus

class DECLSPEC_UUID("4264FF58-2273-11d3-8945-006097C68A9C")
Type73;
#endif

EXTERN_C const CLSID CLSID_Type74;

#ifdef __cplusplus

class DECLSPEC_UUID("66B70B7C-227C-11d3-8945-006097C68A9C")
Type74;
#endif

EXTERN_C const CLSID CLSID_Type75;

#ifdef __cplusplus

class DECLSPEC_UUID("0667F52E-2272-11d3-8945-006097C68A9C")
Type75;
#endif

EXTERN_C const CLSID CLSID_Type76;

#ifdef __cplusplus

class DECLSPEC_UUID("8832784E-2273-11d3-8945-006097C68A9C")
Type76;
#endif

EXTERN_C const CLSID CLSID_Type77;

#ifdef __cplusplus

class DECLSPEC_UUID("A2010C7C-2273-11d3-8945-006097C68A9C")
Type77;
#endif

EXTERN_C const CLSID CLSID_Type78;

#ifdef __cplusplus

class DECLSPEC_UUID("A2010C7D-2273-11d3-8945-006097C68A9C")
Type78;
#endif

EXTERN_C const CLSID CLSID_Type79;

#ifdef __cplusplus

class DECLSPEC_UUID("250EC1C4-2286-11d3-8945-006097C68A9C")
Type79;
#endif

EXTERN_C const CLSID CLSID_Type80;

#ifdef __cplusplus

class DECLSPEC_UUID("F8ED1936-2287-11d3-8945-006097C68A9C")
Type80;
#endif

EXTERN_C const CLSID CLSID_Type81;

#ifdef __cplusplus

class DECLSPEC_UUID("4D3D0582-2283-11d3-8945-006097C68A9C")
Type81;
#endif

EXTERN_C const CLSID CLSID_Type82;

#ifdef __cplusplus

class DECLSPEC_UUID("724228F8-2283-11d3-8945-006097C68A9C")
Type82;
#endif

EXTERN_C const CLSID CLSID_Type83;

#ifdef __cplusplus

class DECLSPEC_UUID("E7C3523C-227E-11d3-8945-006097C68A9C")
Type83;
#endif

EXTERN_C const CLSID CLSID_Type84;

#ifdef __cplusplus

class DECLSPEC_UUID("F2B8BB52-2286-11d3-8945-006097C68A9C")
Type84;
#endif

EXTERN_C const CLSID CLSID_Type89;

#ifdef __cplusplus

class DECLSPEC_UUID("AD8A63C4-2280-11d3-8945-006097C68A9C")
Type89;
#endif

EXTERN_C const CLSID CLSID_Type90;

#ifdef __cplusplus

class DECLSPEC_UUID("007D2B14-228D-11d3-8945-006097C68A9C")
Type90;
#endif

EXTERN_C const CLSID CLSID_Type91;

#ifdef __cplusplus

class DECLSPEC_UUID("21647F26-228D-11d3-8945-006097C68A9C")
Type91;
#endif

EXTERN_C const CLSID CLSID_Type92;

#ifdef __cplusplus

class DECLSPEC_UUID("3ECDC162-228D-11d3-8945-006097C68A9C")
Type92;
#endif

EXTERN_C const CLSID CLSID_Type93;

#ifdef __cplusplus

class DECLSPEC_UUID("5D9A4200-228D-11d3-8945-006097C68A9C")
Type93;
#endif

EXTERN_C const CLSID CLSID_Type94;

#ifdef __cplusplus

class DECLSPEC_UUID("78848E04-228D-11d3-8945-006097C68A9C")
Type94;
#endif

EXTERN_C const CLSID CLSID_Type95;

#ifdef __cplusplus

class DECLSPEC_UUID("99AC4194-228D-11d3-8945-006097C68A9C")
Type95;
#endif

EXTERN_C const CLSID CLSID_Type96;

#ifdef __cplusplus

class DECLSPEC_UUID("B60CDECA-228D-11d3-8945-006097C68A9C")
Type96;
#endif

EXTERN_C const CLSID CLSID_Type97;

#ifdef __cplusplus

class DECLSPEC_UUID("CFFCD3E4-228D-11d3-8945-006097C68A9C")
Type97;
#endif

EXTERN_C const CLSID CLSID_Type98;

#ifdef __cplusplus

class DECLSPEC_UUID("ED9362CE-228D-11d3-8945-006097C68A9C")
Type98;
#endif

EXTERN_C const CLSID CLSID_Type99;

#ifdef __cplusplus

class DECLSPEC_UUID("0A6670E4-228E-11d3-8945-006097C68A9C")
Type99;
#endif
#endif /* __BARSLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


