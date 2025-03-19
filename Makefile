# Compiler settings
CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Iinclude/
RELEASE_CFLAGS := -Werror -O2
TEST_CFLAGS := -O1 -g
LDFLAGS := -lm -lglad -lglfw -lGL -lX11 -lpthread -lXrandr -lXi

# Directories
SRC_DIR := src
GRAPHICS_DIR := $(SRC_DIR)/GFX
#PARSER_DIR := $(SRC_DIR)/VtxParser
BUILD_DIR := build

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
GRAPHICS_SRCS := $(wildcard $(GRAPHICS_DIR)/*.c)
#PARSER_SRCS := $(wildcard $(PARSER_DIR)/*.c)
ALL_SRCS = $(SRCS) $(GRAPHICS_SRCS) #$(PARSER_SRCS)

# Output files
RELEASE_OUTPUT := $(BUILD_DIR)/MyGame
TEST_OUTPUT := $(BUILD_DIR)/test

# Default target
all: release

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Release build
release: $(BUILD_DIR) $(RELEASE_OUTPUT)

$(RELEASE_OUTPUT): $(ALL_SRCS)
	@echo "Compiling release build..."
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "Build successful: $(RELEASE_OUTPUT)"

# Test build
.PHONY: test
test: $(BUILD_DIR) $(TEST_OUTPUT)

$(TEST_OUTPUT): $(ALL_SRCS)
	@echo "Compiling test build..."
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "Build successful: $(TEST_OUTPUT)"

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build files."
