#include "pch.h"
#include "CppUnitTest.h"
#include <atlbase.h>
#include <vector>

#include <initguid.h>
#include "..\Bars\Bars.h"
#include "..\Bars\Bars_i.c"

#include <MathEx.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//#define DUMP_EXPECTED_VALUES

namespace BaristUnitTests
{
	TEST_CLASS(BarsUnitTests)
	{
	public:
      std::vector<UseType> use_types{ utLongitudinal, utTransverse, utSeismic };
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

		void Test(IBend* bend, UseType use, const std::vector<Float64>& expected_values)
		{
			Assert::AreEqual(sizes.size(), expected_values.size());

#if defined DUMP_EXPECTED_VALUES
			std::_tostringstream os;
#endif
			for (auto& material : materials)
			{
				Test(bend, use, material, expected_values);
			}
		}


		void Test(IBend* bend, UseType use, MaterialType material, const std::vector<Float64>& expected_values)
		{
			Assert::AreEqual(sizes.size(), expected_values.size());

#if defined DUMP_EXPECTED_VALUES
			std::_tostringstream os;
#endif
			int i = 0;
			for (auto& size : sizes)
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
				bar_record->put_Use(use);

				Float64 l;
				bend->get_Length(&l);
				Float64 expected = expected_values[i++];
#if !defined DUMP_EXPECTED_VALUES
				Assert::AreEqual(expected, l, 0.001);
#endif

#if defined DUMP_EXPECTED_VALUES
				// writes the computed length to the TextExplorer window
				// this makes it easier to get the expected values
				if (size == sizes.front())
					os << l;
				else
					os << _T(", ") << l;
#endif
			}
#if defined DUMP_EXPECTED_VALUES
		os << std::endl;
		Logger::WriteMessage(os.str().c_str());
#endif
		}

		TEST_METHOD(BendTest_Type50)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type50);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values
			{
				10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25, 10.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type51)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type51);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values1
			{
				 10.3801, 10.4023, 10.4246, 10.4595, 10.4944, 10.5293, 10.6264, 10.6738, 10.7205, 10.907, 11.1259
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type52)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type52);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values1
			{
				 10.5103, 10.5547, 10.5991, 10.6689, 10.7388, 10.8086, 11.0028, 11.0976, 11.191, 11.564, 12.0018
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type53)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type53);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values
			{
				10.5103, 10.5547, 10.5991, 10.6689, 10.7388, 10.8086, 11.0028, 11.0976, 11.191, 11.564, 12.0018
			};

			Test(bend, utLongitudinal, expected_values);

			std::vector<Float64> expected_values2
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type54)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type54);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values1
			{
				10.3786, 10.4214, 10.4643, 10.5071, 10.55, 10.5928, 10.6531, 10.7038, 10.7539, 10.8795, 11.0892
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 10.316, 10.338, 10.36, 10.5071, 10.55, 10.5928, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type55)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type55);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values1
			{
				10.5071, 10.5928, 10.6786, 10.7643, 10.85, 10.9357, 11.0562, 11.1576, 11.2577, 11.5091, 11.9285
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 10.382, 10.4259, 10.4699, 10.7643, 10.85, 10.9357, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type56)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type56);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values1
			{
				 10.5071, 10.5928, 10.6786, 10.7643, 10.85, 10.9357, 11.0562, 11.1576, 11.2577, 11.5091, 11.9285
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 10.382, 10.4259, 10.4699, 10.7643, 10.85, 10.9357, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type57)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type57);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);

			std::vector<Float64> expected_values1
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 10.3347, 10.3629, 10.3911, 10.4452, 10.4777, 10.5103, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 10.3537, 10.3629, 10.3911, 10.4452, 10.4777, 10.5103, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type58)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type58);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);

			std::vector<Float64> expected_values1
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 10.4007, 10.4509, 10.5011, 10.7023, 10.7777, 10.8531, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type59)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type59);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);


			std::vector<Float64> expected_values1
			{
				10.5087, 10.5738, 10.6388, 10.7166, 10.7944, 10.8721, 11.0295, 11.1276, 11.2243, 11.5365, 11.9651
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}
		TEST_METHOD(BendTest_Type60)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type60);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(6.0 + 8.0 / 12.0);
			bend->put_W(2.0);
			bend->put_X(2.0);

			std::vector<Float64> expected_values1
			{
				21.2516, 21.2243, 21.1971, 21.1698, 21.1426, 21.1153, 21.0259, 20.9872, 20.949, 20.7796, 20.5952
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 21.272, 21.2516, 21.2311, 21.1698, 21.1426, 21.1153, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type61)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Y(0.75);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					12.0713, 12.0396, 12.0078, 11.976, 11.9442, 11.9124, 11.8307, 11.7884, 11.7467, 11.5886, 11.396
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 12.0877, 12.0614, 12.035, 11.976, 11.9442, 11.9124, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 12.0877, 12.0614, 12.035, 11.976, 11.9442, 11.9124, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);

				std::vector<Float64> expected_values1
				{
					6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);

				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
            bend->put_Y(0.75);

				std::vector<Float64> expected_values1
				{
					9.369, 9.35311, 9.33722, 9.32133, 9.30544, 9.28955, 9.24869, 9.22754, 9.2067, 9.12764, 9.03135
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Y(0.75);

				std::vector<Float64> expected_values1
				{
					11.3452, 11.3213, 11.2975, 11.2737, 11.2498, 11.226, 11.1647, 11.133, 11.1017, 10.9831, 10.8387
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					11.3452, 11.3213, 11.2975, 11.2737, 11.2498, 11.226, 11.1647, 11.133, 11.1017, 10.9831, 10.8387
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_X(2.0);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					9.369, 9.35311, 9.33722, 9.32133, 9.30544, 9.28955, 9.24869, 9.22754, 9.2067, 9.12764, 9.03135
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type61);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_X(2.0);

				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type62)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Y(0.75);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					12.0713, 12.0396, 12.0078, 11.976, 11.9442, 11.9124, 11.8307, 11.7884, 11.7467, 11.5886, 11.396
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 12.0877, 12.0614, 12.035, 11.976, 11.9442, 11.9124, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 12.0877, 12.0614, 12.035, 11.976, 11.9442, 11.9124, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);

				std::vector<Float64> expected_values1
				{
					6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);

				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_Y(0.75);

				std::vector<Float64> expected_values1
				{
					9.369, 9.35311, 9.33722, 9.32133, 9.30544, 9.28955, 9.24869, 9.22754, 9.2067, 9.12764, 9.03135
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Y(0.75);

				std::vector<Float64> expected_values1
				{
					11.3452, 11.3213, 11.2975, 11.2737, 11.2498, 11.226, 11.1647, 11.133, 11.1017, 10.9831, 10.8387
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					11.3452, 11.3213, 11.2975, 11.2737, 11.2498, 11.226, 11.1647, 11.133, 11.1017, 10.9831, 10.8387
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_X(2.0);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					9.369, 9.35311, 9.33722, 9.32133, 9.30544, 9.28955, 9.24869, 9.22754, 9.2067, 9.12764, 9.03135
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type62);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_X(2.0);

				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type63)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Y(0.75);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					12.0713, 12.0396, 12.0078, 11.976, 11.9442, 11.9124, 11.8307, 11.7884, 11.7467, 11.5886, 11.396
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 12.0877, 12.0614, 12.035, 11.976, 11.9442, 11.9124, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 12.0877, 12.0614, 12.035, 11.976, 11.9442, 11.9124, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);

				std::vector<Float64> expected_values1
				{
					6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 6.66667, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);

				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_Y(0.75);

				std::vector<Float64> expected_values1
				{
					9.369, 9.35311, 9.33722, 9.32133, 9.30544, 9.28955, 9.24869, 9.22754, 9.2067, 9.12764, 9.03135
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Y(0.75);

				std::vector<Float64> expected_values1
				{
					11.3452, 11.3213, 11.2975, 11.2737, 11.2498, 11.226, 11.1647, 11.133, 11.1017, 10.9831, 10.8387
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					11.3452, 11.3213, 11.2975, 11.2737, 11.2498, 11.226, 11.1647, 11.133, 11.1017, 10.9831, 10.8387
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 11.3574, 11.3377, 11.3179, 11.2737, 11.2498, 11.226, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_X(2.0);
				bend->put_Z(0.75);

				std::vector<Float64> expected_values1
				{
					9.369, 9.35311, 9.33722, 9.32133, 9.30544, 9.28955, 9.24869, 9.22754, 9.2067, 9.12764, 9.03135
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 9.37718, 9.36401, 9.35085, 9.32133, 9.30544, 9.28955, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type63);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_X(2.0);

				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type66)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type66);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(6.0 + 8.0 / 12.0);
			bend->put_W(2.0);

			std::vector<Float64> expected_values1
			{
				22.914, 22.9041, 22.8941, 22.8841, 22.8741, 22.8642, 22.8539, 22.8426, 22.8314, 22.8089, 22.7639
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type67)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type67);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(6.0 + 8.0 / 12.0);
			bend->put_W(20.0);
         bend->put_X(2.0);

			std::vector<Float64> expected_values1
			{
				210.094, 209.995, 209.896, 209.797, 209.697, 209.598, 209.496, 209.383, 209.272, 209.047, 208.599
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type68)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type68);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(0.75);
				bend->put_W(10.0);
				bend->put_X(6.0);
				bend->put_Y(4.0);

				std::vector<Float64> expected_values1
				{
					18.1455, 18.1358, 18.126, 18.1162, 18.1065, 18.0967, 18.0744, 18.0618, 18.0493, 18.0057, 17.9494
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 18.1496, 18.1412, 18.1328, 18.1162, 18.1065, 18.0967, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 18.1496, 18.1412, 18.1328, 18.1162, 18.1065, 18.0967, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type68);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(0.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
					10.7262, 10.7182, 10.7103, 10.7023, 10.6944, 10.6864, 10.666, 10.6554, 10.645, 10.6055, 10.5573
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 10.7303, 10.7237, 10.7171, 10.7023, 10.6944, 10.6864, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 10.7303, 10.7237, 10.7171, 10.7023, 10.6944, 10.6864, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type68);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(0.75);

				std::vector<Float64> expected_values1
				{
					0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type69)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type69);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(0.75);
				bend->put_W(10.0);
				bend->put_X(6.0);
				bend->put_Y(4.0);
				bend->put_Z(2.0);

				std::vector<Float64> expected_values1
				{
					18.3819, 18.3719, 18.3619, 18.3519, 18.3419, 18.3319, 18.3085, 18.2955, 18.2827, 18.2372, 18.1789
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 18.3862, 18.3777, 18.3691, 18.3519, 18.3419, 18.3319, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 18.3862, 18.3777, 18.3691, 18.3519, 18.3419, 18.3319, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type69);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(0.75);
				bend->put_W(10.0);
				bend->put_X(6.0);
				bend->put_Y(4.0);

				std::vector<Float64> expected_values1
				{
					16.4057, 16.4037, 16.4016, 16.3995, 16.3975, 16.3954, 16.3925, 16.3901, 16.3877, 16.3817, 16.3716
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 16.406, 16.404, 16.402, 16.3995, 16.3975, 16.3954, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 16.406, 16.404, 16.402, 16.3995, 16.3975, 16.3954, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type69);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(0.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					12.4088, 12.4078, 12.4068, 12.4057, 12.4047, 12.4037, 12.4022, 12.401, 12.3998, 12.3968, 12.3918
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 12.4089, 12.408, 12.407, 12.4057, 12.4047, 12.4037, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 12.4089, 12.408, 12.407, 12.4057, 12.4047, 12.4037, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type69);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(0.75);

				std::vector<Float64> expected_values1
				{
					0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type70)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type70);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					26.9626, 26.9911, 27.0197, 27.0736, 27.1275, 27.1815, 27.3348, 27.4084, 27.481, 27.775, 28.1165
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type70);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
					20.8563, 20.8706, 20.8848, 20.9118, 20.9388, 20.9657, 21.0424, 21.0792, 21.1155, 21.2625, 21.4332
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type70);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					16.8563, 16.8706, 16.8848, 16.9118, 16.9388, 16.9657, 17.0424, 17.0792, 17.1155, 17.2625, 17.4332
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type71)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type71);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(6.0 + 8.0 / 12.0);
			bend->put_W(10.0);
			bend->put_X(10.0);

			std::vector<Float64> expected_values1
			{
				23.7903, 23.7854, 23.7804, 23.7754, 23.7704, 23.7654, 23.7603, 23.7546, 23.7491, 23.7378, 23.7153
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 23.7903, 23.7854, 23.7804, 23.7754, 23.7704, 23.7654, 23.7603, 23.7546, 23.7491, 23.7378, 23.7153 };
			Test(bend, utTransverse, expected_values2);

			std::vector<Float64> expected_values3
			{ 23.7903, 23.7854, 23.7804, 23.7754, 23.7704, 23.7654, 23.7603, 23.7546, 23.7491, 23.7378, 23.7153 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type72)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type72);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 26.8799, 26.9232, 26.9665, 27.0451, 27.0942, 27.1434, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 26.918, 26.9232, 26.9665, 27.0451, 27.0942, 27.1434, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type72);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 20.8149, 20.8366, 20.8582, 20.8975, 20.9221, 20.9467, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 20.834, 20.8366, 20.8582, 20.8975, 20.9221, 20.9467, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type72);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 16.8149, 16.8366, 16.8582, 16.8975, 16.9221, 16.9467, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 16.834, 16.8366, 16.8582, 16.8975, 16.9221, 16.9467, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type73)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type73);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 26.8799, 26.9232, 26.9665, 27.0451, 27.0942, 27.1434, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 26.918, 26.9232, 26.9665, 27.0451, 27.0942, 27.1434, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type73);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 20.8149, 20.8366, 20.8582, 20.8975, 20.9221, 20.9467, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 20.834, 20.8366, 20.8582, 20.8975, 20.9221, 20.9467, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type73);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 16.8149, 16.8366, 16.8582, 16.8975, 16.9221, 16.9467, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 16.834, 16.8366, 16.8582, 16.8975, 16.9221, 16.9467, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type74)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type74);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);
				bend->put_X(2.0);


				std::vector<Float64> expected_values1
				{
					10.619, 10.6031, 10.5872, 10.5713, 10.5554, 10.5396, 10.4987, 10.4775, 10.4567, 10.3776, 10.2814
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 10.6272, 10.614, 10.6008, 10.5713, 10.5554, 10.5396, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 10.6272, 10.614, 10.6008, 10.5713, 10.5554, 10.5396, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type74);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_W(2.0);


				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type74);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(6.0 + 8.0 / 12.0);
				bend->put_X(2.0);


				std::vector<Float64> expected_values1
				{
					8.64283, 8.63489, 8.62694, 8.619, 8.61105, 8.60311, 8.58268, 8.57211, 8.56168, 8.52215, 8.47401
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 8.64692, 8.64034, 8.63376, 8.619, 8.61105, 8.60311, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type75)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type75);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					26.9626, 26.9911, 27.0197, 27.0736, 27.1275, 27.1815, 27.3348, 27.4084, 27.481, 27.775, 28.1165
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type75);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
					20.8563, 20.8706, 20.8848, 20.9118, 20.9388, 20.9657, 21.0424, 21.0792, 21.1155, 21.2625, 21.4332
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type75);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					16.8563, 16.8706, 16.8848, 16.9118, 16.9388, 16.9657, 17.0424, 17.0792, 17.1155, 17.2625, 17.4332
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type76)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type76);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					26.9595, 27.0293, 27.0991, 27.1689, 27.2388, 27.3086, 27.3882, 27.4685, 27.5477, 27.72, 28.0432
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 26.8425, 26.8733, 26.9041, 27.1689, 27.2388, 27.3086, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type76);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
					20.8547, 20.8896, 20.9246, 20.9595, 20.9944, 21.0293, 21.0691, 21.1093, 21.1489, 21.235, 21.3966
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 20.7962, 20.8116, 20.8271, 20.9595, 20.9944, 21.0293, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type76);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					16.8547, 16.8896, 16.9246, 16.9595, 16.9944, 17.0293, 17.0691, 17.1093, 17.1489, 17.235, 17.3966
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 16.7962, 16.8116, 16.8271, 16.9595, 16.9944, 17.0293, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type77)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type77);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					26.9595, 27.0293, 27.0991, 27.1689, 27.2388, 27.3086, 27.3882, 27.4685, 27.5477, 27.72, 28.0432
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 26.8425, 26.8733, 26.9041, 27.1689, 27.2388, 27.3086, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type77);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
					20.8547, 20.8896, 20.9246, 20.9595, 20.9944, 21.0293, 21.0691, 21.1093, 21.1489, 21.235, 21.3966
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 20.7962, 20.8116, 20.8271, 20.9595, 20.9944, 21.0293, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type77);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					16.8547, 16.8896, 16.9246, 16.9595, 16.9944, 17.0293, 17.0691, 17.1093, 17.1489, 17.235, 17.3966
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 16.7962, 16.8116, 16.8271, 16.9595, 16.9944, 17.0293, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type78)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type78);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					26.9595, 27.0293, 27.0991, 27.1689, 27.2388, 27.3086, 27.3882, 27.4685, 27.5477, 27.72, 28.0432
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 26.8425, 26.8733, 26.9041, 27.1689, 27.2388, 27.3086, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type78);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_W(10.0);

				std::vector<Float64> expected_values1
				{
					20.8547, 20.8896, 20.9246, 20.9595, 20.9944, 21.0293, 21.0691, 21.1093, 21.1489, 21.235, 21.3966
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 20.7962, 20.8116, 20.8271, 20.9595, 20.9944, 21.0293, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type78);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(10.75);
				bend->put_X(6.0);

				std::vector<Float64> expected_values1
				{
					16.8547, 16.8896, 16.9246, 16.9595, 16.9944, 17.0293, 17.0691, 17.1093, 17.1489, 17.235, 17.3966
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 16.7962, 16.8116, 16.8271, 16.9595, 16.9944, 17.0293, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}


		TEST_METHOD(BendTest_Type79)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type79);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_X(6.0);
				bend->put_Y(4.0);
				bend->put_Z(3.0);
				bend->put_T1(M_PI/4);
            bend->put_T2(3*M_PI/4);

				std::vector<Float64> expected_values1
				{
					18.0972, 18.1144, 18.1316, 18.1615, 18.1914, 18.2214, 18.3134, 18.3551, 18.3962, 18.5715, 18.7678
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type79);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_X(6.0);
				bend->put_Z(3.0);
				bend->put_T2(3 * M_PI / 4);

				std::vector<Float64> expected_values1
				{
					17.9973, 18.0183, 18.0392, 18.0729, 18.1066, 18.1402, 18.2361, 18.282, 18.3273, 18.511, 18.7243
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type79);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_Y(4.0);
				bend->put_T1(M_PI / 4);

				std::vector<Float64> expected_values1
				{
					12.0998, 12.0961, 12.0924, 12.0886, 12.0849, 12.0811, 12.0773, 12.0731, 12.0689, 12.0604, 12.0435 
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type79);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12.0);

				std::vector<Float64> expected_values1
				{
						 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type80)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type80);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_X(6.0);
				bend->put_Y(4.0);
				bend->put_Z(3.0);
				bend->put_T1(M_PI / 4);
				bend->put_T2(3 * M_PI / 4);

				std::vector<Float64> expected_values1
				{
					17.967, 17.962, 17.957, 17.9521, 17.9471, 17.9421, 17.937, 17.9313, 17.9257, 17.9144, 17.8919
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 17.967, 17.962, 17.957, 17.9521, 17.9471, 17.9421, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 17.967, 17.962, 17.957, 17.9521, 17.9471, 17.9421, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type80);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_X(6.0);
				bend->put_Z(3.0);
				bend->put_T2(3 * M_PI / 4);

				std::vector<Float64> expected_values1
				{
					17.8672, 17.8659, 17.8647, 17.8634, 17.8622, 17.8609, 17.8597, 17.8582, 17.8568, 17.854, 17.8484
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 17.8672, 17.8659, 17.8647, 17.8634, 17.8622, 17.8609, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 17.8672, 17.8659, 17.8647, 17.8634, 17.8622, 17.8609, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type80);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_Y(4.0);
				bend->put_T1(M_PI / 4);

				std::vector<Float64> expected_values1
				{
					12.0998, 12.0961, 12.0924, 12.0886, 12.0849, 12.0811, 12.0773, 12.0731, 12.0689, 12.0604, 12.0435
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 12.0998, 12.0961, 12.0924, 12.0886, 12.0849, 12.0811, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 12.0998, 12.0961, 12.0924, 12.0886, 12.0849, 12.0811, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type80);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12.0);

				std::vector<Float64> expected_values1
				{
						 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 00 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type81)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type81);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_X(6.0);
				bend->put_Y(4.0);
				bend->put_Z(3.0);
				bend->put_T1(M_PI / 4);
				bend->put_T2(3 * M_PI / 4);

				std::vector<Float64> expected_values1
				{
					18.2273, 18.2667, 18.3062, 18.371, 18.4358, 18.5007, 18.6898, 18.7789, 18.8667, 19.2285, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type81);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_X(6.0);
				bend->put_Z(3.0);
				bend->put_T2(3 * M_PI / 4);

				std::vector<Float64> expected_values1
				{
					17.9973, 18.0183, 18.0392, 18.0729, 18.1066, 18.1402, 18.2361, 18.282, 18.3273, 18.511, 18.7243
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type81);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_Y(4.0);
				bend->put_T1(M_PI / 4);

				std::vector<Float64> expected_values1
				{
					 12.23, 12.2485, 12.2669, 12.2981, 12.3293, 12.3604, 12.4537, 12.4968, 12.5394, 12.7174, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type81);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12.0);

				std::vector<Float64> expected_values1
				{
						 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type82)
		{
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type82);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_X(6.0);
				bend->put_Y(4.0);
				bend->put_Z(3.0);
				bend->put_T1(M_PI / 4);
				bend->put_T2(3 * M_PI / 4);

				std::vector<Float64> expected_values1
				{
					18.2273, 18.2667, 18.3062, 18.371, 18.4358, 18.5007, 18.6898, 18.7789, 18.8667, 19.2285, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type82);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_X(6.0);
				bend->put_Z(3.0);
				bend->put_T2(3 * M_PI / 4);

				std::vector<Float64> expected_values1
				{
					17.9973, 18.0183, 18.0392, 18.0729, 18.1066, 18.1402, 18.2361, 18.282, 18.3273, 18.511, 18.7243
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type82);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12);
				bend->put_W(10.0);
				bend->put_Y(4.0);
				bend->put_T1(M_PI / 4);

				std::vector<Float64> expected_values1
				{
					 12.23, 12.2485, 12.2669, 12.2981, 12.3293, 12.3604, 12.4537, 12.4968, 12.5394, 12.7174, 0
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
			{
				CComPtr<IBend> bend;
				HRESULT hr = bend.CoCreateInstance(CLSID_Type82);
				Assert::IsTrue(SUCCEEDED(hr));
				bend->put_U(12.0);

				std::vector<Float64> expected_values1
				{
						 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12
				};

				Test(bend, utLongitudinal, expected_values1);

				std::vector<Float64> expected_values2
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utTransverse, expected_values2);

				std::vector<Float64> expected_values3
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				Test(bend, utSeismic, expected_values3);
			}
		}

		TEST_METHOD(BendTest_Type83)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type83);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(10.25);
         bend->put_W(4.0);


			std::vector<Float64> expected_values1
			{
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			};

			Test(bend, utLongitudinal, expected_values1);

			std::vector<Float64> expected_values2
			{ 28.6101, 28.6468, 28.6835, 28.7474, 28.7886, 28.8299, 0, 0, 0, 0, 0 };
			Test(bend, utTransverse, expected_values2);
			 
			std::vector<Float64> expected_values3
			{ 28.6482, 28.6468, 28.6835, 28.7474, 28.7886, 28.8299, 0, 0, 0, 0, 0 };
			Test(bend, utSeismic, expected_values3);
		}

		TEST_METHOD(BendTest_Type89)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type89);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);
         bend->put_W(10.0);

			std::vector<Float64> expected_values1
			{ 180.25, 180.25, 150.25, 150.25, 150.25, 150.25, 150.25, 150.25, 150.25, 150.25, 150.25 };

			std::vector<Float64> expected_values2
			{ 180.25, 180.25, 180.25, 180.25, 180.25, 180.25, 180.25, 180.25, 180.25, 180.25, 180.25 };

			std::vector<UseType> uses { utLongitudinal, utTransverse, utSeismic };

			for (auto& use : uses)
			{
				Test(bend, use, A706_Grade60, expected_values1);
				Test(bend, use, A706_Grade80, expected_values1);
				Test(bend, use, A1035_Grade100, expected_values1);
				Test(bend, use, A1035_Grade120, expected_values1);
				Test(bend, use, A955_Grade60, expected_values1);
				Test(bend, use, A955_Grade75, expected_values1);
				Test(bend, use, A955_Grade80, expected_values1);
				Test(bend, use, A767_A1094_Grade60, expected_values2);
				Test(bend, use, A767_A1094_Grade100, expected_values2);
			}
		}

		TEST_METHOD(BendTest_Type90)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type90);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type91)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type91);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type92)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type92);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type93)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type93);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type94)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type94);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type95)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type95);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type96)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type96);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type97)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type97);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type98)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type98);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}

		TEST_METHOD(BendTest_Type99)
		{
			CComPtr<IBend> bend;
			HRESULT hr = bend.CoCreateInstance(CLSID_Type99);
			Assert::IsTrue(SUCCEEDED(hr));
			bend->put_U(100.25);

			std::vector<Float64> expected_values
			{
				100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25, 100.25
			};

			Test(bend, utLongitudinal, expected_values);
			Test(bend, utTransverse, expected_values);
			Test(bend, utSeismic, expected_values);
		}
	};
};

