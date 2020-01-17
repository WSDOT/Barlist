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

// CollaborationDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "CollaborationDoc.h"
#include "CollaborationManagerDlg.h"

// CCollaborationDoc

IMPLEMENT_DYNCREATE(CCollaborationDoc, CBarlistDoc)

CCollaborationDoc::CCollaborationDoc()
{
}

CCollaborationDoc::~CCollaborationDoc()
{
}


BEGIN_MESSAGE_MAP(CCollaborationDoc, CBarlistDoc)
   ON_COMMAND(IDC_COLLABORATION_MANAGER, &CCollaborationDoc::OnCollaborationManager)
END_MESSAGE_MAP()

BOOL CCollaborationDoc::OnNewDocument()
{
   if (!CBarlistDoc::OnNewDocument())
   {
      return FALSE;
   }

   return EditCollaboration();
}

BOOL CCollaborationDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   USES_CONVERSION;
   m_vFiles.clear();

   CString strPathName(lpszPathName);
   std::ifstream ifile(strPathName);
   if (ifile.fail() || !ifile.is_open())
   {
      CString strMsg;
      strMsg.Format(_T("Unable to open %s."), lpszPathName);
      AfxMessageBox(strMsg, MB_ICONERROR | MB_OK);
      return FALSE;
   }
   else
   {
      CString strDelim((char)1); // Barlist 4 uses ascii char 1 as a delimiter
      std::vector<CString> vProperties;
      vProperties.reserve(5);

      std::string strLine;
      while (std::getline(ifile, strLine))
      {
         CString str(strLine.c_str());
         if (str.Left(10) == _T("Properties"))
         {
            str = str.Mid(10);

            int curPos = 0;
            CString strToken = str.Tokenize(strDelim, curPos);
            while (strToken != _T(""))
            {
               vProperties.push_back(strToken);
               strToken = str.Tokenize(strDelim, curPos);
            }
         }
         else
         {
            m_vFiles.push_back(strLine);
         }
      }

      ATLASSERT(vProperties.size() == 5);
      if (vProperties.size() < 5)
      {
         for (int i = (int)vProperties.size(); i < 5; i++)
         {
            vProperties.push_back(_T(""));
         }
      }

      m_Barlist->put_Project(CComBSTR(vProperties[0]));
      m_Barlist->put_JobNumber(CComBSTR(vProperties[1]));
      m_Barlist->put_Engineer(CComBSTR(vProperties[2]));
      m_Barlist->put_Company(CComBSTR(vProperties[3]));
      m_Barlist->put_Comments(CComBSTR(vProperties[4]));
   }

   if (!LoadFiles())
      return FALSE;

   NotifyDuplicateGroups();

   GetBarlistEvents(TRUE);

   return TRUE;
}

void CCollaborationDoc::NotifyDuplicateGroups()
{
   if (0 < m_vDuplicateGroups.size())
   {
      CString strMsg(_T("The following Groups where found to be duplicates.\nThe bars within each identical group were merged\n"));
      for (const auto& strDupGroup : m_vDuplicateGroups)
      {
         strMsg += _T("\n") + strDupGroup;
      }
      AfxMessageBox(strMsg, MB_ICONEXCLAMATION | MB_OK);
   }
}

BOOL CCollaborationDoc::SaveTheDocument(LPCTSTR lpszPathName)
{
   USES_CONVERSION;
   CString strPathName(lpszPathName);
   std::ofstream ofile(strPathName);
   if (ofile.bad())
   {
      return FALSE;
   }
   else
   {
      for (const auto& fileName : m_vFiles)
      {
         ofile << fileName << std::endl;
      }
      
      CComBSTR bstrProject, bstrJobNumber, bstrEngineer, bstrCompany, bstrComments;
      m_Barlist->get_Project(&bstrProject);
      m_Barlist->get_JobNumber(&bstrJobNumber);
      m_Barlist->get_Engineer(&bstrEngineer);
      m_Barlist->get_Company(&bstrCompany);
      m_Barlist->get_Comments(&bstrComments);

      CString strProject(bstrProject);
      CString strJobNumber(bstrJobNumber);
      CString strEngineer(bstrEngineer);
      CString strCompany(bstrCompany);
      CString strComments(bstrComments);
      CString strProperties;
      strProperties.Format(_T("Properties%s%c%s%c%s%c%s%c%s%c"), strProject, (char)1, strJobNumber, (char)1, strEngineer, char(1), strCompany, (char)1, strComments, (char)1);
      ofile << OLE2A(strProperties.GetBuffer()) << std::endl;
   }

   return TRUE;
}

BOOL CCollaborationDoc::LoadFiles()
{
   CWaitCursor cursor;

   m_vDuplicateGroups.clear();
   USES_CONVERSION;
   for (const auto& strFile : m_vFiles)
   {
      CComPtr<IBarlist> barlist;
      if (!ReadBarlistFromFile(A2T(strFile.c_str()), &barlist))
      {
         return FALSE;
      }
      else
      {
         Merge(barlist);
      }
   }
   return TRUE;
}

void CCollaborationDoc::Merge(IBarlist* pBarlist)
{
   CComPtr<IGroupCollection> groups;
   m_Barlist->get_Groups(&groups);

   CComPtr<IGroupCollection> new_groups;
   pBarlist->get_Groups(&new_groups);
   long nGroups;
   new_groups->get_Count(&nGroups);
   for (long grpIdx = 0; grpIdx < nGroups; grpIdx++)
   {
      CComPtr<IGroup> new_group;
      new_groups->get_Item(CComVariant(grpIdx), &new_group);

      CComBSTR bstrName;
      new_group->get_Name(&bstrName);

      CComPtr<IGroup> group;
      HRESULT hr = groups->get_Item(CComVariant(bstrName), &group);
      if (FAILED(hr))
      {
         // group does not currently exist... create it
         groups->Add(bstrName);
         hr = groups->get_Item(CComVariant(bstrName), &group);
         ATLASSERT(SUCCEEDED(hr));
      }
      else
      {
         m_vDuplicateGroups.push_back(CString(bstrName));
      }

      CComPtr<IBarRecordCollection> barRecords;
      group->get_BarRecords(&barRecords);

      CComPtr<IBarRecordCollection> newBarRecords;
      new_group->get_BarRecords(&newBarRecords);
      long nBars;
      newBarRecords->get_Count(&nBars);
      for (long barIdx = 0; barIdx < nBars; barIdx++)
      {
         CComPtr<IBarRecord> newBar;
         newBarRecords->get_Item(CComVariant(barIdx), &newBar);

         CComPtr<IBarRecord> clone;
         CopyBar(newBar, &clone);

         barRecords->Add(clone);
      }
   }

   Float64 bridge, wall, traffic;
   m_Barlist->get_BridgeGrateInletQuantity(&bridge);
   m_Barlist->get_RetainingWallQuantity(&wall);
   m_Barlist->get_TrafficBarrierQuantity(&traffic);

   Float64 new_bridge, new_wall, new_traffic;
   pBarlist->get_BridgeGrateInletQuantity(&new_bridge);
   pBarlist->get_RetainingWallQuantity(&new_wall);
   pBarlist->get_TrafficBarrierQuantity(&new_traffic);

   m_Barlist->put_BridgeGrateInletQuantity(bridge + new_bridge);
   m_Barlist->put_RetainingWallQuantity(wall + new_wall);
   m_Barlist->put_TrafficBarrierQuantity(traffic + new_traffic);
}

// CCollaborationDoc diagnostics

#ifdef _DEBUG
void CCollaborationDoc::AssertValid() const
{
   CBarlistDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CCollaborationDoc::Dump(CDumpContext& dc) const
{
   CBarlistDoc::Dump(dc);
}
#endif
#endif //_DEBUG


// CCollaborationDoc commands

UINT CCollaborationDoc::GetToolbarResourceID()
{
   return IDR_COLLABORATION;
}

void CCollaborationDoc::OnCollaborationManager()
{
   EditCollaboration();
}

BOOL CCollaborationDoc::EditCollaboration()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CCollaborationManagerDlg dlg;
   dlg.m_vFiles = m_vFiles;
   if (dlg.DoModal() == IDOK)
   {
      m_vFiles = dlg.m_vFiles;

      // Stop listening to events
      GetBarlistEvents(FALSE);

      // Throw out the old barlist
      m_Barlist.Release();
      m_Barlist.CoCreateInstance(CLSID_Barlist);

      // Load the files to create the new barlist
      LoadFiles();
      NotifyDuplicateGroups();

      // Listen to events
      GetBarlistEvents(TRUE);
      return TRUE;
   }
   return FALSE;
}
