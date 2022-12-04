.PHONY: all build clean distclean
all: build/flat

build/flat: build/Makefile
	${MAKE} -C build/

build/Makefile: build CMakeLists.txt
	cd build && cmake ..

build:
	mkdir -p build

clean:
	${MAKE} -C build/ clean

distclean:
	rm -rf build
