# The build system we use is CMAKE not GNUMake. This Makefile is simply a
# wrapper to call cmake in fewer keystrokes.
ifndef build_directory
  build_directory := build
endif
ifndef prefix
  # Switch to /usr for deployment
  prefix := "./local"
endif

all:
	cmake . -B$(build_directory) -DCMAKE_INSTALL_PREFIX=$(prefix)
	cmake --build $(build_directory) --target install -- $(MAKEFLAGS)

clean:
	rm -rf $(build_directory)
