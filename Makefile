BUILD_DEBUG		:= build/debug

CMAKE_ARGS		:=

CORES			?= $(shell nproc 2>/dev/null || echo 4)

.PHONY: all debug runt rune update clean clean-full help

all: debug

$(BUILD_DEBUG)/CMakeCache.txt: CMakeLists.txt
	@echo "Configuring Debug build..."
	@mkdir -p $(BUILD_DEBUG)
	@cmake -S . -B $(BUILD_DEBUG) -DCMAKE_BUILD_TYPE=Debug $(CMAKE_ARGS)

debug: $(BUILD_DEBUG)/CMakeCache.txt
	@echo "Building Debug..."
	@cmake --build $(BUILD_DEBUG) -j $(CORES)

runt: debug
	@if [ -z "$(NAME)" ]; then echo "Specify NAME"; exit 1; fi
	@echo "Running Test '$(NAME)'..."
	@./$(BUILD_DEBUG)/test/test_$(NAME)

rune: debug
	@if [ -z "$(NAME)" ]; then echo "Specify NAME"; exit 1; fi
	@echo "Running Example '$(NAME)'..."
	@./$(BUILD_DEBUG)/example/example_$(NAME)

update:
	@echo "Updating dependencies..."
	@touch CMakeLists.txt
	@$(MAKE) debug

clean:
	@if [ -n "$(NAME)" ]; then \
		echo "Cleaning target '$(NAME)'..."; \
		rm -rf $(BUILD_DEBUG)/test/CMakeFiles/test_$(NAME).dir; \
		rm -rf $(BUILD_DEBUG)/example/CMakeFiles/example_$(NAME).dir; \
		rm -f $(BUILD_DEBUG)/test/test_$(NAME) $(BUILD_DEBUG)/example/example_$(NAME); \
	else \
		echo "Cleaning local files..."; \
		rm -rf $(BUILD_DEBUG)/test $(BUILD_DEBUG)/example; \
	fi

clean-full:
	@echo "Cleaning build..."
	@rm -rf build

help:
	@echo "Usage:"
	@echo "  make                        - Build debug version"
	@echo "  make debug                  - Build debug version"
	@echo "  make runt NAME=x            - Build and run test_x (debug)"
	@echo "  make rune NAME=x            - Build and run example_x (debug)"
	@echo "  make update                 - Re-fetch/update dependencies"
	@echo "  make clean [NAME=x]         - Remove local object files"
	@echo "  make clean-full             - Remove all build files"