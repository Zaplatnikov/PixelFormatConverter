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
			cv::Mat bgrImage;
			cv::cvtColor(rgbImage, bgrImage, cv::COLOR_RGB2BGR);

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
			if (srcFrame.size != dstFrame.size)
				Assert::Fail(L"Size not equal");
			if (srcFrame.frameID != dstFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (srcFrame.sourceID != dstFrame.sourceID)
				Assert::Fail(L"SourceID not equal");



			// Compare data with original
			for (size_t i = 0; i < (size_t)srcFrame.size; i = i + 3) {
				if (abs((int)dstFrame.data[i] - (int)bgrImage.data[i]) > 5) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...


		};


		TEST_METHOD(RGB24_to_UYVY)
		{
			// Load image
			cv::Mat srcImage = cv::imread("test.jpg");
			if (srcImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert to RGB24
			cv::Mat rgbImage;
			cv::cvtColor(srcImage, rgbImage, cv::COLOR_BGR2RGB);

			// Create src Frame object
			zs::Frame rgbFrame;
			rgbFrame.width = (uint32_t)srcImage.size().width;
			rgbFrame.height = (uint32_t)srcImage.size().height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[(size_t)rgbFrame.width * (size_t)rgbFrame.height * 3];

			// Copy data
			memcpy(rgbFrame.data, rgbImage.data, (size_t)rgbFrame.width * (size_t)rgbFrame.height * 3);

			// Convert to UYVY
			zs::Frame uyuvFrame;
			uyuvFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, uyuvFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);


			// Compare atributes
			if (rgbFrame.width != uyuvFrame.width)
				Assert::Fail(L"Width not equal");
			if (rgbFrame.height != uyuvFrame.height)
				Assert::Fail(L"Height not equal");
			if (rgbFrame.size == uyuvFrame.size)
				Assert::Fail(L"Size not valid");
			if (rgbFrame.frameID != uyuvFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (rgbFrame.sourceID != uyuvFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			int j = 0;
			for (int i = 0; i < yuvImage.size().width * yuvImage.size().height * 3; i = i + 6) {
				if (abs((int)uyuvFrame.data[j + 1] - (int)yuvImage.data[i]) > 5) {
					Assert::Fail(L"Y data not equal");
				}//if...
				if (abs((int)uyuvFrame.data[j + 3] - (int)yuvImage.data[i + 3]) > 5) {
					Assert::Fail(L"Y data not equal");
				}//if...
				j = j + 4;
			}//for...

		};


		TEST_METHOD(RGB24_to_YUY2)
		{
			// Load image
			cv::Mat srcImage = cv::imread("test.jpg");
			if (srcImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert to RGB24
			cv::Mat rgbImage;
			cv::cvtColor(srcImage, rgbImage, cv::COLOR_BGR2RGB);

			// Create src Frame object
			zs::Frame rgbFrame;
			rgbFrame.width = (uint32_t)srcImage.size().width;
			rgbFrame.height = (uint32_t)srcImage.size().height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[(size_t)rgbFrame.width * (size_t)rgbFrame.height * 3];

			// Copy data
			memcpy(rgbFrame.data, rgbImage.data, (size_t)rgbFrame.width * (size_t)rgbFrame.height * 3);

			// Convert to YUY2
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);


			// Compare atributes
			if (rgbFrame.width != yuy2Frame.width)
				Assert::Fail(L"Width not equal");
			if (rgbFrame.height != yuy2Frame.height)
				Assert::Fail(L"Height not equal");
			if (rgbFrame.size == yuy2Frame.size)
				Assert::Fail(L"Size not valid");
			if (rgbFrame.frameID != yuy2Frame.frameID)
				Assert::Fail(L"frameID not equal");
			if (rgbFrame.sourceID != yuy2Frame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			int j = 0;
			for (int i = 0; i < yuvImage.size().width * yuvImage.size().height * 3; i = i + 6) {
				if (abs((int)yuy2Frame.data[j] - (int)yuvImage.data[i]) > 5) {
					Assert::Fail(L"Y data not equal");
				}//if...
				if (abs((int)yuy2Frame.data[j + 2] - (int)yuvImage.data[i + 3]) > 5) {
					Assert::Fail(L"Y data not equal");
				}//if...
				j = j + 4;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(RGB24_to_Y800) {

			// Load image
			cv::Mat srcImage = cv::imread("test.jpg");
			if (srcImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert to RGB
			cv::Mat rgbImage;
			cv::cvtColor(srcImage, rgbImage, cv::COLOR_BGR2RGB);

			// Convert to GRAY
			cv::Mat grayImage;
			cv::cvtColor(rgbImage, grayImage, cv::COLOR_RGB2GRAY);

			// Create src Frame object
			zs::Frame rgbFrame;
			rgbFrame.width = (uint32_t)srcImage.size().width;
			rgbFrame.height = (uint32_t)srcImage.size().height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[(size_t)rgbFrame.width * (size_t)rgbFrame.height * 3];

			// Copy data
			memcpy(rgbFrame.data, rgbImage.data, (size_t)rgbFrame.width * (size_t)rgbFrame.height * 3);

			// Convert to YUY2
			zs::Frame grayFrame;
			grayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, grayFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare atributes
			if (rgbFrame.width != grayFrame.width)
				Assert::Fail(L"Width not equal");
			if (rgbFrame.height != grayFrame.height)
				Assert::Fail(L"Height not equal");
			if (rgbFrame.size != grayFrame.size * 3)
				Assert::Fail(L"Size not valid");
			if (rgbFrame.frameID != grayFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (rgbFrame.sourceID != grayFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (abs((int)grayFrame.data[i] - (int)grayImage.data[i]) > 5) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...


		};//TEST_METHOD...


		TEST_METHOD(RGB24_to_NV12) {

			// Load image
			cv::Mat srcImage = cv::imread("test.jpg");
			if (srcImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert to RGB
			cv::Mat rgbImage;
			cv::cvtColor(srcImage, rgbImage, cv::COLOR_BGR2RGB);

			// Create src Frame object
			zs::Frame rgbFrame;
			rgbFrame.width = (uint32_t)srcImage.size().width;
			rgbFrame.height = (uint32_t)srcImage.size().height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[(size_t)rgbFrame.width * (size_t)rgbFrame.height * 3];

			// Copy RGB data
			memcpy(rgbFrame.data, rgbImage.data, (size_t)rgbFrame.width * (size_t)rgbFrame.height * 3);

			// Convert to NV12
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);

			// Convert to NV12
			zs::Frame nv12Frame;
			nv12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare atributes
			if (rgbFrame.width != nv12Frame.width)
				Assert::Fail(L"Width not equal");
			if (rgbFrame.height != nv12Frame.height)
				Assert::Fail(L"Height not equal");
			if (rgbFrame.size == nv12Frame.size)
				Assert::Fail(L"Size not valid");
			if (rgbFrame.frameID != nv12Frame.frameID)
				Assert::Fail(L"frameID not equal");
			if (rgbFrame.sourceID != nv12Frame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare Y data
			for (size_t i = 0; i < (size_t)nv12Frame.width * (size_t)nv12Frame.height; ++i) {
				if (abs((int)yuvImage.data[i * 3] - (int)nv12Frame.data[i]) > 1) {
					Assert::Fail(L"Y data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...



		TEST_METHOD(BGR24_to_RGB24) {

			// Load image
			cv::Mat bgrImage = cv::imread("test.jpg");
			if (bgrImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Create frame
			zs::Frame bgrFrame;
			bgrFrame.width = (uint32_t)bgrImage.size().width;
			bgrFrame.height = (uint32_t)bgrImage.size().height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.sourceID = 1;
			bgrFrame.frameID = 2;
			bgrFrame.data = new uint8_t[(size_t)bgrFrame.width * (size_t)bgrFrame.height * 3];

			// Copy data
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert to BGR24
			zs::Frame dstFrame;
			dstFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, dstFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare atributes
			if (bgrFrame.width != dstFrame.width)
				Assert::Fail(L"Width not equal");
			if (bgrFrame.height != dstFrame.height)
				Assert::Fail(L"Height not equal");
			if (bgrFrame.size != dstFrame.size)
				Assert::Fail(L"Size not equal");
			if (bgrFrame.frameID != dstFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (bgrFrame.sourceID != dstFrame.sourceID)
				Assert::Fail(L"SourceID not equal");


			// Compare data with original
			for (size_t i = 0; i < (size_t)bgrFrame.size; i = i + 3) {
				if (abs((int)dstFrame.data[i] - (int)bgrImage.data[i]) > 5) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(BGR24_to_UYVY) {

			// Load image
			cv::Mat bgrImage = cv::imread("test.jpg");
			if (bgrImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Create src Frame object
			zs::Frame bgrFrame;
			bgrFrame.width = (uint32_t)bgrImage.size().width;
			bgrFrame.height = (uint32_t)bgrImage.size().height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.sourceID = 1;
			bgrFrame.frameID = 2;
			bgrFrame.data = new uint8_t[bgrFrame.size];

			// Copy data
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert to UYVY
			zs::Frame uyuvFrame;
			uyuvFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, uyuvFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare
			cv::Mat yuvImage;
			cv::cvtColor(bgrImage, yuvImage, cv::COLOR_BGR2YUV);


			// Compare atributes
			if (bgrFrame.width != uyuvFrame.width)
				Assert::Fail(L"Width not equal");
			if (bgrFrame.height != uyuvFrame.height)
				Assert::Fail(L"Height not equal");
			if (bgrFrame.size == uyuvFrame.size)
				Assert::Fail(L"Size not valid");
			if (bgrFrame.frameID != uyuvFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (bgrFrame.sourceID != uyuvFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			int j = 0;
			for (int i = 0; i < yuvImage.size().width * yuvImage.size().height * 3; i = i + 6) {
				if (abs((int)uyuvFrame.data[j + 1] - (int)yuvImage.data[i]) > 5) {
					Assert::Fail(L"Y data not equal");
				}//if...
				if (abs((int)uyuvFrame.data[j + 3] - (int)yuvImage.data[i + 3]) > 5) {
					Assert::Fail(L"Y data not equal");
				}//if...
				j = j + 4;
			}//for...

		};//TEST_METHOD...



	};
}
