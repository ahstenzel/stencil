/*
  image.cpp
  Definition for image loading & processing class
 */

#include "../include/image.hpp"

image::image() {}

image::~image() {}

void image::save(const string& filename) {

}

image imageFromFile(const string& filename) {
  // Open file
  ifstream file(filename, ios::binary);
  if (!file) { 
    throw runtime_error("Failed to open image file"); 
  }

  // Read main header
  vector<char> fileBuff;
  fileBuff.resize(INIT_HEADER_SIZE, '\0');
  file.read(fileBuff.data(), fileBuff.size());
  file.seekg(0);

  // Verify file signature
  if (!(fileBuff[0] == 'B' && fileBuff[1] == 'M')) {
    file.close();
    throw runtime_error("File is not a BMP or is corrupted");
  }

  // Get full file
  image finalImage;
  finalImage.fileSize = *reinterpret_cast<uint32_t*>(&fileBuff[2]);
  finalImage.dataOffset = *reinterpret_cast<uint32_t*>(&fileBuff[10]);
  finalImage.headerSize = *reinterpret_cast<uint32_t*>(&fileBuff[14]);
  fileBuff.resize(finalImage.fileSize, '\0');
  file.read(fileBuff.data(), fileBuff.size());
  file.close();

  // Extract info header values
  switch (finalImage.headerSize) {
  case BITMAPINFOHEADER:
    finalImage.imageWidth = *reinterpret_cast<int32_t*>(&fileBuff[18]);
    finalImage.imageHeight = *reinterpret_cast<int32_t*>(&fileBuff[22]);
    finalImage.colorDepth = *reinterpret_cast<uint16_t*>(&fileBuff[28]);
    break;
  default:
    throw runtime_error("Unsupported header format");
    break;
  }
}