CC := gcc

CPPFLAGS := -I../include
CFLAGS := -O0 -Wall -std=c99

TARGET := z80test
BUILD_DIR := build

CORE_MODULES := \
	cpu \
	memory \
	util_bit_operations \
	16-bit_arithmetic_group \
	16-bit_load_group \
	8-bit_arithmetic_group \
	8-bit_load_group \
	BSR_and_test_group \
	call_and_return_group \
	EBTS_group \
	GPA_and_CPU_control_group \
	IO_group \
	jump_group \
	rotate_and_shift_group

CORE_OBJECTS := $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(CORE_MODULES)))
OBJECTS := $(BUILD_DIR)/z80test.o $(CORE_OBJECTS)

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/z80test.o: z80test.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: ../src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET)
