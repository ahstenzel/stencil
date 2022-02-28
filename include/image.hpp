#pragma once
/*
 * image.hpp
 * Declaration for image loading & processing class
 */

#include "common.hpp"

namespace stencil {
  using scalar_t = double;
  constexpr uint32_t BITMAPFILEHEADER = 14;
  constexpr uint32_t BITMAPCOREHEADER = 12;
  constexpr uint32_t BITMAPINFOHEADER = 40;
  constexpr uint32_t BITMAPV2INFOHEADER = 52;
  constexpr uint32_t BITMAPV3INFOHEADER = 56;
  constexpr uint32_t BITMAPV4INFOHEADER = 108;
  constexpr uint32_t BITMAPV5INFOHEADER = 124;
  constexpr uint32_t OS22XBITMAPHEADER = 64;

  struct color_t {
    color_t(scalar_t red = 0, scalar_t green = 0, scalar_t blue = 0) :
      r(red), b(blue), g(green) {}
    friend std::ostream& operator<<(std::ostream& os, const color_t& color);
    bool operator==(const color_t& rhs);
    bool operator!=(const color_t& rhs);
    scalar_t r;
    scalar_t g;
    scalar_t b;
  };

  class image {
  public:
    class iterator_t {
    public:
      iterator_t(unsigned int x, unsigned int y, image* img);
      
      color_t& operator*();
      color_t* operator->();
      iterator_t& operator++();
      iterator_t operator++(int);
      bool operator==(const iterator_t& rhs);
      bool operator!=(const iterator_t& rhs);
    private:
      unsigned int x_, y_;
      image* img_;
    };

    image();
    ~image();

    iterator_t begin();
    iterator_t end();

    void save(const std::string& filename);
    color_t& pixel(unsigned int x, unsigned int y);
    unsigned int getWidth();
    unsigned int getHeight();

    static image load(const std::string& filename);

  private:
    void resize(unsigned int w, unsigned int h);

    uint32_t fileSize = 0;
    uint32_t dataOffset = 0;
    uint32_t headerSize = 0;
    int32_t imageWidth = 0;
    int32_t imageHeight = 0;
    uint16_t bitsPerPixel = 0;
    uint32_t compressionMethod = 0;
    int32_t xResolution = 0;
    int32_t yResolution = 0;
    std::vector<color_t> pixelData;
  };
}