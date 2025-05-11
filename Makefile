CC := gcc
CFLAGS := -std=c99 -MMD -MP -ggdb -Wall -Wextra -fsanitize=address
LDFLAGS := -fsanitize=address

BUILD_DIR := build
TARGET := $(BUILD_DIR)/testbob

SRCS := src/bob.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	@echo "Compiling target exec..."
	$(CC) $^ -o $@ $(LDFLAGS)

# compile rule for each .c file
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run clean
run: $(TARGET)
	@echo "Running target exec..."
	./$(TARGET)

clean:
	@echo "Cleaning build folder..."
	rm -rf $(BUILD_DIR)

-include $(DEPS)
