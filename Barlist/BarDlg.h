///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2020  Washington State Department of Transportation
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
#pragma once

#include "resource.h"
#include "BarDlgStateMachine.h"
#include <MfcTools\CacheEdit.h>
#include <MfcTools\CacheCheckBox.h>
#include <MfcTools\MetaFileStatic.h>

interface IBarInfoMgr;
interface IBarlist;
interface IGroup;
interface IBarRecord;
interface IStatusMessageCollection;

enum StatusType;

void DDX_Text(CDataExchange* pDX, int nIDC, CComBSTR& bstr);

// Want to be able to update the bend image as the user scrolls
// over the list of bend... the only way to do this is with
// an owner draw combo box.
class CBendTypeComboBox : public CComboBox
{
public:
   virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

// CBarDlg dialog

class CBarDlg : public CDialog
{
	DECLARE_DYNAMIC(CBarDlg)

public:
	CBarDlg(IBarInfoMgr* pBarInfoMgr,IBarlist* pBarlist,long groupIdx=-1,long barIdx=-1,CWnd* pParent = NULL);   // standard constructor
	virtual ~CBarDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BAR };
#endif

protected:
   bool m_bIsCollaboration;

   CComPtr<IBarInfoMgr> m_BarInfoMgr;
   CComPtr<IBarlist> m_Barlist;

   CBarDlgStateMachine m_SM;
   friend CBarDlgStateMachine;

   CBendTypeComboBox m_cbBendType;
   friend CBendTypeComboBox;

   CCacheCheckBox m_cbEpoxy;

   CMetaFileStatic m_BendGuide;
   CCacheEdit m_ctrlNumEach;
   CFont m_Font;

   long m_GroupIdx, m_BarIdx;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   void OnStateChanged();
   long GetBarRecordCount();

   void SelectBar(long groupIdx, long barIdx);

   void UpdateBarData(BOOL bSaveAndValidate, long groupIdx, long barIdx, IBarRecord* pBarRecord);
   void UpdateMessages(IBarRecord* pBarRecord);
   void GetStatusMessages(LPCTSTR lpszMsgGroup, CString& strMsgs, IStatusMessageCollection* pMessages);

   void UpdateGroups();
   void UpdateBarSizes();
   void UpdateBendTypes();
   
   void UpdateStatus();
   void UpdateStatus(IGroup* pGroup);
   void UpdateStatus(IBarRecord* pBarRecord);
   void UpdateStatus(const StatusType& status);

   void UpdateVaries();

   void UpdateMarkNumbers();
   void UpdateDimensions(IBarRecord* pBarRecord);
   void UpdateDimensions(long bendType,bool bVaries);

   void UpdateBendGuide(IBarRecord* pBarRecord);
   void UpdateBendGuide(long bendType);

   BOOL CheckEditState();
   CString AutoIncrementMark(const CString& strMark);


	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
   afx_msg void OnClickedAddGroup();
   afx_msg void OnSelchangeGroups();
   afx_msg void OnCbnSelchangeMark();
   afx_msg void OnBnClickedVaries();
   afx_msg void OnBnClickedDelete();
   afx_msg void OnBnClickedUpdate();
   afx_msg void OnCbnSelchangeType();
   afx_msg void OnBnClickedAddBar();
   afx_msg void OnBnClickedSave();
   afx_msg void OnCbnEditchangeMark();
   afx_msg void OnChange();
   afx_msg void OnBnClickedCancel();
   afx_msg void OnBnClickedHelp();
   afx_msg void OnCbnSelchangeMaterials();
};
