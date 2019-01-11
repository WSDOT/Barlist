///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 2009-2019  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// BarlistDoc.h : interface of the CBarlistDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BarlistDOC_H__A5D5FE52_139D_46C7_84CF_35817D298084__INCLUDED_)
#define AFX_BarlistDOC_H__A5D5FE52_139D_46C7_84CF_35817D298084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <EAF\EAFDocument.h>
#include <Bars\Bars.h>

#include <array>
#include <UnitMgt\IndirectMeasure.h>

CString FormatMass(Float64 mass, bool bUnits = true);
CString FormatLength(Float64 length,bool bUnits=true);
bool ParseLength(const CString& strValue,Float64* pValue);

class CBarlistDoc : public CEAFDocument
{
protected: // create from serialization only
	CBarlistDoc();
	DECLARE_DYNCREATE(CBarlistDoc)

// Attributes
public:
   void GetBarlist(IBarlist** ppBarlist);
   virtual void DoIntegrateWithUI(BOOL bIntegrate);

   void EditBar(long groupIdx=-1, long barIdx=-1);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarlistDoc)
	public:
	virtual BOOL OnNewDocument() override;
	//}}AFX_VIRTUAL

   virtual BOOL Init() override;

   virtual BOOL OpenTheDocument(LPCTSTR lpszPathName) override;
   virtual BOOL SaveTheDocument(LPCTSTR lpszPathName) override;

   virtual CString GetToolbarSectionName() override;
   virtual BOOL GetStatusBarMessageString(UINT nID,CString& rMessage) const override;
   virtual BOOL GetToolTipMessageString(UINT nID, CString& rMessage) const override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual CString GetDocumentationRootLocation() override;

   CString AutoIncrementMark(const CString& strMark) const;

   void SelectGroup(long groupIdx);

   void CopyBar(IBarRecord* pSource, IBarRecord** ppClone) const;

// Implementation
public:
	virtual ~CBarlistDoc();
#ifdef _DEBUG
	virtual void AssertValid() const override;
	virtual void Dump(CDumpContext& dc) const override;
#endif

protected:

   CComPtr<IBarInfoMgr> m_BarInfoMgr;
   CComPtr<IBarlist> m_Barlist;
   DWORD m_dwBarlistEventCookie;

   CComPtr<IAddinMgr> m_AddinMgr;

   UINT m_ToolbarID;

   UINT m_MarkIncrement;

// Generated message map functions
protected:
	//{{AFX_MSG(CBarlistDoc)
   afx_msg void OnViewOptions();
   afx_msg void OnAddGroup();
   afx_msg void OnAddBar();
   afx_msg void OnQNI();
   afx_msg void OnProperties();
   afx_msg void OnAddin(UINT cmd);
   //}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   DECLARE_INTERFACE_MAP()
   BEGIN_INTERFACE_PART(Events,IBarlistEvents)
      STDMETHOD(OnNotIncludedQuantitiesChanged)();
      STDMETHOD(OnGroupAdded)(IGroup* pGroup);
      STDMETHOD(OnGroupRemoved)(BSTR Name);
      STDMETHOD(OnGroupChanged)(IGroup* pGroup);
      STDMETHOD(OnGroupMoved)(IGroup* pGroup, long idxFrom, long idxTo);
      STDMETHOD(OnBarRecordAdded)(IGroup* pGroup, IBarRecord* pNewRecord);
      STDMETHOD(OnBarRecordChanged)(IGroup* pGroup, IBarRecord* pBarRecord);
      STDMETHOD(OnBarRecordRemoved)(IGroup* pGroup, BSTR bstrMark);
      STDMETHOD(OnBarRecordsSorted)(IGroup* pGroup);
      STDMETHOD(OnBarRecordMoved)(IGroup* pGroup, IBarRecord* pBarRecord, long idxFrom, long idxTo);
   END_INTERFACE_PART(Events)

   virtual HINSTANCE GetResourceInstance() override;
   virtual void DeleteContents() override;

   virtual CString GetDocumentationURL() override;
   virtual CString GetDocumentationMapFile() override;
   virtual CString GetDocumentationSetName() override;
   bool m_bGettingDocumentationMapFile{ false };

   void GetBarlistEvents(BOOL bListenForEvents);

   //virtual void OnUnitsModeChanging() override;
   virtual void OnUnitsModeChanged(eafTypes::UnitMode newUnitMode) override;

public:
   afx_msg void OnViewReport();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BarlistDOC_H__A5D5FE52_139D_46C7_84CF_35817D298084__INCLUDED_)
