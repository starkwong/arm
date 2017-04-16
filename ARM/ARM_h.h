

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Apr 15 19:57:02 2017
 */
/* Compiler settings for ARM.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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


#ifndef __ARM_h_h__
#define __ARM_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IARM_FWD_DEFINED__
#define __IARM_FWD_DEFINED__
typedef interface IARM IARM;
#endif 	/* __IARM_FWD_DEFINED__ */


#ifndef __ARM_FWD_DEFINED__
#define __ARM_FWD_DEFINED__

#ifdef __cplusplus
typedef class ARM ARM;
#else
typedef struct ARM ARM;
#endif /* __cplusplus */

#endif 	/* __ARM_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ARM_LIBRARY_DEFINED__
#define __ARM_LIBRARY_DEFINED__

/* library ARM */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ARM;

#ifndef __IARM_DISPINTERFACE_DEFINED__
#define __IARM_DISPINTERFACE_DEFINED__

/* dispinterface IARM */
/* [uuid] */ 


EXTERN_C const IID DIID_IARM;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B097B5D8-7DB9-482A-B63E-F28F07A53CBE")
    IARM : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IARMVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IARM * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IARM * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IARM * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IARM * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IARM * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IARM * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IARM * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IARMVtbl;

    interface IARM
    {
        CONST_VTBL struct IARMVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IARM_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IARM_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IARM_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IARM_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IARM_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IARM_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IARM_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IARM_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ARM;

#ifdef __cplusplus

class DECLSPEC_UUID("6900BA26-3E90-44B6-8C37-F1F2049FEAF7")
ARM;
#endif
#endif /* __ARM_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


