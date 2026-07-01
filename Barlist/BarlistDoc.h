///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2026  Washington State Department of Transportation
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

#pragma once

#include <EAF\EAFDocument.h>
#include <Bars\Barlist.h>
#include <Bars\BarInfoMgr.h>
#include <Bars\Signal.h>
#include "PluginManager.h"

#include <array>
#include <memory>
#include <Units\IndirectMeasure.h>

#include "Report.h"
#include "..\Common\Formatter.h"

// Schema-generated XML DOM type (Schema\Barlist.hxx) -- only forward
// declared here since CreateXML/CreateBarlist only need it as a
// return/parameter type; the full definition is pulled in by whichever
// .cpp file actually implements or calls them.
class Barlist;

class CBarlistDoc : public CEAFDocument
{
protected: // create from serialization only
	CBarlistDoc();
	DECLARE_DYNCREATE(CBarlistDoc)

// Attributes
public:
   CBarlist& GetBarlist();
   CBarInfoMgr& GetBarInfoMgr();
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

   enum class ReportOptions {
	   REPORT_TOTAL_QUANTITIES, ///< Reports only the total quantities
	   REPORT_TOTAL_AND_GROUP_QUANTITIES ///< Reports total quantities and quantities per bar group
   };


   void SetReportOptions(const ReportOptions& reportOptions);
   ReportOptions GetReportOptions() const;

   Barlist CreateXML(CBarlist& barlist);
   BOOL CreateBarlist(LPCSTR strXML, CBarlist& barlist);
   BOOL CreateBarlist(Barlist& barlistXML, CBarlist& barlist);

   virtual BOOL OpenTheDocument(LPCTSTR lpszPathName) override;
   virtual BOOL SaveTheDocument(LPCTSTR lpszPathName) override;

   BOOL ReadBarlistFromFile(LPCTSTR lpszPathName, CBarlist& barlist);

   virtual CString GetToolbarSectionName() override;
   virtual BOOL GetStatusBarMessageString(UINT nID,CString& rMessage) const override;
   virtual BOOL GetToolTipMessageString(UINT nID, CString& rMessage) const override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual void SetModifiedFlag(BOOL bModified = TRUE) override;

   virtual CString GetDocumentationRootLocation() override;

   CString AutoIncrementMark(const CString& strMark) const;

   void SelectGroup(long groupIdx);

   std::shared_ptr<CBarRecord> CopyBar(CBarRecord& source) const;

   CReport& GetReport();


// Implementation
public:
	virtual ~CBarlistDoc();
#ifdef _DEBUG
	virtual void AssertValid() const override;
	virtual void Dump(CDumpContext& dc) const override;
#endif

protected:

   std::unique_ptr<CBarInfoMgr> m_BarInfoMgr;
   std::unique_ptr<CBarlist> m_Barlist;

   // Tokens for this document's subscription to m_Barlist's Signals (see
   // GetBarlistEvents) -- replaces the old IBarlistEvents connection point.
   Signal<CGroup&>::Token m_GroupAddedToken{};
   Signal<const std::_tstring&>::Token m_GroupRemovedToken{};
   Signal<CGroup&>::Token m_GroupChangedToken{};
   Signal<CGroup&, long, long>::Token m_GroupMovedToken{};
   Signal<CGroup&, CBarRecord&>::Token m_BarRecordAddedToken{};
   Signal<CGroup&, CBarRecord&>::Token m_BarRecordChangedToken{};
   Signal<CGroup&, const std::_tstring&>::Token m_BarRecordRemovedToken{};
   Signal<CGroup&>::Token m_BarRecordsSortedToken{};
   Signal<CGroup&, CBarRecord&, long, long>::Token m_BarRecordMovedToken{};
   bool m_bListeningForBarlistEvents{ false };

   PluginManager m_PluginMgr;

   UINT m_ToolbarID;

   UINT m_MarkIncrement;

   bool m_bDirtyReport{ true };
   CReport m_Report;

   ReportOptions m_reportOptions;

   virtual UINT GetToolbarResourceID();


// Generated message map functions
protected:
	//{{AFX_MSG(CBarlistDoc)
   afx_msg void OnViewOptions();
   afx_msg void OnAddGroup();
   afx_msg void OnAddBar();
   afx_msg void OnProperties();
   afx_msg void OnAddin(UINT cmd);
   afx_msg void OnViewReport();
   //}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void OnGroupAdded(CGroup& group);
   void OnGroupRemoved(const std::_tstring& name);
   void OnGroupChanged(CGroup& group);
   void OnGroupMoved(CGroup& group, long idxFrom, long idxTo);
   void OnBarRecordAdded(CGroup& group, CBarRecord& newRecord);
   void OnBarRecordChanged(CGroup& group, CBarRecord& barRecord);
   void OnBarRecordRemoved(CGroup& group, const std::_tstring& mark);
   void OnBarRecordsSorted(CGroup& group);
   void OnBarRecordMoved(CGroup& group, CBarRecord& barRecord, long idxFrom, long idxTo);

   virtual HINSTANCE GetResourceInstance() override;
   virtual void DeleteContents() override;

   virtual CString GetDocumentationURL() override;
   virtual CString GetDocumentationMapFile() override;
   virtual CString GetDocumentationSetName() override;
   bool m_bGettingDocumentationMapFile{ false };

   void GetBarlistEvents(BOOL bListenForEvents);

   //virtual void OnUnitsModeChanging() override;
   virtual void OnUnitsModeChanged(WBFL::EAF::UnitMode newUnitMode) override;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
