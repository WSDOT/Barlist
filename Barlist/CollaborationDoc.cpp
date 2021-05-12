///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2021  Washington State Department of Transportation
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

#include "Events.h"

#pragma Reminder("Once we switch to C++ 17/20, we can use the native std::filesystem instead of boost::filesystem")
#include <boost\filesystem.hpp>

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

   boost::filesystem::path collaboration_file_path(lpszPathName); // create a path for the filename
   std::ifstream ifile(collaboration_file_path.generic_string()); // open a file stream to read from

   if (ifile.fail() || !ifile.is_open())
   {
      CString strMsg;
      strMsg.Format(_T("Unable to open %s."), lpszPathName);
      AfxMessageBox(strMsg, MB_ICONERROR | MB_OK);
      return FALSE;
   }
   else
   {
      collaboration_file_path = collaboration_file_path.parent_path(); // we need only the path, not path + filename, to determine path from relatively located files

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
            boost::filesystem::path rel_path(strLine); // relative path to barlist file used in the collaboration

            // convert the relative path to a full path
            auto full_path_name = boost::filesystem::canonical(rel_path, collaboration_file_path);
            m_vFiles.push_back(full_path_name.string());
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

   boost::filesystem::path collaboration_file_path(lpszPathName); // create a file path object
   std::ofstream ofile(collaboration_file_path.generic_string()); // open a file stream
   if (ofile.bad())
   {
      return FALSE;
   }
   else
   {
      collaboration_file_path.remove_filename(); // remove the filename so we have just the path

      // iterate over the files the make up the collaboration... these a full path filenames
      for (const auto& fileName : m_vFiles)
      {
         boost::filesystem::path this_file(fileName);

         // determine the relative location of the barlist file compared to the collaboration file
         boost::filesystem::path rel_path = this_file.lexically_relative(collaboration_file_path);

         if (rel_path.compare(_T("")) == 0)
         {
            // there isn't a relative path so we have to store the full path
            rel_path = this_file;
         }

         ofile << rel_path.generic_string() << std::endl;
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
   BOOL bUpdate = FALSE;
   { // scoping the managage stage 
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

         bUpdate = TRUE;
      }
   }

   if (bUpdate)
   {
      SetModifiedFlag();
      UpdateAllViews(nullptr, HINT_PROJECT_CHANGED, nullptr); // asserts fire if this is called in the scope of AfxGetStaticModuleState
   }
   return bUpdate;
}
