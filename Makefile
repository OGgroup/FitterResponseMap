# Makefile for the lazy
build_directory := build

all:
	cmake . -B$(build_directory) -DCMAKE_INSTALL_PREFIX="./local"
	cmake --build $(build_directory) --target install -- $(MAKEFLAGS)

clean:
	rm -rf $(build_directory)
