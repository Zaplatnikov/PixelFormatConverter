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


		TEST_METHOD(BGR24_to_YUY2) {

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
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare
			cv::Mat yuvImage;
			cv::cvtColor(bgrImage, yuvImage, cv::COLOR_BGR2YUV);


			// Compare atributes
			if (bgrFrame.width != yuy2Frame.width)
				Assert::Fail(L"Width not equal");
			if (bgrFrame.height != yuy2Frame.height)
				Assert::Fail(L"Height not equal");
			if (bgrFrame.size == yuy2Frame.size)
				Assert::Fail(L"Size not valid");
			if (bgrFrame.frameID != yuy2Frame.frameID)
				Assert::Fail(L"frameID not equal");
			if (bgrFrame.sourceID != yuy2Frame.sourceID)
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


		TEST_METHOD(BGR24_to_Y800) {

			// Load image
			cv::Mat bgrImage = cv::imread("test.jpg");
			if (bgrImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert to GRAY
			cv::Mat grayImage;
			cv::cvtColor(bgrImage, grayImage, cv::COLOR_BGR2GRAY);

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

			// Convert to YUY2
			zs::Frame grayFrame;
			grayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, grayFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare atributes
			if (bgrFrame.width != grayFrame.width)
				Assert::Fail(L"Width not equal");
			if (bgrFrame.height != grayFrame.height)
				Assert::Fail(L"Height not equal");
			if (bgrFrame.size != grayFrame.size * 3)
				Assert::Fail(L"Size not valid");
			if (bgrFrame.frameID != grayFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (bgrFrame.sourceID != grayFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (abs((int)grayFrame.data[i] - (int)grayImage.data[i]) > 5) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(BGR24_to_NV12) {

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
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert image to YUV
			cv::Mat yuvImage;
			cv::cvtColor(bgrImage, yuvImage, cv::COLOR_BGR2YUV);

			// Convert frame to NV12
			zs::Frame nv12Frame;
			nv12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare atributes
			if (bgrFrame.width != nv12Frame.width)
				Assert::Fail(L"Width not equal");
			if (bgrFrame.height != nv12Frame.height)
				Assert::Fail(L"Height not equal");
			if (bgrFrame.size == nv12Frame.size)
				Assert::Fail(L"Size not valid");
			if (bgrFrame.frameID != nv12Frame.frameID)
				Assert::Fail(L"frameID not equal");
			if (bgrFrame.sourceID != nv12Frame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare Y data
			for (size_t i = 0; i < (size_t)nv12Frame.width * (size_t)nv12Frame.height; ++i) {
				if (abs((int)yuvImage.data[i * 3] - (int)nv12Frame.data[i]) > 1) {
					Assert::Fail(L"Y data not equal");
					return;
				}//if...
			}//for...

			// Compare U and V data
			size_t p = 0;
			uint8_t U0, V0, U1, V1;
			for (size_t i = 0; i < (size_t)nv12Frame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)nv12Frame.width; j = j + 2) {
					U0 = yuvImage.data[i * nv12Frame.width * 3 + j * 3 + 1];
					V0 = yuvImage.data[i * nv12Frame.width * 3 + j * 3 + 2];
					U1 = nv12Frame.data[((size_t)nv12Frame.height + p) * (size_t)nv12Frame.width + j];
					V1 = nv12Frame.data[((size_t)nv12Frame.height + p) * (size_t)nv12Frame.width + j + 1];
					if (abs((int)U0 - (int)U1) > 10) {
						Assert::Fail(L"U data not equal");
						return;
					}//if...
					if (abs((int)V0 - (int)V1) > 10) {
						Assert::Fail(L"V data not equal");
						return;
					}//if...
				}//for...
				++p;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(UYVY_to_RGB24) {

			// Load image
			cv::Mat bgrImage = cv::imread("test.jpg");
			if (bgrImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert to rgb
			cv::Mat rgbImage;
			cv::cvtColor(bgrImage, rgbImage, cv::COLOR_BGR2RGB);

			// Create RGB Frame object
			zs::Frame sourceRGBFrame;
			sourceRGBFrame.width = (uint32_t)rgbImage.size().width;
			sourceRGBFrame.height = (uint32_t)rgbImage.size().height;
			sourceRGBFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			sourceRGBFrame.size = sourceRGBFrame.width * sourceRGBFrame.height * 3;
			sourceRGBFrame.sourceID = 1;
			sourceRGBFrame.frameID = 2;
			sourceRGBFrame.data = new uint8_t[sourceRGBFrame.size];
			memcpy(sourceRGBFrame.data, rgbImage.data, sourceRGBFrame.size);

			// Convert RGB to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(sourceRGBFrame, uyvyFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Convert UYVY to RGB
			zs::Frame resultRGBFrame;
			resultRGBFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			if (!converter.Convert(uyvyFrame, resultRGBFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}//if...

			// Compare atributes
			if (sourceRGBFrame.width != resultRGBFrame.width)
				Assert::Fail(L"Width not equal");
			if (sourceRGBFrame.height != resultRGBFrame.height)
				Assert::Fail(L"Height not equal");
			if (sourceRGBFrame.size != resultRGBFrame.size)
				Assert::Fail(L"Size not valid");
			if (sourceRGBFrame.frameID != resultRGBFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (sourceRGBFrame.sourceID != resultRGBFrame.sourceID)
				Assert::Fail(L"SourceID not equal");
			
			// Compare data
			for (size_t i = 0; i < (size_t)sourceRGBFrame.size; i = i + 6) {
				if (abs((int)sourceRGBFrame.data[i] - (int)resultRGBFrame.data[i]) > 10) {
					Assert::Fail(L"R data not equal");
				}//if...
				if (abs((int)sourceRGBFrame.data[i + 1] - (int)resultRGBFrame.data[i + 1]) > 10) {
					Assert::Fail(L"G data not equal");
				}//if...
				if (abs((int)sourceRGBFrame.data[i + 2] - (int)resultRGBFrame.data[i + 2]) > 10) {
					Assert::Fail(L"B data not equal");
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(RGB24_to_YUV1) {

			// Load image
			cv::Mat bgrImage = cv::imread("test.jpg");
			if (bgrImage.empty())
				Assert::Fail(L"Test image not loaded");

			// Convert image to rgb
			cv::Mat rgbImage;
			cv::cvtColor(bgrImage, rgbImage, cv::COLOR_BGR2RGB);

			// Create RGB Frame object
			zs::Frame rgbFrame;
			rgbFrame.width = (uint32_t)rgbImage.size().width;
			rgbFrame.height = (uint32_t)rgbImage.size().height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[rgbFrame.size];
			memcpy(rgbFrame.data, rgbImage.data, rgbFrame.size);

			// Convert image to YUV
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);

			// Convert frame to YUV
			zs::Frame yuvFrame;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, yuvFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (rgbFrame.width != yuvFrame.width)
				Assert::Fail(L"Width not equal");
			if (rgbFrame.height != yuvFrame.height)
				Assert::Fail(L"Height not equal");
			if (rgbFrame.size != yuvFrame.size)
				Assert::Fail(L"Size not valid");
			if (rgbFrame.frameID != yuvFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (rgbFrame.sourceID != yuvFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			uint8_t val0, val1;
			for (size_t i = 0; i < (size_t)yuvFrame.size; ++i) {
				val0 = yuvFrame.data[i];
				val1 = yuvImage.data[i];
				if (abs((int)val0 - (int)val1) > 1) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...		

		};//TEST_METHOD...


		TEST_METHOD(YUV1_to_RGB24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV YUV image
			cv::Mat yuvImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				yuvImage.data[i] = (uint8_t)(rand() % 255);

			// Create YUV frame
			zs::Frame yuvFrame;
			yuvFrame.width = width;
			yuvFrame.height = height;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			yuvFrame.size = yuvFrame.width * yuvFrame.height * 3;
			yuvFrame.data = new uint8_t[yuvFrame.size];
			memcpy(yuvFrame.data, yuvImage.data, yuvFrame.size);

			// Convert image to RGB
			cv::Mat rgbImage;
			cv::cvtColor(yuvImage, rgbImage, cv::COLOR_YUV2RGB);

			// Convert frame to RGB
			zs::Frame rgbFrame;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuvFrame, rgbFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare data
			uint8_t val0, val1;
			for (size_t i = 0; i < (size_t)yuvFrame.size; ++i) {
				val0 = rgbFrame.data[i];
				val1 = rgbImage.data[i];
				if (abs((int)val0 - (int)val1) > 5) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...



	};
}
