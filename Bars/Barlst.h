

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Jan 07 10:26:56 2019
 */
/* Compiler settings for ..\SDK\Include\Barlst.idl:
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Barlst_h__
#define __Barlst_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBarInfoMgr_FWD_DEFINED__
#define __IBarInfoMgr_FWD_DEFINED__
typedef interface IBarInfoMgr IBarInfoMgr;

#endif 	/* __IBarInfoMgr_FWD_DEFINED__ */


#ifndef __IHookData_FWD_DEFINED__
#define __IHookData_FWD_DEFINED__
typedef interface IHookData IHookData;

#endif 	/* __IHookData_FWD_DEFINED__ */


#ifndef __IBendData_FWD_DEFINED__
#define __IBendData_FWD_DEFINED__
typedef interface IBendData IBendData;

#endif 	/* __IBendData_FWD_DEFINED__ */


#ifndef __IBarData_FWD_DEFINED__
#define __IBarData_FWD_DEFINED__
typedef interface IBarData IBarData;

#endif 	/* __IBarData_FWD_DEFINED__ */


#ifndef __IBarCollection_FWD_DEFINED__
#define __IBarCollection_FWD_DEFINED__
typedef interface IBarCollection IBarCollection;

#endif 	/* __IBarCollection_FWD_DEFINED__ */


#ifndef __IBarRecord_FWD_DEFINED__
#define __IBarRecord_FWD_DEFINED__
typedef interface IBarRecord IBarRecord;

#endif 	/* __IBarRecord_FWD_DEFINED__ */


#ifndef __IBarRecordEvents_FWD_DEFINED__
#define __IBarRecordEvents_FWD_DEFINED__
typedef interface IBarRecordEvents IBarRecordEvents;

#endif 	/* __IBarRecordEvents_FWD_DEFINED__ */


#ifndef __IBarlist_FWD_DEFINED__
#define __IBarlist_FWD_DEFINED__
typedef interface IBarlist IBarlist;

#endif 	/* __IBarlist_FWD_DEFINED__ */


#ifndef __IBarlistEvents_FWD_DEFINED__
#define __IBarlistEvents_FWD_DEFINED__
typedef interface IBarlistEvents IBarlistEvents;

#endif 	/* __IBarlistEvents_FWD_DEFINED__ */


#ifndef __IGroup_FWD_DEFINED__
#define __IGroup_FWD_DEFINED__
typedef interface IGroup IGroup;

#endif 	/* __IGroup_FWD_DEFINED__ */


#ifndef __IGroupEvents_FWD_DEFINED__
#define __IGroupEvents_FWD_DEFINED__
typedef interface IGroupEvents IGroupEvents;

#endif 	/* __IGroupEvents_FWD_DEFINED__ */


#ifndef __IGroupCollection_FWD_DEFINED__
#define __IGroupCollection_FWD_DEFINED__
typedef interface IGroupCollection IGroupCollection;

#endif 	/* __IGroupCollection_FWD_DEFINED__ */


#ifndef __IGroupCollectionEvents_FWD_DEFINED__
#define __IGroupCollectionEvents_FWD_DEFINED__
typedef interface IGroupCollectionEvents IGroupCollectionEvents;

#endif 	/* __IGroupCollectionEvents_FWD_DEFINED__ */


#ifndef __IBarRecordCollection_FWD_DEFINED__
#define __IBarRecordCollection_FWD_DEFINED__
typedef interface IBarRecordCollection IBarRecordCollection;

#endif 	/* __IBarRecordCollection_FWD_DEFINED__ */


#ifndef __IBarRecordCollectionEvents_FWD_DEFINED__
#define __IBarRecordCollectionEvents_FWD_DEFINED__
typedef interface IBarRecordCollectionEvents IBarRecordCollectionEvents;

#endif 	/* __IBarRecordCollectionEvents_FWD_DEFINED__ */


#ifndef __IBend_FWD_DEFINED__
#define __IBend_FWD_DEFINED__
typedef interface IBend IBend;

#endif 	/* __IBend_FWD_DEFINED__ */


#ifndef __IBendEvents_FWD_DEFINED__
#define __IBendEvents_FWD_DEFINED__
typedef interface IBendEvents IBendEvents;

#endif 	/* __IBendEvents_FWD_DEFINED__ */


#ifndef __IStatusMessage_FWD_DEFINED__
#define __IStatusMessage_FWD_DEFINED__
typedef interface IStatusMessage IStatusMessage;

#endif 	/* __IStatusMessage_FWD_DEFINED__ */


#ifndef __IStatusMessageCollection_FWD_DEFINED__
#define __IStatusMessageCollection_FWD_DEFINED__
typedef interface IStatusMessageCollection IStatusMessageCollection;

#endif 	/* __IStatusMessageCollection_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Barlst_0000_0000 */
/* [local] */ 









typedef /* [helpstring][uuid][public] */  DECLSPEC_UUID("21A44900-1381-11d3-8933-006097C68A9C") 
enum HookType
    {
        ht90	= 0,
        ht135	= ( ht90 + 1 ) ,
        ht180	= ( ht135 + 1 ) 
    } 	HookType;

typedef /* [helpstring][uuid][public] */  DECLSPEC_UUID("21A44901-1381-11d3-8933-006097C68A9C") 
enum UseType
    {
        utLongitudinal	= 0,
        utTransverse	= ( utLongitudinal + 1 ) ,
        utSeismic	= ( utTransverse + 1 ) 
    } 	UseType;

typedef /* [helpstring][uuid][public] */  DECLSPEC_UUID("DCB18CC0-147C-11d3-8934-006097C68A9C") 
enum StatusType
    {
        stOK	= 0,
        stWarning	= 1,
        stError	= 2
    } 	StatusType;

typedef /* [helpstring][uuid][public] */  DECLSPEC_UUID("AC18BAF6-22A6-11d3-8945-006097C68A9C") 
enum DimensionType
    {
        dimU	= 0,
        dimW	= 1,
        dimX	= 2,
        dimY	= 3,
        dimZ	= 4,
        dimT1	= 5,
        dimT2	= 6
    } 	DimensionType;

typedef /* [helpstring][uuid][public] */  DECLSPEC_UUID("F220640C-825C-11d3-89D1-006097C68A9C") 
enum MoveType
    {
        mtBefore	= 1,
        mtAfter	= 2
    } 	MoveType;



extern RPC_IF_HANDLE __MIDL_itf_Barlst_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Barlst_0000_0000_v0_0_s_ifspec;

#ifndef __IBarInfoMgr_INTERFACE_DEFINED__
#define __IBarInfoMgr_INTERFACE_DEFINED__

/* interface IBarInfoMgr */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBarInfoMgr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F1F5C067-12AA-11D3-8932-006097C68A9C")
    IBarInfoMgr : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Specification( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Bars( 
            /* [retval][out] */ IBarCollection **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarInfoMgrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarInfoMgr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarInfoMgr * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarInfoMgr * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBarInfoMgr * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBarInfoMgr * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBarInfoMgr * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBarInfoMgr * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Specification )( 
            IBarInfoMgr * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Bars )( 
            IBarInfoMgr * This,
            /* [retval][out] */ IBarCollection **pVal);
        
        END_INTERFACE
    } IBarInfoMgrVtbl;

    interface IBarInfoMgr
    {
        CONST_VTBL struct IBarInfoMgrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarInfoMgr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarInfoMgr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarInfoMgr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarInfoMgr_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBarInfoMgr_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBarInfoMgr_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBarInfoMgr_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBarInfoMgr_get_Specification(This,pVal)	\
    ( (This)->lpVtbl -> get_Specification(This,pVal) ) 

#define IBarInfoMgr_get_Bars(This,pVal)	\
    ( (This)->lpVtbl -> get_Bars(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarInfoMgr_INTERFACE_DEFINED__ */


#ifndef __IHookData_INTERFACE_DEFINED__
#define __IHookData_INTERFACE_DEFINED__

/* interface IHookData */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHookData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F1F5C069-12AA-11D3-8932-006097C68A9C")
    IHookData : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ HookType *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Use( 
            /* [retval][out] */ UseType *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Tail( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MinTail( 
            /* [retval][out] */ double *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHookDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHookData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHookData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHookData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHookData * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHookData * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHookData * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHookData * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IHookData * This,
            /* [retval][out] */ HookType *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Use )( 
            IHookData * This,
            /* [retval][out] */ UseType *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Tail )( 
            IHookData * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MinTail )( 
            IHookData * This,
            /* [retval][out] */ double *pVal);
        
        END_INTERFACE
    } IHookDataVtbl;

    interface IHookData
    {
        CONST_VTBL struct IHookDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHookData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHookData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHookData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHookData_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHookData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHookData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHookData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHookData_get_Type(This,pVal)	\
    ( (This)->lpVtbl -> get_Type(This,pVal) ) 

#define IHookData_get_Use(This,pVal)	\
    ( (This)->lpVtbl -> get_Use(This,pVal) ) 

#define IHookData_get_Tail(This,pVal)	\
    ( (This)->lpVtbl -> get_Tail(This,pVal) ) 

#define IHookData_get_MinTail(This,pVal)	\
    ( (This)->lpVtbl -> get_MinTail(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHookData_INTERFACE_DEFINED__ */


#ifndef __IBendData_INTERFACE_DEFINED__
#define __IBendData_INTERFACE_DEFINED__

/* interface IBendData */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBendData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1454C8E3-1380-11D3-8933-006097C68A9C")
    IBendData : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Use( 
            /* [retval][out] */ UseType *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InsideDiameter( 
            /* [retval][out] */ double *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBendDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBendData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBendData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBendData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBendData * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBendData * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBendData * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBendData * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Use )( 
            IBendData * This,
            /* [retval][out] */ UseType *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InsideDiameter )( 
            IBendData * This,
            /* [retval][out] */ double *pVal);
        
        END_INTERFACE
    } IBendDataVtbl;

    interface IBendData
    {
        CONST_VTBL struct IBendDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBendData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBendData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBendData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBendData_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBendData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBendData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBendData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBendData_get_Use(This,pVal)	\
    ( (This)->lpVtbl -> get_Use(This,pVal) ) 

#define IBendData_get_InsideDiameter(This,pVal)	\
    ( (This)->lpVtbl -> get_InsideDiameter(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBendData_INTERFACE_DEFINED__ */


#ifndef __IBarData_INTERFACE_DEFINED__
#define __IBarData_INTERFACE_DEFINED__

/* interface IBarData */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBarData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1454C8E5-1380-11D3-8933-006097C68A9C")
    IBarData : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Diameter( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mass( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NormalLength( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxLength( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BendData( 
            /* [in] */ UseType use,
            /* [retval][out] */ IBendData **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HookData( 
            /* [in] */ UseType use,
            /* [in] */ HookType hook,
            /* [retval][out] */ IHookData **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBarData * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBarData * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBarData * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBarData * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Diameter )( 
            IBarData * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mass )( 
            IBarData * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NormalLength )( 
            IBarData * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxLength )( 
            IBarData * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BendData )( 
            IBarData * This,
            /* [in] */ UseType use,
            /* [retval][out] */ IBendData **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HookData )( 
            IBarData * This,
            /* [in] */ UseType use,
            /* [in] */ HookType hook,
            /* [retval][out] */ IHookData **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IBarData * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IBarDataVtbl;

    interface IBarData
    {
        CONST_VTBL struct IBarDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarData_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBarData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBarData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBarData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBarData_get_Diameter(This,pVal)	\
    ( (This)->lpVtbl -> get_Diameter(This,pVal) ) 

#define IBarData_get_Mass(This,pVal)	\
    ( (This)->lpVtbl -> get_Mass(This,pVal) ) 

#define IBarData_get_NormalLength(This,pVal)	\
    ( (This)->lpVtbl -> get_NormalLength(This,pVal) ) 

#define IBarData_get_MaxLength(This,pVal)	\
    ( (This)->lpVtbl -> get_MaxLength(This,pVal) ) 

#define IBarData_get_BendData(This,use,pVal)	\
    ( (This)->lpVtbl -> get_BendData(This,use,pVal) ) 

#define IBarData_get_HookData(This,use,hook,pVal)	\
    ( (This)->lpVtbl -> get_HookData(This,use,hook,pVal) ) 

#define IBarData_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarData_INTERFACE_DEFINED__ */


#ifndef __IBarCollection_INTERFACE_DEFINED__
#define __IBarCollection_INTERFACE_DEFINED__

/* interface IBarCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBarCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1454C8E9-1380-11D3-8933-006097C68A9C")
    IBarCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT varIndex,
            /* [retval][out] */ IBarData **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBarCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBarCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBarCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBarCollection * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IBarCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IBarCollection * This,
            /* [retval][out] */ LPUNKNOWN *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IBarCollection * This,
            /* [in] */ VARIANT varIndex,
            /* [retval][out] */ IBarData **pVal);
        
        END_INTERFACE
    } IBarCollectionVtbl;

    interface IBarCollection
    {
        CONST_VTBL struct IBarCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBarCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBarCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBarCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBarCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IBarCollection_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#define IBarCollection_get_Item(This,varIndex,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,varIndex,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarCollection_INTERFACE_DEFINED__ */


#ifndef __IBarRecord_INTERFACE_DEFINED__
#define __IBarRecord_INTERFACE_DEFINED__

/* interface IBarRecord */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBarRecord;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFC2D3A7-143B-11D3-8934-006097C68A9C")
    IBarRecord : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mark( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mark( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Location( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Location( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumReqd( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NumReqd( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Use( 
            /* [retval][out] */ UseType *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Use( 
            /* [in] */ UseType newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LumpSum( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LumpSum( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Substructure( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Substructure( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Epoxy( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Epoxy( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Varies( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumEach( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NumEach( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PrimaryBend( 
            /* [retval][out] */ IBend **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PrimaryBend( 
            /* [in] */ IBend *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VariesBend( 
            /* [retval][out] */ IBend **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_VariesBend( 
            /* [in] */ IBend *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BendType( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Size( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ StatusType *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mass( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BarData( 
            /* [retval][out] */ IBarData **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BarData( 
            /* [in] */ IBarData *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_StatusMessages( 
            /* [retval][out] */ IStatusMessageCollection **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarRecordVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarRecord * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarRecord * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarRecord * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBarRecord * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBarRecord * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBarRecord * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBarRecord * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mark )( 
            IBarRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Mark )( 
            IBarRecord * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Location )( 
            IBarRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Location )( 
            IBarRecord * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumReqd )( 
            IBarRecord * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NumReqd )( 
            IBarRecord * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Use )( 
            IBarRecord * This,
            /* [retval][out] */ UseType *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Use )( 
            IBarRecord * This,
            /* [in] */ UseType newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LumpSum )( 
            IBarRecord * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LumpSum )( 
            IBarRecord * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Substructure )( 
            IBarRecord * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Substructure )( 
            IBarRecord * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Epoxy )( 
            IBarRecord * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Epoxy )( 
            IBarRecord * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Varies )( 
            IBarRecord * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumEach )( 
            IBarRecord * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NumEach )( 
            IBarRecord * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PrimaryBend )( 
            IBarRecord * This,
            /* [retval][out] */ IBend **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PrimaryBend )( 
            IBarRecord * This,
            /* [in] */ IBend *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VariesBend )( 
            IBarRecord * This,
            /* [retval][out] */ IBend **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_VariesBend )( 
            IBarRecord * This,
            /* [in] */ IBend *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BendType )( 
            IBarRecord * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Size )( 
            IBarRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IBarRecord * This,
            /* [retval][out] */ StatusType *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mass )( 
            IBarRecord * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BarData )( 
            IBarRecord * This,
            /* [retval][out] */ IBarData **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BarData )( 
            IBarRecord * This,
            /* [in] */ IBarData *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_StatusMessages )( 
            IBarRecord * This,
            /* [retval][out] */ IStatusMessageCollection **pVal);
        
        END_INTERFACE
    } IBarRecordVtbl;

    interface IBarRecord
    {
        CONST_VTBL struct IBarRecordVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarRecord_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarRecord_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarRecord_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarRecord_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBarRecord_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBarRecord_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBarRecord_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBarRecord_get_Mark(This,pVal)	\
    ( (This)->lpVtbl -> get_Mark(This,pVal) ) 

#define IBarRecord_put_Mark(This,newVal)	\
    ( (This)->lpVtbl -> put_Mark(This,newVal) ) 

#define IBarRecord_get_Location(This,pVal)	\
    ( (This)->lpVtbl -> get_Location(This,pVal) ) 

#define IBarRecord_put_Location(This,newVal)	\
    ( (This)->lpVtbl -> put_Location(This,newVal) ) 

#define IBarRecord_get_NumReqd(This,pVal)	\
    ( (This)->lpVtbl -> get_NumReqd(This,pVal) ) 

#define IBarRecord_put_NumReqd(This,newVal)	\
    ( (This)->lpVtbl -> put_NumReqd(This,newVal) ) 

#define IBarRecord_get_Use(This,pVal)	\
    ( (This)->lpVtbl -> get_Use(This,pVal) ) 

#define IBarRecord_put_Use(This,newVal)	\
    ( (This)->lpVtbl -> put_Use(This,newVal) ) 

#define IBarRecord_get_LumpSum(This,pVal)	\
    ( (This)->lpVtbl -> get_LumpSum(This,pVal) ) 

#define IBarRecord_put_LumpSum(This,newVal)	\
    ( (This)->lpVtbl -> put_LumpSum(This,newVal) ) 

#define IBarRecord_get_Substructure(This,pVal)	\
    ( (This)->lpVtbl -> get_Substructure(This,pVal) ) 

#define IBarRecord_put_Substructure(This,newVal)	\
    ( (This)->lpVtbl -> put_Substructure(This,newVal) ) 

#define IBarRecord_get_Epoxy(This,pVal)	\
    ( (This)->lpVtbl -> get_Epoxy(This,pVal) ) 

#define IBarRecord_put_Epoxy(This,newVal)	\
    ( (This)->lpVtbl -> put_Epoxy(This,newVal) ) 

#define IBarRecord_get_Varies(This,pVal)	\
    ( (This)->lpVtbl -> get_Varies(This,pVal) ) 

#define IBarRecord_get_NumEach(This,pVal)	\
    ( (This)->lpVtbl -> get_NumEach(This,pVal) ) 

#define IBarRecord_put_NumEach(This,newVal)	\
    ( (This)->lpVtbl -> put_NumEach(This,newVal) ) 

#define IBarRecord_get_PrimaryBend(This,pVal)	\
    ( (This)->lpVtbl -> get_PrimaryBend(This,pVal) ) 

#define IBarRecord_put_PrimaryBend(This,newVal)	\
    ( (This)->lpVtbl -> put_PrimaryBend(This,newVal) ) 

#define IBarRecord_get_VariesBend(This,pVal)	\
    ( (This)->lpVtbl -> get_VariesBend(This,pVal) ) 

#define IBarRecord_put_VariesBend(This,newVal)	\
    ( (This)->lpVtbl -> put_VariesBend(This,newVal) ) 

#define IBarRecord_get_BendType(This,pVal)	\
    ( (This)->lpVtbl -> get_BendType(This,pVal) ) 

#define IBarRecord_get_Size(This,pVal)	\
    ( (This)->lpVtbl -> get_Size(This,pVal) ) 

#define IBarRecord_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define IBarRecord_get_Mass(This,pVal)	\
    ( (This)->lpVtbl -> get_Mass(This,pVal) ) 

#define IBarRecord_get_BarData(This,pVal)	\
    ( (This)->lpVtbl -> get_BarData(This,pVal) ) 

#define IBarRecord_put_BarData(This,newVal)	\
    ( (This)->lpVtbl -> put_BarData(This,newVal) ) 

#define IBarRecord_get_StatusMessages(This,pVal)	\
    ( (This)->lpVtbl -> get_StatusMessages(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarRecord_INTERFACE_DEFINED__ */


#ifndef __IBarRecordEvents_INTERFACE_DEFINED__
#define __IBarRecordEvents_INTERFACE_DEFINED__

/* interface IBarRecordEvents */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IBarRecordEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C90BC1E0-1869-11d3-893B-006097C68A9C")
    IBarRecordEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordChanged( 
            /* [in] */ IBarRecord *pBarRecord) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarRecordEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarRecordEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarRecordEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarRecordEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordChanged )( 
            IBarRecordEvents * This,
            /* [in] */ IBarRecord *pBarRecord);
        
        END_INTERFACE
    } IBarRecordEventsVtbl;

    interface IBarRecordEvents
    {
        CONST_VTBL struct IBarRecordEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarRecordEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarRecordEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarRecordEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarRecordEvents_OnBarRecordChanged(This,pBarRecord)	\
    ( (This)->lpVtbl -> OnBarRecordChanged(This,pBarRecord) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarRecordEvents_INTERFACE_DEFINED__ */


#ifndef __IBarlist_INTERFACE_DEFINED__
#define __IBarlist_INTERFACE_DEFINED__

/* interface IBarlist */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBarlist;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFC2D3A9-143B-11D3-8934-006097C68A9C")
    IBarlist : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Groups( 
            /* [retval][out] */ IGroupCollection **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TrafficBarrierQuantity( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TrafficBarrierQuantity( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BridgeGrateInletQuantity( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BridgeGrateInletQuantity( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RetainingWallQuantity( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RetainingWallQuantity( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SuperstructureMass( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SuperstructureMassEpoxy( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SubstructureMass( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SubstructureMassEpoxy( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ StatusType *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Project( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Project( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_JobNumber( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_JobNumber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Engineer( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Engineer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Company( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Company( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Comments( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Comments( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarlistVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarlist * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarlist * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarlist * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBarlist * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBarlist * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBarlist * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBarlist * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Groups )( 
            IBarlist * This,
            /* [retval][out] */ IGroupCollection **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TrafficBarrierQuantity )( 
            IBarlist * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TrafficBarrierQuantity )( 
            IBarlist * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BridgeGrateInletQuantity )( 
            IBarlist * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BridgeGrateInletQuantity )( 
            IBarlist * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RetainingWallQuantity )( 
            IBarlist * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RetainingWallQuantity )( 
            IBarlist * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SuperstructureMass )( 
            IBarlist * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SuperstructureMassEpoxy )( 
            IBarlist * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubstructureMass )( 
            IBarlist * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubstructureMassEpoxy )( 
            IBarlist * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IBarlist * This,
            /* [retval][out] */ StatusType *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Project )( 
            IBarlist * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Project )( 
            IBarlist * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_JobNumber )( 
            IBarlist * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_JobNumber )( 
            IBarlist * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Engineer )( 
            IBarlist * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Engineer )( 
            IBarlist * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Company )( 
            IBarlist * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Company )( 
            IBarlist * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Comments )( 
            IBarlist * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Comments )( 
            IBarlist * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IBarlistVtbl;

    interface IBarlist
    {
        CONST_VTBL struct IBarlistVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarlist_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarlist_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarlist_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarlist_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBarlist_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBarlist_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBarlist_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBarlist_get_Groups(This,pVal)	\
    ( (This)->lpVtbl -> get_Groups(This,pVal) ) 

#define IBarlist_get_TrafficBarrierQuantity(This,pVal)	\
    ( (This)->lpVtbl -> get_TrafficBarrierQuantity(This,pVal) ) 

#define IBarlist_put_TrafficBarrierQuantity(This,newVal)	\
    ( (This)->lpVtbl -> put_TrafficBarrierQuantity(This,newVal) ) 

#define IBarlist_get_BridgeGrateInletQuantity(This,pVal)	\
    ( (This)->lpVtbl -> get_BridgeGrateInletQuantity(This,pVal) ) 

#define IBarlist_put_BridgeGrateInletQuantity(This,newVal)	\
    ( (This)->lpVtbl -> put_BridgeGrateInletQuantity(This,newVal) ) 

#define IBarlist_get_RetainingWallQuantity(This,pVal)	\
    ( (This)->lpVtbl -> get_RetainingWallQuantity(This,pVal) ) 

#define IBarlist_put_RetainingWallQuantity(This,newVal)	\
    ( (This)->lpVtbl -> put_RetainingWallQuantity(This,newVal) ) 

#define IBarlist_get_SuperstructureMass(This,pVal)	\
    ( (This)->lpVtbl -> get_SuperstructureMass(This,pVal) ) 

#define IBarlist_get_SuperstructureMassEpoxy(This,pVal)	\
    ( (This)->lpVtbl -> get_SuperstructureMassEpoxy(This,pVal) ) 

#define IBarlist_get_SubstructureMass(This,pVal)	\
    ( (This)->lpVtbl -> get_SubstructureMass(This,pVal) ) 

#define IBarlist_get_SubstructureMassEpoxy(This,pVal)	\
    ( (This)->lpVtbl -> get_SubstructureMassEpoxy(This,pVal) ) 

#define IBarlist_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define IBarlist_get_Project(This,pVal)	\
    ( (This)->lpVtbl -> get_Project(This,pVal) ) 

#define IBarlist_put_Project(This,newVal)	\
    ( (This)->lpVtbl -> put_Project(This,newVal) ) 

#define IBarlist_get_JobNumber(This,pVal)	\
    ( (This)->lpVtbl -> get_JobNumber(This,pVal) ) 

#define IBarlist_put_JobNumber(This,newVal)	\
    ( (This)->lpVtbl -> put_JobNumber(This,newVal) ) 

#define IBarlist_get_Engineer(This,pVal)	\
    ( (This)->lpVtbl -> get_Engineer(This,pVal) ) 

#define IBarlist_put_Engineer(This,newVal)	\
    ( (This)->lpVtbl -> put_Engineer(This,newVal) ) 

#define IBarlist_get_Company(This,pVal)	\
    ( (This)->lpVtbl -> get_Company(This,pVal) ) 

#define IBarlist_put_Company(This,newVal)	\
    ( (This)->lpVtbl -> put_Company(This,newVal) ) 

#define IBarlist_get_Comments(This,pVal)	\
    ( (This)->lpVtbl -> get_Comments(This,pVal) ) 

#define IBarlist_put_Comments(This,newVal)	\
    ( (This)->lpVtbl -> put_Comments(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarlist_INTERFACE_DEFINED__ */


#ifndef __IBarlistEvents_INTERFACE_DEFINED__
#define __IBarlistEvents_INTERFACE_DEFINED__

/* interface IBarlistEvents */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IBarlistEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E43ECCBC-1875-11d3-893B-006097C68A9C")
    IBarlistEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnNotIncludedQuantitiesChanged( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupAdded( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupRemoved( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupChanged( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupMoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordAdded( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pNewRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordChanged( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordRemoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ BSTR bstrMark) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordsSorted( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordMoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarlistEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarlistEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarlistEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarlistEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnNotIncludedQuantitiesChanged )( 
            IBarlistEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupAdded )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupRemoved )( 
            IBarlistEvents * This,
            /* [in] */ BSTR Name);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupChanged )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupMoved )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordAdded )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pNewRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordChanged )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordRemoved )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ BSTR bstrMark);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordsSorted )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordMoved )( 
            IBarlistEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo);
        
        END_INTERFACE
    } IBarlistEventsVtbl;

    interface IBarlistEvents
    {
        CONST_VTBL struct IBarlistEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarlistEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarlistEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarlistEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarlistEvents_OnNotIncludedQuantitiesChanged(This)	\
    ( (This)->lpVtbl -> OnNotIncludedQuantitiesChanged(This) ) 

#define IBarlistEvents_OnGroupAdded(This,pGroup)	\
    ( (This)->lpVtbl -> OnGroupAdded(This,pGroup) ) 

#define IBarlistEvents_OnGroupRemoved(This,Name)	\
    ( (This)->lpVtbl -> OnGroupRemoved(This,Name) ) 

#define IBarlistEvents_OnGroupChanged(This,pGroup)	\
    ( (This)->lpVtbl -> OnGroupChanged(This,pGroup) ) 

#define IBarlistEvents_OnGroupMoved(This,pGroup,idxFrom,idxTo)	\
    ( (This)->lpVtbl -> OnGroupMoved(This,pGroup,idxFrom,idxTo) ) 

#define IBarlistEvents_OnBarRecordAdded(This,pGroup,pNewRecord)	\
    ( (This)->lpVtbl -> OnBarRecordAdded(This,pGroup,pNewRecord) ) 

#define IBarlistEvents_OnBarRecordChanged(This,pGroup,pBarRecord)	\
    ( (This)->lpVtbl -> OnBarRecordChanged(This,pGroup,pBarRecord) ) 

#define IBarlistEvents_OnBarRecordRemoved(This,pGroup,bstrMark)	\
    ( (This)->lpVtbl -> OnBarRecordRemoved(This,pGroup,bstrMark) ) 

#define IBarlistEvents_OnBarRecordsSorted(This,pGroup)	\
    ( (This)->lpVtbl -> OnBarRecordsSorted(This,pGroup) ) 

#define IBarlistEvents_OnBarRecordMoved(This,pGroup,pBarRecord,idxFrom,idxTo)	\
    ( (This)->lpVtbl -> OnBarRecordMoved(This,pGroup,pBarRecord,idxFrom,idxTo) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarlistEvents_INTERFACE_DEFINED__ */


#ifndef __IGroup_INTERFACE_DEFINED__
#define __IGroup_INTERFACE_DEFINED__

/* interface IGroup */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IGroup;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFC2D3AB-143B-11D3-8934-006097C68A9C")
    IGroup : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BarRecords( 
            /* [retval][out] */ IBarRecordCollection **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SuperstructureMass( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SubstructureMass( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SubstructureMassEpoxy( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SuperstructureMassEpoxy( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ StatusType *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGroupVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGroup * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGroup * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGroup * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGroup * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGroup * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGroup * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGroup * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IGroup * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IGroup * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BarRecords )( 
            IGroup * This,
            /* [retval][out] */ IBarRecordCollection **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SuperstructureMass )( 
            IGroup * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubstructureMass )( 
            IGroup * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubstructureMassEpoxy )( 
            IGroup * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SuperstructureMassEpoxy )( 
            IGroup * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IGroup * This,
            /* [retval][out] */ StatusType *pVal);
        
        END_INTERFACE
    } IGroupVtbl;

    interface IGroup
    {
        CONST_VTBL struct IGroupVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGroup_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGroup_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGroup_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGroup_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGroup_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGroup_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGroup_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGroup_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IGroup_put_Name(This,newVal)	\
    ( (This)->lpVtbl -> put_Name(This,newVal) ) 

#define IGroup_get_BarRecords(This,pVal)	\
    ( (This)->lpVtbl -> get_BarRecords(This,pVal) ) 

#define IGroup_get_SuperstructureMass(This,pVal)	\
    ( (This)->lpVtbl -> get_SuperstructureMass(This,pVal) ) 

#define IGroup_get_SubstructureMass(This,pVal)	\
    ( (This)->lpVtbl -> get_SubstructureMass(This,pVal) ) 

#define IGroup_get_SubstructureMassEpoxy(This,pVal)	\
    ( (This)->lpVtbl -> get_SubstructureMassEpoxy(This,pVal) ) 

#define IGroup_get_SuperstructureMassEpoxy(This,pVal)	\
    ( (This)->lpVtbl -> get_SuperstructureMassEpoxy(This,pVal) ) 

#define IGroup_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGroup_INTERFACE_DEFINED__ */


#ifndef __IGroupEvents_INTERFACE_DEFINED__
#define __IGroupEvents_INTERFACE_DEFINED__

/* interface IGroupEvents */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IGroupEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9E3CC3A4-1870-11d3-893B-006097C68A9C")
    IGroupEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupChanged( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordAdded( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pNewRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordChanged( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordRemoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ BSTR bstrMark) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordsSorted( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordMoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGroupEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGroupEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGroupEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGroupEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupChanged )( 
            IGroupEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordAdded )( 
            IGroupEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pNewRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordChanged )( 
            IGroupEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordRemoved )( 
            IGroupEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ BSTR bstrMark);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordsSorted )( 
            IGroupEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordMoved )( 
            IGroupEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo);
        
        END_INTERFACE
    } IGroupEventsVtbl;

    interface IGroupEvents
    {
        CONST_VTBL struct IGroupEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGroupEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGroupEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGroupEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGroupEvents_OnGroupChanged(This,pGroup)	\
    ( (This)->lpVtbl -> OnGroupChanged(This,pGroup) ) 

#define IGroupEvents_OnBarRecordAdded(This,pGroup,pNewRecord)	\
    ( (This)->lpVtbl -> OnBarRecordAdded(This,pGroup,pNewRecord) ) 

#define IGroupEvents_OnBarRecordChanged(This,pGroup,pBarRecord)	\
    ( (This)->lpVtbl -> OnBarRecordChanged(This,pGroup,pBarRecord) ) 

#define IGroupEvents_OnBarRecordRemoved(This,pGroup,bstrMark)	\
    ( (This)->lpVtbl -> OnBarRecordRemoved(This,pGroup,bstrMark) ) 

#define IGroupEvents_OnBarRecordsSorted(This,pGroup)	\
    ( (This)->lpVtbl -> OnBarRecordsSorted(This,pGroup) ) 

#define IGroupEvents_OnBarRecordMoved(This,pGroup,pBarRecord,idxFrom,idxTo)	\
    ( (This)->lpVtbl -> OnBarRecordMoved(This,pGroup,pBarRecord,idxFrom,idxTo) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGroupEvents_INTERFACE_DEFINED__ */


#ifndef __IGroupCollection_INTERFACE_DEFINED__
#define __IGroupCollection_INTERFACE_DEFINED__

/* interface IGroupCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IGroupCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFC2D3AD-143B-11D3-8934-006097C68A9C")
    IGroupCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT varIndex,
            /* [retval][out] */ IGroup **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR group) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT varIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ VARIANT grp,
            /* [in] */ MoveType mt,
            /* [in] */ VARIANT target) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveUp( 
            /* [in] */ VARIANT grp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveDown( 
            /* [in] */ VARIANT grp) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGroupCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGroupCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGroupCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGroupCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGroupCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGroupCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGroupCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGroupCollection * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IGroupCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IGroupCollection * This,
            /* [retval][out] */ LPUNKNOWN *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IGroupCollection * This,
            /* [in] */ VARIANT varIndex,
            /* [retval][out] */ IGroup **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IGroupCollection * This,
            /* [in] */ BSTR group);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IGroupCollection * This,
            /* [in] */ VARIANT varIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IGroupCollection * This,
            /* [in] */ VARIANT grp,
            /* [in] */ MoveType mt,
            /* [in] */ VARIANT target);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveUp )( 
            IGroupCollection * This,
            /* [in] */ VARIANT grp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveDown )( 
            IGroupCollection * This,
            /* [in] */ VARIANT grp);
        
        END_INTERFACE
    } IGroupCollectionVtbl;

    interface IGroupCollection
    {
        CONST_VTBL struct IGroupCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGroupCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGroupCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGroupCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGroupCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGroupCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGroupCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGroupCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGroupCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IGroupCollection_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#define IGroupCollection_get_Item(This,varIndex,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,varIndex,pVal) ) 

#define IGroupCollection_Add(This,group)	\
    ( (This)->lpVtbl -> Add(This,group) ) 

#define IGroupCollection_Remove(This,varIndex)	\
    ( (This)->lpVtbl -> Remove(This,varIndex) ) 

#define IGroupCollection_Move(This,grp,mt,target)	\
    ( (This)->lpVtbl -> Move(This,grp,mt,target) ) 

#define IGroupCollection_MoveUp(This,grp)	\
    ( (This)->lpVtbl -> MoveUp(This,grp) ) 

#define IGroupCollection_MoveDown(This,grp)	\
    ( (This)->lpVtbl -> MoveDown(This,grp) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGroupCollection_INTERFACE_DEFINED__ */


#ifndef __IGroupCollectionEvents_INTERFACE_DEFINED__
#define __IGroupCollectionEvents_INTERFACE_DEFINED__

/* interface IGroupCollectionEvents */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IGroupCollectionEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EA833EF0-1870-11d3-893B-006097C68A9C")
    IGroupCollectionEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupAdded( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupRemoved( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupChanged( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnGroupMoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordAdded( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pNewRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordChanged( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordRemoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ BSTR bstrMark) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordsSorted( 
            /* [in] */ IGroup *pGroup) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordMoved( 
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGroupCollectionEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGroupCollectionEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGroupCollectionEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGroupCollectionEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupAdded )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupRemoved )( 
            IGroupCollectionEvents * This,
            /* [in] */ BSTR name);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupChanged )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnGroupMoved )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordAdded )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pNewRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordChanged )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordRemoved )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ BSTR bstrMark);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordsSorted )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordMoved )( 
            IGroupCollectionEvents * This,
            /* [in] */ IGroup *pGroup,
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo);
        
        END_INTERFACE
    } IGroupCollectionEventsVtbl;

    interface IGroupCollectionEvents
    {
        CONST_VTBL struct IGroupCollectionEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGroupCollectionEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGroupCollectionEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGroupCollectionEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGroupCollectionEvents_OnGroupAdded(This,pGroup)	\
    ( (This)->lpVtbl -> OnGroupAdded(This,pGroup) ) 

#define IGroupCollectionEvents_OnGroupRemoved(This,name)	\
    ( (This)->lpVtbl -> OnGroupRemoved(This,name) ) 

#define IGroupCollectionEvents_OnGroupChanged(This,pGroup)	\
    ( (This)->lpVtbl -> OnGroupChanged(This,pGroup) ) 

#define IGroupCollectionEvents_OnGroupMoved(This,pGroup,idxFrom,idxTo)	\
    ( (This)->lpVtbl -> OnGroupMoved(This,pGroup,idxFrom,idxTo) ) 

#define IGroupCollectionEvents_OnBarRecordAdded(This,pGroup,pNewRecord)	\
    ( (This)->lpVtbl -> OnBarRecordAdded(This,pGroup,pNewRecord) ) 

#define IGroupCollectionEvents_OnBarRecordChanged(This,pGroup,pBarRecord)	\
    ( (This)->lpVtbl -> OnBarRecordChanged(This,pGroup,pBarRecord) ) 

#define IGroupCollectionEvents_OnBarRecordRemoved(This,pGroup,bstrMark)	\
    ( (This)->lpVtbl -> OnBarRecordRemoved(This,pGroup,bstrMark) ) 

#define IGroupCollectionEvents_OnBarRecordsSorted(This,pGroup)	\
    ( (This)->lpVtbl -> OnBarRecordsSorted(This,pGroup) ) 

#define IGroupCollectionEvents_OnBarRecordMoved(This,pGroup,pBarRecord,idxFrom,idxTo)	\
    ( (This)->lpVtbl -> OnBarRecordMoved(This,pGroup,pBarRecord,idxFrom,idxTo) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGroupCollectionEvents_INTERFACE_DEFINED__ */


#ifndef __IBarRecordCollection_INTERFACE_DEFINED__
#define __IBarRecordCollection_INTERFACE_DEFINED__

/* interface IBarRecordCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBarRecordCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFC2D3AF-143B-11D3-8934-006097C68A9C")
    IBarRecordCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT varIndex,
            /* [retval][out] */ IBarRecord **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IBarRecord *pBarRecord) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT varIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Sort( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Replace( 
            /* [in] */ VARIANT varIndex,
            /* [in] */ IBarRecord *pBarRecord) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ VARIANT rec,
            /* [in] */ MoveType mt,
            /* [in] */ VARIANT target) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveUp( 
            /* [in] */ VARIANT rec) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveDown( 
            /* [in] */ VARIANT rec) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarRecordCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarRecordCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarRecordCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarRecordCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBarRecordCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBarRecordCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBarRecordCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBarRecordCollection * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IBarRecordCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IBarRecordCollection * This,
            /* [retval][out] */ LPUNKNOWN *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IBarRecordCollection * This,
            /* [in] */ VARIANT varIndex,
            /* [retval][out] */ IBarRecord **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IBarRecordCollection * This,
            /* [in] */ IBarRecord *pBarRecord);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IBarRecordCollection * This,
            /* [in] */ VARIANT varIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Sort )( 
            IBarRecordCollection * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Replace )( 
            IBarRecordCollection * This,
            /* [in] */ VARIANT varIndex,
            /* [in] */ IBarRecord *pBarRecord);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IBarRecordCollection * This,
            /* [in] */ VARIANT rec,
            /* [in] */ MoveType mt,
            /* [in] */ VARIANT target);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveUp )( 
            IBarRecordCollection * This,
            /* [in] */ VARIANT rec);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveDown )( 
            IBarRecordCollection * This,
            /* [in] */ VARIANT rec);
        
        END_INTERFACE
    } IBarRecordCollectionVtbl;

    interface IBarRecordCollection
    {
        CONST_VTBL struct IBarRecordCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarRecordCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarRecordCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarRecordCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarRecordCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBarRecordCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBarRecordCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBarRecordCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBarRecordCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IBarRecordCollection_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#define IBarRecordCollection_get_Item(This,varIndex,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,varIndex,pVal) ) 

#define IBarRecordCollection_Add(This,pBarRecord)	\
    ( (This)->lpVtbl -> Add(This,pBarRecord) ) 

#define IBarRecordCollection_Remove(This,varIndex)	\
    ( (This)->lpVtbl -> Remove(This,varIndex) ) 

#define IBarRecordCollection_Sort(This)	\
    ( (This)->lpVtbl -> Sort(This) ) 

#define IBarRecordCollection_Replace(This,varIndex,pBarRecord)	\
    ( (This)->lpVtbl -> Replace(This,varIndex,pBarRecord) ) 

#define IBarRecordCollection_Move(This,rec,mt,target)	\
    ( (This)->lpVtbl -> Move(This,rec,mt,target) ) 

#define IBarRecordCollection_MoveUp(This,rec)	\
    ( (This)->lpVtbl -> MoveUp(This,rec) ) 

#define IBarRecordCollection_MoveDown(This,rec)	\
    ( (This)->lpVtbl -> MoveDown(This,rec) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarRecordCollection_INTERFACE_DEFINED__ */


#ifndef __IBarRecordCollectionEvents_INTERFACE_DEFINED__
#define __IBarRecordCollectionEvents_INTERFACE_DEFINED__

/* interface IBarRecordCollectionEvents */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IBarRecordCollectionEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B52921F0-186D-11d3-893B-006097C68A9C")
    IBarRecordCollectionEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordAdded( 
            /* [in] */ IBarRecord *pNewRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordRemoved( 
            /* [in] */ BSTR mark) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordChanged( 
            /* [in] */ IBarRecord *pBarRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordsSorted( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBarRecordMoved( 
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarRecordCollectionEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarRecordCollectionEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarRecordCollectionEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarRecordCollectionEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordAdded )( 
            IBarRecordCollectionEvents * This,
            /* [in] */ IBarRecord *pNewRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordRemoved )( 
            IBarRecordCollectionEvents * This,
            /* [in] */ BSTR mark);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordChanged )( 
            IBarRecordCollectionEvents * This,
            /* [in] */ IBarRecord *pBarRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordsSorted )( 
            IBarRecordCollectionEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBarRecordMoved )( 
            IBarRecordCollectionEvents * This,
            /* [in] */ IBarRecord *pBarRecord,
            /* [in] */ long idxFrom,
            /* [in] */ long idxTo);
        
        END_INTERFACE
    } IBarRecordCollectionEventsVtbl;

    interface IBarRecordCollectionEvents
    {
        CONST_VTBL struct IBarRecordCollectionEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarRecordCollectionEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarRecordCollectionEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarRecordCollectionEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarRecordCollectionEvents_OnBarRecordAdded(This,pNewRecord)	\
    ( (This)->lpVtbl -> OnBarRecordAdded(This,pNewRecord) ) 

#define IBarRecordCollectionEvents_OnBarRecordRemoved(This,mark)	\
    ( (This)->lpVtbl -> OnBarRecordRemoved(This,mark) ) 

#define IBarRecordCollectionEvents_OnBarRecordChanged(This,pBarRecord)	\
    ( (This)->lpVtbl -> OnBarRecordChanged(This,pBarRecord) ) 

#define IBarRecordCollectionEvents_OnBarRecordsSorted(This)	\
    ( (This)->lpVtbl -> OnBarRecordsSorted(This) ) 

#define IBarRecordCollectionEvents_OnBarRecordMoved(This,pBarRecord,idxFrom,idxTo)	\
    ( (This)->lpVtbl -> OnBarRecordMoved(This,pBarRecord,idxFrom,idxTo) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarRecordCollectionEvents_INTERFACE_DEFINED__ */


#ifndef __IBend_INTERFACE_DEFINED__
#define __IBend_INTERFACE_DEFINED__

/* interface IBend */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBend;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2A04F206-147C-11d3-8934-006097C68A9C")
    IBend : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_U( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_U( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_W( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_W( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_X( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Y( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Z( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Z( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_T1( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_T1( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_T2( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_T2( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ StatusType *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_StatusMessages( 
            /* [retval][out] */ IStatusMessageCollection **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Length( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BendType( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BarRecord( 
            /* [retval][out] */ IBarRecord **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BarRecord( 
            /* [in] */ IBarRecord *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SupportsDimension( 
            /* [in] */ DimensionType type,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBendVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBend * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBend * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBend * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBend * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBend * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBend * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBend * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_U )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_U )( 
            IBend * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_W )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_W )( 
            IBend * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_X )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_X )( 
            IBend * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Y )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Y )( 
            IBend * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Z )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Z )( 
            IBend * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_T1 )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_T1 )( 
            IBend * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_T2 )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_T2 )( 
            IBend * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IBend * This,
            /* [retval][out] */ StatusType *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_StatusMessages )( 
            IBend * This,
            /* [retval][out] */ IStatusMessageCollection **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Length )( 
            IBend * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BendType )( 
            IBend * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BarRecord )( 
            IBend * This,
            /* [retval][out] */ IBarRecord **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BarRecord )( 
            IBend * This,
            /* [in] */ IBarRecord *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SupportsDimension )( 
            IBend * This,
            /* [in] */ DimensionType type,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        END_INTERFACE
    } IBendVtbl;

    interface IBend
    {
        CONST_VTBL struct IBendVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBend_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBend_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBend_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBend_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBend_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBend_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBend_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBend_get_U(This,pVal)	\
    ( (This)->lpVtbl -> get_U(This,pVal) ) 

#define IBend_put_U(This,newVal)	\
    ( (This)->lpVtbl -> put_U(This,newVal) ) 

#define IBend_get_W(This,pVal)	\
    ( (This)->lpVtbl -> get_W(This,pVal) ) 

#define IBend_put_W(This,newVal)	\
    ( (This)->lpVtbl -> put_W(This,newVal) ) 

#define IBend_get_X(This,pVal)	\
    ( (This)->lpVtbl -> get_X(This,pVal) ) 

#define IBend_put_X(This,newVal)	\
    ( (This)->lpVtbl -> put_X(This,newVal) ) 

#define IBend_get_Y(This,pVal)	\
    ( (This)->lpVtbl -> get_Y(This,pVal) ) 

#define IBend_put_Y(This,newVal)	\
    ( (This)->lpVtbl -> put_Y(This,newVal) ) 

#define IBend_get_Z(This,pVal)	\
    ( (This)->lpVtbl -> get_Z(This,pVal) ) 

#define IBend_put_Z(This,newVal)	\
    ( (This)->lpVtbl -> put_Z(This,newVal) ) 

#define IBend_get_T1(This,pVal)	\
    ( (This)->lpVtbl -> get_T1(This,pVal) ) 

#define IBend_put_T1(This,newVal)	\
    ( (This)->lpVtbl -> put_T1(This,newVal) ) 

#define IBend_get_T2(This,pVal)	\
    ( (This)->lpVtbl -> get_T2(This,pVal) ) 

#define IBend_put_T2(This,newVal)	\
    ( (This)->lpVtbl -> put_T2(This,newVal) ) 

#define IBend_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define IBend_get_StatusMessages(This,pVal)	\
    ( (This)->lpVtbl -> get_StatusMessages(This,pVal) ) 

#define IBend_get_Length(This,pVal)	\
    ( (This)->lpVtbl -> get_Length(This,pVal) ) 

#define IBend_get_BendType(This,pVal)	\
    ( (This)->lpVtbl -> get_BendType(This,pVal) ) 

#define IBend_get_BarRecord(This,pVal)	\
    ( (This)->lpVtbl -> get_BarRecord(This,pVal) ) 

#define IBend_put_BarRecord(This,newVal)	\
    ( (This)->lpVtbl -> put_BarRecord(This,newVal) ) 

#define IBend_get_SupportsDimension(This,type,pbResult)	\
    ( (This)->lpVtbl -> get_SupportsDimension(This,type,pbResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBend_INTERFACE_DEFINED__ */


#ifndef __IBendEvents_INTERFACE_DEFINED__
#define __IBendEvents_INTERFACE_DEFINED__

/* interface IBendEvents */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IBendEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C87B98E2-1862-11d3-893B-006097C68A9C")
    IBendEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBendChanged( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBendEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBendEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBendEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBendEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBendChanged )( 
            IBendEvents * This);
        
        END_INTERFACE
    } IBendEventsVtbl;

    interface IBendEvents
    {
        CONST_VTBL struct IBendEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBendEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBendEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBendEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBendEvents_OnBendChanged(This)	\
    ( (This)->lpVtbl -> OnBendChanged(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBendEvents_INTERFACE_DEFINED__ */


#ifndef __IStatusMessage_INTERFACE_DEFINED__
#define __IStatusMessage_INTERFACE_DEFINED__

/* interface IStatusMessage */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStatusMessage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0A69D045-1DB1-11D3-893F-006097C68A9C")
    IStatusMessage : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Val1( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Val2( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IStatusMessageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStatusMessage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStatusMessage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStatusMessage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStatusMessage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStatusMessage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStatusMessage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStatusMessage * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Text )( 
            IStatusMessage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Val1 )( 
            IStatusMessage * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Val2 )( 
            IStatusMessage * This,
            /* [retval][out] */ VARIANT *pVal);
        
        END_INTERFACE
    } IStatusMessageVtbl;

    interface IStatusMessage
    {
        CONST_VTBL struct IStatusMessageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStatusMessage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IStatusMessage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IStatusMessage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IStatusMessage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IStatusMessage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IStatusMessage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IStatusMessage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IStatusMessage_get_Text(This,pVal)	\
    ( (This)->lpVtbl -> get_Text(This,pVal) ) 

#define IStatusMessage_get_Val1(This,pVal)	\
    ( (This)->lpVtbl -> get_Val1(This,pVal) ) 

#define IStatusMessage_get_Val2(This,pVal)	\
    ( (This)->lpVtbl -> get_Val2(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IStatusMessage_INTERFACE_DEFINED__ */


#ifndef __IStatusMessageCollection_INTERFACE_DEFINED__
#define __IStatusMessageCollection_INTERFACE_DEFINED__

/* interface IStatusMessageCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStatusMessageCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9DE5701C-1919-11D3-893C-006097C68A9C")
    IStatusMessageCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IStatusMessage *pMsg) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long index,
            /* [retval][out] */ IStatusMessage **ppVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IStatusMessageCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStatusMessageCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStatusMessageCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStatusMessageCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStatusMessageCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStatusMessageCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStatusMessageCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStatusMessageCollection * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IStatusMessageCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IStatusMessageCollection * This,
            /* [in] */ IStatusMessage *pMsg);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IStatusMessageCollection * This,
            /* [retval][out] */ LPUNKNOWN *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IStatusMessageCollection * This,
            /* [in] */ long index,
            /* [retval][out] */ IStatusMessage **ppVal);
        
        END_INTERFACE
    } IStatusMessageCollectionVtbl;

    interface IStatusMessageCollection
    {
        CONST_VTBL struct IStatusMessageCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStatusMessageCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IStatusMessageCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IStatusMessageCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IStatusMessageCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IStatusMessageCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IStatusMessageCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IStatusMessageCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IStatusMessageCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IStatusMessageCollection_Add(This,pMsg)	\
    ( (This)->lpVtbl -> Add(This,pMsg) ) 

#define IStatusMessageCollection_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#define IStatusMessageCollection_get_Item(This,index,ppVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,ppVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IStatusMessageCollection_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


