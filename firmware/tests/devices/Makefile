TARGET_WATCHDOG=watchdog_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -Wl,--wrap=sys_log_print_event_from_module,--wrap=sys_log_new_line,--wrap=sys_log_print_msg,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_float

WATCHDOG_TEST_FLAGS=$(FLAGS),--wrap=wdt_init,--wrap=wdt_reset,--wrap=tps382x_init,--wrap=tps382x_trigger

.PHONY: all
all: watchdog_test

.PHONY: watchdog_test
watchdog_test: $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o
	$(CC) $(WATCHDOG_TEST_FLAGS) $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o -o $(BUILD_DIR)/$(TARGET_WATCHDOG) -lcmocka

# Devices
$(BUILD_DIR)/watchdog.o: ../../devices/watchdog/watchdog.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/watchdog_test.o: watchdog_test.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

# Mockups
$(BUILD_DIR)/sys_log_wrap.o: ../mockups/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/wdt_wrap.o: ../mockups/wdt_wrap.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x_wrap.o: ../mockups/tps382x_wrap.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_WATCHDOG) $(BUILD_DIR)/*.o
