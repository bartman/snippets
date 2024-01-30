.PHONY: all build clean distclean
all: build/all

build/all: build/Makefile
	cmake --build build

build/Makefile: build CMakeLists.txt
	cmake -S. -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DCMAKE_BUILD_TYPE=Debug

build:
	mkdir -p build

clean:
	cmake --build build --target clean

distclean:
	rm -rf build
