#include <cstring>
#include "PixelFormatConverter.h"


#define MIN_FRAME_WIDTH 4
#define MIN_FRAME_HEIGHT 4


zs::PixelFormatConverter::PixelFormatConverter() {


}//zs::PixelFormatConverter::PixelFormatConverter...


zs::PixelFormatConverter::~PixelFormatConverter() {


}//zs::PixelFormatConverter::~PixelFormatConverter...


bool zs::PixelFormatConverter::Convert(Frame& src, Frame& dst) {

	// Check input pixel format
	switch (src.fourcc) {
	case (uint32_t)ValidFourccCodes::RGB24:
		// Check output pixel format
		switch (dst.fourcc) {
		case (uint32_t)ValidFourccCodes::RGB24:
			return Copy(src, dst);
		case (uint32_t)ValidFourccCodes::BGR24:
			return RGB24_to_BGR24(src, dst);
		case (uint32_t)ValidFourccCodes::UYVY:
			return RGB24_to_UYVY(src, dst);
		case (uint32_t)ValidFourccCodes::YUY2:
			return RGB24_to_YUY2(src, dst);
		case (uint32_t)ValidFourccCodes::Y800:
			return RGB24_to_Y800(src, dst);
		case (uint32_t)ValidFourccCodes::NV12:
			return RGB24_to_NV12(src, dst);
		default:
			return false;
		}
		break;
	case (uint32_t)ValidFourccCodes::BGR24:
		// Check output pixel format
		switch (dst.fourcc) {
		case (uint32_t)ValidFourccCodes::RGB24:
			return BGR24_to_RGB24(src, dst);
		case (uint32_t)ValidFourccCodes::BGR24:
			return Copy(src, dst);
		case (uint32_t)ValidFourccCodes::UYVY:
			return BGR24_to_UYVY(src, dst);
		case (uint32_t)ValidFourccCodes::YUY2:
			return BGR24_to_YUY2(src, dst);
		case (uint32_t)ValidFourccCodes::Y800:
			return BGR24_to_Y800(src, dst);
		case (uint32_t)ValidFourccCodes::NV12:
			return BGR24_to_NV12(src, dst);
		default:
			return false;
		}
		break;
	case (uint32_t)ValidFourccCodes::UYVY:
		// Check output pixel format
		switch (dst.fourcc) {
		case (uint32_t)ValidFourccCodes::RGB24:
			return UYVY_to_RGB24(src, dst);
		case (uint32_t)ValidFourccCodes::BGR24:
			return UYVY_to_BGR24(src, dst);
		case (uint32_t)ValidFourccCodes::UYVY:
			return Copy(src, dst);
		case (uint32_t)ValidFourccCodes::YUY2:
			return UYVY_to_YUY2(src, dst);
		case (uint32_t)ValidFourccCodes::Y800:
			return UYVY_to_Y800(src, dst);
		case (uint32_t)ValidFourccCodes::NV12:
			return UYVY_to_NV12(src, dst);
		default:
			return false;
		}
		break;
	case (uint32_t)ValidFourccCodes::YUY2:
		// Check output pixel format
		switch (dst.fourcc) {
		case (uint32_t)ValidFourccCodes::RGB24:
			return YUY2_to_RGB24(src, dst);
		case (uint32_t)ValidFourccCodes::BGR24:
			return YUY2_to_BGR24(src, dst);
		case (uint32_t)ValidFourccCodes::UYVY:
			return YUY2_to_UYVY(src, dst);
		case (uint32_t)ValidFourccCodes::YUY2:
			return Copy(src, dst);
		case (uint32_t)ValidFourccCodes::Y800:
			return YUY2_to_Y800(src, dst);
		case (uint32_t)ValidFourccCodes::NV12:
			return YUY2_to_NV12(src, dst);
		default:
			return false;
		}
		break;
	case (uint32_t)ValidFourccCodes::Y800:
		// Check output pixel format
		switch (dst.fourcc) {
		case (uint32_t)ValidFourccCodes::RGB24:
			return Y800_to_RGB24(src, dst);
		case (uint32_t)ValidFourccCodes::BGR24:
			return Y800_to_BGR24(src, dst);
		case (uint32_t)ValidFourccCodes::UYVY:
			return Y800_to_UYVY(src, dst);
		case (uint32_t)ValidFourccCodes::YUY2:
			return Y800_to_YUY2(src, dst);
		case (uint32_t)ValidFourccCodes::Y800:
			return Copy(src, dst);
		case (uint32_t)ValidFourccCodes::NV12:
			return Y800_to_NV12(src, dst);
		default:
			return false;
		}
		break;
	case (uint32_t)ValidFourccCodes::NV12:
		// Check output pixel format
		switch (dst.fourcc) {
		case (uint32_t)ValidFourccCodes::RGB24:
			return NV12_to_RGB24(src, dst);
		case (uint32_t)ValidFourccCodes::BGR24:
			return NV12_to_RGB24(src, dst);
		case (uint32_t)ValidFourccCodes::UYVY:
			return NV12_to_UYVY(src, dst);
		case (uint32_t)ValidFourccCodes::YUY2:
			return NV12_to_YUY2(src, dst);
		case (uint32_t)ValidFourccCodes::Y800:
			return NV12_to_Y800(src, dst);
		case (uint32_t)ValidFourccCodes::NV12:
			return Copy(src, dst);
		default:
			return false;
		}
		break;
	default:
		return false;
	}

	return false;

}//bool zs::PixelFormatConverter::Convert...


void zs::PixelFormatConverter::GetVersion(uint32_t& major, uint32_t& minor) {

	major = majorVersion;
	minor = minorVersion;

}//void zs::PixelFormatConverter::GetVersion...


bool zs::PixelFormatConverter::Copy(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size < src.width * src.height ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output data
	if (dst.size != src.size || dst.data == nullptr) {
		delete[] dst.data;
		dst.size = src.size;
		dst.data = new uint8_t[src.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.fourcc = src.fourcc;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Copy data
	memcpy(dst.data, src.data, src.size);

	return true;

}//bool zs::PixelFormatConverter::Copy...


bool zs::PixelFormatConverter::RGB24_to_BGR24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.size) {
		delete[] dst.data;
		dst.size = src.size;
		dst.data = new uint8_t[src.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	for (size_t i = 0; i < (size_t)src.size; i = i + 3) {
		dst.data[i] = src.data[i + 2];
		dst.data[i + 1] = src.data[i + 1];
		dst.data[i + 2] = src.data[i];
	}

	return true;

}//bool zs::PixelFormatConverter::RGB24_to_BGR24...


bool zs::PixelFormatConverter::RGB24_to_UYVY(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float R0, G0, B0, R1, G1, B1;
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 6) {
		R0 = (float)src.data[i];
		G0 = (float)src.data[i + 1];
		B0 = (float)src.data[i + 2];
		R1 = (float)src.data[i + 3];
		G1 = (float)src.data[i + 4];
		B1 = (float)src.data[i + 5];
		dst.data[j + 1] = (uint8_t)(0.299f * R0 + 0.587f * G0 + 0.114f * B0);
		dst.data[j + 3] = (uint8_t)(0.299f * R1 + 0.587f * G1 + 0.114f * B1);
		R0 = (R0 + R1) / 2.0f;
		G0 = (G0 + G1) / 2.0f;
		B0 = (B0 + B1) / 2.0f;
		dst.data[j] = (uint8_t)(-0.147f * R0 - 0.289f * G0 + 0.436f * B0);
		dst.data[j + 2] = (uint8_t)(0.615f * R0 - 0.515f * G0 + 0.100f * B0);
		j += 4;
	}

	return true;

}//bool zs::PixelFormatConverter::RGB24_to_UYVY...


bool zs::PixelFormatConverter::RGB24_to_YUY2(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float R0, G0, B0, R1, G1, B1;
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 6) {
		R0 = (float)src.data[i];
		G0 = (float)src.data[i + 1];
		B0 = (float)src.data[i + 2];
		R1 = (float)src.data[i + 3];
		G1 = (float)src.data[i + 4];
		B1 = (float)src.data[i + 5];
		dst.data[j] = (uint8_t)(0.299f * R0 + 0.587f * G0 + 0.114f * B0);
		dst.data[j + 2] = (uint8_t)(0.299f * R1 + 0.587f * G1 + 0.114f * B1);
		R0 = (R0 + R1) / 2.0f;
		G0 = (G0 + G1) / 2.0f;
		B0 = (B0 + B1) / 2.0f;
		dst.data[j + 1] = (uint8_t)(-0.147f * R0 - 0.289f * G0 + 0.436f * B0);
		dst.data[j + 3] = (uint8_t)(0.615f * R0 - 0.515f * G0 + 0.100f * B0);
		j += 4;
	}//for...

	return true;

}//bool zs::PixelFormatConverter::RGB24_to_YUY2...


bool zs::PixelFormatConverter::RGB24_to_Y800(Frame& src, Frame& dst) {
	
	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height) {
		delete[] dst.data;
		dst.size = src.width * src.height;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 3) {
		dst.data[j] = (uint8_t)(0.3f * (float)src.data[i] + 0.59f * (float)src.data[i + 1] + 0.11f * (float)src.data[i + 2]);
		++j;
	}

	return true;

}//bool zs::PixelFormatConverter::RGB24_to_Y800...


bool zs::PixelFormatConverter::RGB24_to_NV12(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != (uint32_t)((float)(src.width * src.height) * 1.5f)) {
		delete[] dst.data;
		dst.size = (uint32_t)((float)(src.width * src.height) * 1.5f);
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float R00, G00, B00, R01, G01, B01, R10, G10, B10, R11, G11, B11;
	size_t pos;
	size_t k = 0;
	size_t t = 0;
	size_t p = 0;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		t = 0;
		for (size_t j = 0; j < (size_t)src.width * 3; j = j + 6) {
			pos = i * (size_t)src.width * 3 + j;
			R00 = (float)src.data[pos];
			G00 = (float)src.data[pos + 1];
			B00 = (float)src.data[pos + 2];
			pos = pos + 3;
			R01 = (float)src.data[pos];
			G01 = (float)src.data[pos + 1];
			B01 = (float)src.data[pos + 2];
			pos = (i + 1) * (size_t)src.width * 3 + j;
			R10 = (float)src.data[pos];
			G10 = (float)src.data[pos + 1];
			B10 = (float)src.data[pos + 2];
			pos = pos + 3;
			R11 = (float)src.data[pos];
			G11 = (float)src.data[pos + 1];
			B11 = (float)src.data[pos + 2];
			dst.data[k * dst.width + t] = (uint8_t)(0.299f * R00 + 0.587f * G00 + 0.114f * B00);
			dst.data[k * dst.width + t + 1] = (uint8_t)(0.299f * R01 + 0.587f * G01 + 0.114f * B01);
			dst.data[(k + 1) * dst.width + t] = (uint8_t)(0.299f * R10 + 0.587f * G10 + 0.114f * B10);
			dst.data[(k + 1) * dst.width + t + 1] = (uint8_t)(0.299f * R11 + 0.587f * G11 + 0.114f * B11);
			R00 = (R00 + R01 + R10 + R11) / 4.0f;
			G00 = (G00 + G01 + G10 + G11) / 4.0f;
			B00 = (B00 + B01 + B10 + B11) / 4.0f;
			dst.data[(dst.height + p) * dst.width + t] = (uint8_t)(-0.147f * R00 - 0.289f * G00 + 0.436f * B00);
			dst.data[(dst.height + p) * dst.width + t + 1] = (uint8_t)(0.615f * R00 - 0.515f * G00 + 0.100f * B00);
			t = t + 2;
		}
		k = k + 2;
		++p;
	}

	return true;

}//bool zs::PixelFormatConverter::RGB24_to_NV12...


bool zs::PixelFormatConverter::BGR24_to_RGB24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.size) {
		delete[] dst.data;
		dst.size = src.size;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	for (size_t i = 0; i < (size_t)src.size; i = i + 3) {
		dst.data[i] = src.data[i + 2];
		dst.data[i + 1] = src.data[i + 1];
		dst.data[i + 2] = src.data[i];
	}

	return true;

}//bool zs::PixelFormatConverter::BGR24_to_RGB24...


bool zs::PixelFormatConverter::BGR24_to_UYVY(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float R0, G0, B0, R1, G1, B1;
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 6) {
		R0 = (float)src.data[i + 2];
		G0 = (float)src.data[i + 1];
		B0 = (float)src.data[i];
		R1 = (float)src.data[i + 5];
		G1 = (float)src.data[i + 4];
		B1 = (float)src.data[i + 3];
		dst.data[j + 1] = (uint8_t)(0.299f * R0 + 0.587f * G0 + 0.114f * B0);
		dst.data[j + 3] = (uint8_t)(0.299f * R1 + 0.587f * G1 + 0.114f * B1);
		R0 = (R0 + R1) / 2.0f;
		G0 = (G0 + G1) / 2.0f;
		B0 = (B0 + B1) / 2.0f;
		dst.data[j] = (uint8_t)(-0.147f * R0 - 0.289f * G0 + 0.436f * B0);
		dst.data[j + 2] = (uint8_t)(0.615f * R0 - 0.515f * G0 + 0.100f * B0);
		j += 4;
	}

	return true;

}//bool zs::PixelFormatConverter::BGR24_to_UYVY...


bool zs::PixelFormatConverter::BGR24_to_YUY2(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float R0, G0, B0, R1, G1, B1;
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 6) {
		R0 = (float)src.data[i + 2];
		G0 = (float)src.data[i + 1];
		B0 = (float)src.data[i];
		R1 = (float)src.data[i + 5];
		G1 = (float)src.data[i + 4];
		B1 = (float)src.data[i + 3];
		dst.data[j] = (uint8_t)(0.299f * R0 + 0.587f * G0 + 0.114f * B0);
		dst.data[j + 2] = (uint8_t)(0.299f * R1 + 0.587f * G1 + 0.114f * B1);
		R0 = (R0 + R1) / 2.0f;
		G0 = (G0 + G1) / 2.0f;
		B0 = (B0 + B1) / 2.0f;
		dst.data[j + 1] = (uint8_t)(-0.147f * R0 - 0.289f * G0 + 0.436f * B0);
		dst.data[j + 3] = (uint8_t)(0.615f * R0 - 0.515f * G0 + 0.100f * B0);
		j += 4;
	}//for...

	return true;

}//bool zs::PixelFormatConverter::BGR24_to_YUY2...


bool zs::PixelFormatConverter::BGR24_to_Y800(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height) {
		delete[] dst.data;
		dst.size = src.width * src.height;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 3) {
		dst.data[j] = (uint8_t)(0.3f * (float)src.data[i + 2] + 0.59f * (float)src.data[i + 1] + 0.11f * (float)src.data[i]);
		++j;
	}

	return true;

}//bool zs::PixelFormatConverter::BGR24_to_Y800...


bool zs::PixelFormatConverter::BGR24_to_NV12(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 3 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != (uint32_t)((float)(src.width * src.height) * 1.5f)) {
		delete[] dst.data;
		dst.size = (uint32_t)((float)(src.width * src.height) * 1.5f);
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float R00, G00, B00, R01, G01, B01, R10, G10, B10, R11, G11, B11;
	size_t pos;
	size_t k = 0;
	size_t t = 0;
	size_t p = 0;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		t = 0;
		for (size_t j = 0; j < (size_t)src.width * 3; j = j + 6) {
			pos = i * (size_t)src.width * 3 + j;
			B00 = (float)src.data[pos];
			G00 = (float)src.data[pos + 1];
			R00 = (float)src.data[pos + 2];
			pos = pos + 3;
			B01 = (float)src.data[pos];
			G01 = (float)src.data[pos + 1];
			R01 = (float)src.data[pos + 2];
			pos = (i + 1) * (size_t)src.width * 3 + j;
			B10 = (float)src.data[pos];
			G10 = (float)src.data[pos + 1];
			R10 = (float)src.data[pos + 2];
			pos = pos + 3;
			B11 = (float)src.data[pos];
			G11 = (float)src.data[pos + 1];
			R11 = (float)src.data[pos + 2];
			dst.data[k * dst.width + t] = (uint8_t)(0.299f * R00 + 0.587f * G00 + 0.114f * B00);
			dst.data[k * dst.width + t + 1] = (uint8_t)(0.299f * R01 + 0.587f * G01 + 0.114f * B01);
			dst.data[(k + 1) * dst.width + t] = (uint8_t)(0.299f * R10 + 0.587f * G10 + 0.114f * B10);
			dst.data[(k + 1) * dst.width + t + 1] = (uint8_t)(0.299f * R11 + 0.587f * G11 + 0.114f * B11);
			R00 = (R00 + R01 + R10 + R11) / 4.0f;
			G00 = (G00 + G01 + G10 + G11) / 4.0f;
			B00 = (B00 + B01 + B10 + B11) / 4.0f;
			dst.data[(dst.height + p) * dst.width + t] = (uint8_t)(-0.147f * R00 - 0.289f * G00 + 0.436f * B00);
			dst.data[(dst.height + p) * dst.width + t + 1] = (uint8_t)(0.615f * R00 - 0.515f * G00 + 0.100f * B00);
			t = t + 2;
		}
		k = k + 2;
		++p;
	}

	return true;

}//bool zs::PixelFormatConverter::BGR24_to_NV12...


bool zs::PixelFormatConverter::UYVY_to_RGB24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	size_t j = 0;
	float Y0, Y1, U, V;
	for (size_t i = 0; i < (size_t)src.size; i = i + 4) {
		U = (float)src.data[i];
		Y0 = (float)src.data[i + 1];
		V = (float)src.data[i + 2];
		Y1 = (float)src.data[i + 3];
		dst.data[j] = (uint8_t)(Y0 + 1.140f * V);
		dst.data[j + 1] = (uint8_t)(Y0 - 0.395f * U - 0.581f * V);
		dst.data[j + 2] = (uint8_t)(Y0 + 2.032f * U);
		dst.data[j + 3] = (uint8_t)(Y1 + 1.140f * V);
		dst.data[j + 4] = (uint8_t)(Y1 - 0.395f * U - 0.581f * V);
		dst.data[j + 5] = (uint8_t)(Y1 + 2.032f * U);
		j += 6;
	}

	return true;

}//bool zs::PixelFormatConverter::UYVY_to_RGB24...


bool zs::PixelFormatConverter::UYVY_to_BGR24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	size_t j = 0;
	float Y0, Y1, U, V;
	for (size_t i = 0; i < (size_t)src.size; i = i + 4) {
		U = (float)src.data[i];
		Y0 = (float)src.data[i + 1];
		V = (float)src.data[i + 2];
		Y1 = (float)src.data[i + 3];
		dst.data[j + 2] = (uint8_t)(Y0 + 1.140f * V);
		dst.data[j + 1] = (uint8_t)(Y0 - 0.395f * U - 0.581f * V);
		dst.data[j] = (uint8_t)(Y0 + 2.032f * U);
		dst.data[j + 5] = (uint8_t)(Y1 + 1.140f * V);
		dst.data[j + 4] = (uint8_t)(Y1 - 0.395f * U - 0.581f * V);
		dst.data[j + 3] = (uint8_t)(Y1 + 2.032f * U);
		j += 6;
	}

	return true;

}//bool zs::PixelFormatConverter::UYVY_to_BGR24...


bool zs::PixelFormatConverter::UYVY_to_YUY2(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.size) {
		delete[] dst.data;
		dst.size = src.size;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	for (size_t i = 0; i < (size_t)src.size; i = i + 4) {
		dst.data[i] = src.data[i + 1];
		dst.data[i + 1] = src.data[i];
		dst.data[i + 2] = src.data[i + 3];
		dst.data[i + 3] = src.data[i + 2];
	}

	return true;

}//bool zs::PixelFormatConverter::UYVY_to_YUY2...


bool zs::PixelFormatConverter::UYVY_to_Y800(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height) {
		delete[] dst.data;
		dst.size = src.width * src.height;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 2) {
		dst.data[j] = src.data[i + 1];
		++j;
	}

	return true;

}//bool zs::PixelFormatConverter::UYVY_to_Y800...


bool zs::PixelFormatConverter::UYVY_to_NV12(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != (uint32_t)((float)(src.width * src.height) * 1.5f)) {
		delete[] dst.data;
		dst.size = (uint32_t)((float)(src.width * src.height) * 1.5f);
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert	
	float U, V;
	size_t p = 0;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		for (size_t j = 0; j < (size_t)src.width; j = j + 2) {
			dst.data[i * src.width + j] = src.data[i * src.width * 2 + j * 2 + 1];
			dst.data[i * src.width + j + 1] = src.data[i * src.width * 2 + j * 2 + 3];
			dst.data[(i + 1) * src.width + j] = src.data[(i + 1) * src.width * 2 + j * 2 + 1];
			dst.data[(i + 1) * src.width + j + 1] = src.data[(i + 1) * src.width * 2 + j * 2 + 3];
			U = ((float)src.data[i * src.width * 2 + j * 2] + (float)src.data[(i + 1) * src.width * 2 + j * 2] / 2.0f);
			V = ((float)src.data[i * src.width * 2 + j * 2 + 2] + (float)src.data[(i + 1) * src.width * 2 + j * 2 + 2] / 2.0f);
			dst.data[(src.height + p) * src.width + j] = (uint8_t)U;
			dst.data[(src.height + p) * src.width + j + 1] = (uint8_t)V;
		}//for...
		++p;
	}//for...

	return true;

}//bool zs::PixelFormatConverter::UYVY_to_NV12...


bool zs::PixelFormatConverter::YUY2_to_RGB24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	size_t j = 0;
	float Y0, Y1, U, V;
	for (size_t i = 0; i < (size_t)src.size; i = i + 4) {
		U = (float)src.data[i + 1];
		Y0 = (float)src.data[i];
		V = (float)src.data[i + 3];
		Y1 = (float)src.data[i + 2];
		dst.data[j] = (uint8_t)(Y0 + 1.140f * V);
		dst.data[j + 1] = (uint8_t)(Y0 - 0.395f * U - 0.581f * V);
		dst.data[j + 2] = (uint8_t)(Y0 + 2.032f * U);
		dst.data[j + 3] = (uint8_t)(Y1 + 1.140f * V);
		dst.data[j + 4] = (uint8_t)(Y1 - 0.395f * U - 0.581f * V);
		dst.data[j + 5] = (uint8_t)(Y1 + 2.032f * U);
		j += 6;
	}

	return true;

}//bool zs::PixelFormatConverter::YUY2_to_RGB24...


bool zs::PixelFormatConverter::YUY2_to_BGR24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	size_t j = 0;
	float Y0, Y1, U, V;
	for (size_t i = 0; i < (size_t)src.size; i = i + 4) {
		U = (float)src.data[i + 1];
		Y0 = (float)src.data[i];
		V = (float)src.data[i + 3];
		Y1 = (float)src.data[i + 2];
		dst.data[j + 2] = (uint8_t)(Y0 + 1.140f * V);
		dst.data[j + 1] = (uint8_t)(Y0 - 0.395f * U - 0.581f * V);
		dst.data[j] = (uint8_t)(Y0 + 2.032f * U);
		dst.data[j + 5] = (uint8_t)(Y1 + 1.140f * V);
		dst.data[j + 4] = (uint8_t)(Y1 - 0.395f * U - 0.581f * V);
		dst.data[j + 3] = (uint8_t)(Y1 + 2.032f * U);
		j += 6;
	}

	return true;

}//bool zs::PixelFormatConverter::YUY2_to_BGR24...


bool zs::PixelFormatConverter::YUY2_to_UYVY(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.size) {
		delete[] dst.data;
		dst.size = src.size;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	for (size_t i = 0; i < (size_t)src.size; i = i + 4) {
		dst.data[i + 1] = src.data[i];
		dst.data[i] = src.data[i + 1];
		dst.data[i + 3] = src.data[i + 2];
		dst.data[i + 2] = src.data[i + 3];
	}

	return true;

}//bool zs::PixelFormatConverter::YUY2_to_UYVY...


bool zs::PixelFormatConverter::YUY2_to_Y800(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height) {
		delete[] dst.data;
		dst.size = src.width * src.height;
		dst.data = new uint8_t[dst.size];
	}

	// Convert
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 2) {
		dst.data[j] = src.data[i];
		++j;
	}

	return true;

}//bool zs::PixelFormatConverter::YUY2_to_Y800...


bool zs::PixelFormatConverter::YUY2_to_NV12(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height * 2 ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != (uint32_t)((float)(src.width * src.height) * 1.5f)) {
		delete[] dst.data;
		dst.size = (uint32_t)((float)(src.width * src.height) * 1.5f);
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float U, V;
	size_t p = 0;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		for (size_t j = 0; j < (size_t)src.width; j = j + 2) {
			dst.data[i * src.width + j] = src.data[i * src.width * 2 + j * 2];
			dst.data[i * src.width + j + 1] = src.data[i * src.width * 2 + j * 2 + 2];
			dst.data[(i + 1) * src.width + j] = src.data[(i + 1) * src.width * 2 + j * 2];
			dst.data[(i + 1) * src.width + j + 1] = src.data[(i + 1) * src.width * 2 + j * 2 + 2];
			U = ((float)src.data[i * src.width * 2 + j * 2 + 1] + (float)src.data[(i + 1) * src.width * 2 + j * 2 + 1] / 2.0f);
			V = ((float)src.data[i * src.width * 2 + j * 2 + 3] + (float)src.data[(i + 1) * src.width * 2 + j * 2 + 3] / 2.0f);
			dst.data[(src.height + p) * src.width + j] = (uint8_t)U;
			dst.data[(src.height + p) * src.width + j + 1] = (uint8_t)V;
		}//for...
		++p;
	}//for...

	return true;

}//bool zs::PixelFormatConverter::YUY2_to_NV12...


bool zs::PixelFormatConverter::Y800_to_RGB24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; ++i) {
		dst.data[j] = src.data[i];
		dst.data[j + 1] = src.data[i];
		dst.data[j + 2] = src.data[i];
		j = j + 3;
	}

	return true;

}//bool zs::PixelFormatConverter::Y800_to_RGB24...


bool zs::PixelFormatConverter::Y800_to_BGR24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; ++i) {
		dst.data[j] = src.data[i];
		dst.data[j + 1] = src.data[i];
		dst.data[j + 2] = src.data[i];
		j = j + 3;
	}

	return true;

}//bool zs::PixelFormatConverter::Y800_to_BGR24...


bool zs::PixelFormatConverter::Y800_to_UYVY(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 2) {
		dst.data[j] = 0;
		dst.data[j + 1] = src.data[i];
		dst.data[j + 2] = 0;
		dst.data[j + 3] = src.data[i + 1];
		j = j + 4;
	}

	return true;

}//bool zs::PixelFormatConverter::Y800_to_UYVY...


bool zs::PixelFormatConverter::Y800_to_YUY2(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Conver data
	size_t j = 0;
	for (size_t i = 0; i < (size_t)src.size; i = i + 2) {
		dst.data[j] = src.data[i];
		dst.data[j + 1] = 0;
		dst.data[j + 2] = src.data[i + 1];
		dst.data[j + 3] = 0;
		j = j + 4;
	}

	return true;

}//bool zs::PixelFormatConverter::Y800_to_YUY2...


bool zs::PixelFormatConverter::Y800_to_NV12(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != src.width * src.height ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != (uint32_t)((float)(src.width * src.height) * 1.5f)) {
		delete[] dst.data;
		dst.size = (uint32_t)((float)(src.width * src.height) * 1.5f);
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	memcpy(dst.data, src.data, (size_t)src.width * (size_t)src.height);
	memset(&dst.data[src.width * src.height], 0, ((size_t)src.width * (size_t)src.height) / 2);

	return true;

}//bool zs::PixelFormatConverter::Y800_to_NV12...


bool zs::PixelFormatConverter::NV12_to_RGB24(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != (uint32_t)((float)(src.width * src.height) * 1.5f) ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float Y00, Y01, Y10, Y11, U, V;
	size_t k = 0;
	size_t t = 0;
	size_t p = 0;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		t = 0;
		for (size_t j = 0; j < (size_t)src.width; j = j + 2) {
			Y00 = (float)src.data[i * src.width + j];
			Y01 = (float)src.data[i * src.width + j + 1];
			Y10 = (float)src.data[(i + 1) * src.width + j];
			Y11 = (float)src.data[(i + 1) * src.width + j + 1];
			U = (float)src.data[(src.height + p) * src.width + j];
			V = (float)src.data[(src.height + p) * src.width + j + 1];
			dst.data[k * dst.width * 3 + t] = (uint8_t)(Y00 + 1.140f * V);
			dst.data[k * dst.width * 3 + t + 1] = (uint8_t)(Y00 - 0.395f * U - 0.581f * V);
			dst.data[k * dst.width * 3 + t + 2] = (uint8_t)(Y00 + 2.032f * U);
			dst.data[k * dst.width * 3 + t + 3] = (uint8_t)(Y01 + 1.140f * V);
			dst.data[k * dst.width * 3 + t + 4] = (uint8_t)(Y01 - 0.395f * U - 0.581f * V);
			dst.data[k * dst.width * 3 + t + 5] = (uint8_t)(Y01 + 2.032f * U);
			dst.data[(k + 1) * dst.width * 3 + t] = (uint8_t)(Y10 + 1.140f * V);
			dst.data[(k + 1) * dst.width * 3 + t + 1] = (uint8_t)(Y10 - 0.395f * U - 0.581f * V);
			dst.data[(k + 1) * dst.width * 3 + t + 2] = (uint8_t)(Y10 + 2.032f * U);
			dst.data[(k + 1) * dst.width * 3 + t + 3] = (uint8_t)(Y11 + 1.140f * V);
			dst.data[(k + 1) * dst.width * 3 + t + 4] = (uint8_t)(Y11 - 0.395f * U - 0.581f * V);
			dst.data[(k + 1) * dst.width * 3 + t + 5] = (uint8_t)(Y11 + 2.032f * U);
			t += 6;
		}
		k = k + 2;
		++p;
	}

	return true;

}//bool zs::PixelFormatConverter::NV12_to_RGB24...


bool zs::PixelFormatConverter::NV12_to_BGR24(Frame& src, Frame& dst) {
	
	// Check input data
	if (src.data == nullptr ||
		src.size != (uint32_t)((float)(src.width * src.height) * 1.5f) ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 3) {
		delete[] dst.data;
		dst.size = src.width * src.height * 3;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	float Y00, Y01, Y10, Y11, U, V;
	size_t k = 0;
	size_t t = 0;
	size_t p = 0;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		t = 0;
		for (size_t j = 0; j < (size_t)src.width; j = j + 2) {
			Y00 = (float)src.data[i * src.width + j];
			Y01 = (float)src.data[i * src.width + j + 1];
			Y10 = (float)src.data[(i + 1) * src.width + j];
			Y11 = (float)src.data[(i + 1) * src.width + j + 1];
			U = (float)src.data[(src.height + p) * src.width + j];
			V = (float)src.data[(src.height + p) * src.width + j + 1];
			dst.data[k * dst.width * 3 + t + 2] = (uint8_t)(Y00 + 1.140f * V);
			dst.data[k * dst.width * 3 + t + 1] = (uint8_t)(Y00 - 0.395f * U - 0.581f * V);
			dst.data[k * dst.width * 3 + t] = (uint8_t)(Y00 + 2.032f * U);
			dst.data[k * dst.width * 3 + t + 5] = (uint8_t)(Y01 + 1.140f * V);
			dst.data[k * dst.width * 3 + t + 4] = (uint8_t)(Y01 - 0.395f * U - 0.581f * V);
			dst.data[k * dst.width * 3 + t + 3] = (uint8_t)(Y01 + 2.032f * U);
			dst.data[(k + 1) * dst.width * 3 + t + 2] = (uint8_t)(Y10 + 1.140f * V);
			dst.data[(k + 1) * dst.width * 3 + t + 1] = (uint8_t)(Y10 - 0.395f * U - 0.581f * V);
			dst.data[(k + 1) * dst.width * 3 + t] = (uint8_t)(Y10 + 2.032f * U);
			dst.data[(k + 1) * dst.width * 3 + t + 5] = (uint8_t)(Y11 + 1.140f * V);
			dst.data[(k + 1) * dst.width * 3 + t + 4] = (uint8_t)(Y11 - 0.395f * U - 0.581f * V);
			dst.data[(k + 1) * dst.width * 3 + t + 3] = (uint8_t)(Y11 + 2.032f * U);
			t += 6;
		}
		k = k + 2;
		++p;
	}

	return true;

}//bool zs::PixelFormatConverter::NV12_to_BGR24...


bool zs::PixelFormatConverter::NV12_to_UYVY(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != (uint32_t)((float)(src.width * src.height) * 1.5f) ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	size_t k = src.height;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		for (size_t j = 0; j < (size_t)src.width; j = j + 2) {
			dst.data[i * src.width * 2 + j * 2 + 1] = src.data[i * src.width + j];
			dst.data[i * src.width * 2 + j * 2 + 3] = src.data[i * src.width + j + 1];
			dst.data[(i + 1) * src.width * 2 + j * 2 + 1] = src.data[(i + 1) * src.width + j];
			dst.data[(i + 1) * src.width * 2 + j * 2 + 3] = src.data[(i + 1) * src.width + j + 1];
			dst.data[i * src.width * 2 + j * 2] = src.data[k * src.width + j];
			dst.data[i * src.width * 2 + j * 2 + 2] = src.data[k * src.width + j + 1];
			dst.data[(i + 1) * src.width * 2 + j * 2] = src.data[k * src.width + j];
			dst.data[(i + 1) * src.width * 2 + j * 2 + 2] = src.data[k * src.width + j + 1];
		}//for...
		++k;
	}//for...

	return true;

}//bool zs::PixelFormatConverter::NV12_to_UYVY...


bool zs::PixelFormatConverter::NV12_to_YUY2(Frame& src, Frame& dst) {

	// Check input data
	if (src.data == nullptr ||
		src.size != (uint32_t)((float)(src.width * src.height) * 1.5f) ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height * 2) {
		delete[] dst.data;
		dst.size = src.width * src.height * 2;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	size_t k = src.height;
	for (size_t i = 0; i < (size_t)src.height; i = i + 2) {
		for (size_t j = 0; j < (size_t)src.width; j = j + 2) {
			dst.data[i * src.width * 2 + j * 2] = src.data[i * src.width + j];
			dst.data[i * src.width * 2 + j * 2 + 2] = src.data[i * src.width + j + 1];
			dst.data[(i + 1) * src.width * 2 + j * 2] = src.data[(i + 1) * src.width + j];
			dst.data[(i + 1) * src.width * 2 + j * 2 + 2] = src.data[(i + 1) * src.width + j + 1];
			dst.data[i * src.width * 2 + j * 2 + 1] = src.data[k * src.width + j];
			dst.data[i * src.width * 2 + j * 2 + 3] = src.data[k * src.width + j + 1];
			dst.data[(i + 1) * src.width * 2 + j * 2 + 1] = src.data[k * src.width + j];
			dst.data[(i + 1) * src.width * 2 + j * 2 + 3] = src.data[k * src.width + j + 1];
		}//for...
		++k;
	}//for...

	return true;

}//bool zs::PixelFormatConverter::NV12_to_YUY2...


bool zs::PixelFormatConverter::NV12_to_Y800(Frame& src, Frame& dst) {
	
	// Check input data
	if (src.data == nullptr ||
		src.size != (uint32_t)((float)(src.width * src.height) * 1.5f) ||
		src.width < MIN_FRAME_WIDTH ||
		src.height < MIN_FRAME_HEIGHT)
		return false;

	// Check output frame
	if (dst.data == nullptr || dst.size != src.width * src.height) {
		delete[] dst.data;
		dst.size = src.width * src.height;
		dst.data = new uint8_t[dst.size];
	}

	// Copy atributes
	dst.width = src.width;
	dst.height = src.height;
	dst.sourceID = src.sourceID;
	dst.frameID = src.frameID;

	// Convert
	memcpy(dst.data, src.data, (size_t)src.width * (size_t)src.height);

	return true;

}//bool zs::PixelFormatConverter::NV12_to_Y800...