#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include <algorithm>
#include <tchar.h>
#include <MathEx.h>

#include "..\Bars\FabricationConstraints.h"
#include "..\Bars\BarData.h"
#include "..\Bars\BarException.h"
#include "..\Bars\BarInfoMgr.h"
#include "..\Bars\BarCollection.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// CFabricationConstraints is the bend/hook geometry math shared by every one of the 44
// bend-type classes (Type50.cpp...Type99.cpp) -- it was previously only exercised
// indirectly through their end-to-end ComputeLength() results. These tests check the
// formulas directly, against real bar-size data (via CBarInfoMgr) where possible, and
// against minimal hand-built CBarData objects where a specific, otherwise-unused branch
// (e.g. BendMeasureType::InsideDiameter, or a use/hook combination with no data) needs to
// be exercised deliberately.
namespace BarlistUnitTests
{
	TEST_CLASS(FabricationConstraints)
	{
	public:
		inline static std::unique_ptr<CBarInfoMgr> bar_information_mgr = nullptr;

		TEST_CLASS_INITIALIZE(FabricationConstraints_Initialize)
		{
			bar_information_mgr = std::make_unique<CBarInfoMgr>();
		}

		TEST_CLASS_CLEANUP(FabricationConstraints_Cleanup)
		{
			bar_information_mgr.reset();
		}

		// ---- GetBendDeduction(OR, angle) -- the raw two-argument overload ----

		TEST_METHOD(GetBendDeduction_RightAngle_EqualsRadius)
		{
			// tan(90 deg / 2) == tan(45 deg) == 1, so a right-angle deduction always
			// equals the outside radius exactly.
			Assert::AreEqual(10.0, CFabricationConstraints::GetBendDeduction(10.0, PI_OVER_2), 0.0001);
		}

		TEST_METHOD(GetBendDeduction_180Degrees_AvoidsTangentSingularity)
		{
			// tan(180 deg / 2) == tan(90 deg) is undefined, so 180-degree bends are
			// special-cased to deduct exactly the outside radius instead of computing
			// OR * tan(angle / 2).
			Assert::AreEqual(10.0, CFabricationConstraints::GetBendDeduction(10.0, M_PI), 0.0001);
		}

		// ---- Bend/hook deduction and radius, against real bar data ----

		TEST_METHOD(GetBendDeduction_ForBarUse_AlwaysMatchesOutsideBendRadius)
		{
			// This overload always deducts at a 90-degree angle regardless of `use`
			// (bends -- as opposed to hooks -- are always modeled this way), and since
			// tan(45 deg) == 1, the deduction always equals the outside bend radius.
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));

			for (UseType use : { UseType::utLongitudinal, UseType::utTransverse, UseType::utSeismic })
			{
				Float64 expected = CFabricationConstraints::GetOutsideBendRadius(*barData, use);
				Assert::AreEqual(expected, CFabricationConstraints::GetBendDeduction(*barData, use), 0.0001);
			}
		}

		TEST_METHOD(GetHookDeduction_AllHookTypes_MatchOutsideBendRadius)
		{
			// ht90 and ht135 both deduct at a 90-degree angle (tan(45 deg) == 1) and
			// ht180 hits the same 180-degree special case as GetBendDeduction above --
			// so all three hook types currently produce the same deduction.
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));

			Float64 expected = CFabricationConstraints::GetOutsideBendRadius(*barData, UseType::utLongitudinal);
			Assert::AreEqual(expected, CFabricationConstraints::GetHookDeduction(*barData, UseType::utLongitudinal, HookType::ht90), 0.0001);
			Assert::AreEqual(expected, CFabricationConstraints::GetHookDeduction(*barData, UseType::utLongitudinal, HookType::ht180), 0.0001);

			Float64 expectedTransverse = CFabricationConstraints::GetOutsideBendRadius(*barData, UseType::utTransverse);
			Assert::AreEqual(expectedTransverse, CFabricationConstraints::GetHookDeduction(*barData, UseType::utTransverse, HookType::ht135), 0.0001);
		}

		TEST_METHOD(GetHookRadius_DelegatesToCenterlineBendRadius)
		{
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));

			Assert::AreEqual(CFabricationConstraints::GetCenterlineBendRadius(*barData, UseType::utLongitudinal),
				CFabricationConstraints::GetHookRadius(*barData, UseType::utLongitudinal), 0.0001);
		}

		TEST_METHOD(GetBendDiametersAndRadii_AreConsistentWithEachOther)
		{
			// OD = ID + 2*db, CD = ID + db, and each radius is half its diameter --
			// verify the whole family stays internally consistent for real bar data.
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));
			Float64 db = barData->GetDiameter();

			Float64 id = CFabricationConstraints::GetInsideBendDiameter(*barData, UseType::utLongitudinal);
			Float64 od = CFabricationConstraints::GetOutsideBendDiameter(*barData, UseType::utLongitudinal);
			Float64 cd = CFabricationConstraints::GetCenterlineBendDiameter(*barData, UseType::utLongitudinal);

			Assert::AreEqual(id + 2 * db, od, 0.0001);
			Assert::AreEqual(id + db, cd, 0.0001);
			Assert::AreEqual(id / 2, CFabricationConstraints::GetInsideBendRadius(*barData, UseType::utLongitudinal), 0.0001);
			Assert::AreEqual(od / 2, CFabricationConstraints::GetOutsideBendRadius(*barData, UseType::utLongitudinal), 0.0001);
			Assert::AreEqual(cd / 2, CFabricationConstraints::GetCenterlineBendRadius(*barData, UseType::utLongitudinal), 0.0001);
		}

		// ---- GetTailLength ----

		TEST_METHOD(GetTailLength_UsesBarDiameterMultiplier_WhenAboveMinimum)
		{
			// #4/Longitudinal/ht90 has a min tail of 0.0, so the multiplier branch
			// (T * db) always wins.
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));
			const CHookData* hookData = barData->GetHookData(UseType::utLongitudinal, HookType::ht90);

			Float64 byMultiplier = hookData->GetTail() * barData->GetDiameter();
			Assert::IsTrue(byMultiplier > hookData->GetMinTail());
			Assert::AreEqual(byMultiplier, CFabricationConstraints::GetTailLength(*barData, UseType::utLongitudinal, HookType::ht90), 0.0001);
		}

		TEST_METHOD(GetTailLength_ClampsToMinimum_WhenMultiplierIsBelowIt)
		{
			// #4/Longitudinal/ht180 has a 2.5" minimum tail that exceeds 4 * db for a
			// #4 (0.5" diameter) bar, so the minimum-tail clamp branch wins.
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));
			const CHookData* hookData = barData->GetHookData(UseType::utLongitudinal, HookType::ht180);

			Float64 byMultiplier = hookData->GetTail() * barData->GetDiameter();
			Assert::IsTrue(byMultiplier < hookData->GetMinTail());
			Assert::AreEqual(hookData->GetMinTail(), CFabricationConstraints::GetTailLength(*barData, UseType::utLongitudinal, HookType::ht180), 0.0001);
		}

		TEST_METHOD(GetTailLength_ThrowsWhenHookNotDefinedForUse)
		{
			// #4/Longitudinal only defines ht90 and ht180 hooks -- ht135 is not defined
			// per LRFD/BDM for that use, so this must throw rather than dereference null.
			const CBarCollection& steelBars = bar_information_mgr->GetBars(MaterialType::A706_Grade60);
			const CBarData* barData = steelBars.Find(_T("#4"));
			Assert::IsNull(barData->GetHookData(UseType::utLongitudinal, HookType::ht135));

			bool bThrew = false;
			try
			{
				CFabricationConstraints::GetTailLength(*barData, UseType::utLongitudinal, HookType::ht135);
			}
			catch (const CBarException&)
			{
				bThrew = true;
			}
			Assert::IsTrue(bThrew);
		}

		// ---- GetInsideBendDiameter: both BendMeasureType branches, and the throw path ----

		TEST_METHOD(GetInsideBendDiameter_ScalesByBarDiameter_WhenMeasureIsBarDiameter)
		{
			CBarData barData;
			barData.SetBarData(_T("test"), /*db*/ 2.0, /*mass*/ 0.0, /*length*/ 0.0, /*maxLength*/ 0.0);
			barData.AddBendData(UseType::utLongitudinal, /*ID*/ 6.0, BendMeasureType::BarDiameter);

			Assert::AreEqual(12.0, CFabricationConstraints::GetInsideBendDiameter(barData, UseType::utLongitudinal), 0.0001);
		}

		TEST_METHOD(GetInsideBendDiameter_UsesAbsoluteValue_WhenMeasureIsInsideDiameter)
		{
			CBarData barData;
			barData.SetBarData(_T("test"), /*db*/ 2.0, /*mass*/ 0.0, /*length*/ 0.0, /*maxLength*/ 0.0);
			barData.AddBendData(UseType::utLongitudinal, /*ID*/ 6.0, BendMeasureType::InsideDiameter);

			Assert::AreEqual(6.0, CFabricationConstraints::GetInsideBendDiameter(barData, UseType::utLongitudinal), 0.0001);
		}

		TEST_METHOD(GetInsideBendDiameter_ThrowsWhenBendNotDefinedForUse)
		{
			CBarData barData;
			barData.SetBarData(_T("test"), 2.0, 0.0, 0.0, 0.0);
			barData.AddBendData(UseType::utLongitudinal, 6.0, BendMeasureType::BarDiameter);
			// Transverse was never registered.
			Assert::IsNull(barData.GetBendData(UseType::utTransverse));

			bool bThrew = false;
			try
			{
				CFabricationConstraints::GetInsideBendDiameter(barData, UseType::utTransverse);
			}
			catch (const CBarException&)
			{
				bThrew = true;
			}
			Assert::IsTrue(bThrew);
		}
	};
}
