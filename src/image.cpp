/*
 * image.cpp
 * Definition for image loading & processing class
 */

#include "../include/image.hpp"

namespace stencil {
  std::ostream& operator<<(std::ostream& os, const color_t& color) {
    os << "{" << color.r << "," << color.g << "," << color.b << "}";
    return os;
  }

  bool color_t::operator==(const color_t& rhs) {
    return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
  }

  bool color_t::operator!=(const color_t& rhs) {
    return !(*this == rhs);
  }

  image::iterator_t::iterator_t(unsigned int x, unsigned int y, image* img) :
    x_(x), y_(y), img_(img) {}
    
  color_t& image::iterator_t::operator*() { return img_->pixel(x_, y_); }

  color_t* image::iterator_t::operator->() { return &(img_->pixel(x_, y_)); }

  image::iterator_t& image::iterator_t::operator++() {
    x_++;
    if (x_ >= img_->getWidth()) {
      x_ = 0;
      y_++;
    }
    return *this;
  }

  image::iterator_t image::iterator_t::operator++(int) {
    iterator_t tmp(*this);
    operator++();
    return tmp;
  }

  bool image::iterator_t::operator==(const image::iterator_t& rhs) {
    return (x_ == rhs.x_ && y_ == rhs.y_ && img_ == rhs.img_);
  }
  bool image::iterator_t::operator!=(const image::iterator_t& rhs) {
    return !(*this == rhs);
  }

  image::image() {}

  image::~image() {}

  image::iterator_t image::begin()  { return iterator_t(0, 0, this); }

  image::iterator_t image::end()    { return iterator_t(0, imageHeight, this); }

  void image::save(const std::string& filename) {}

  color_t& image::pixel(unsigned int x, unsigned int y) {
    return pixelData.at((y * imageWidth) + x);
  }

  void image::resize(unsigned int w, unsigned int h) {
    pixelData.clear();
    pixelData.resize(w * h);
    imageWidth = w;
    imageHeight = h;
  }

  unsigned int image::getWidth() { return (unsigned int)abs(imageWidth); }

  unsigned int image::getHeight() { return (unsigned int)abs(imageHeight); }

  image image::load(const std::string& filename) {
    // Open file
    std::ifstream file(filename, std::ios::binary);
    if (!file) { 
      throw std::runtime_error("Failed to open image file"); 
    }

    // Read main header
    std::vector<char> fileBuff;
    fileBuff.resize(BITMAPFILEHEADER + 4, 0);
    file.read(fileBuff.data(), fileBuff.size());
    file.seekg(0);

    // Verify file signature
    if (!(fileBuff[0x0] == 'B' && fileBuff[0x1] == 'M')) {
      file.close();
      throw std::runtime_error("File is not a BMP or is corrupted");
    }

    // Get full file
    image finalImage;
    finalImage.fileSize = *reinterpret_cast<uint32_t*>(&fileBuff[0x2]);
    finalImage.dataOffset = *reinterpret_cast<uint32_t*>(&fileBuff[0xA]);
    finalImage.headerSize = *reinterpret_cast<uint32_t*>(&fileBuff[0xE]);
    fileBuff.resize(finalImage.fileSize, 0);
    file.read(fileBuff.data(), fileBuff.size());
    file.close();

    // Extract info header values
    uint32_t colorTableSize = 0;
    uint32_t colorTableOffset = BITMAPFILEHEADER + finalImage.headerSize;
    uint32_t rawImageSize = *reinterpret_cast<uint32_t*>(&fileBuff[0x22]);
    switch (finalImage.headerSize) {
    case BITMAPINFOHEADER:
      finalImage.bitsPerPixel = *reinterpret_cast<uint16_t*>(&fileBuff[0x1C]);
      finalImage.compressionMethod = *reinterpret_cast<uint32_t*>(&fileBuff[0x1E]);
      finalImage.xResolution = *reinterpret_cast<int32_t*>(&fileBuff[0x26]);
      finalImage.yResolution = *reinterpret_cast<int32_t*>(&fileBuff[0x2A]);
      colorTableSize = *reinterpret_cast<int32_t*>(&fileBuff[0x2E]);
      finalImage.resize(*reinterpret_cast<int32_t*>(&fileBuff[0x12]), *reinterpret_cast<int32_t*>(&fileBuff[0x16]));
      break;
    default:
      throw std::runtime_error("Unsupported header format");
      break;
    }
    if (finalImage.compressionMethod != 0) {
      throw std::runtime_error("Unsupported compression method");
    }

    // Extract color table
    std::vector<color_t> colorTable;
    if (finalImage.bitsPerPixel < 16) {
      uint32_t entrySize = (finalImage.headerSize == BITMAPCOREHEADER) ? 3 : 4;
      for (uint32_t i = 0; i < colorTableSize; ++i) {
        uint32_t pos = colorTableOffset + (i * entrySize);
        uint32_t entry = *reinterpret_cast<uint32_t*>(&fileBuff[pos]);
        color_t color(entry);
        colorTable.push_back(color);
      }
    }

    // Extract pixel data
    uint32_t rowSize = 4 * (((finalImage.bitsPerPixel * finalImage.imageWidth) + 31) / 32);
    for (int32_t y = 0; y < abs(finalImage.imageHeight); ++y) {
      // Get a full row of pixels
      std::vector<uint8_t> rowData;
      rowData.resize(rowSize);
      for (uint32_t x = 0; x < rowSize; ++x) {
        rowData[x] = *reinterpret_cast<uint8_t*>(&fileBuff[finalImage.dataOffset + (uint64_t(rowSize) * y) + x]);
      } 

      // Decode row based on color depth
      switch (finalImage.bitsPerPixel) {
      case 24:
        // Color stored as sequential bytes
        // Order is B,G,R
        for(uint32_t x = 0; x < rowSize; x += 3) {
          scalar_t b = rowData[x] / (scalar_t)255;
          scalar_t g = rowData[x + 1] / (scalar_t)255;
          scalar_t r = rowData[x + 2] / (scalar_t)255;
          finalImage.pixel(x / 3, y) = color_t(r, g, b);
        }
        break;
      case 16:
        // Color stored as 5-bit sequences every 2 bytes
        // Order is B,G,R from least -> most significant, most significant bit not used
        for(uint32_t x = 0; x < rowSize; x += 2) {
          uint16_t c = *reinterpret_cast<uint16_t*>(&rowData[x]);
          scalar_t b = (c & 0b00011111) / (scalar_t)31;
          scalar_t g = ((c >> 5) & 0b00011111) / (scalar_t)31;
          scalar_t r = ((c >> 10) & 0b00011111) / (scalar_t)31;
          finalImage.pixel(x / 2, y) = color_t(r, g, b);
        }
        break;
      case 32:
        // Color stored as sequential bytes
        // In order of B,G,R,A
        for(uint32_t x = 0; x < rowSize; x += 4) {
          scalar_t b = rowData[x] / (scalar_t)255;
          scalar_t g = rowData[x + 1] / (scalar_t)255;
          scalar_t r = rowData[x + 2] / (scalar_t)255;
          finalImage.pixel(x / 4, y) = color_t(r, g, b);
        }
        break;
      case 1:
      case 2:
      case 4:
      case 8:
      default:
        throw std::runtime_error("Unsupported color depth");
        break;
      }
    }

    return finalImage;
  }
}