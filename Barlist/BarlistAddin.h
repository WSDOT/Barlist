

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Jan 07 10:00:32 2019
 */
/* Compiler settings for ..\SDK\Include\BarlistAddin.idl:
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

#ifndef __BarlistAddin_h__
#define __BarlistAddin_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBarlistAddin_FWD_DEFINED__
#define __IBarlistAddin_FWD_DEFINED__
typedef interface IBarlistAddin IBarlistAddin;

#endif 	/* __IBarlistAddin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "barlst.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IBarlistAddin_INTERFACE_DEFINED__
#define __IBarlistAddin_INTERFACE_DEFINED__

/* interface IBarlistAddin */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IBarlistAddin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6BF9E1B0-1F41-11d3-8941-006097C68A9C")
    IBarlistAddin : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Go( 
            /* [in] */ IBarlist *pBarlist) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_MenuItem( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBarlistAddinVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBarlistAddin * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBarlistAddin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBarlistAddin * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Go )( 
            IBarlistAddin * This,
            /* [in] */ IBarlist *pBarlist);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MenuItem )( 
            IBarlistAddin * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IBarlistAddinVtbl;

    interface IBarlistAddin
    {
        CONST_VTBL struct IBarlistAddinVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBarlistAddin_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBarlistAddin_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBarlistAddin_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBarlistAddin_Go(This,pBarlist)	\
    ( (This)->lpVtbl -> Go(This,pBarlist) ) 

#define IBarlistAddin_get_MenuItem(This,pVal)	\
    ( (This)->lpVtbl -> get_MenuItem(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBarlistAddin_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


