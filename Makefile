.PHONY: build init test

build:
	cmake --build build

init:
	cmake -B build

test:
	cmake --build build --target run_test

