### C++ software library to convert pixel formats

PixelFormatConverter lib is C++ software library, designed to convert images to various pixel formats (library). The library has a simple interface. The library is distributed as source codes and is compatible with any operating systems that support the C ++ language compiler (C++11 standard). The library includes the following source code files:
•	**VideoDataStructures.h** is header file describing data structures for images and video frames;
•	**PixelFormatConverter.h** is header file containing a description of the only PixelFormatConverter program class;
•	**PixelFormatConverter.cpp** – source code file containing the implementation of the methods of the program class PixelFormatConverter.

### Key features:
- Cross-platform
- Fast
- Simple interface

### Supported pixel formats:

The library supports the following pixel formats: **RGBR (RGB24)**, **BGRB (BGR24)**, **UYVY**, **Y800** (grayscale), **YUY2**, **YUV1** and **NV12**. The numerical values of the pixel formats (the value of the FOURCC code) are determined by the ValidFourccCodes enum declared in the VideoDataStructures.h file. The library supports conversion between the specified formats. Bellow illustrations of the location of pixel bytes in various formats for a 4x4 pixel image.

### RGBR (RGB24):
![](https://github.com/Zaplatnikov/PixelFormatConverter/blob/master/Docs/Source%20images/RGBR%20(RGB24)%20Pixel%20Format%2025%25.png)

### BGRB (BGR24):
![](https://github.com/Zaplatnikov/PixelFormatConverter/blob/master/Docs/Source%20images/BGRB%20(BGR24)%20Pixel%20Format%2025%25.png)

### Y800 (Grayscale):
![](https://github.com/Zaplatnikov/PixelFormatConverter/blob/master/Docs/Source%20images/Y800%20Pixel%20Format%2025%25.png)

### UYVY:
![](https://github.com/Zaplatnikov/PixelFormatConverter/blob/master/Docs/Source%20images/UYVY%20Pixel%20Format%2025%25.png)

### YUY2:
![](https://github.com/Zaplatnikov/PixelFormatConverter/blob/master/Docs/Source%20images/YUY2%20Pixel%20Format%2025%25.png)

### YUV1:
![](https://github.com/Zaplatnikov/PixelFormatConverter/blob/master/Docs/Source%20images/YUV1%20Pixel%20Format%2025%25.png)

### NV12
![](https://github.com/Zaplatnikov/PixelFormatConverter/blob/master/Docs/Source%20images/NV12%20Pixel%20Format%2025%25.png)



