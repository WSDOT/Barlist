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

// Bars objects form an event hierarchy: CBend changes propagate up through
// CBarRecord, CBarRecordCollection, CGroup, CGroupCollection, and finally
// CBarlist, each level re-firing the event it received (plus its own
// directly-fired events, e.g. CGroupCollection::OnGroupAdded). These tests
// cover both: that each event fires at its point of origin, and that it
// propagates correctly to every level above it.
namespace BarlistUnitTests
{
	TEST_CLASS(BarlistEvents)
	{
	public:
		inline static std::unique_ptr<CBarInfoMgr> bar_information_mgr = nullptr;

		TEST_CLASS_INITIALIZE(BarlistEvents_Initialize)
		{
			bar_information_mgr = std::make_unique<CBarInfoMgr>();
		}

		TEST_CLASS_CLEANUP(BarlistEvents_Cleanup)
		{
			bar_information_mgr.reset();
		}

		// Builds a bar record that is safe to insert into a live CGroup's bar
		// record collection. CGroup::UpdateStatus() runs automatically every
		// time a bar record is added/changed/removed (connected in CGroup's
		// constructor) and calls CBarRecord::GetStatus(), which unconditionally
		// dereferences the primary bend and bar data -- both must already be
		// set before the record is ever added to a group.
		std::shared_ptr<CBarRecord> MakeBarRecord(const std::_tstring& mark = _T("A1"))
		{
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));

			auto bend = std::make_shared<CType50>();
			bend->SetU(10.25);

			auto record = std::make_shared<CBarRecord>();
			record->SetBarData(barData);
			record->SetMaterial(MaterialType::A706_Grade60);
			record->SetNumReqd(1);
			record->SetPrimaryBend(bend);
			record->SetMark(mark);
			return record;
		}

		// ---- CBend ----

		TEST_METHOD(Bend_SetDimension_FiresOnBendChanged)
		{
			auto bend = std::make_shared<CType50>();

			int callCount = 0;
			bend->OnBendChanged.Connect([&callCount] { callCount++; });

			bend->SetU(10.25);

			Assert::AreEqual(1, callCount);
		}

		// ---- CBarRecord ----

		TEST_METHOD(BarRecord_Setters_FireOnBarRecordChanged)
		{
			auto record = std::make_shared<CBarRecord>();

			int callCount = 0;
			record->OnBarRecordChanged.Connect([&callCount](CBarRecord&) { callCount++; });

			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));

			record->SetMaterial(MaterialType::A706_Grade60);
			Assert::AreEqual(1, callCount);

			record->SetMark(_T("A1"));
			Assert::AreEqual(2, callCount);

			record->SetLocation(_T("Deck"));
			Assert::AreEqual(3, callCount);

			record->SetNumReqd(4);
			Assert::AreEqual(4, callCount);

			record->SetUse(UseType::utTransverse);
			Assert::AreEqual(5, callCount);

			record->SetSubstructure(true);
			Assert::AreEqual(6, callCount);

			record->SetEpoxy(true);
			Assert::AreEqual(7, callCount);

			record->SetNumEach(2);
			Assert::AreEqual(8, callCount);

			record->SetBarData(barData);
			Assert::AreEqual(9, callCount);
		}

		TEST_METHOD(BarRecord_SetPrimaryBend_FiresOnBarRecordChanged)
		{
			auto record = std::make_shared<CBarRecord>();

			int callCount = 0;
			record->OnBarRecordChanged.Connect([&callCount](CBarRecord&) { callCount++; });

			record->SetPrimaryBend(std::make_shared<CType50>());

			Assert::AreEqual(1, callCount);
		}

		TEST_METHOD(BarRecord_SetVariesBend_FiresOnBarRecordChanged)
		{
			auto record = std::make_shared<CBarRecord>();
			record->SetPrimaryBend(std::make_shared<CType50>());

			int callCount = 0;
			record->OnBarRecordChanged.Connect([&callCount](CBarRecord&) { callCount++; });

			record->SetVariesBend(std::make_shared<CType50>());

			Assert::AreEqual(1, callCount);
		}

		TEST_METHOD(BarRecord_PrimaryBendChange_PropagatesToOnBarRecordChanged)
		{
			auto record = std::make_shared<CBarRecord>();
			auto bend = std::make_shared<CType50>();
			record->SetPrimaryBend(bend);

			int callCount = 0;
			CBarRecord* firedWith = nullptr;
			record->OnBarRecordChanged.Connect([&](CBarRecord& r) { callCount++; firedWith = &r; });

			bend->SetU(12.0); // change the bend directly, not through the record

			Assert::AreEqual(1, callCount);
			Assert::IsTrue(firedWith == record.get());
		}

		TEST_METHOD(BarRecord_VariesBendChange_PropagatesToOnBarRecordChanged)
		{
			auto record = std::make_shared<CBarRecord>();
			record->SetPrimaryBend(std::make_shared<CType50>());
			auto variesBend = std::make_shared<CType50>();
			record->SetVariesBend(variesBend);

			int callCount = 0;
			record->OnBarRecordChanged.Connect([&callCount](CBarRecord&) { callCount++; });

			variesBend->SetU(12.0);

			Assert::AreEqual(1, callCount);
		}

		// ---- CBarRecordCollection ----

		TEST_METHOD(BarRecordCollection_Replace_FiresOnBarRecordChanged)
		{
			CBarRecordCollection bars;
			bars.Add(MakeBarRecord(_T("A1")));

			int callCount = 0;
			CBarRecord* firedWith = nullptr;
			bars.OnBarRecordChanged.Connect([&](CBarRecord& r) { callCount++; firedWith = &r; });

			auto replacement = MakeBarRecord(_T("A2"));
			bars.Replace(0, replacement);

			Assert::AreEqual(1, callCount);
			Assert::IsTrue(firedWith == replacement.get());
		}

		// ---- Bar record events propagating through Group / GroupCollection / Barlist ----

		TEST_METHOD(AddBarRecord_PropagatesThroughGroupGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			CBarRecordCollection& bars = group->GetBarRecords();

			int collectionCount = 0, groupCount = 0, groupCollectionCount = 0, barlistCount = 0;
			bars.OnBarRecordAdded.Connect([&](CBarRecord&) { collectionCount++; });
			group->OnBarRecordAdded.Connect([&](CGroup&, CBarRecord&) { groupCount++; });
			barlist.GetGroups().OnBarRecordAdded.Connect([&](CGroup&, CBarRecord&) { groupCollectionCount++; });
			barlist.OnBarRecordAdded.Connect([&](CGroup&, CBarRecord&) { barlistCount++; });

			bars.Add(MakeBarRecord(_T("A1")));

			Assert::AreEqual(1, collectionCount);
			Assert::AreEqual(1, groupCount);
			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
		}

		TEST_METHOD(ChangeBarRecord_PropagatesThroughGroupGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			CBarRecordCollection& bars = group->GetBarRecords();
			auto record = MakeBarRecord(_T("A1"));
			bars.Add(record);

			int collectionCount = 0, groupCount = 0, groupCollectionCount = 0, barlistCount = 0;
			bars.OnBarRecordChanged.Connect([&](CBarRecord&) { collectionCount++; });
			group->OnBarRecordChanged.Connect([&](CGroup&, CBarRecord&) { groupCount++; });
			barlist.GetGroups().OnBarRecordChanged.Connect([&](CGroup&, CBarRecord&) { groupCollectionCount++; });
			barlist.OnBarRecordChanged.Connect([&](CGroup&, CBarRecord&) { barlistCount++; });

			record->SetLocation(_T("Deck")); // change made directly on the record, not through the collection

			Assert::AreEqual(1, collectionCount);
			Assert::AreEqual(1, groupCount);
			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
		}

		TEST_METHOD(RemoveBarRecord_PropagatesThroughGroupGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			CBarRecordCollection& bars = group->GetBarRecords();
			bars.Add(MakeBarRecord(_T("A1")));

			int collectionCount = 0, groupCount = 0, groupCollectionCount = 0, barlistCount = 0;
			std::_tstring capturedMark;
			bars.OnBarRecordRemoved.Connect([&](const std::_tstring& mark) { collectionCount++; capturedMark = mark; });
			group->OnBarRecordRemoved.Connect([&](CGroup&, const std::_tstring&) { groupCount++; });
			barlist.GetGroups().OnBarRecordRemoved.Connect([&](CGroup&, const std::_tstring&) { groupCollectionCount++; });
			barlist.OnBarRecordRemoved.Connect([&](CGroup&, const std::_tstring&) { barlistCount++; });

			bars.Remove(0);

			Assert::AreEqual(1, collectionCount);
			Assert::AreEqual(1, groupCount);
			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
			Assert::AreEqual(std::_tstring(_T("A1")), capturedMark);
		}

		TEST_METHOD(SortBarRecords_PropagatesThroughGroupGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			CBarRecordCollection& bars = group->GetBarRecords();
			bars.Add(MakeBarRecord(_T("B2")));
			bars.Add(MakeBarRecord(_T("A1")));

			int collectionCount = 0, groupCount = 0, groupCollectionCount = 0, barlistCount = 0;
			bars.OnBarRecordsSorted.Connect([&] { collectionCount++; });
			group->OnBarRecordsSorted.Connect([&](CGroup&) { groupCount++; });
			barlist.GetGroups().OnBarRecordsSorted.Connect([&](CGroup&) { groupCollectionCount++; });
			barlist.OnBarRecordsSorted.Connect([&](CGroup&) { barlistCount++; });

			bars.Sort();

			Assert::AreEqual(1, collectionCount);
			Assert::AreEqual(1, groupCount);
			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
		}

		TEST_METHOD(MoveBarRecord_PropagatesThroughGroupGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));
			CBarRecordCollection& bars = group->GetBarRecords();
			bars.Add(MakeBarRecord(_T("A1")));
			bars.Add(MakeBarRecord(_T("A2")));

			int collectionCount = 0, groupCount = 0, groupCollectionCount = 0, barlistCount = 0;
			bars.OnBarRecordMoved.Connect([&](CBarRecord&, long, long) { collectionCount++; });
			group->OnBarRecordMoved.Connect([&](CGroup&, CBarRecord&, long, long) { groupCount++; });
			barlist.GetGroups().OnBarRecordMoved.Connect([&](CGroup&, CBarRecord&, long, long) { groupCollectionCount++; });
			barlist.OnBarRecordMoved.Connect([&](CGroup&, CBarRecord&, long, long) { barlistCount++; });

			bars.MoveDown(0); // moves the first record past the second

			Assert::AreEqual(1, collectionCount);
			Assert::AreEqual(1, groupCount);
			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
		}

		// ---- Group events propagating through GroupCollection / Barlist ----

		TEST_METHOD(AddGroup_PropagatesThroughGroupCollectionAndBarlist)
		{
			CBarlist barlist;

			int groupCollectionCount = 0, barlistCount = 0;
			barlist.GetGroups().OnGroupAdded.Connect([&](CGroup&) { groupCollectionCount++; });
			barlist.OnGroupAdded.Connect([&](CGroup&) { barlistCount++; });

			barlist.GetGroups().Add(_T("Group A"));

			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
		}

		TEST_METHOD(RenameGroup_PropagatesThroughGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			auto group = barlist.GetGroups().Add(_T("Group A"));

			int groupCollectionCount = 0, barlistCount = 0;
			barlist.GetGroups().OnGroupChanged.Connect([&](CGroup&) { groupCollectionCount++; });
			barlist.OnGroupChanged.Connect([&](CGroup&) { barlistCount++; });

			group->SetName(_T("Group B"));

			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
		}

		TEST_METHOD(RemoveGroup_PropagatesThroughGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			barlist.GetGroups().Add(_T("Group A"));

			int groupCollectionCount = 0, barlistCount = 0;
			std::_tstring capturedName;
			barlist.GetGroups().OnGroupRemoved.Connect([&](const std::_tstring& name) { groupCollectionCount++; capturedName = name; });
			barlist.OnGroupRemoved.Connect([&](const std::_tstring&) { barlistCount++; });

			barlist.GetGroups().Remove(0);

			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
			Assert::AreEqual(std::_tstring(_T("Group A")), capturedName);
		}

		TEST_METHOD(MoveGroup_PropagatesThroughGroupCollectionAndBarlist)
		{
			CBarlist barlist;
			barlist.GetGroups().Add(_T("Group A"));
			barlist.GetGroups().Add(_T("Group B"));

			int groupCollectionCount = 0, barlistCount = 0;
			barlist.GetGroups().OnGroupMoved.Connect([&](CGroup&, long, long) { groupCollectionCount++; });
			barlist.OnGroupMoved.Connect([&](CGroup&, long, long) { barlistCount++; });

			barlist.GetGroups().MoveDown(0); // moves the first group past the second

			Assert::AreEqual(1, groupCollectionCount);
			Assert::AreEqual(1, barlistCount);
		}

		// ---- CBarlist::OnNotIncludedQuantitiesChanged ----

		TEST_METHOD(Barlist_OnNotIncludedQuantitiesChanged_CanBeFiredManually)
		{
			// Nothing in Bars.dll fires this signal today (see the comment on
			// its declaration in Barlist.h) -- it exists only for interface
			// parity with the original IBarlistEvents. This confirms the
			// Signal itself still connects and fires correctly, so it's ready
			// if a future caller invokes it.
			CBarlist barlist;

			int callCount = 0;
			barlist.OnNotIncludedQuantitiesChanged.Connect([&callCount] { callCount++; });

			barlist.OnNotIncludedQuantitiesChanged();

			Assert::AreEqual(1, callCount);
		}
	};
}
