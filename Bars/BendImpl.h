///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2025  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This software was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
// PURPOSE.  See the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////


// BendImpl.h: interface for the CBendImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BENDIMPL_H__D6F6D554_1514_11D3_8937_006097C68A9C__INCLUDED_)
#define AFX_BENDIMPL_H__D6F6D554_1514_11D3_8937_006097C68A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <MathEx.h>
#include "BarComponent.h"
#include "BarsCP.h"
#include "StatusMgr.h"
#include "BarException.h"

template < class T,const CLSID* pclsid >
class CBendImpl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<T, pclsid>,
	public IDispatchImpl<IBend, &IID_IBend, &LIBID_BARSLib>,
   public CProxyIBendEvents<T>,
   public IConnectionPointContainerImpl<T>
{
public:
	CBendImpl(long bendType,
             bool bU,
             bool bW = false,
             bool bX = false,
             bool bY = false,
             bool bZ = false,
             bool bT1 = false,
             bool bT2 = false) :
   m_bU(bU),
   m_bW(bW),
   m_bX(bX),
   m_bY(bY),
   m_bZ(bZ),
   m_bT1(bT1),
   m_bT2(bT2)
   {
      m_U = 0;
      m_W = 0;
      m_X = 0;
      m_Y = 0;
      m_Z = 0;
      m_T1 = 0;
      m_T2 = 0;

      m_BendType = bendType;

      m_bIsDirty = true;
      m_Length = 0;

      m_pBarRecord = 0;

   }

	virtual ~CBendImpl()
   {
   }

   HRESULT FinalConstruct()
   {
      ResetStatusMsgs();
      return S_OK;
   };

   void FinalRelease()
   {
      Clear();
   }

BEGIN_COM_MAP(T)
	COM_INTERFACE_ENTRY(IBend)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

public:
   // IBend
	STDMETHOD(get_BarRecord)(/*[out, retval]*/ IBarRecord* *pVal)
   {
      *pVal = m_pBarRecord;
      (*pVal)->AddRef();

   	return S_OK;
   }

	STDMETHOD(put_BarRecord)(/*[in]*/ IBarRecord* newVal)
   {
      m_pBarRecord = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_U)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = m_U;
      return S_OK;
   }

   STDMETHOD(put_U)(/*[in]*/ Float64 newVal)
   {
      m_U = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_W)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = m_W;
      return S_OK;
   }

   STDMETHOD(put_W)(/*[in]*/ Float64 newVal)
   {
      m_W = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_X)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = m_X;
      return S_OK;
   }

   STDMETHOD(put_X)(/*[in]*/ Float64 newVal)
   {
      m_X = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_Y)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = m_Y;
      return S_OK;
   }

   STDMETHOD(put_Y)(/*[in]*/ Float64 newVal)
   {
      m_Y = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_Z)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = m_Z;
      return S_OK;
   }

   STDMETHOD(put_Z)(/*[in]*/ Float64 newVal)
   {
      m_Z = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_T1)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = m_T1;
      return S_OK;
   }

   STDMETHOD(put_T1)(/*[in]*/ Float64 newVal)
   {
      m_T1 = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_T2)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = m_T2;
      return S_OK;
   }

   STDMETHOD(put_T2)(/*[in]*/ Float64 newVal)
   {
      m_T2 = newVal;
      m_bIsDirty = true;
      this->Fire_OnBendChanged();
      return S_OK;
   }

   STDMETHOD(get_Status)(/*[out,retval]*/ StatusType* pVal)
   {
      ComputeLength(); // Forces status to be updated.
      *pVal = m_StatusMgr.GetStatusLevel();
      return S_OK;
   }

   STDMETHOD(get_StatusMessages)(/*[out,retval]*/ IStatusMessageCollection** pVal)
   {
      *pVal = m_StatusMgr.GetStatusMessages();
      return S_OK;
   }

   STDMETHOD(get_Length)(/*[out,retval]*/ Float64* pVal)
   {
      *pVal = GetLength();
      return S_OK;
   }

	STDMETHOD(get_BendType)(/*[out, retval]*/ long *pVal)
   {
      *pVal = m_BendType;
      return S_OK;
   }

   STDMETHOD(get_SupportsDimension)(/*[in]*/ DimensionType type,/*[out,retval]*/ VARIANT_BOOL* pbResult)
   {
      switch( type )
      {
      case dimU:
         *pbResult = m_bU ? VARIANT_TRUE : VARIANT_FALSE;
         break;

      case dimW:
         *pbResult = m_bW ? VARIANT_TRUE : VARIANT_FALSE;
         break;

      case dimX:
         *pbResult = m_bX ? VARIANT_TRUE : VARIANT_FALSE;
         break;

      case dimY:
         *pbResult = m_bY ? VARIANT_TRUE : VARIANT_FALSE;
         break;

      case dimZ:
         *pbResult = m_bZ ? VARIANT_TRUE : VARIANT_FALSE;
         break;

      case dimT1:
         *pbResult = m_bT1 ? VARIANT_TRUE : VARIANT_FALSE;
         break;

      case dimT2:
         *pbResult = m_bT2 ? VARIANT_TRUE : VARIANT_FALSE;
         break;

      default:
         return E_INVALIDARG;
      }

      return S_OK;
   }

protected:
   IBarRecord* m_pBarRecord;
   bool m_bU;
   bool m_bW;
   bool m_bX;
   bool m_bY;
   bool m_bZ;
   bool m_bT1;
   bool m_bT2;

   void SetStatusLevel( StatusType status )
   {
      m_StatusMgr.SetStatusLevel( status );
   }

   void AddStatusMsg(BSTR bstrMsg, VARIANT v1, VARIANT v2)
   {
      m_StatusMgr.AddStatusMsg(bstrMsg, v1, v2);
   }

   void AddStatusMsg(BSTR bstrMsg, VARIANT v1, VARIANT v2, VARIANT v3)
   {
      m_StatusMgr.AddStatusMsg(bstrMsg, v1, v2, v3);
   }

   virtual void PreValidateBend()
   {
      if ( m_U <= 0 )
      {
         SetStatusLevel( stError );
         CComBSTR msg;
         msg.LoadString( ERR_MUSTBEGREATERTHAN );
         AddStatusMsg( msg,
                       CComVariant("U"),
                       CComVariant(0.0));
      }

      // GFRP Bars can only be bent into Type 50, 51, 54, 57, 66, 67, 71, 80 (w/X=0), and 89.
      // Type 74 was added 3/25/2020 for Satus Creek Project per project requirements with concurence from THE concrete specialist
      MaterialType material;
      m_pBarRecord->get_Material(&material);
      if (material == D7957) // GFRP
      {
         int is_bend_type_supported = 1; // 0 = supported, 1 = unsupported, 2 = special case of unsupported for Type 80 bends
         switch (m_BendType)
         {
         case 50:
         case 51:
         case 54:
         case 57:
         case 66:
         case 67:
         case 71:
         case 74:
         case 89:
            is_bend_type_supported = 0;
            break;

         case 80:
            ATLASSERT(m_bX == true);
            is_bend_type_supported = (IsZero(m_X) ? 0 : 2);
            break;

         default:
            is_bend_type_supported = 1;
         }

         if (0 < is_bend_type_supported)
         {
            SetStatusLevel(stError);
            CComBSTR msg;
            if (is_bend_type_supported == 1)
            {
               msg.LoadString(ERR_INVALIDGFRPBEND);
               AddStatusMsg(msg, CComVariant(m_BendType), CComVariant());
            }
            else
            {
               msg.LoadString(ERR_INVALIDGFRPBEND80);
               AddStatusMsg(msg, CComVariant(), CComVariant());
            }
         }
      }
   }

   void Clear()
   {
      std::vector<CBarComponent*>::iterator iter = m_Components.begin();
      std::vector<CBarComponent*>::iterator end = m_Components.end();
      for ( ; iter != end; iter++ )
      {
         CBarComponent* pComponent = *iter;
         delete pComponent;
      }
      m_Components.clear();
   }

   virtual void BuildBend()
   {
      Clear();
   }

   void CheckNormalLength()
   {
      CComPtr<IBarData> pBarData;
      GetBarData(&pBarData);

      Float64 length;
      pBarData->get_NormalLength(&length);

      if (length < m_Length)
      {
         SetStatusLevel(stWarning);
         CComBSTR msg;
         msg.LoadString(WARN_EXCEEDSNORMAL);
         AddStatusMsg(msg,CComVariant(m_Length), CComVariant(length));
      }
   }

   void CheckMaxLength()
   {
      CComPtr<IBarData> pBarData;
      GetBarData(&pBarData);
      
      Float64 maxlength;
      pBarData->get_MaxLength(&maxlength);

      if (0 < maxlength && maxlength < m_Length)
      {
         SetStatusLevel(stError);
         CComBSTR msg;
         msg.LoadString(ERR_EXCEEDSMAX);
         AddStatusMsg(msg, CComVariant(m_Length), CComVariant(maxlength));
      }
   }

   virtual void PostValidateBend()
   {
      CheckNormalLength();
      CheckMaxLength();
   }

   void AddBarComponent(CBarComponent* pComponent)
   {
      m_Components.push_back( pComponent );
   }

   HRESULT GetBarData(IBarData** ppBarData)
   {
      return m_pBarRecord->get_BarData(ppBarData);
   }

   UseType GetUseType()
   {
      UseType use;
      m_pBarRecord->get_Use(&use);
      return use;
   }
   
   Float64 GetLength()
   {
      if ( m_bIsDirty )
         ComputeLength();

      return m_Length;
   }

   Float64 GetU()
   {
      return m_U;
   }

   Float64 GetW()
   {
      return m_W;
   }

   Float64 GetX()
   {
      return m_X;
   }

   Float64 GetY()
   {
      return m_Y;
   }

   Float64 GetZ()
   {
      return m_Z;
   }

   Float64 GetT1()
   {
      return m_T1;
   }

   Float64 GetT2()
   {
      return m_T2;
   }

   StatusType GetStatusLevel()
   {
      return m_StatusMgr.GetStatusLevel();
   }


private:
   Float64 m_U;
   Float64 m_W;
   Float64 m_X;
   Float64 m_Y;
   Float64 m_Z;
   Float64 m_T1;
   Float64 m_T2;

   long m_BendType;

   bool m_bIsDirty;
   Float64 m_Length;

   std::vector<CBarComponent*> m_Components;

   CStatusMgr m_StatusMgr;


   void ResetStatusMsgs()
   {
      m_StatusMgr.ResetStatusMsgs();
   }

   virtual void ComputeLength()
   {
      if ( m_bIsDirty )
      {
         ResetStatusMsgs();

         m_Length = 0;

         try
         {
            PreValidateBend();
            BuildBend();
      
            std::vector<CBarComponent*>::iterator iter = m_Components.begin();
            std::vector<CBarComponent*>::iterator end = m_Components.end();
            for( ; iter != end; iter++)
            {
               CBarComponent* pComponent = *iter;
               m_Length += pComponent->Length();
            }
         }
         catch( CBarException& e)
         {
            SetStatusLevel( stError );
            AddStatusMsg( CComBSTR(e.GetReason().c_str()), CComVariant(), CComVariant() );
         }

         m_bIsDirty = false;

         PostValidateBend();
      }
   }

public :

BEGIN_CONNECTION_POINT_MAP(T)
	CONNECTION_POINT_ENTRY(IID_IBendEvents)
END_CONNECTION_POINT_MAP()
};

#endif // !defined(AFX_BENDIMPL_H__D6F6D554_1514_11D3_8937_006097C68A9C__INCLUDED_)
