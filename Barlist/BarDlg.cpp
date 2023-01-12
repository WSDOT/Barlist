///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2023  Washington State Department of Transportation
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

// BarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BarDlg.h"
#include "afxdialogex.h"

#include "BendFactory.h"

#include "Helpers.h"

#include "CollaborationDoc.h" // for FormatWeight and FormatLength
#include <Bars\Bars.h>

#include <array>

#include <MfcTools\Prompts.h>
#include <MfcTools\Format.h>
#include <MfcTools\CustomDDX.h>

#include <EAF\EAFApp.h>

// CBarDlg dialog

#include <afxpriv.h> // for AfxSetWindowText

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DDX_Text(CDataExchange* pDX, int nIDC, CComBSTR& bstr)
{
   USES_CONVERSION;
   HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
   if (pDX->m_bSaveAndValidate)
   {
      int nLen = ::GetWindowTextLength(hWndCtrl) + 1;
      LPTSTR text = new TCHAR[nLen];
      ::GetWindowText(hWndCtrl, text, nLen);
      bstr = T2BSTR(text);
      delete[] text;
   }
   else
   {
      AfxSetWindowText(hWndCtrl, OLE2T(bstr));
   }
}

void DDV_NonEmptyString(CDataExchange* pDX, LPCTSTR name, CComBSTR& bstrText)
{
   bool bError = false;
   if (bstrText.Length() == 0 || bstrText == _T(""))
   {
      bError = true;
   }

   if (bError)
   {
      CString str;
      str.Format(_T("%s cannot be blank"), name);
      AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
      pDX->Fail();
   }
}

template <typename T>
void DDX_CacheEditText(CDataExchange* pDX, int nIDC, T& value)
{
   if (pDX->m_bSaveAndValidate)
   {
      CWnd* pWnd = pDX->m_pDlgWnd->GetDlgItem(nIDC);
      if (pWnd->IsKindOf(RUNTIME_CLASS(CCacheEdit)) && pWnd->IsWindowEnabled() == FALSE)
      {
         value = (T)((CCacheEdit*)(pWnd))->GetDefaultValue();
      }
      else
      {
         DDX_Text(pDX, nIDC, value);
      }
   }
   else
   {
      DDX_Text(pDX, nIDC, value);
      CWnd* pWnd = pDX->m_pDlgWnd->GetDlgItem(nIDC);
      if (pWnd->IsKindOf(RUNTIME_CLASS(CCacheEdit)))
      {
         CString strValue;
         pWnd->GetWindowText(strValue);
         ((CCacheEdit*)(pWnd))->SetDefaultValue((Float64)value,strValue);
      }
   }
}

void DDX_CBString(CDataExchange* pDX, int nIDC, CComBSTR& bstr)
{
   USES_CONVERSION;
   std::_tstring str;
   if (!pDX->m_bSaveAndValidate)
   {
      str = (OLE2T(bstr));
   }
   
   DDX_CBString(pDX, nIDC, str);

   if (pDX->m_bSaveAndValidate)
   {
      bstr = T2BSTR(str.c_str());
   }
}

template <class U>
void DDX_MyUnitValue(CDataExchange* pDX, int nIDC, Float64& data, const U& umIndirectMeasure)
{
   CWnd* pWnd = pDX->m_pDlgWnd->GetDlgItem(nIDC);
   if (pDX->m_bSaveAndValidate)
   {
      if (pWnd->IsWindowEnabled())
      {
         Float64 f;
         DDX_Text(pDX, nIDC, f);
         data = WBFL::Units::ConvertToSysUnits(f, umIndirectMeasure.UnitOfMeasure);
      }
      else
      {
         data = 0.0;
      }
   }
   else
   {
      CString strValue;
      if (data != Float64_Inf || pWnd->IsWindowEnabled()) // Infinite values create a blank line
      {
         strValue.Format(_T("%*.*f"), umIndirectMeasure.Width, umIndirectMeasure.Precision, WBFL::Units::ConvertFromSysUnits(data, umIndirectMeasure.UnitOfMeasure));
         strValue.TrimLeft();
      }
      DDX_Text(pDX, nIDC, strValue);

      if (pWnd->IsKindOf(RUNTIME_CLASS(CCacheEdit)))
      {
         ((CCacheEdit*)(pWnd))->SetDefaultValue(data, strValue);
      }
   }
}

void DDX_LengthValue(CDataExchange* pDX, int nIDC, Float64& data)
{
   CWnd* pWnd = pDX->m_pDlgWnd->GetDlgItem(nIDC);
   if (pDX->m_bSaveAndValidate)
   {
      if (pWnd->IsWindowEnabled())
      {
         CString strValue;
         DDX_Text(pDX, nIDC, strValue);
         if (!Formatter::ParseLength(strValue, &data))
         {
            pDX->PrepareEditCtrl(nIDC);
            AfxMessageBox(_T("Invalid format"), MB_OK);
            pDX->Fail();
         }
      }
      else
      {
         data = 0.0;
      }
   }
   else
   {
      CString strValue;
      if (data != Float64_Inf || pWnd->IsWindowEnabled()) // Infinite values create a blank line
      {
         strValue = Formatter::FormatLength(data, false);
         strValue.TrimLeft();
      }

      DDX_Text(pDX, nIDC, strValue);

      if (pWnd->IsKindOf(RUNTIME_CLASS(CCacheEdit)))
      {
         ((CCacheEdit*)(pWnd))->SetDefaultValue(data, strValue);
      }
   }
}

void DDX_Check_VariantBool(CDataExchange* pDX, int nIDC, VARIANT_BOOL& value)
{
   HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
   if (pDX->m_bSaveAndValidate)
   {
      int val = (int)::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L);
      ASSERT(0 <= val && val <= 2);
      value = (val == BST_CHECKED ? VARIANT_TRUE : VARIANT_FALSE);
   }
   else
   {
      int val = (value == VARIANT_TRUE ? BST_CHECKED : BST_UNCHECKED);
      ::SendMessage(hWndCtrl, BM_SETCHECK, (WPARAM)val, 0L);
   }
}


void CBendTypeComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
   ASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);

   CDC dc;
   dc.Attach(lpDrawItemStruct->hDC);

   COLORREF oldTextColor = dc.GetTextColor();
   COLORREF oldBkColor = dc.GetBkColor();

   CString lpszText;
   GetLBText(lpDrawItemStruct->itemID, lpszText);

   if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
      (lpDrawItemStruct->itemState & ODS_SELECTED))
   {
      dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
      dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
      dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));

      // Tell the parent page to update the girder image
      long bendType = (long)GetItemData(lpDrawItemStruct->itemID);
      CBarDlg* pParent = (CBarDlg*)GetParent();
      pParent->UpdateBendGuide(bendType);
   }
   else
   {
      dc.FillSolidRect(&lpDrawItemStruct->rcItem, oldBkColor);
   }

   dc.DrawText(lpszText, &lpDrawItemStruct->rcItem, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

   if (lpDrawItemStruct->itemState & ODS_FOCUS)
   {
      dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
   }

   dc.SetTextColor(oldTextColor);
   dc.SetBkColor(oldBkColor);

   dc.Detach();
}

IMPLEMENT_DYNAMIC(CBarDlg, CDialog)

CBarDlg::CBarDlg(IBarInfoMgr* pBarInfoMgr, IBarlist* pBarlist, long groupIdx, long barIdx,CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BAR, pParent), m_BarInfoMgr(pBarInfoMgr), m_Barlist(pBarlist), m_GroupIdx(groupIdx), m_BarIdx(barIdx), m_SM(this)
{
   m_bIsCollaboration = false;
}

CBarDlg::~CBarDlg()
{
}

void CBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_NUM_EACH, m_ctrlNumEach);
   DDX_Control(pDX, IDC_VARIES_U, m_ctrlVariesU);
   DDX_Control(pDX, IDC_VARIES_W, m_ctrlVariesW);
   DDX_Control(pDX, IDC_VARIES_X, m_ctrlVariesX);
   DDX_Control(pDX, IDC_VARIES_Y, m_ctrlVariesY);
   DDX_Control(pDX, IDC_VARIES_Z, m_ctrlVariesZ);
   DDX_Control(pDX, IDC_VARIES_T1, m_ctrlVariesT1);
   DDX_Control(pDX, IDC_VARIES_T2, m_ctrlVariesT2);
   DDX_Control(pDX, IDC_TYPE, m_cbBendType);
   DDX_Control(pDX, IDC_EPOXY, m_cbEpoxy);
}


BEGIN_MESSAGE_MAP(CBarDlg, CDialog)
   ON_CBN_SELCHANGE(IDC_MATERIALS, &CBarDlg::OnCbnSelchangeMaterials)
   ON_BN_CLICKED(ID_ADD_GROUP, &CBarDlg::OnClickedAddGroup)
   ON_CBN_SELCHANGE(IDC_GROUPS, &CBarDlg::OnSelchangeGroups)
   ON_CBN_SELCHANGE(IDC_MARK, &CBarDlg::OnCbnSelchangeMark)
   ON_BN_CLICKED(IDC_VARIES, &CBarDlg::OnBnClickedVaries)
   ON_BN_CLICKED(IDC_DELETE, &CBarDlg::OnBnClickedDelete)
   ON_BN_CLICKED(IDC_UPDATE_BAR, &CBarDlg::OnBnClickedUpdate)
   ON_CBN_SELCHANGE(IDC_TYPE, &CBarDlg::OnCbnSelchangeType)
   ON_BN_CLICKED(IDC_ADD_BAR, &CBarDlg::OnBnClickedAddBar)
   ON_BN_CLICKED(IDC_SAVE, &CBarDlg::OnBnClickedSave)
   ON_CBN_EDITCHANGE(IDC_MARK, &CBarDlg::OnCbnEditchangeMark)

   ON_CBN_SELCHANGE(IDC_BAR_SIZE, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_NUM_REQUIRED, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_NUM_EACH, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_U, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_W, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_X, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_Y, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_Z, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_T1, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_T2, &CBarDlg::OnChange)

   ON_EN_CHANGE(IDC_VARIES_U, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_VARIES_W, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_VARIES_X, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_VARIES_Y, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_VARIES_Z, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_VARIES_T1, &CBarDlg::OnChange)
   ON_EN_CHANGE(IDC_VARIES_T2, &CBarDlg::OnChange)
   
   ON_BN_CLICKED(IDC_LONGITUDINAL, &CBarDlg::OnChange)
   ON_BN_CLICKED(IDC_TRANSVERSE, &CBarDlg::OnChange)
   ON_BN_CLICKED(IDC_SEISMIC, &CBarDlg::OnChange)

   ON_BN_CLICKED(IDC_LUMP_SUM, &CBarDlg::OnChange)
   ON_BN_CLICKED(IDC_SUBSTRUCTURE, &CBarDlg::OnChange)
   ON_BN_CLICKED(IDC_EPOXY, &CBarDlg::OnChange)
   ON_BN_CLICKED(IDC_VARIES, &CBarDlg::OnChange)
   ON_BN_CLICKED(IDCANCEL, &CBarDlg::OnBnClickedCancel)
   ON_BN_CLICKED(IDCHELP, &CBarDlg::OnBnClickedHelp)
END_MESSAGE_MAP()


// CBarDlg message handlers


BOOL CBarDlg::OnInitDialog()
{
   UpdateGroups();
   UpdateBendTypes();

   CWnd* pWnd = GetDlgItem(IDC_DIMENSIONS_GROUP);
   CEAFApp* pApp = EAFGetApp();
   if (pApp->GetUnitsMode() == eafTypes::umUS)
   {
      pWnd->SetWindowText(_T("Dimensions (Length in feet [space] inches, Angles in degrees)"));
   }
   else
   {
      pWnd->SetWindowText(_T("Dimensions (Length in meters, Angles in degrees)"));
   }

   m_Font.CreatePointFont(80, _T("Symbol"));
   GetDlgItem(IDC_T1_LABEL)->SetFont(&m_Font);
   GetDlgItem(IDC_T2_LABEL)->SetFont(&m_Font);
   GetDlgItem(IDC_VARIES_T1_LABEL)->SetFont(&m_Font);
   GetDlgItem(IDC_VARIES_T2_LABEL)->SetFont(&m_Font);

   CheckRadioButton(IDC_LONGITUDINAL, IDC_SEISMIC, IDC_LONGITUDINAL);

   GetDlgItem(IDC_NUM_REQUIRED)->SetWindowText(_T("1"));
   GetDlgItem(IDC_NUM_EACH)->SetWindowText(_T("1"));

   CComboBox* pcbMaterial = (CComboBox*)GetDlgItem(IDC_MATERIALS);
   for (int i = 0; i < MATERIAL_COUNT; i++)
   {
      MaterialType material = (MaterialType)i;
      CString strMaterial = GetMaterialSpecification(material);
      CString strName = GetMaterialCommonName(material);
      strMaterial += _T(" - ") + strName;
      pcbMaterial->SetItemData(pcbMaterial->AddString(strMaterial), (DWORD_PTR)material);
   }
   pcbMaterial->SetCurSel(0);
   UpdateBarSizes();

   CDialog::OnInitDialog();

   UpdateVaries();

   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_MARK);
   pCB->LimitText(4); // mark numbers can be only 4 charaters long
   pCB->SetCurSel(m_BarIdx);

   CEdit* pEdit = (CEdit*)GetDlgItem(IDC_LOCATION);
   pEdit->LimitText(28); // location description is limited to 28 characters

   pEdit = (CEdit*)GetDlgItem(IDC_NUM_REQUIRED);
   pEdit->LimitText(4);

   pEdit = (CEdit*)GetDlgItem(IDC_NUM_EACH);
   pEdit->LimitText(2);
   
   CButton* pSave = (CButton*)GetDlgItem(IDC_SAVE);
   HICON hSaveIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SAVE), IMAGE_ICON, 16, 16, 0);
   pSave->SetIcon(hSaveIcon);

   CButton* pHelp = (CButton*)GetDlgItem(IDCHELP);
   HICON hHelpIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_HELP), IMAGE_ICON, 16, 16, 0);
   pHelp->SetIcon(hHelpIcon);

   // Determine initial state
   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   long nGroups;
   groups->get_Count(&nGroups);
   if (0 < nGroups)
   {
      UpdateMarkNumbers();

      CComPtr<IGroup> group;
      m_GroupIdx = Max(0L, m_GroupIdx);
      groups->get_Item(CComVariant(m_GroupIdx), &group);
      CComPtr<IBarRecordCollection> bars;
      group->get_BarRecords(&bars);

      long nBars;
      bars->get_Count(&nBars);
      if (0 < nBars)
      {
         // there are bars in the group so we are in the editing state
         m_SM.SetState(State::Editing);

         CComPtr<IBarRecord> barRecord;
         m_BarIdx = Max(0L, m_BarIdx);
         bars->get_Item(CComVariant(m_BarIdx), &barRecord);
         pCB->SetCurSel(m_BarIdx);
         UpdateBarData(FALSE, m_GroupIdx, m_BarIdx, barRecord);
         UpdateDimensions(barRecord);
      }
      else
      {
         // there aren't any bars in the groups so we are in the new bar state
         m_SM.SetState(State::NoBars);
         UpdateDimensions(nullptr);
      }
   }
   else
   {
      // there aren't any groups so we are in the empty list state
      m_SM.SetState(State::EmptyList);
   }

   CDataExchange dx(this,TRUE);
   MaterialType material;
   DDX_CBItemData(&dx, IDC_MATERIALS, material);
   m_cbEpoxy.EnableWindow(CanBeEpoxyCoated(material));

   UpdateStatus();
   OnStateChanged();

   CEAFDocument* pDoc = EAFGetDocument();
   if (pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      m_bIsCollaboration = true;
      GetDlgItem(ID_ADD_GROUP)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_ADD_BAR)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_DELETE)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_SAVE)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_UPDATE_BAR)->ShowWindow(SW_HIDE);
      SetDefID(IDCANCEL);
      GetDlgItem(IDCANCEL)->SetWindowText(_T("Close"));
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CBarDlg::OnStateChanged()
{
   BOOL bEnableAddBar;
   BOOL bEnableDeleteBar;
   BOOL bEnableUpdateBar;
   switch (m_SM.GetState())
   {
   case State::Editing:
      bEnableAddBar = TRUE;
      bEnableDeleteBar = TRUE;
      bEnableUpdateBar = TRUE;
      break;
   case State::EditingWithChange:
      bEnableAddBar = TRUE;
      bEnableDeleteBar = TRUE;
      bEnableUpdateBar = TRUE;
      break;
   case State::NoBars:
   case State::NewBar:
      bEnableAddBar = TRUE;
      bEnableDeleteBar = FALSE;
      bEnableUpdateBar = FALSE;
      break;
   case State::EmptyList:
      bEnableAddBar = FALSE;
      bEnableDeleteBar = FALSE;
      bEnableUpdateBar = FALSE;
      break;
   }

   GetDlgItem(IDC_ADD_BAR)->EnableWindow(bEnableAddBar);
   GetDlgItem(IDC_DELETE)->EnableWindow(bEnableDeleteBar);
   GetDlgItem(IDC_UPDATE_BAR)->EnableWindow(bEnableUpdateBar);
}

long CBarDlg::GetBarRecordCount()
{
   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);
   long nBars;
   bars->get_Count(&nBars);
   return nBars;
}

void CBarDlg::UpdateGroups()
{
   USES_CONVERSION;
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_GROUPS);
   int curSel = pCB->GetCurSel();
   pCB->ResetContent();

   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   long nGroups;
   groups->get_Count(&nGroups);
   for (long idx = 0; idx < nGroups; idx++)
   {
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(idx), &group);
      CComBSTR bstrName;
      group->get_Name(&bstrName);
      pCB->AddString(OLE2T(bstrName));
   }

   if (0 <= m_GroupIdx)
   {
      pCB->SetCurSel(m_GroupIdx);
   }
   else
   {
      m_GroupIdx = 0;
      pCB->SetCurSel(m_GroupIdx);
   }

   CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
   pDoc->SelectGroup(m_GroupIdx);
}

void CBarDlg::UpdateBarSizes()
{
   USES_CONVERSION;

   // Capture the current state of the combo box so it can
   // be reset if possible
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_BAR_SIZE);
   int curSel = pCB->GetCurSel();
   CString strCurrentSize;
   pCB->GetWindowText(strCurrentSize);


   // Fill the combo box with the bar sizes available for the
   // current material
   CDataExchange dx(this, TRUE);
   MaterialType material;
   DDX_CBItemData(&dx, IDC_MATERIALS, material);

   CComPtr<IBarCollection> bars;
   m_BarInfoMgr->get_Bars(material, &bars);

   CComPtr<IUnknown> unknown;
   bars->get__NewEnum(&unknown);
   CComQIPtr<IEnumVARIANT> enumVariant(unknown);

   pCB->ResetContent();

   CComVariant variant;
   ULONG nFetched;
   while (enumVariant->Next(1, &variant, &nFetched) == S_OK)
   {
      CComPtr<IBarData> barData;
      (variant.punkVal)->QueryInterface(&barData);

      CComBSTR bstrSize;
      barData->get_Name(&bstrSize);

      // not all materials have the all the bar sizes. look at gfrp bars
      // put this name in the combo box
      pCB->AddString(OLE2T(bstrSize));

      variant.Clear();
   }

   // Restore the selection if possible... even though different materials
   // have a different list of bars, the may have some of the same bars.
   // First try to restore by bar name text string because the same bars
   // may be at different indicies in the list.
   dx.m_bSaveAndValidate = FALSE;
   DDX_CBString(&dx, IDC_BAR_SIZE, strCurrentSize);
   int i = pCB->GetCurSel(); // check if something was selected
   if (i == CB_ERR && pCB->SetCurSel(curSel) == CB_ERR)
   {
      // still not selected... just select the first item
      pCB->SetCurSel(0);
   }
}

void CBarDlg::UpdateBendTypes()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_TYPE);

   const auto& vBendTypes = CBendFactory::GetBends();
   for (const auto& bend : vBendTypes)
   {
      pCB->SetItemData(pCB->AddString(CBendFactory::GetBendName(bend)),(DWORD_PTR)bend);
   }

   pCB->SetCurSel(0);
}

void CBarDlg::OnClickedAddGroup()
{
   if (!CheckEditState())
      return;

   CString strGroup;
   BOOL bResult = AfxQuestion(_T("New Group"), _T("Group Name:"), _T(""), strGroup);
   if (bResult)
   {
      CComPtr<IGroupCollection> groups;
      m_Barlist->get_Groups(&groups);
      if (FAILED(groups->Add(CComBSTR(strGroup))))
      {
         AfxMessageBox(_T("The barlist already contains a group with the same name."), MB_ICONINFORMATION | MB_OK);
      }
      else
      {
         long nGroups;
         groups->get_Count(&nGroups);
         m_GroupIdx = nGroups-1; // update group index
         m_SM.StateChange(Action::AddGroup);
         UpdateGroups();
      }
   }
}

void CBarDlg::OnSelchangeGroups()
{
   if (!CheckEditState())
      return;

   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_GROUPS);
   int curSel = pCB->GetCurSel();
   if (curSel != CB_ERR)
   {
      m_GroupIdx = (long)curSel;

      CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
      pDoc->SelectGroup(m_GroupIdx);

      CComPtr<IGroupCollection> groups;
      m_Barlist->get_Groups(&groups);
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(m_GroupIdx), &group);

      CComPtr<IBarRecordCollection> bars;
      group->get_BarRecords(&bars);

      long nBars;
      bars->get_Count(&nBars);

      m_BarIdx = (nBars == 0 ? -1 : 0);

      m_SM.StateChange(Action::SelectGroup);

      UpdateMarkNumbers();
      OnCbnSelchangeMark();
      UpdateStatus();
   }
}

void CBarDlg::OnCbnSelchangeMark()
{
   if (!CheckEditState())
      return;

   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_MARK);
   int curSel = pCB->GetCurSel();
   if (curSel != CB_ERR)
   {
      m_BarIdx = (long)curSel;

      CComPtr<IGroupCollection> groups;
      m_Barlist->get_Groups(&groups);
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(m_GroupIdx), &group);
      CComPtr<IBarRecordCollection> bars;
      group->get_BarRecords(&bars);
      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(m_BarIdx), &bar);

      if (bar)
      {
         UpdateBarData(FALSE, m_GroupIdx, m_BarIdx, bar);

         UpdateVaries();
         UpdateDimensions(bar);
         UpdateStatus(bar);
      }
   }
}


void CBarDlg::OnCbnEditchangeMark()
{
   m_SM.StateChange(Action::EditRecord);
}

void CBarDlg::UpdateStatus()
{
   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);

   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);
   CComPtr<IBarRecord> bar;
   bars->get_Item(CComVariant(m_BarIdx), &bar);

   if (bar)
   {
      UpdateStatus(bar);
   }
   else
   {
      UpdateStatus(group);
   }
}

void CBarDlg::UpdateStatus(IGroup* pGroup)
{
   if (pGroup)
   {
      StatusType status;
      pGroup->get_Status(&status);
      UpdateStatus(status);
      UpdateMessages(nullptr);
   }
}

void CBarDlg::UpdateStatus(IBarRecord* pBarRecord)
{
   if (pBarRecord)
   {
      StatusType status;
      pBarRecord->get_Status(&status);
      UpdateStatus(status);
      UpdateMessages(pBarRecord);
   }
}

void CBarDlg::UpdateStatus(const StatusType& status)
{
   CStatic* pIcon = (CStatic*)GetDlgItem(IDC_STATUS);
   if (status == stOK)
   {
      pIcon->SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_GREEN), IMAGE_ICON, 12, 12, 0));
   }
   else if (status == stWarning)
   {
      pIcon->SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_YELLOW), IMAGE_ICON, 12, 12, 0));
   }
   else
   {
      ATLASSERT(status == stError);
      pIcon->SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RED), IMAGE_ICON, 12, 12, 0));
   }
}

void CBarDlg::UpdateMarkNumbers()
{
   USES_CONVERSION;

   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_MARK);
   int curSel = pCB->GetCurSel();
   pCB->ResetContent();

   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);

   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   long nBars;
   bars->get_Count(&nBars);
   for (int i = 0; i < nBars; i++)
   {
      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(i), &bar);

      CComBSTR bstrMark;
      bar->get_Mark(&bstrMark);

      pCB->AddString(OLE2T(bstrMark));
   }

   if (curSel == CB_ERR)
   {
      pCB->SetCurSel(m_BarIdx);
   }
   else
   {
      m_BarIdx = curSel;
      pCB->SetCurSel(curSel);
   }
}

void CBarDlg::UpdateBarData(BOOL bSaveAndValidate, long groupIdx, long barIdx, IBarRecord* pBarRecord)
{
   CDataExchange dx(this, bSaveAndValidate);
   CDataExchange* pDX = &dx;

   // get the current material... if the material changes, then
   // we have to update the bar size list
   CDataExchange dx2(this, TRUE);
   MaterialType current_material;
   DDX_CBItemData(&dx2, IDC_MATERIALS, current_material);

   CEAFApp* pApp = EAFGetApp();
   const auto* pDisplayUnits = pApp->GetDisplayUnits();

   MaterialType material;
   CComBSTR bstrMark;
   CComBSTR bstrLocation;
   CComBSTR bstrSize;
   long bendType;
   long nReqd;
   long nEach;
   UseType use;
   VARIANT_BOOL vbSubstructure;
   VARIANT_BOOL vbEpoxy;
   VARIANT_BOOL vbVaries;

   const int primary = 0;
   const int varies = 1;
   std::array<Float64, 2> u{ 0,0 }, w{ 0,0 }, x{ 0,0 }, y{ 0,0 }, z{ 0,0 }, t1{ 0,0 }, t2{ 0,0 };

   CComPtr<IBend> primaryBend;

   if (!bSaveAndValidate)
   {
      pBarRecord->get_Material(&material);
      pBarRecord->get_Mark(&bstrMark);
      pBarRecord->get_Location(&bstrLocation);
      pBarRecord->get_BendType(&bendType);
      pBarRecord->get_Size(&bstrSize);
      pBarRecord->get_NumReqd(&nReqd);
      pBarRecord->get_Use(&use);
      pBarRecord->get_Substructure(&vbSubstructure);
      pBarRecord->get_Epoxy(&vbEpoxy);
      pBarRecord->get_Varies(&vbVaries);
      pBarRecord->get_NumEach(&nEach);

      pBarRecord->get_PrimaryBend(&primaryBend);
      primaryBend->get_U(&u[primary]);
      primaryBend->get_W(&w[primary]);
      primaryBend->get_X(&x[primary]);
      primaryBend->get_Y(&y[primary]);
      primaryBend->get_Z(&z[primary]);
      primaryBend->get_T1(&t1[primary]);
      primaryBend->get_T2(&t2[primary]);

      if (vbVaries == VARIANT_TRUE)
      {
         CComPtr<IBend> variesBend;
         pBarRecord->get_VariesBend(&variesBend);
         variesBend->get_U(&u[varies]);
         variesBend->get_W(&w[varies]);
         variesBend->get_X(&x[varies]);
         variesBend->get_Y(&y[varies]);
         variesBend->get_Z(&z[varies]);
         variesBend->get_T1(&t1[varies]);
         variesBend->get_T2(&t2[varies]);
      }

      // Now that we know the bar size, fill up the bar size combo box so the DDX below will
      // select the correct value. Only have to do this if the material type changes
      if (material != current_material)
      {
         UpdateBarSizes();
      }
   }

   DDX_CBItemData(pDX, IDC_MATERIALS, material);

   DDX_Text(pDX, IDC_MARK, bstrMark);
   DDV_NonEmptyString(pDX, _T("Mark No."), bstrMark);

   DDX_Text(pDX, IDC_LOCATION, bstrLocation);
   DDV_NonEmptyString(pDX, _T("Location"), bstrLocation);

   DDX_CBItemData(pDX, IDC_TYPE, bendType);
   DDX_CBString(pDX, IDC_BAR_SIZE, bstrSize);
   DDX_Text(pDX, IDC_NUM_REQUIRED, nReqd);
   DDV_MinMaxLong(pDX, nReqd, 1, 9999);

   DDX_RadioEnum(pDX, IDC_LONGITUDINAL, use);
   DDX_Check_VariantBool(pDX, IDC_SUBSTRUCTURE, vbSubstructure);
   DDX_Check_VariantBool(pDX, IDC_EPOXY, vbEpoxy);
   DDX_Check_VariantBool(pDX, IDC_VARIES, vbVaries);
   DDX_CacheEditText(pDX, IDC_NUM_EACH, nEach);
   if (vbVaries == VARIANT_TRUE)
   {
      DDV_MinMaxLong(pDX, nEach, 1, 99);
   }

   DDX_LengthValue(pDX, IDC_U, u[primary]);
   DDX_LengthValue(pDX, IDC_W, w[primary]);
   DDX_LengthValue(pDX, IDC_X, x[primary]);
   DDX_LengthValue(pDX, IDC_Y, y[primary]);
   DDX_LengthValue(pDX, IDC_Z, z[primary]);
   DDX_MyUnitValue(pDX, IDC_T1, t1[primary], pDisplayUnits->Angle);
   DDX_MyUnitValue(pDX, IDC_T2, t2[primary], pDisplayUnits->Angle);

   DDX_LengthValue(pDX, IDC_VARIES_U, u[varies]);
   DDX_LengthValue(pDX, IDC_VARIES_W, w[varies]);
   DDX_LengthValue(pDX, IDC_VARIES_X, x[varies]);
   DDX_LengthValue(pDX, IDC_VARIES_Y, y[varies]);
   DDX_LengthValue(pDX, IDC_VARIES_Z, z[varies]);
   DDX_MyUnitValue(pDX, IDC_VARIES_T1, t1[varies], pDisplayUnits->Angle);
   DDX_MyUnitValue(pDX, IDC_VARIES_T2, t2[varies], pDisplayUnits->Angle);

   if (bSaveAndValidate)
   {
      if (vbVaries == VARIANT_TRUE && (nReqd % nEach != 0))
      {
         pDX->PrepareEditCtrl(IDC_NUM_EACH);
         AfxMessageBox(_T("No. Each must be an even multipe of No. Reqd"), MB_ICONERROR | MB_OK);
         pDX->Fail();
      }

      pBarRecord->put_Material(material);

      pBarRecord->put_Mark(bstrMark);
      pBarRecord->put_Location(bstrLocation);

      CComPtr<IBarCollection> bars;
      m_BarInfoMgr->get_Bars(material,&bars);
      CComPtr<IBarData> barData;
      bars->get_Item(CComVariant(bstrSize), &barData);
      pBarRecord->put_BarData(barData);

      pBarRecord->put_NumReqd(nReqd);

      pBarRecord->put_Use(use);

      pBarRecord->put_Substructure(vbSubstructure);
      pBarRecord->put_Epoxy(vbEpoxy);

      CComPtr<IBend> primaryBend;
      CBendFactory::CreateBend(bendType, &primaryBend);
      primaryBend->put_U(u[primary]);
      primaryBend->put_W(w[primary]);
      primaryBend->put_X(x[primary]);
      primaryBend->put_Y(y[primary]);
      primaryBend->put_Z(z[primary]);
      primaryBend->put_T1(t1[primary]);
      primaryBend->put_T2(t2[primary]);
      pBarRecord->put_PrimaryBend(primaryBend);

      // Varies Bend
      if (vbVaries == VARIANT_TRUE)
      {
         pBarRecord->put_NumEach(nEach);

         CComPtr<IBend> variesBend;
         CBendFactory::CreateBend(bendType, &variesBend);
         variesBend->put_U(u[varies]);
         variesBend->put_W(w[varies]);
         variesBend->put_X(x[varies]);
         variesBend->put_Y(y[varies]);
         variesBend->put_Z(z[varies]);
         variesBend->put_T1(t1[varies]);
         variesBend->put_T2(t2[varies]);

         pBarRecord->put_VariesBend(variesBend);
      }
   }

   // don't use m_cbEpoxy here... we want to set the enabled state
   // without messing with the check mark
   GetDlgItem(IDC_EPOXY)->EnableWindow(CanBeEpoxyCoated(material));

   // after a bar record is either put into the UI or extracted from the UI
   // we are in editing state
   m_SM.SetState(State::Editing);
}

void CBarDlg::UpdateMessages(IBarRecord* pBarRecord)
{
   if (pBarRecord)
   {
      CString strMsgs;

      CComPtr<IBend> primaryBend;
      pBarRecord->get_PrimaryBend(&primaryBend);
      Float64 length;
      primaryBend->get_Length(&length);
      CString strLength = Formatter::FormatLength(length);
      strMsgs += _T("Primary bend length: ") + strLength + _T("\r\n");

      VARIANT_BOOL vbVaries;
      pBarRecord->get_Varies(&vbVaries);
      if (vbVaries == VARIANT_TRUE)
      {
         CComPtr<IBend> variesBend;
         pBarRecord->get_VariesBend(&variesBend);
         Float64 length;
         variesBend->get_Length(&length);
         CString strLength = Formatter::FormatLength(length);
         strMsgs += _T("Varies bend length: ") + strLength + _T("\r\n");
      }

      CComPtr<IStatusMessageCollection> statusMessages;
      pBarRecord->get_StatusMessages(&statusMessages);
      GetStatusMessages(_T("*** Bar Record ***"), strMsgs, statusMessages);

      statusMessages.Release();
      primaryBend->get_StatusMessages(&statusMessages);
      GetStatusMessages(_T("*** Primary Bend ***"), strMsgs, statusMessages);

      if (vbVaries == VARIANT_TRUE)
      {
         CComPtr<IBend> variesBend;
         pBarRecord->get_VariesBend(&variesBend);
         statusMessages.Release();
         variesBend->get_StatusMessages(&statusMessages);
         GetStatusMessages(_T("*** Varies Bend ***"), strMsgs, statusMessages);
      }

      GetDlgItem(IDC_STATUS_MESSAGE)->SetWindowText(strMsgs);
   }
}

void CBarDlg::GetStatusMessages(LPCTSTR lpszMsgGroup,CString& strMsgs, IStatusMessageCollection* pMessages)
{
   long nMessages;
   pMessages->get_Count(&nMessages);
   if (0 < nMessages)
   {
      if (0 < strMsgs.GetLength())
      {
         strMsgs += _T("\r\n");
      }
      strMsgs += lpszMsgGroup;
      strMsgs += _T("\r\n");
   }

   for (long i = 0; i < nMessages; i++)
   {
      CComPtr<IStatusMessage> statusMessage;
      pMessages->get_Item(i, &statusMessage);

      CString strMsg = Formatter::FormatStatusMessage(statusMessage);

      if (i != 0)
      {
         strMsgs += _T("\r\n");
      }
      strMsgs += strMsg;
   }
}

void CBarDlg::OnBnClickedVaries()
{
   BOOL bEnable = (IsDlgButtonChecked(IDC_VARIES) == BST_CHECKED) ? TRUE : FALSE;

   m_ctrlNumEach.EnableWindow(bEnable);

   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_TYPE);
   int curSel = pCB->GetCurSel();
   if (curSel != CB_ERR)
   {
      long bendType = (long)pCB->GetItemData(curSel);
      UpdateDimensions(bendType, bEnable ? true : false);
   }
}

void CBarDlg::UpdateVaries()
{
   // OnBnClickedVaries causes the state to change
   // we don't want a state change so record the state and reset it
   State state = m_SM.GetState();
   OnBnClickedVaries();
   m_SM.SetState(state);
}

void CBarDlg::UpdateBendGuide(IBarRecord* pBarRecord)
{
   long bendType = -1;
   if (pBarRecord)
   {
      pBarRecord->get_BendType(&bendType);
   }
   UpdateBendGuide(bendType);
}

void CBarDlg::UpdateBendGuide(long bendType)
{
   CDataExchange dc(this, FALSE);
   CString strBend = CBendFactory::GetBendName(bendType);
   strBend.Replace(_T(" "), _T("_"));
   strBend.MakeUpper();
   DDX_MetaFileStatic(&dc, IDC_BEND_GUIDE, m_BendGuide, AfxGetInstanceHandle(), strBend, _T("Metafile"), EMF_RESIZE);
}

void CBarDlg::UpdateDimensions(IBarRecord* pBarRecord)
{
   long bendType = 50;
   VARIANT_BOOL vbVaries = VARIANT_FALSE;
   if (pBarRecord)
   {
      pBarRecord->get_BendType(&bendType);
      pBarRecord->get_Varies(&vbVaries);
   }
   UpdateDimensions(bendType, vbVaries == VARIANT_TRUE);
   UpdateBendGuide(bendType);
}


#define UPDATE(_dim_,_IDC_,ctrlVaries) \
bend->get_SupportsDimension(_dim_, &vbSupported);\
GetDlgItem(_IDC_)->EnableWindow(vbSupported == VARIANT_TRUE); \
ctrlVaries.EnableWindow(bVaries && vbSupported == VARIANT_TRUE);\
if(vbSupported==VARIANT_FALSE)GetDlgItem(_IDC_)->SetWindowText(_T(""));
void CBarDlg::UpdateDimensions(long bendType, bool bVaries)
{
   State state = m_SM.GetState();

   CComPtr<IBend> bend;
   CBendFactory::CreateBend(bendType,&bend);
   VARIANT_BOOL vbSupported;
   UPDATE(dimU,IDC_U, m_ctrlVariesU);
   UPDATE(dimW,IDC_W, m_ctrlVariesW);
   UPDATE(dimX,IDC_X, m_ctrlVariesX);
   UPDATE(dimY,IDC_Y, m_ctrlVariesY);
   UPDATE(dimZ,IDC_Z, m_ctrlVariesZ);
   UPDATE(dimT1,IDC_T1, m_ctrlVariesT1);
   UPDATE(dimT2,IDC_T2, m_ctrlVariesT2);

   m_SM.SetState(state);
}

void CBarDlg::SelectBar(long groupIdx, long barIdx)
{
   m_GroupIdx = groupIdx;
   m_BarIdx = barIdx;
   CComboBox* pcbGroup = (CComboBox*)GetDlgItem(IDC_GROUPS);
   CComboBox* pcbBar = (CComboBox*)GetDlgItem(IDC_MARK);

   pcbGroup->SetCurSel((int)groupIdx);
   UpdateMarkNumbers();

   if (0 <= barIdx)
   {
      pcbBar->SetCurSel(barIdx);
      OnCbnSelchangeMark();
   }
   else
   {
      UpdateDimensions(nullptr);
   }

   UpdateStatus();
}

void CBarDlg::OnBnClickedDelete()
{
   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);
   bars->Remove(CComVariant(m_BarIdx));

   // update the bar index
   // generally, we want to select the next bar, but if this is the last bar
   // then use index of the new last bar
   long nBars;
   bars->get_Count(&nBars);
   m_BarIdx = Min(m_BarIdx + 1, nBars - 1);

   m_SM.StateChange(Action::DeleteRecord);

   SelectBar(m_GroupIdx, m_BarIdx);
}

void CBarDlg::OnBnClickedUpdate()
{
   CComPtr<IBarRecord> barRecord;
   barRecord.CoCreateInstance(CLSID_BarRecord);
   UpdateBarData(TRUE, m_GroupIdx, m_BarIdx, barRecord);

   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   if (SUCCEEDED(bars->Replace(CComVariant(m_BarIdx), barRecord)))
   {
      m_SM.StateChange(Action::UpdateRecord);

      UpdateVaries();
      UpdateDimensions(barRecord);
      UpdateStatus(barRecord);
   }
   else
   {
      // Attempted to replace a non-existent bar record. This happens when there is a new bar mark at the bottom of the drop down list that
      // was generated from the last AddBar. There isn't a bar to update so add the bar instead
      OnBnClickedAddBar();
   }
}

void CBarDlg::OnCbnSelchangeType()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_TYPE);
   int curSel = pCB->GetCurSel();
   if (curSel != CB_ERR)
   {
      long bendType = (long)(pCB->GetItemData(curSel));
      bool bVaries = IsDlgButtonChecked(IDC_VARIES) == BST_CHECKED ? true : false;
      UpdateDimensions(bendType, bVaries);
      m_SM.StateChange(Action::EditRecord);
   }
}

void CBarDlg::OnCbnSelchangeMaterials()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_MATERIALS);
   int curSel = pCB->GetCurSel();
   if (curSel != CB_ERR)
   {
      MaterialType material = (MaterialType)(pCB->GetItemData(curSel));
      m_cbEpoxy.EnableWindow(CanBeEpoxyCoated(material));
      m_SM.StateChange(Action::EditRecord);
      UpdateBarSizes(); // bar sizes are a function of material type... update the bar size list
   }
}

void CBarDlg::OnBnClickedAddBar()
{
   CComPtr<IBarRecord> barRecord;
   barRecord.CoCreateInstance(CLSID_BarRecord);
   UpdateBarData(TRUE, m_GroupIdx, m_BarIdx, barRecord);

   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   long nBars;
   bars->get_Count(&nBars);
   m_BarIdx = nBars; // this is before adding the bar record to the bars collection, so this is the index

   bars->Add(barRecord);

   UpdateVaries();
   UpdateDimensions(barRecord);
   UpdateStatus(barRecord);

   SelectBar(m_GroupIdx, m_BarIdx);

   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_MARK);
   CComBSTR bstrMark;
   barRecord->get_Mark(&bstrMark);
   bstrMark = AutoIncrementMark(CString(bstrMark));
   pCB->SetCurSel(pCB->AddString(OLE2T(bstrMark)));

   m_SM.StateChange(Action::AddRecord);
}

CString CBarDlg::AutoIncrementMark(const CString& strMark)
{
   CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
   return pDoc->AutoIncrementMark(strMark);
}


void CBarDlg::OnBnClickedSave()
{
   if (CheckEditState())
   {
      EAFGetMainFrame()->PostMessage(WM_COMMAND, ID_FILE_SAVE);
   }
}

BOOL CBarDlg::CheckEditState()
{
   State state = m_SM.GetState();

   if (m_bIsCollaboration)
   {
      if (state == State::EditingWithChange)
      {
         AfxMessageBox(_T("Bars cannot be modified in a collaboration project. All changes to this bar record will be discarded."), MB_ICONWARNING | MB_OK);
      }
      return TRUE;
   }

   int result = IDYES;
   if (state == State::EditingWithChange || state == State::NewBar)
   {
      // the data in the dialog is in a modified state
      // ask the user if they want to save the data before proceeding

      result = AfxMessageBox(_T("There are unsaved changes to the current bar.\nWould you like to save them before proceeded?\nYes = Save changes and proceed\nNo = Discard changes and proceed\nCancel = Don't proceed"), MB_ICONQUESTION | MB_YESNOCANCEL);
      if (result == IDYES)
      {
         if (state == State::EditingWithChange)
         {
            OnBnClickedUpdate();
         }
         else
         {
            ATLASSERT(state == State::NewBar);
            OnBnClickedAddBar();
         }
      }
   }

   return result == IDCANCEL ? FALSE : TRUE;
}

void CBarDlg::OnChange()
{
   m_SM.StateChange(Action::EditRecord);
}

void CBarDlg::OnBnClickedCancel()
{
   if (CheckEditState())
   {
      CDialog::OnCancel();
   }
}

void CBarDlg::OnBnClickedHelp()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(), IDH_BARS);
}
