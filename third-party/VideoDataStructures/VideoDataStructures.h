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
		/// Class destructor
		~Frame() { delete[] data; };

	};

}
