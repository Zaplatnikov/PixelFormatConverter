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

		};


	};
}
