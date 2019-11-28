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

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			zs::Frame srcFrame;
			srcFrame.width = width;
			srcFrame.height = height;
			srcFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			srcFrame.size = srcFrame.width * srcFrame.height;
			srcFrame.data = new uint8_t[srcFrame.size];
			for (size_t i = 0; i < srcFrame.size; ++i)
				srcFrame.data[i] = (uint8_t)(rand() % 255);

			zs::Frame dstFrame;
			dstFrame.fourcc = srcFrame.fourcc;
			zs::PixelFormatConverter converter;
			if (!converter.Convert(srcFrame, dstFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
			}

			for (size_t i = 0; i < srcFrame.size; ++i)
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

		};


		TEST_METHOD(RGB24_to_BGR24)
		{

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat rgbImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				rgbImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to BGR image
			cv::Mat bgrImage;
			cv::cvtColor(rgbImage, bgrImage, cv::COLOR_RGB2BGR);

			// Create RGB frame
			zs::Frame rgbFrame;
			rgbFrame.width = width;
			rgbFrame.height = height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[rgbFrame.size];
			memcpy(rgbFrame.data, rgbImage.data, rgbFrame.size);

			// Convert RGB frame to BGR frame
			zs::Frame bgrFrame;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, bgrFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (bgrFrame.width != bgrFrame.width)
				Assert::Fail(L"Width not equal");
			if (bgrFrame.height != bgrFrame.height)
				Assert::Fail(L"Height not equal");
			if (bgrFrame.size != bgrFrame.size)
				Assert::Fail(L"Size not equal");
			if (bgrFrame.frameID != bgrFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (bgrFrame.sourceID != bgrFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			for (size_t i = 0; i < (size_t)bgrFrame.size; i = i + 3) {
				if (abs((int)bgrFrame.data[i] - (int)bgrImage.data[i]) > 1) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};


		TEST_METHOD(RGB24_to_UYVY)
		{
			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat rgbImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				rgbImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to YUV image
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);

			// Create RGB frame
			zs::Frame rgbFrame;
			rgbFrame.width = width;
			rgbFrame.height = height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[rgbFrame.size];
			memcpy(rgbFrame.data, rgbImage.data, rgbFrame.size);

			// Convert RGB frame to BGR frame
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, uyvyFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (rgbFrame.width != uyvyFrame.width)
				Assert::Fail(L"Width not equal");
			if (rgbFrame.height != uyvyFrame.height)
				Assert::Fail(L"Height not equal");
			if (rgbFrame.frameID != uyvyFrame.frameID)
				Assert::Fail(L"frameID not equal");
			if (rgbFrame.sourceID != uyvyFrame.sourceID)
				Assert::Fail(L"SourceID not equal");

			// Compare data
			size_t j = 0;
			for (size_t i = 0; i < (size_t)uyvyFrame.size; i = i + 4) {
				if (abs((int)uyvyFrame.data[i + 1] - (int)yuvImage.data[j]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i + 3] - (int)yuvImage.data[j + 3]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i] - (int)yuvImage.data[j + 1]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i + 2] - (int)yuvImage.data[j + 2]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				j += 6;
			}//for...

		};


		TEST_METHOD(RGB24_to_YUY2)
		{
			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat rgbImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				rgbImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to YUV image
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);

			// Create RGB frame
			zs::Frame rgbFrame;
			rgbFrame.width = width;
			rgbFrame.height = height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[rgbFrame.size];
			memcpy(rgbFrame.data, rgbImage.data, rgbFrame.size);

			// Convert RGB frame to BGR frame
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (rgbFrame.width != yuy2Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (rgbFrame.height != yuy2Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (rgbFrame.frameID != yuy2Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (rgbFrame.sourceID != yuy2Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t j = 0;
			for (size_t i = 0; i < (size_t)yuy2Frame.size; i = i + 4) {
				if (abs((int)yuy2Frame.data[i] - (int)yuvImage.data[j]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 2] - (int)yuvImage.data[j + 3]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 1] - (int)yuvImage.data[j + 1]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 3] - (int)yuvImage.data[j + 2]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				j += 6;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(RGB24_to_Y800) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat rgbImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				rgbImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to Y800 image
			cv::Mat grayImage;
			cv::cvtColor(rgbImage, grayImage, cv::COLOR_RGB2GRAY);

			// Create RGB frame
			zs::Frame rgbFrame;
			rgbFrame.width = width;
			rgbFrame.height = height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[rgbFrame.size];
			memcpy(rgbFrame.data, rgbImage.data, rgbFrame.size);

			// Convert RGB frame to BGR frame
			zs::Frame grayFrame;
			grayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, grayFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (rgbFrame.width != grayFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (rgbFrame.height != grayFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (rgbFrame.frameID != grayFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (rgbFrame.sourceID != grayFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (abs((int)grayFrame.data[i] - (int)grayImage.data[i]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(RGB24_to_NV12) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat rgbImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				rgbImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to YUV image
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);

			// Create RGB frame
			zs::Frame rgbFrame;
			rgbFrame.width = width;
			rgbFrame.height = height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.sourceID = 1;
			rgbFrame.frameID = 2;
			rgbFrame.data = new uint8_t[rgbFrame.size];
			memcpy(rgbFrame.data, rgbImage.data, rgbFrame.size);

			// Convert RGB frame to NV12 frame
			zs::Frame nv12Frame;
			nv12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (rgbFrame.width != nv12Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (rgbFrame.height != nv12Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (rgbFrame.frameID != nv12Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (rgbFrame.sourceID != nv12Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t p = nv12Frame.height;
			for (size_t i = 0; i < (size_t)nv12Frame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)nv12Frame.width; j = j + 2) {
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3] -
						(int)nv12Frame.data[i * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"Y00 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 3] -
						(int)nv12Frame.data[i * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"Y01 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[(i + 1) * (size_t)nv12Frame.width * 3 + j * 3] -
						(int)nv12Frame.data[(i + 1) * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"Y10 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[(i + 1) * (size_t)nv12Frame.width * 3 + j * 3 + 3] -
						(int)nv12Frame.data[(i + 1) * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"Y11 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 1] -
						(int)nv12Frame.data[p * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"U data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 2] -
						(int)nv12Frame.data[p * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"V data not equal");
						return;
					}//if...
				}//for...
				++p;
			}//for...

		};//TEST_METHOD...
		

		TEST_METHOD(BGR24_to_RGB24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat bgrImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				bgrImage.data[i] = (uint8_t)(rand() % 255);

			// Convert BGR image to RGB image
			cv::Mat rgbImage;
			cv::cvtColor(bgrImage, rgbImage, cv::COLOR_BGR2RGB);

			// Create BGR frame
			zs::Frame bgrFrame;
			bgrFrame.width = width;
			bgrFrame.height = height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.sourceID = 1;
			bgrFrame.frameID = 2;
			bgrFrame.data = new uint8_t[bgrFrame.size];
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert BGR frame to RGB frame
			zs::Frame rgbFrame;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, rgbFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (rgbFrame.width != bgrFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (rgbFrame.height != bgrFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (rgbFrame.size != bgrFrame.size) {
				Assert::Fail(L"Size not equal");
				return;
			}//if...
			if (rgbFrame.frameID != bgrFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (rgbFrame.sourceID != bgrFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)rgbFrame.size; ++i) {
				if (abs((int)rgbFrame.data[i] - (int)rgbImage.data[i]) > 1) {
					Assert::Fail(L"RGB Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(BGR24_to_UYVY) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat bgrImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				bgrImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to YUV image
			cv::Mat yuvImage;
			cv::cvtColor(bgrImage, yuvImage, cv::COLOR_BGR2YUV);

			// Create RGB frame
			zs::Frame bgrFrame;
			bgrFrame.width = width;
			bgrFrame.height = height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.sourceID = 1;
			bgrFrame.frameID = 2;
			bgrFrame.data = new uint8_t[bgrFrame.size];
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert RGB frame to BGR frame
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, uyvyFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (bgrFrame.width != uyvyFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...				
			if (bgrFrame.height != uyvyFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...				
			if (bgrFrame.frameID != uyvyFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (bgrFrame.sourceID != uyvyFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...				

			// Compare data
			size_t j = 0;
			for (size_t i = 0; i < (size_t)uyvyFrame.size; i = i + 4) {
				if (abs((int)uyvyFrame.data[i + 1] - (int)yuvImage.data[j]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i + 3] - (int)yuvImage.data[j + 3]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i] - (int)yuvImage.data[j + 1]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i + 2] - (int)yuvImage.data[j + 2]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				j += 6;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(BGR24_to_YUY2) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat bgrImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				bgrImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to YUV image
			cv::Mat yuvImage;
			cv::cvtColor(bgrImage, yuvImage, cv::COLOR_BGR2YUV);

			// Create RGB frame
			zs::Frame bgrFrame;
			bgrFrame.width = width;
			bgrFrame.height = height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.sourceID = 1;
			bgrFrame.frameID = 2;
			bgrFrame.data = new uint8_t[bgrFrame.size];
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert RGB frame to BGR frame
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (bgrFrame.width != yuy2Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...				
			if (bgrFrame.height != yuy2Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...				
			if (bgrFrame.frameID != yuy2Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (bgrFrame.sourceID != yuy2Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...				

			// Compare data
			size_t j = 0;
			for (size_t i = 0; i < (size_t)yuy2Frame.size; i = i + 4) {
				if (abs((int)yuy2Frame.data[i] - (int)yuvImage.data[j]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 2] - (int)yuvImage.data[j + 3]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 1] - (int)yuvImage.data[j + 1]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 3] - (int)yuvImage.data[j + 2]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
				j += 6;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(BGR24_to_Y800) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat bgrImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				bgrImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to Y800 image
			cv::Mat grayImage;
			cv::cvtColor(bgrImage, grayImage, cv::COLOR_BGR2GRAY);

			// Create RGB frame
			zs::Frame bgrFrame;
			bgrFrame.width = width;
			bgrFrame.height = height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.sourceID = 1;
			bgrFrame.frameID = 2;
			bgrFrame.data = new uint8_t[bgrFrame.size];
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert RGB frame to BGR frame
			zs::Frame grayFrame;
			grayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, grayFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (bgrFrame.width != grayFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (bgrFrame.height != grayFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (bgrFrame.frameID != grayFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (bgrFrame.sourceID != grayFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (abs((int)grayFrame.data[i] - (int)grayImage.data[i]) > 1) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(BGR24_to_NV12) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV RGB image
			cv::Mat bgrImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				bgrImage.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB image to YUV image
			cv::Mat yuvImage;
			cv::cvtColor(bgrImage, yuvImage, cv::COLOR_BGR2YUV);

			// Create RGB frame
			zs::Frame bgrFrame;
			bgrFrame.width = width;
			bgrFrame.height = height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.sourceID = 1;
			bgrFrame.frameID = 2;
			bgrFrame.data = new uint8_t[bgrFrame.size];
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert BGR frame to NV12 frame
			zs::Frame nv12Frame;
			nv12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (bgrFrame.width != nv12Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (bgrFrame.height != nv12Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (bgrFrame.frameID != nv12Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (bgrFrame.sourceID != nv12Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t p = nv12Frame.height;
			for (size_t i = 0; i < (size_t)nv12Frame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)nv12Frame.width; j = j + 2) {
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3] -
						(int)nv12Frame.data[i * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"Y00 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 3] -
						(int)nv12Frame.data[i * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"Y01 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[(i + 1) * (size_t)nv12Frame.width * 3 + j * 3] -
						(int)nv12Frame.data[(i + 1) * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"Y10 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[(i + 1) * (size_t)nv12Frame.width * 3 + j * 3 + 3] -
						(int)nv12Frame.data[(i + 1) * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"Y11 data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 1] -
						(int)nv12Frame.data[p * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"U data not equal");
						return;
					}//if...
					if (abs((int)yuvImage.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 2] -
						(int)nv12Frame.data[p * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"V data not equal");
						return;
					}//if...
				}//for...
				++p;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(UYVY_to_RGB24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create source RGB image
			zs::Frame sourceRGBFrame;
			sourceRGBFrame.width = width;
			sourceRGBFrame.height = height;
			sourceRGBFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			sourceRGBFrame.size = sourceRGBFrame.width * sourceRGBFrame.height * 3;
			sourceRGBFrame.sourceID = 1;
			sourceRGBFrame.frameID = 2;
			sourceRGBFrame.data = new uint8_t[sourceRGBFrame.size];

			// Convert source RGB to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(sourceRGBFrame, uyvyFrame)) {
				Assert::Fail(L"Convert function RGB_to_UYVY returned FALSE");
				return;
			}//if...

			// Convert UYVY to RGB
			zs::Frame resultRGBFrame;
			resultRGBFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			if (!converter.Convert(uyvyFrame, resultRGBFrame)) {
				Assert::Fail(L"Convert function UYVY_to_RGB returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (sourceRGBFrame.width != resultRGBFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (sourceRGBFrame.height != resultRGBFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (sourceRGBFrame.size != resultRGBFrame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (sourceRGBFrame.frameID != resultRGBFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (sourceRGBFrame.sourceID != resultRGBFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...
				
			// Compare data
			for (size_t i = 0; i < (size_t)resultRGBFrame.size; i = i + 6) {
				if (abs((int)sourceRGBFrame.data[i] - (int)resultRGBFrame.data[i]) > 1) {
					Assert::Fail(L"R data not equal");
					return;
				}//if...
				if (abs((int)sourceRGBFrame.data[i + 1] - (int)resultRGBFrame.data[i + 1]) > 1) {
					Assert::Fail(L"G data not equal");
					return;
				}//if...
				if (abs((int)sourceRGBFrame.data[i + 2] - (int)resultRGBFrame.data[i + 2]) > 1) {
					Assert::Fail(L"B data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(RGB24_to_YUV1) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV YUV image
			cv::Mat rgbImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				rgbImage.data[i] = (uint8_t)(rand() % 255);

			// Create YUV frame
			zs::Frame rgbFrame;
			rgbFrame.width = width;
			rgbFrame.height = height;
			rgbFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			rgbFrame.size = rgbFrame.width * rgbFrame.height * 3;
			rgbFrame.data = new uint8_t[rgbFrame.size];
			memcpy(rgbFrame.data, rgbImage.data, rgbFrame.size);

			// Convert image to YUV1
			cv::Mat yuvImage;
			cv::cvtColor(rgbImage, yuvImage, cv::COLOR_RGB2YUV);

			// Convert frame to YUV1
			zs::Frame yuvFrame;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, yuvFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

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
				if (abs((int)val0 - (int)val1) > 1) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(BGR24_to_YUV1) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create OpenCV YUV image
			cv::Mat bgrImage = cv::Mat(cv::Size(width, height), CV_8UC3);
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				bgrImage.data[i] = (uint8_t)(rand() % 255);

			// Create YUV frame
			zs::Frame bgrFrame;
			bgrFrame.width = width;
			bgrFrame.height = height;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			bgrFrame.size = bgrFrame.width * bgrFrame.height * 3;
			bgrFrame.data = new uint8_t[bgrFrame.size];
			memcpy(bgrFrame.data, bgrImage.data, bgrFrame.size);

			// Convert image to YUV1
			cv::Mat yuvImage;
			cv::cvtColor(bgrImage, yuvImage, cv::COLOR_BGR2YUV);

			// Convert frame to YUV1
			zs::Frame yuvFrame;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, yuvFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

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


		TEST_METHOD(YUV1_to_BGR24) {

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
			cv::Mat bgrImage;
			cv::cvtColor(yuvImage, bgrImage, cv::COLOR_YUV2BGR);

			// Convert frame to RGB
			zs::Frame bgrFrame;
			bgrFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuvFrame, bgrFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare data
			uint8_t val0, val1;
			for (size_t i = 0; i < (size_t)yuvFrame.size; ++i) {
				val0 = bgrFrame.data[i];
				val1 = bgrImage.data[i];
				if (abs((int)val0 - (int)val1) > 1) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUV1_to_UYVY) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create YUV frame
			zs::Frame yuvFrame;
			yuvFrame.width = width;
			yuvFrame.height = height;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			yuvFrame.size = yuvFrame.width * yuvFrame.height * 3;
			yuvFrame.data = new uint8_t[yuvFrame.size];
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				yuvFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuvFrame, uyvyFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)uyvyFrame.size; i = i + 4) {
				if (abs((int)uyvyFrame.data[i] - (int)yuvFrame.data[p + 1]) > 1) {
					Assert::Fail(L"U data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i + 1] - (int)yuvFrame.data[p]) > 1) {
					Assert::Fail(L"Y0 data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i + 2] - (int)yuvFrame.data[p + 2]) > 1) {
					Assert::Fail(L"V data not equal");
					return;
				}//if...
				if (abs((int)uyvyFrame.data[i + 3] - (int)yuvFrame.data[p + 3]) > 1) {
					Assert::Fail(L"Y1 data not equal");
					return;
				}//if...
				p = p + 6;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUV1_to_YUY2) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create YUV frame
			zs::Frame yuvFrame;
			yuvFrame.width = width;
			yuvFrame.height = height;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			yuvFrame.size = yuvFrame.width * yuvFrame.height * 3;
			yuvFrame.data = new uint8_t[yuvFrame.size];
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				yuvFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuvFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)yuy2Frame.size; i = i + 4) {
				if (abs((int)yuy2Frame.data[i] - (int)yuvFrame.data[p]) > 1) {
					Assert::Fail(L"Y0 data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 1] - (int)yuvFrame.data[p + 1]) > 1) {
					Assert::Fail(L"U data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 3] - (int)yuvFrame.data[p + 2]) > 1) {
					Assert::Fail(L"V data not equal");
					return;
				}//if...
				if (abs((int)yuy2Frame.data[i + 2] - (int)yuvFrame.data[p + 3]) > 1) {
					Assert::Fail(L"Y1 data not equal");
					return;
				}//if...
				p = p + 6;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUV1_to_Y800) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create YUV frame
			zs::Frame yuvFrame;
			yuvFrame.width = width;
			yuvFrame.height = height;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			yuvFrame.size = yuvFrame.width * yuvFrame.height * 3;
			yuvFrame.data = new uint8_t[yuvFrame.size];
			for (size_t i = 0; i < (size_t)width * (size_t)height * 3; ++i)
				yuvFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame grayFrame;
			grayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuvFrame, grayFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (yuvFrame.data[p] != grayFrame.data[i]) {
					Assert::Fail(L"Y data not equal");
					return;
				}//if...
				p = p + 3;
			}//for...

		}//TEST_METHOD...


		TEST_METHOD(YUV1_to_NV12) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create YUV frame
			zs::Frame yuvFrame;
			yuvFrame.width = width;
			yuvFrame.height = height;
			yuvFrame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			yuvFrame.size = yuvFrame.width * yuvFrame.height * 3;
			yuvFrame.data = new uint8_t[yuvFrame.size];
			for (size_t i = 0; i < yuvFrame.size; ++i)
				yuvFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to NV12
			zs::Frame nv12Frame;
			nv12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuvFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare data
			size_t p = nv12Frame.height;
			for (size_t i = 0; i < (size_t)nv12Frame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)nv12Frame.width; j = j + 2) {
					if (abs((int)yuvFrame.data[i * (size_t)nv12Frame.width * 3 + j * 3] -
						(int)nv12Frame.data[i * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"Y00 data not equal");
						return;
					}//if...
					if (abs((int)yuvFrame.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 3] -
						(int)nv12Frame.data[i * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"Y01 data not equal");
						return;
					}//if...
					if (abs((int)yuvFrame.data[(i + 1) * (size_t)nv12Frame.width * 3 + j * 3] -
						(int)nv12Frame.data[(i + 1) * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"Y10 data not equal");
						return;
					}//if...
					if (abs((int)yuvFrame.data[(i + 1) * (size_t)nv12Frame.width * 3 + j * 3 + 3] -
						(int)nv12Frame.data[(i + 1) * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"Y11 data not equal");
						return;
					}//if...
					if (abs((int)yuvFrame.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 1] -
						(int)nv12Frame.data[p * (size_t)nv12Frame.width + j]) > 1) {
						Assert::Fail(L"U data not equal");
						return;
					}//if...
					if (abs((int)yuvFrame.data[i * (size_t)nv12Frame.width * 3 + j * 3 + 2] -
						(int)nv12Frame.data[p * (size_t)nv12Frame.width + j + 1]) > 1) {
						Assert::Fail(L"V data not equal");
						return;
					}//if...
				}//for...
				++p;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(UYVY_to_BGR24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create source BGR image
			zs::Frame sourceBGRFrame;
			sourceBGRFrame.width = width;
			sourceBGRFrame.height = height;
			sourceBGRFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			sourceBGRFrame.size = sourceBGRFrame.width * sourceBGRFrame.height * 3;
			sourceBGRFrame.sourceID = 1;
			sourceBGRFrame.frameID = 2;
			sourceBGRFrame.data = new uint8_t[sourceBGRFrame.size];

			// Convert source RGB to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(sourceBGRFrame, uyvyFrame)) {
				Assert::Fail(L"Convert function RGB_to_UYVY returned FALSE");
				return;
			}//if...

			// Convert UYVY to RGB
			zs::Frame resultBGRFrame;
			resultBGRFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			if (!converter.Convert(uyvyFrame, resultBGRFrame)) {
				Assert::Fail(L"Convert function UYVY_to_BGR returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (sourceBGRFrame.width != resultBGRFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (sourceBGRFrame.height != resultBGRFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (sourceBGRFrame.size != resultBGRFrame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (sourceBGRFrame.frameID != resultBGRFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (sourceBGRFrame.sourceID != resultBGRFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)resultBGRFrame.size; i = i + 6) {
				if (abs((int)sourceBGRFrame.data[i] - (int)resultBGRFrame.data[i]) > 1) {
					Assert::Fail(L"R data not equal");
					return;
				}//if...
				if (abs((int)sourceBGRFrame.data[i + 1] - (int)resultBGRFrame.data[i + 1]) > 1) {
					Assert::Fail(L"G data not equal");
					return;
				}//if...
				if (abs((int)sourceBGRFrame.data[i + 2] - (int)resultBGRFrame.data[i + 2]) > 1) {
					Assert::Fail(L"B data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(UYVY_to_YUY2) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create YUY2 frame
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			yuy2Frame.width = width;
			yuy2Frame.height = height;
			yuy2Frame.frameID = 2;
			yuy2Frame.sourceID = 1;
			yuy2Frame.size = yuy2Frame.width * yuy2Frame.height * 2;
			yuy2Frame.data = new uint8_t[yuy2Frame.size];
			for (size_t i = 0; i < (size_t)yuy2Frame.size; ++i)
				yuy2Frame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuy2Frame, uyvyFrame)) {
				Assert::Fail(L"Convert function UYVY_to_BGR returned FALSE");
				return;
			}//if...

			// Convert frame to YUY2
			zs::Frame resultYUY2Frame;
			resultYUY2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			if (!converter.Convert(uyvyFrame, resultYUY2Frame)) {
				Assert::Fail(L"Convert function UYVY_to_BGR returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (yuy2Frame.width != resultYUY2Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (yuy2Frame.height != resultYUY2Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (yuy2Frame.size != resultYUY2Frame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (yuy2Frame.frameID != resultYUY2Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (yuy2Frame.sourceID != resultYUY2Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)yuy2Frame.size; ++i) {
				if (yuy2Frame.data[i] != resultYUY2Frame.data[i]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(UYVY_to_Y800) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create GRAY frame
			zs::Frame grayFrame;
			grayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			grayFrame.width = width;
			grayFrame.height = height;
			grayFrame.frameID = 2;
			grayFrame.sourceID = 1;
			grayFrame.size = grayFrame.width * grayFrame.height;
			grayFrame.data = new uint8_t[grayFrame.size];
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i)
				grayFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, uyvyFrame)) {
				Assert::Fail(L"Convert function UYVY_to_BGR returned FALSE");
				return;
			}//if...

			// Convert frame to Y800
			zs::Frame resultGrayFrame;
			resultGrayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			if (!converter.Convert(uyvyFrame, resultGrayFrame)) {
				Assert::Fail(L"Convert function UYVY_to_BGR returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != resultGrayFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != resultGrayFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (grayFrame.size != resultGrayFrame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (grayFrame.frameID != resultGrayFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != resultGrayFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (grayFrame.data[i] != resultGrayFrame.data[i]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(UYVY_to_NV12) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create NV12 frame
			zs::Frame nv12Frame;
			nv12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			nv12Frame.width = width;
			nv12Frame.height = height;
			nv12Frame.frameID = 2;
			nv12Frame.sourceID = 1;
			nv12Frame.size = nv12Frame.width * (nv12Frame.height + nv12Frame.height / 2);
			nv12Frame.data = new uint8_t[nv12Frame.size];
			for (size_t i = 0; i < (size_t)nv12Frame.size; ++i)
				nv12Frame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(nv12Frame, uyvyFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert frame to NV12
			zs::Frame resultNV12Frame;
			resultNV12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			if (!converter.Convert(uyvyFrame, resultNV12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (nv12Frame.width != resultNV12Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (nv12Frame.height != resultNV12Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (nv12Frame.size != resultNV12Frame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (nv12Frame.frameID != resultNV12Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (nv12Frame.sourceID != resultNV12Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)nv12Frame.size; ++i) {
				if (nv12Frame.data[i] != resultNV12Frame.data[i]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(UYVY_to_YUV1) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create YUV1 frame
			zs::Frame yuv1Frame;
			yuv1Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			yuv1Frame.width = width;
			yuv1Frame.height = height;
			yuv1Frame.frameID = 2;
			yuv1Frame.sourceID = 1;
			yuv1Frame.size = yuv1Frame.width * yuv1Frame.height * 3;
			yuv1Frame.data = new uint8_t[yuv1Frame.size];
			for (size_t i = 0; i < (size_t)yuv1Frame.size; ++i)
				yuv1Frame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuv1Frame, uyvyFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert frame to YUV1
			zs::Frame resultYUV1Frame;
			resultYUV1Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			if (!converter.Convert(uyvyFrame, resultYUV1Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (yuv1Frame.width != resultYUV1Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (yuv1Frame.height != resultYUV1Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (yuv1Frame.size != resultYUV1Frame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (yuv1Frame.frameID != resultYUV1Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (yuv1Frame.sourceID != resultYUV1Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)resultYUV1Frame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)resultYUV1Frame.width; j = j + 2) {
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3]) {
						Assert::Fail(L"Y00 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3]) {
						Assert::Fail(L"Y01 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3] !=
						yuv1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3]) {
						Assert::Fail(L"Y10 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3] !=
						yuv1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3]) {
						Assert::Fail(L"Y11 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 1] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 1]) {
						Assert::Fail(L"U Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 2] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 2]) {
						Assert::Fail(L"Y Data not equal");
						return;
					}//if...
				}//for...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUY2_to_RGB24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create source RGB image
			zs::Frame sourceRGBFrame;
			sourceRGBFrame.width = width;
			sourceRGBFrame.height = height;
			sourceRGBFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			sourceRGBFrame.size = sourceRGBFrame.width * sourceRGBFrame.height * 3;
			sourceRGBFrame.sourceID = 1;
			sourceRGBFrame.frameID = 2;
			sourceRGBFrame.data = new uint8_t[sourceRGBFrame.size];

			// Convert source RGB to UYVY
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(sourceRGBFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function RGB_to_UYVY returned FALSE");
				return;
			}//if...

			// Convert UYVY to RGB
			zs::Frame resultRGBFrame;
			resultRGBFrame.fourcc = MAKE_FOURCC_CODE('R', 'G', 'B', 'R');
			if (!converter.Convert(yuy2Frame, resultRGBFrame)) {
				Assert::Fail(L"Convert function UYVY_to_RGB returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (sourceRGBFrame.width != resultRGBFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (sourceRGBFrame.height != resultRGBFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (sourceRGBFrame.size != resultRGBFrame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (sourceRGBFrame.frameID != resultRGBFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (sourceRGBFrame.sourceID != resultRGBFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)resultRGBFrame.size; i = i + 6) {
				if (abs((int)sourceRGBFrame.data[i] - (int)resultRGBFrame.data[i]) > 1) {
					Assert::Fail(L"R data not equal");
					return;
				}//if...
				if (abs((int)sourceRGBFrame.data[i + 1] - (int)resultRGBFrame.data[i + 1]) > 1) {
					Assert::Fail(L"G data not equal");
					return;
				}//if...
				if (abs((int)sourceRGBFrame.data[i + 2] - (int)resultRGBFrame.data[i + 2]) > 1) {
					Assert::Fail(L"B data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUY2_to_BGR24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create source BGR image
			zs::Frame sourceBGRFrame;
			sourceBGRFrame.width = width;
			sourceBGRFrame.height = height;
			sourceBGRFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			sourceBGRFrame.size = sourceBGRFrame.width * sourceBGRFrame.height * 3;
			sourceBGRFrame.sourceID = 1;
			sourceBGRFrame.frameID = 2;
			sourceBGRFrame.data = new uint8_t[sourceBGRFrame.size];

			// Convert source RGB to UYVY
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(sourceBGRFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function RGB_to_UYVY returned FALSE");
				return;
			}//if...

			// Convert UYVY to RGB
			zs::Frame resultBGRFrame;
			resultBGRFrame.fourcc = MAKE_FOURCC_CODE('B', 'G', 'R', 'B');
			if (!converter.Convert(yuy2Frame, resultBGRFrame)) {
				Assert::Fail(L"Convert function UYVY_to_BGR returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (sourceBGRFrame.width != resultBGRFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (sourceBGRFrame.height != resultBGRFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (sourceBGRFrame.size != resultBGRFrame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (sourceBGRFrame.frameID != resultBGRFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (sourceBGRFrame.sourceID != resultBGRFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)resultBGRFrame.size; i = i + 6) {
				if (abs((int)sourceBGRFrame.data[i] - (int)resultBGRFrame.data[i]) > 1) {
					Assert::Fail(L"R data not equal");
					return;
				}//if...
				if (abs((int)sourceBGRFrame.data[i + 1] - (int)resultBGRFrame.data[i + 1]) > 1) {
					Assert::Fail(L"G data not equal");
					return;
				}//if...
				if (abs((int)sourceBGRFrame.data[i + 2] - (int)resultBGRFrame.data[i + 2]) > 1) {
					Assert::Fail(L"B data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUY2_to_UYVY) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create UYVY frame
			zs::Frame uyvyFrame;
			uyvyFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			uyvyFrame.width = width;
			uyvyFrame.height = height;
			uyvyFrame.frameID = 2;
			uyvyFrame.sourceID = 1;
			uyvyFrame.size = uyvyFrame.width * uyvyFrame.height * 2;
			uyvyFrame.data = new uint8_t[uyvyFrame.size];
			for (size_t i = 0; i < (size_t)uyvyFrame.size; ++i)
				uyvyFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to YUY2
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(uyvyFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert frame to UYVY
			zs::Frame resultUYVYFrame;
			resultUYVYFrame.fourcc = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y');
			if (!converter.Convert(yuy2Frame, resultUYVYFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (uyvyFrame.width != resultUYVYFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (uyvyFrame.height != resultUYVYFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (uyvyFrame.size != resultUYVYFrame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (uyvyFrame.frameID != resultUYVYFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (uyvyFrame.sourceID != resultUYVYFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)uyvyFrame.size; ++i) {
				if (uyvyFrame.data[i] != resultUYVYFrame.data[i]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUY2_to_Y800) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create GRAY frame
			zs::Frame grayFrame;
			grayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			grayFrame.width = width;
			grayFrame.height = height;
			grayFrame.frameID = 2;
			grayFrame.sourceID = 1;
			grayFrame.size = grayFrame.width * grayFrame.height;
			grayFrame.data = new uint8_t[grayFrame.size];
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i)
				grayFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert frame to Y800
			zs::Frame resultGrayFrame;
			resultGrayFrame.fourcc = MAKE_FOURCC_CODE('Y', '8', '0', '0');
			if (!converter.Convert(yuy2Frame, resultGrayFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != resultGrayFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != resultGrayFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (grayFrame.size != resultGrayFrame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (grayFrame.frameID != resultGrayFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != resultGrayFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (grayFrame.data[i] != resultGrayFrame.data[i]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUY2_to_NV12) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create NV12 frame
			zs::Frame nv12Frame;
			nv12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			nv12Frame.width = width;
			nv12Frame.height = height;
			nv12Frame.frameID = 2;
			nv12Frame.sourceID = 1;
			nv12Frame.size = nv12Frame.width * (nv12Frame.height + nv12Frame.height / 2);
			nv12Frame.data = new uint8_t[nv12Frame.size];
			for (size_t i = 0; i < (size_t)nv12Frame.size; ++i)
				nv12Frame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to UYVY
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(nv12Frame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert frame to NV12
			zs::Frame resultNV12Frame;
			resultNV12Frame.fourcc = MAKE_FOURCC_CODE('N', 'V', '1', '2');
			if (!converter.Convert(yuy2Frame, resultNV12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (nv12Frame.width != resultNV12Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (nv12Frame.height != resultNV12Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (nv12Frame.size != resultNV12Frame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (nv12Frame.frameID != resultNV12Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (nv12Frame.sourceID != resultNV12Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)nv12Frame.size; ++i) {
				if (nv12Frame.data[i] != resultNV12Frame.data[i]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(YUY2_to_YUV1) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create YUV1 frame
			zs::Frame yuv1Frame;
			yuv1Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			yuv1Frame.width = width;
			yuv1Frame.height = height;
			yuv1Frame.frameID = 2;
			yuv1Frame.sourceID = 1;
			yuv1Frame.size = yuv1Frame.width * yuv1Frame.height * 3;
			yuv1Frame.data = new uint8_t[yuv1Frame.size];
			for (size_t i = 0; i < (size_t)yuv1Frame.size; ++i)
				yuv1Frame.data[i] = (uint8_t)(rand() % 255);

			// Convert frame to YUY2
			zs::Frame yuy2Frame;
			yuy2Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2');
			zs::PixelFormatConverter converter;
			if (!converter.Convert(yuv1Frame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert frame to YUV1
			zs::Frame resultYUV1Frame;
			resultYUV1Frame.fourcc = MAKE_FOURCC_CODE('Y', 'U', 'V', '1');
			if (!converter.Convert(yuy2Frame, resultYUV1Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (yuv1Frame.width != resultYUV1Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (yuv1Frame.height != resultYUV1Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (yuv1Frame.size != resultYUV1Frame.size) {
				Assert::Fail(L"Size not valid");
				return;
			}//if...				
			if (yuv1Frame.frameID != resultYUV1Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (yuv1Frame.sourceID != resultYUV1Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)resultYUV1Frame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)resultYUV1Frame.width; j = j + 2) {
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3]) {
						Assert::Fail(L"Y00 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3]) {
						Assert::Fail(L"Y01 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3] !=
						yuv1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3]) {
						Assert::Fail(L"Y10 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3] !=
						yuv1Frame.data[(i + 1) * (size_t)resultYUV1Frame.width * 3 + j * 3 + 3]) {
						Assert::Fail(L"Y11 Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 1] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 1]) {
						Assert::Fail(L"U Data not equal");
						return;
					}//if...
					if (resultYUV1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 2] !=
						yuv1Frame.data[i * (size_t)resultYUV1Frame.width * 3 + j * 3 + 2]) {
						Assert::Fail(L"Y Data not equal");
						return;
					}//if...
				}//for...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(Y800_to_RGB24) {

			// Create GRAY frame
			const uint32_t width = 1280;
			const uint32_t height = 1024;
			zs::Frame grayFrame(width, height, MAKE_FOURCC_CODE('Y', '8', '0', '0'));
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				grayFrame.data[i] = (uint8_t)(rand() % 255);
			}//for...

			// Conevert to RGB24
			zs::Frame rgbFrame(width, height, MAKE_FOURCC_CODE('R', 'G', 'B', 'R'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, rgbFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != rgbFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != rgbFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...			
			if (grayFrame.frameID != rgbFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != rgbFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (grayFrame.data[i] != rgbFrame.data[p]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
				if (grayFrame.data[i] != rgbFrame.data[p + 1]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
				if (grayFrame.data[i] != rgbFrame.data[p + 2]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
				p = p + 3;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(Y800_to_BGR24) {

			// Create GRAY frame
			const uint32_t width = 1280;
			const uint32_t height = 1024;
			zs::Frame grayFrame(width, height, MAKE_FOURCC_CODE('Y', '8', '0', '0'));
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				grayFrame.data[i] = (uint8_t)(rand() % 255);
			}//for...

			// Conevert to BGR24
			zs::Frame bgrFrame(width, height, MAKE_FOURCC_CODE('B', 'G', 'R', 'B'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, bgrFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != bgrFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != bgrFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...			
			if (grayFrame.frameID != bgrFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != bgrFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (grayFrame.data[i] != bgrFrame.data[p]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
				if (grayFrame.data[i] != bgrFrame.data[p + 1]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
				if (grayFrame.data[i] != bgrFrame.data[p + 2]) {
					Assert::Fail(L"Data not equal");
					return;
				}//if...
				p = p + 3;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(Y800_to_UYVY) {

			// Create GRAY frame
			const uint32_t width = 1280;
			const uint32_t height = 1024;
			zs::Frame grayFrame(width, height, MAKE_FOURCC_CODE('Y', '8', '0', '0'));
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				grayFrame.data[i] = (uint8_t)(rand() % 255);
			}//for...

			// Conevert to UYVY
			zs::Frame yuvyFrame(width, height, MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, yuvyFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != yuvyFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != yuvyFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...			
			if (grayFrame.frameID != yuvyFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != yuvyFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)grayFrame.size; i = i + 2) {
				if (grayFrame.data[i] != yuvyFrame.data[p + 1]) {
					Assert::Fail(L"Y0 Data not equal");
					return;
				}//if...
				if (grayFrame.data[i + 1] != yuvyFrame.data[p + 3]) {
					Assert::Fail(L"Y1 Data not equal");
					return;
				}//if...
				if (0 != yuvyFrame.data[p]) {
					Assert::Fail(L"U Data not equal");
					return;
				}//if...
				if (0 != yuvyFrame.data[p + 2]) {
					Assert::Fail(L"V Data not equal");
					return;
				}//if...
				p = p + 4;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(Y800_to_YUY2) {

			// Create GRAY frame
			const uint32_t width = 1280;
			const uint32_t height = 1024;
			zs::Frame grayFrame(width, height, MAKE_FOURCC_CODE('Y', '8', '0', '0'));
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				grayFrame.data[i] = (uint8_t)(rand() % 255);
			}//for...

			// Conevert to YUY2
			zs::Frame yuy2Frame(width, height, MAKE_FOURCC_CODE('Y', 'U', 'Y', '2'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, yuy2Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != yuy2Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != yuy2Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...			
			if (grayFrame.frameID != yuy2Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != yuy2Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)grayFrame.size; i = i + 2) {
				if (grayFrame.data[i] != yuy2Frame.data[p]) {
					Assert::Fail(L"Y0 Data not equal");
					return;
				}//if...
				if (grayFrame.data[i + 1] != yuy2Frame.data[p + 2]) {
					Assert::Fail(L"Y1 Data not equal");
					return;
				}//if...
				if (0 != yuy2Frame.data[p + 1]) {
					Assert::Fail(L"U Data not equal");
					return;
				}//if...
				if (0 != yuy2Frame.data[p + 3]) {
					Assert::Fail(L"V Data not equal");
					return;
				}//if...
				p = p + 4;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(Y800_to_NV12) {

			// Create GRAY frame
			const uint32_t width = 1280;
			const uint32_t height = 1024;
			zs::Frame grayFrame(width, height, MAKE_FOURCC_CODE('Y', '8', '0', '0'));
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				grayFrame.data[i] = (uint8_t)(rand() % 255);
			}//for...

			// Conevert to NV12
			zs::Frame nv12Frame(width, height, MAKE_FOURCC_CODE('N', 'V', '1', '2'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != nv12Frame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != nv12Frame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...			
			if (grayFrame.frameID != nv12Frame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != nv12Frame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (grayFrame.data[i] != nv12Frame.data[i]) {
					Assert::Fail(L"Y Data not equal");
					return;
				}//if...
			}//for...

			// Check U and V data
			for (size_t i = 0; i < (size_t)grayFrame.size / 2; ++i) {
				if (0 != nv12Frame.data[i + (size_t)grayFrame.size]) {
					Assert::Fail(L"U and V Data not valid");
					return;
				}//if...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(Y800_to_YUV1) {

			// Create GRAY frame
			const uint32_t width = 1280;
			const uint32_t height = 1024;
			zs::Frame grayFrame(width, height, MAKE_FOURCC_CODE('Y', '8', '0', '0'));
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				grayFrame.data[i] = (uint8_t)(rand() % 255);
			}//for...

			// Conevert to YUV1
			zs::Frame yuvFrame(width, height, MAKE_FOURCC_CODE('Y', 'U', 'V', '1'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(grayFrame, yuvFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (grayFrame.width != yuvFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (grayFrame.height != yuvFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...			
			if (grayFrame.frameID != yuvFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (grayFrame.sourceID != yuvFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			size_t p = 0;
			for (size_t i = 0; i < (size_t)grayFrame.size; ++i) {
				if (grayFrame.data[i] != yuvFrame.data[p]) {
					Assert::Fail(L"Y0 Data not equal");
					return;
				}//if...
				if (0 != yuvFrame.data[p + 1]) {
					Assert::Fail(L"U Data not equal");
					return;
				}//if...
				if (0 != yuvFrame.data[p + 2]) {
					Assert::Fail(L"V Data not equal");
					return;
				}//if...
				p = p + 3;
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(NV12_to_RGB24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create RGB frame
			zs::Frame rgbFrame(width, height, MAKE_FOURCC_CODE('R', 'G', 'B', 'R'));
			for (size_t i = 0; i < (size_t)rgbFrame.size; ++i)
				rgbFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB to NV12
			zs::Frame nv12Frame(width, height, MAKE_FOURCC_CODE('N', 'V', '1', '2'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(rgbFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert NV12 to RGB
			zs::Frame resultRgbFrame(width, height, MAKE_FOURCC_CODE('R', 'G', 'B', 'R'));
			if (!converter.Convert(nv12Frame, resultRgbFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (rgbFrame.width != resultRgbFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (rgbFrame.height != resultRgbFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (rgbFrame.frameID != resultRgbFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (rgbFrame.sourceID != resultRgbFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			uint8_t val1;
			uint8_t val2;
			for (size_t i = 0; i < (size_t)rgbFrame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)rgbFrame.width; j = j + 2) {
					val1 = rgbFrame.data[i * (size_t)rgbFrame.width * 3 + j * 3];
					val2 = resultRgbFrame.data[i * (size_t)rgbFrame.width * 3 + j * 3];
					if (abs((int)val1 - (int)val2) > 20) {
						Assert::Fail(L"R data not equal");
						return;
					}//if...
					val1 = rgbFrame.data[i * (size_t)rgbFrame.width * 3 + j * 3 + 1];
					val2 = resultRgbFrame.data[i * (size_t)rgbFrame.width * 3 + j * 3 + 1];
					if (abs((int)val1 - (int)val2) > 20) {
						Assert::Fail(L"G data not equal");
						return;
					}//if...
					val1 = rgbFrame.data[i * (size_t)rgbFrame.width * 3 + j * 3 + 2];
					val2 = resultRgbFrame.data[i * (size_t)rgbFrame.width * 3 + j * 3 + 2];
					if (abs((int)val1 - (int)val2) > 20) {
						Assert::Fail(L"B data not equal");
						return;
					}//if...
				}//for...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(NV12_to_BGR24) {

			const uint32_t width = 1280;
			const uint32_t height = 1024;

			// Create RGB frame
			zs::Frame bgrFrame(width, height, MAKE_FOURCC_CODE('B', 'G', 'R', 'B'));
			for (size_t i = 0; i < (size_t)bgrFrame.size; ++i)
				bgrFrame.data[i] = (uint8_t)(rand() % 255);

			// Convert RGB to NV12
			zs::Frame nv12Frame(width, height, MAKE_FOURCC_CODE('N', 'V', '1', '2'));
			zs::PixelFormatConverter converter;
			if (!converter.Convert(bgrFrame, nv12Frame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Convert NV12 to RGB
			zs::Frame resultBgrFrame(width, height, MAKE_FOURCC_CODE('B', 'G', 'R', 'B'));
			if (!converter.Convert(nv12Frame, resultBgrFrame)) {
				Assert::Fail(L"Convert function returned FALSE");
				return;
			}//if...

			// Compare atributes
			if (bgrFrame.width != resultBgrFrame.width) {
				Assert::Fail(L"Width not equal");
				return;
			}//if...
			if (bgrFrame.height != resultBgrFrame.height) {
				Assert::Fail(L"Height not equal");
				return;
			}//if...
			if (bgrFrame.frameID != resultBgrFrame.frameID) {
				Assert::Fail(L"frameID not equal");
				return;
			}//if...
			if (bgrFrame.sourceID != resultBgrFrame.sourceID) {
				Assert::Fail(L"SourceID not equal");
				return;
			}//if...

			// Compare data
			uint8_t val1;
			uint8_t val2;
			for (size_t i = 0; i < (size_t)bgrFrame.height; i = i + 2) {
				for (size_t j = 0; j < (size_t)bgrFrame.width; j = j + 2) {
					val1 = bgrFrame.data[i * (size_t)bgrFrame.width * 3 + j * 3];
					val2 = resultBgrFrame.data[i * (size_t)bgrFrame.width * 3 + j * 3];
					if (abs((int)val1 - (int)val2) > 20) {
						Assert::Fail(L"R data not equal");
						return;
					}//if...
					val1 = bgrFrame.data[i * (size_t)bgrFrame.width * 3 + j * 3 + 1];
					val2 = resultBgrFrame.data[i * (size_t)bgrFrame.width * 3 + j * 3 + 1];
					if (abs((int)val1 - (int)val2) > 20) {
						Assert::Fail(L"G data not equal");
						return;
					}//if...
					val1 = bgrFrame.data[i * (size_t)bgrFrame.width * 3 + j * 3 + 2];
					val2 = resultBgrFrame.data[i * (size_t)bgrFrame.width * 3 + j * 3 + 2];
					if (abs((int)val1 - (int)val2) > 20) {
						Assert::Fail(L"B data not equal");
						return;
					}//if...
				}//for...
			}//for...

		};//TEST_METHOD...


		TEST_METHOD(NV12_to_UYVY) {

			


		};//TEST_METHOD...

	};
}
