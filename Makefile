# Toryum Core AI - Build System

BUILD_DIR = build

.PHONY: all clean run

all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make

run: all
	@./$(BUILD_DIR)/toryum_kernel

clean:
	@rm -rf $(BUILD_DIR)