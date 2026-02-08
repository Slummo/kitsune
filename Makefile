BUILD_DEBUG		:= build/debug
BUILD_RELEASE	:= build/release

all: setup_debug

setup_debug:
	cmake -S . -B $(BUILD_DEBUG) -DCMAKE_BUILD_TYPE=Debug

debug:
	cmake --build $(BUILD_DEBUG)

setup_release:
	cmake -S . -B $(BUILD_RELEASE) -DCMAKE_BUILD_TYPE=Release

release:
	cmake --build $(BUILD_RELEASE)

run_test:
	$(BUILD_DEBUG)/test/test_$(NAME)

run_example:
	$(BUILD_DEBUG)/example/$(NAME)

clean:
	rm -rf ./build/*

help:
	@echo "make:			setup debug build"
	@echo "make setup_debug:	setup debug build"
	@echo "make debug:		run debug build"
	@echo "make setup_release:	setup release build"
	@echo "make release:		run debug build"
	@echo "make run_test:		run test specified in NAME (debug only)"
	@echo "make run_example:	run example specified in NAME (debug only)"
	@echo "make clean:		remove build files"

.PHONY: all setup_debug debug setup_release release run_test run_example clean help