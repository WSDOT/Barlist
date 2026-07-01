#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include <string>
#include <tchar.h>

#include "..\Bars\Barlist.h"
#include "..\Bars\Group.h"
#include "..\Bars\BarRecord.h"
#include "..\Bars\BendImpl.h"
#include "..\Bars\BarInfoMgr.h"
#include "..\Bars\BarCollection.h"
#include "..\Bars\BarData.h"
#include "..\Bars\Type50.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// CGroup accumulates quantities per MaterialType/substructure-or-superstructure/epoxy-or-not
// every time its bar records change (CGroup::UpdateStatus), and CBarlist aggregates each
// group's quantities into its own per-group totals plus a grand total across all groups
// (CBarlist::Update). These tests cover both layers: that a group buckets quantities
// correctly (including the GFRP/D7957 special case, where the quantity is length rather
// than mass), and that CBarlist's per-group and grand totals stay correct as groups and
// bar records are added, changed, and removed.
namespace BarlistUnitTests
{
	TEST_CLASS(Quantities)
	{
	public:
		inline static std::unique_ptr<CBarInfoMgr> bar_information_mgr = nullptr;

		TEST_CLASS_INITIALIZE(Quantities_Initialize)
		{
			bar_information_mgr = std::make_unique<CBarInfoMgr>();
		}

		TEST_CLASS_CLEANUP(Quantities_Cleanup)
		{
			bar_information_mgr.reset();
		}

		// Builds a bar record with a straight (CType50) primary bend of the given length,
		// safe to insert into a live CGroup's bar record collection (see BarlistEvents.cpp's
		// MakeBarRecord for why bar data and the primary bend must be set before insertion).
		std::shared_ptr<CBarRecord> MakeBarRecord(MaterialType material, const std::_tstring& mark, Float64 length,
			long numReqd = 1, bool bSubstructure = false, bool bEpoxy = false)
		{
			const CBarCollection& barSizes = bar_information_mgr->GetBars(material);
			const CBarData* barData = barSizes.Find(_T("#4"));

			auto bend = std::make_shared<CType50>();
			bend->SetU(length);

			auto record = std::make_shared<CBarRecord>();
			record->SetBarData(barData);
			record->SetMaterial(material);
			record->SetNumReqd(numReqd);
			record->SetPrimaryBend(bend);
			record->SetMark(mark);
			record->SetSubstructure(bSubstructure);
			record->SetEpoxy(bEpoxy);
			return record;
		}

		// The mass CGroup/CBarlist should compute for a non-GFRP bar record: numReqd * length
		// * the material's unit mass, i.e. the same inputs CBarRecord::GetMass() uses -- this
		// avoids hard-coding a unit-weight table value that could change independently of the
		// subtotal/aggregation logic under test.
		Float64 ExpectedMass(MaterialType material, Float64 length, long numReqd = 1)
		{
			const CBarCollection& barSizes = bar_information_mgr->GetBars(material);
			const CBarData* barData = barSizes.Find(_T("#4"));
			return numReqd * length * barData->GetMass();
		}

		// ---- CGroup::GetQuantity ----

		TEST_METHOD(Group_Quantity_SingleBarAccumulatesInCorrectBucket)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 3, /*sub*/false, /*epoxy*/false));

			Float64 expected = ExpectedMass(MaterialType::A706_Grade60, 10.0, 3);
			Assert::AreEqual(expected, group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);

			// every other bucket for this material must be untouched
			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade60, true, false), 0.0001);
			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade60, false, true), 0.0001);
			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade60, true, true), 0.0001);

			// a different material's buckets must be untouched
			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade80, false, false), 0.0001);
		}

		TEST_METHOD(Group_Quantity_SumsMultipleBarsInSameBucket)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 2));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A2"), 6.0, 5));

			Float64 expected = ExpectedMass(MaterialType::A706_Grade60, 10.0, 2) + ExpectedMass(MaterialType::A706_Grade60, 6.0, 5);
			Assert::AreEqual(expected, group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
		}

		TEST_METHOD(Group_Quantity_SeparatesSubstructureFromSuperstructure)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 1, /*sub*/false));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A2"), 6.0, 1, /*sub*/true));

			Assert::AreEqual(ExpectedMass(MaterialType::A706_Grade60, 10.0), group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
			Assert::AreEqual(ExpectedMass(MaterialType::A706_Grade60, 6.0), group->GetQuantity(MaterialType::A706_Grade60, false, true), 0.0001);
		}

		TEST_METHOD(Group_Quantity_SeparatesEpoxyFromUncoated)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 1, false, /*epoxy*/false));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A2"), 6.0, 1, false, /*epoxy*/true));

			Assert::AreEqual(ExpectedMass(MaterialType::A706_Grade60, 10.0), group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
			Assert::AreEqual(ExpectedMass(MaterialType::A706_Grade60, 6.0), group->GetQuantity(MaterialType::A706_Grade60, true, false), 0.0001);
		}

		TEST_METHOD(Group_Quantity_GFRPIsLengthBasedNotMassBased)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::D7957, _T("A1"), 12.0, 4));

			// GFRP's unit "mass" is 0 in the bar tables (quantities are by length for GFRP),
			// so a mass-based calculation would wrongly yield 0; the correct quantity is
			// numReqd * length.
			Assert::AreEqual(0.0, ExpectedMass(MaterialType::D7957, 12.0, 4), 0.0001);
			Assert::AreEqual(48.0, group->GetQuantity(MaterialType::D7957, false, false), 0.0001);
		}

		TEST_METHOD(Group_Quantity_NonGFRPZeroesOnErrorStatus)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record = MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 3);
			record->SetVariesBend(std::make_shared<CType50>());
			record->SetNumEach(2); // 3 is not evenly divisible by 2 -> stError (see CBarRecord::UpdateStatus)
			group->GetBarRecords().Add(record);

			Assert::IsTrue(StatusType::stError == record->GetStatus());
			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
		}

		// CGroup::UpdateStatus computes the GFRP/D7957 quantity inline from bend length and
		// never checks the bar record's status, unlike the mass path (CBarRecord::GetMass),
		// which zeroes out on stError. This test documents that real asymmetry rather than
		// assuming it -- if GFRP's error handling is ever aligned with the mass path, this
		// test's expected value should change from the full quantity to 0.
		TEST_METHOD(Group_Quantity_GFRPDoesNotZeroOnErrorStatus)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record = MakeBarRecord(MaterialType::D7957, _T("A1"), 10.0, 3);
			auto variesBend = std::make_shared<CType50>();
			variesBend->SetU(10.0); // same length as the primary bend, so the averaged length is also 10.0
			record->SetVariesBend(variesBend);
			record->SetNumEach(2); // unbalanced -> stError
			group->GetBarRecords().Add(record);

			Assert::IsTrue(StatusType::stError == record->GetStatus());
			Assert::AreEqual(30.0, group->GetQuantity(MaterialType::D7957, false, false), 0.0001);
		}

		// Bars.dll itself does not restrict which material types may be marked epoxy-coated --
		// that restriction (CanBeEpoxyCoated) lives only in Barlist.exe's UI layer (Helpers.h).
		TEST_METHOD(Group_Quantity_DoesNotValidateEpoxyAgainstMaterialType)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A1035_Grade100, _T("A1"), 10.0, 1, false, /*epoxy*/true));

			Assert::AreEqual(ExpectedMass(MaterialType::A1035_Grade100, 10.0), group->GetQuantity(MaterialType::A1035_Grade100, true, false), 0.0001);
		}

		TEST_METHOD(Group_Quantity_UpdatesAfterBarRecordRemoved)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			group->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0));

			Assert::AreEqual(ExpectedMass(MaterialType::A706_Grade60, 10.0), group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);

			group->GetBarRecords().Remove(0);

			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
		}

		TEST_METHOD(Group_Quantity_MovesBetweenBucketsWhenLocationChanges)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			auto record = MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 1, /*sub*/false);
			group->GetBarRecords().Add(record);

			Float64 expected = ExpectedMass(MaterialType::A706_Grade60, 10.0);
			Assert::AreEqual(expected, group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade60, false, true), 0.0001);

			record->SetSubstructure(true);

			Assert::AreEqual(0.0, group->GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
			Assert::AreEqual(expected, group->GetQuantity(MaterialType::A706_Grade60, false, true), 0.0001);
		}

		// ---- CBarlist::GetQuantity / GetQuantityByGroup (per-group totals and grand total) ----

		TEST_METHOD(Barlist_GrandTotal_IsZeroWithNoGroups)
		{
			CBarlist barlist;
			Assert::AreEqual(0.0, barlist.GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
			Assert::AreEqual(0.0, barlist.GetQuantity(MaterialType::D7957, true, true), 0.0001);
		}

		TEST_METHOD(Barlist_GrandTotal_SumsQuantitiesAcrossGroups)
		{
			CBarlist barlist;
			auto groupA = barlist.GetGroups().Add(_T("Group A"));
			auto groupB = barlist.GetGroups().Add(_T("Group B"));
			groupA->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 2));
			groupB->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("B1"), 6.0, 5));

			Float64 expected = ExpectedMass(MaterialType::A706_Grade60, 10.0, 2) + ExpectedMass(MaterialType::A706_Grade60, 6.0, 5);
			Assert::AreEqual(expected, barlist.GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);
		}

		TEST_METHOD(Barlist_GetQuantityByGroup_MatchesEachGroupIndividually)
		{
			CBarlist barlist;
			auto groupA = barlist.GetGroups().Add(_T("Group A"));
			auto groupB = barlist.GetGroups().Add(_T("Group B"));
			groupA->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0, 2));
			groupB->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("B1"), 6.0, 5));

			Assert::AreEqual(groupA->GetQuantity(MaterialType::A706_Grade60, false, false),
				barlist.GetQuantityByGroup(_T("Group A"), MaterialType::A706_Grade60, false, false), 0.0001);
			Assert::AreEqual(groupB->GetQuantity(MaterialType::A706_Grade60, false, false),
				barlist.GetQuantityByGroup(_T("Group B"), MaterialType::A706_Grade60, false, false), 0.0001);
		}

		TEST_METHOD(Barlist_GetQuantityByGroup_ThrowsForUnknownGroupName)
		{
			CBarlist barlist;
			barlist.GetGroups().Add(_T("Group A"));

			bool bThrew = false;
			try
			{
				barlist.GetQuantityByGroup(_T("No Such Group"), MaterialType::A706_Grade60, false, false);
			}
			catch (const std::out_of_range&)
			{
				bThrew = true;
			}
			Assert::IsTrue(bThrew);
		}

		TEST_METHOD(Barlist_GrandTotal_UpdatesAfterGroupRemoved)
		{
			CBarlist barlist;
			auto groupA = barlist.GetGroups().Add(_T("Group A"));
			auto groupB = barlist.GetGroups().Add(_T("Group B"));
			groupA->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("A1"), 10.0));
			groupB->GetBarRecords().Add(MakeBarRecord(MaterialType::A706_Grade60, _T("B1"), 6.0));

			barlist.GetGroups().Remove(0); // removes Group A

			Assert::AreEqual(ExpectedMass(MaterialType::A706_Grade60, 6.0), barlist.GetQuantity(MaterialType::A706_Grade60, false, false), 0.0001);

			bool bThrew = false;
			try
			{
				barlist.GetQuantityByGroup(_T("Group A"), MaterialType::A706_Grade60, false, false);
			}
			catch (const std::out_of_range&)
			{
				bThrew = true;
			}
			Assert::IsTrue(bThrew);
		}
	};
}
