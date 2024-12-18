# goldenratiopyramid
## description
This codebase draws a pyramid to the screen that adheres to the proportion: 1, square root of phi, phi. phi being the golden ratio.
# Compilation
## Cmake
### Release build
	mkdir build
	cd build
	cmake .. -DCMAKE_BUILD_TYPE=Release
 	make
  ./goldenratiopyramid
### Debug build
	mkdir build
	cd build
	cmake .. -DCMAKE_BUILD_TYPE=Debug
 	make
  ../goldenratiopyramid
## Makefile
### build and run
	$ make run 

### prerequisites
 	sudo apt-get install  libsdl2-dev libsdl2-image-dev -y


 

