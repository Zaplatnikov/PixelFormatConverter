#include "pch.h"
#include "CppUnitTest.h"
#include <PixelFormatConverter.h>
#include <opencv2/opencv.hpp>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PixelFormatConverterUnitTests
{
	TEST_CLASS(PixelFormatConverterUnitTests)
	{
	public:
		
		TEST_METHOD(Copy)
		{
			// Load image
			cv::Mat srcImage = cv::imread("test.jpg");
			if (srcImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Create image
			zs::Frame srcFrame;
			srcFrame.width = (uint32_t)srcImage.size().width;
			srcFrame.height = (uint32_t)srcImage.size().height;
			srcFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			srcFrame.size = srcFrame.width * srcFrame.height * 3;
			srcFrame.sourceID = 1;
			srcFrame.frameID = 2;
			srcFrame.data = new uint8_t[(size_t)srcFrame.width * (size_t)srcFrame.height * 3];

			// Copy data
			memcpy(srcFrame.data, srcImage.data, (size_t)srcFrame.width * (size_t)srcFrame.height * 3);

			// Convert
			zs::Frame dstFrame;
			dstFrame.fourcc = srcFrame.fourcc;
			zs::PixelFormatConverter converter;
			if (!converter.Convert(srcFrame, dstFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)srcFrame.width * (size_t)srcFrame.height * 3; ++i)
				if (srcFrame.data[i] != dstFrame.data[i])
					Assert::Fail(L"Data not equal");
			if (srcFrame.width != dstFrame.width)
				Assert::Fail(L"Width not equal");
			if (srcFrame.height != dstFrame.height)
				Assert::Fail(L"Height not equal");
			if (srcFrame.fourcc != dstFrame.fourcc)
				Assert::Fail(L"Fourcc not equal");
			if (srcFrame.size != dstFrame.size)
				Assert::Fail(L"Size not equal");
			if (srcFrame.frameID != dstFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (srcFrame.sourceID != dstFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			srcImage.release();

		};

		TEST_METHOD(RGB24_to_BGR24)
		{
			// Load image
			cv::Mat srcImage = cv::imread("test.jpg");
			if (srcImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert to RGB24
			cv::Mat rgbImage;
			cv::cvtColor(srcImage, rgbImage, cv::COLOR_BGR2RGB);

			// Create frame
			zs::Frame srcFrame;
			srcFrame.width = (uint32_t)srcImage.size().width;
			srcFrame.height = (uint32_t)srcImage.size().height;
			srcFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			srcFrame.size = srcFrame.width * srcFrame.height * 3;
			srcFrame.sourceID = 1;
			srcFrame.frameID = 2;
			srcFrame.data = new uint8_t[(size_t)srcFrame.width * (size_t)srcFrame.height * 3];

			// Copy data
			memcpy(srcFrame.data, rgbImage.data, (size_t)srcFrame.width * (size_t)srcFrame.height * 3);

			// Convert to BGR24
			zs::Frame dstFrame;
			dstFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(srcFrame, dstFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare atributes
			if (srcFrame.width != dstFrame.width)
				Assert::Fail(L"Width not equal");
			if (srcFrame.height != dstFrame.height)
				Assert::Fail(L"Height not equal");
			if (srcFrame.fourcc != dstFrame.fourcc)
				Assert::Fail(L"Fourcc not equal");
			if (srcFrame.size != dstFrame.size)
				Assert::Fail(L"Size not equal");
			if (srcFrame.frameID != dstFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (srcFrame.sourceID != dstFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data with original
			for (size_t i = 0; i < (size_t)srcFrame.width * (size_t)srcFrame.height * 3; ++i)
				if (dstFrame.data[i] != srcImage.data[i])
					Assert::Fail(L"Data not equal");

		}


	};
}
