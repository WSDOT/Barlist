#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include <string>
#include <tchar.h>

#include "..\Bars\Barlist.h"
#include "..\Bars\BarRecord.h"
#include "..\Bars\BendImpl.h"
#include "..\Bars\BarInfoMgr.h"
#include "..\Bars\BarCollection.h"
#include "..\Bars\BarData.h"
#include "..\Bars\Type50.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// CBarRecord::UpdateStatus() (Bars\BarRecord.cpp) sets stWarning for two independent
// reasons -- a mark number reused elsewhere in the barlist, and a #18 bar bent at Grade 75
// or higher (a CRSI safety recommendation) -- plus stError for a varies-bend NumReqd/NumEach
// mismatch. The varies-bend error path is already covered by Quantities.cpp's mass/quantity
// tests; these tests cover the two warning paths directly.
namespace BarlistUnitTests
{
	TEST_CLASS(BarStatus)
	{
	public:
		inline static std::unique_ptr<CBarInfoMgr> bar_information_mgr = nullptr;

		TEST_CLASS_INITIALIZE(BarStatus_Initialize)
		{
			bar_information_mgr = std::make_unique<CBarInfoMgr>();
		}

		TEST_CLASS_CLEANUP(BarStatus_Cleanup)
		{
			bar_information_mgr.reset();
		}

		std::shared_ptr<CBarRecord> MakeBarRecord(MaterialType material, const std::_tstring& size, const std::_tstring& mark)
		{
			const CBarCollection& barSizes = bar_information_mgr->GetBars(material);
			const CBarData* barData = barSizes.Find(size);

			auto bend = std::make_shared<CType50>();
			bend->SetU(10.0);

			auto record = std::make_shared<CBarRecord>();
			record->SetBarData(barData);
			record->SetMaterial(material);
			record->SetNumReqd(1);
			record->SetPrimaryBend(bend);
			record->SetMark(mark);
			return record;
		}

		// ---- Duplicate mark number ----

		TEST_METHOD(DuplicateMark_AcrossGroups_SetsWarningOnBothRecords)
		{
			CBarlist barlist;
			auto groupA = barlist.GetGroups().Add(_T("Group A"));
			auto groupB = barlist.GetGroups().Add(_T("Group B"));

			auto record1 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A1"));
			auto record2 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A1")); // same mark, different group
			groupA->GetBarRecords().Add(record1);
			groupB->GetBarRecords().Add(record2);

			Assert::IsTrue(StatusType::stWarning == record1->GetStatus());
			Assert::IsTrue(StatusType::stWarning == record2->GetStatus());
		}

		TEST_METHOD(UniqueMarks_NoWarning)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record1 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A1"));
			auto record2 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A2"));
			group->GetBarRecords().Add(record1);
			group->GetBarRecords().Add(record2);

			Assert::IsTrue(StatusType::stOK == record1->GetStatus());
			Assert::IsTrue(StatusType::stOK == record2->GetStatus());
		}

		TEST_METHOD(DuplicateMark_WarningClearsAfterRename)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record1 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A1"));
			auto record2 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A1"));
			group->GetBarRecords().Add(record1);
			group->GetBarRecords().Add(record2);
			Assert::IsTrue(StatusType::stWarning == record1->GetStatus());

			record2->SetMark(_T("A2"));

			Assert::IsTrue(StatusType::stOK == record1->GetStatus());
			Assert::IsTrue(StatusType::stOK == record2->GetStatus());
		}

		TEST_METHOD(DuplicateMarkWarning_DoesNotZeroOutMass)
		{
			// Only stError zeroes CBarRecord::GetMass() -- stWarning (a reused mark) must
			// not affect the quantity calculation.
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record1 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A1"));
			auto record2 = MakeBarRecord(MaterialType::A706_Grade60, _T("#4"), _T("A1"));
			group->GetBarRecords().Add(record1);
			group->GetBarRecords().Add(record2);

			Assert::IsTrue(StatusType::stWarning == record1->GetStatus());
			Assert::AreNotEqual(0.0, record1->GetMass(), 0.0001);
		}

		// ---- #18 bar, Grade 75+ ----

		TEST_METHOD(Size18Bar_Grade75AndHigher_SetsWarning)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			// A706_Grade80 is in the "grade 75 and higher" set (BarRecord.cpp) and shares
			// the plain-steel bar-size table (which defines "#18") via CBarInfoMgr::GetBars.
			auto record = MakeBarRecord(MaterialType::A706_Grade80, _T("#18"), _T("A1"));
			group->GetBarRecords().Add(record);

			Assert::IsTrue(StatusType::stWarning == record->GetStatus());
		}

		TEST_METHOD(OtherSizeBar_Grade75AndHigher_NoWarning)
		{
			// Same grade as above, but not a #18 bar -- the warning is specific to #18.
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record = MakeBarRecord(MaterialType::A706_Grade80, _T("#4"), _T("A1"));
			group->GetBarRecords().Add(record);

			Assert::IsTrue(StatusType::stOK == record->GetStatus());
		}

		TEST_METHOD(Size18Bar_BelowGrade75_NoWarning)
		{
			// Same #18 size as the warning case, but A706_Grade60 is below the Grade 75
			// threshold -- the warning is specific to grade, not just size.
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record = MakeBarRecord(MaterialType::A706_Grade60, _T("#18"), _T("A1"));
			group->GetBarRecords().Add(record);

			Assert::IsTrue(StatusType::stOK == record->GetStatus());
		}
	};
}
