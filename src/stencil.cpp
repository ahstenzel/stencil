#include "../include/common.hpp"
#include "../include/image.hpp"

using namespace stencil;

int main(int argc, char** argv) {
  // Read command line arguments
  std::string file;
  if (argc == 2) {
    file = argv[1];
  }
  else {
    std::cout << "Usage: stencil.exe FILENAME" << std::endl;
    return 1;
  }

  // Load image
  image pic;
  try {
    pic = image::load(file);
  }
  catch(std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  // Read pixel data
  for(auto it : pic) {
    std::cout << it << std::endl;
  }
	return 0;
}
