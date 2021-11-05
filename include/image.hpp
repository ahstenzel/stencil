/*
  image.hpp
  Declaration for image loading & processing class
 */

#ifndef STENCIL_IMAGE_HPP
#define STENCIL_IMAGE_HPP

#include "common.hpp"

constexpr size_t INIT_HEADER_SIZE = 14;
constexpr size_t BITMAPCOREHEADER = 12;
constexpr size_t BITMAPINFOHEADER = 40;
constexpr size_t BITMAPV2INFOHEADER = 52;
constexpr size_t BITMAPV3INFOHEADER = 56;
constexpr size_t BITMAPV3INFOHEADER = 108;
constexpr size_t BITMAPV5HEADER = 124;
constexpr size_t OS22XBITMAPHEADER = 64;

class image {
public:
  image();
  ~image();

  void save(const string& filename);

  uint32_t fileSize = 0;
  uint32_t dataOffset = 0;
  uint32_t headerSize = 0;
  int32_t imageWidth = 0;
  int32_t imageHeight = 0;
  uint16_t colorDepth = 0;
  uint32_t compressionMethod = 0;
  uint32_t imageSize = 0;
  int32_t xResolution = 0;
  int32_t yResolution = 0;
  vector<uint32_t> colorTable;
  vector<uint32_t> pixelData;
};

image imageFromFile(const string& filename);

#endif  // STENCIL_IMAGE_HPP