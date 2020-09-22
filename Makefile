# Makefile for the lazy
build_directory := build
ifndef prefix
  # Switch to /usr for deployment
  prefix := "./local"
endif

all:
	cmake . -B$(build_directory) -DCMAKE_INSTALL_PREFIX=$(prefix)
	cmake --build $(build_directory) --target install -- $(MAKEFLAGS)

clean:
	rm -rf $(build_directory)
