#include "pch.h"
#include "CppUnitTest.h"
#include <atlbase.h>
#include <vector>

#include <initguid.h>
#include "..\Bars\Bars.h"
#include "..\Bars\Bars_i.c"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace BaristUnitTests
{
	TEST_CLASS(BarsUnitTests)
	{
	public:
		std::vector<MaterialType> materials{ A706_Grade60, A706_Grade80,A1035_Grade100, A1035_Grade120,A767_A1094_Grade60, A767_A1094_Grade80, A767_A1094_Grade100,A955_Grade60, A955_Grade75, A955_Grade80 };
		std::vector<CComBSTR> sizes{ _T("#3"),_T("#4") ,_T("#5") ,_T("#6") ,_T("#7") ,_T("#8") ,_T("#9") ,_T("#10") ,_T("#11") ,_T("#14") ,_T("#18") };
		inline static CComPtr<IBarInfoMgr> bar_information_mgr = nullptr;

		TEST_CLASS_INITIALIZE(BarsUnitTests_Initialize)
		{
			HRESULT hr = bar_information_mgr.CoCreateInstance(CLSID_BarInfoMgr);
			Assert::IsTrue(SUCCEEDED(hr));
		}

		TEST_CLASS_CLEANUP(BarsUnitTests_Cleanup)
		{
			bar_information_mgr.Release();
		}

		void Test(IBend* bend, const std::vector<Float64>& expected_values)
		{
			Assert::AreEqual(sizes.size() * materials.size(), expected_values.size());

			int i = 0;
			for (auto& size : sizes)
			{
				for (auto& material : materials)
				{
					CComPtr<IBarCollection> bars;
					bar_information_mgr->get_Bars(material, &bars);
					CComPtr<IBarData> barData;
					bars->get_Item(CComVariant(size), &barData);

					CComPtr<IBarRecord> bar_record;
					bar_record.CoCreateInstance(CLSID_BarRecord);
					bar_record->put_BarData(barData);
					bar_record->put_Material(material);
					bar_record->put_NumReqd(1);
					bar_record->put_PrimaryBend(bend);

					Float64 l;
					bend->get_Length(&l);
					Float64 expected = expected_values[i++];
					Assert::AreEqual(expected, l, 0.0001);

					// writes the computed length to the TextExplorer window
					// this makes it easier to get the expected values
					//std::_tostringstream os;
					//os << l << _T(", ");
					//Logger::WriteMessage(os.str().c_str());
				}
			}
		}

		TEST_METHOD(BendTest_Type50)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type50);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values
			{
				10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25
			};

			Test(bend, expected_values);
		}

		TEST_METHOD(BendTest_Type74)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type74);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(6.0 + 8.0 / 12.0);
			bend->put_W(2.0);
			bend->put_X(2.0);


			std::vector<Float64> expected_values
			{
				10.619, 10.619, 10.619, 10.619, 10.619, 10.619, 10.619, 10.619, 10.619, 10.619, 10.6031, 10.6031, 10.6031, 10.6031, 10.6031, 10.6031, 10.6031, 10.6031, 10.6031, 10.6031, 10.5872, 10.5872, 10.5872, 10.5872, 10.5872, 10.5872, 10.5872, 10.5872, 10.5872, 10.5872, 10.5713, 10.5713, 10.5713, 10.5713, 10.5713, 10.5713, 10.5713, 10.5713, 10.5713, 10.5713, 10.5554, 10.5554, 10.5554, 10.5554, 10.5554, 10.5554, 10.5554, 10.5554, 10.5554, 10.5554, 10.5396, 10.5396, 10.5396, 10.5396, 10.5396, 10.5396, 10.5396, 10.5396, 10.5396, 10.5396, 10.4987, 10.4987, 10.4987, 10.4987, 10.4987, 10.4987, 10.4987, 10.4987, 10.4987, 10.4987, 10.4775, 10.4775, 10.4775, 10.4775, 10.4775, 10.4775, 10.4775, 10.4775, 10.4775, 10.4775, 10.4567, 10.4567, 10.4567, 10.4567, 10.4567, 10.4567, 10.4567, 10.4567, 10.4567, 10.4567, 10.3776, 10.3776, 10.3776, 10.3776, 10.3776, 10.3776, 10.3776, 10.3776, 10.3776, 10.3776, 10.2814, 10.2814, 10.2814, 10.2814, 10.2814, 10.2814, 10.2814, 10.2814, 10.2814, 10.2814
			};

			Test(bend, expected_values);
		}
	};
}
