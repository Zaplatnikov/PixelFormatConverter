#pragma once
// VERSION: 1.0
// AUTHOR: Zaplatnikov Sergey (s.zaplatnikov@zaplatnikov.com)
#include <cstdint>


namespace zs {


#define MAKE_FOURCC_CODE(a,b,c,d) ( (uint32_t) (((d)<<24) | ((c)<<16) | ((b)<<8) | (a)) )


	/**
	\brief enum of valid FOURCC codes
	*/
	enum class ValidFourccCodes {

		RGB24 = MAKE_FOURCC_CODE('R', 'G', 'B', 'R'),
		BGR24 = MAKE_FOURCC_CODE('B', 'G', 'R', 'B'),
		UYVY = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y'),
		Y800 = MAKE_FOURCC_CODE('Y', '8', '0', '0'),
		YUY2 = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2'),
		YUV1 = MAKE_FOURCC_CODE('Y', 'U', 'V', '1'),
		NV12 = MAKE_FOURCC_CODE('N', 'V', '1', '2'),
		JPEG = MAKE_FOURCC_CODE('J', 'P', 'E', 'G'),
		JPG2 = MAKE_FOURCC_CODE('J', 'P', 'G', '2'),
		H264 = MAKE_FOURCC_CODE('H', '2', '6', '4'),
		H265 = MAKE_FOURCC_CODE('H', '2', '6', '5')

	};


	/**
	\brief Universal video frame class
	*/
	class Frame {

	public:

		/// Pointer to data buffer
		uint8_t* data;
		/// Frame width (pixels)
		uint32_t width;
		/// Frame height (pixels)
		uint32_t height;
		/// Frame data size (bytes)
		uint32_t size;
		/// FOURCC code of data format
		uint32_t fourcc;
		/// ID of video source
		uint32_t sourceID;
		/// ID of frame
		uint32_t frameID;
		/// Class constructor
		Frame() : data(nullptr), width(0), height(0), size(0), fourcc(0), sourceID(0), frameID(0) { };
		/// Class constructor
		Frame(uint32_t width, uint32_t height, uint32_t fourcc) : data(nullptr), width(0), height(0), size(0), fourcc(0), sourceID(0), frameID(0) {
			
			if (width == 0 || height == 0)
				return;
			switch (fourcc) {
			case (uint32_t)ValidFourccCodes::RGB24:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 3;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::BGR24:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 3;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::UYVY:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 2;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::YUY2:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 2;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::Y800:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::NV12:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * (height + height / 2);
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::YUV1:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 3;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::JPEG:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 4;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::JPG2:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 4;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::H264:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 4;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			case (uint32_t)ValidFourccCodes::H265:
				this->width = width;
				this->height = height;
				this->fourcc = fourcc;
				size = width * height * 4;
				data = new uint8_t[size];
				memset(data, 0, size);
				break;
			default:
				return;
			}		
		};
		/// Class destructor
		~Frame() { delete[] data; };

	};

}
