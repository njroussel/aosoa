.PHONY: help format clean build all

help:
	@echo "format - format code with clang-format"
	@echo "clean - clean build folder"
	@echo "build - build executable"
	@echo "all - format > cleanbuild"

format:
	find src/ include/ | \
	grep -E ".*(\.cpp|\.h)$$" | \
	xargs clang-format -i

build:
	mkdir -p build
	cmake -B build -DCMAKE_BUILD_TYPE=Release -GNinja . && ninja -C build

debug:
	mkdir -p build
	cmake -B build -GNinja . && ninja -C build

clean:
	rm -rf build

all:
	format clean build
