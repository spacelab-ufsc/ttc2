TARGET_TCA4311A=tca4311a_unit_test
TARGET_ISIS_ANTENNA=isis_antenna_unit_test
TARGET_SI446X=si446x_unit_test
TARGET_INA22X=ina22x_unit_test
TARGET_TPS382X=tps382x_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -I../../tests/freertos_sim/ -Wl,--wrap=sys_log_init,--wrap=sys_log_print_event,--wrap=sys_log_print_event_from_module,--wrap=sys_log_print_msg,--wrap=sys_log_print_str,--wrap=sys_log_new_line,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_hex,--wrap=sys_log_dump_hex,--wrap=sys_log_print_float,--wrap=sys_log_print_byte,--wrap=sys_log_print_system_time,--wrap=sys_log_print_license_msg,--wrap=sys_log_print_splash_screen,--wrap=sys_log_print_firmware_version

TCA4311A_TEST_FLAGS=$(FLAGS),--wrap=i2c_init,--wrap=i2c_write,--wrap=i2c_read,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

ISIS_ANTENNA_TEST_FLAGS=$(FLAGS),--wrap=tca4311a_init,--wrap=tca4311a_enable,--wrap=tca4311a_disable,--wrap=tca4311a_is_ready,--wrap=tca4311a_write,--wrap=tca4311a_read,--wrap=tca4311a_write_byte,--wrap=tca4311a_read_byte

SI446X_TEST_FLAGS=$(FLAGS),--wrap=spi_init,--wrap=spi_select_slave,--wrap=spi_write,--wrap=spi_read,--wrap=spi_transfer,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

INA22X_TEST_FLAGS=$(FLAGS),--wrap=i2c_init,--wrap=i2c_write,--wrap=i2c_read

TPS382X_TEST_FLAGS=$(FLAGS),--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle,--wrap=gpio_init_mr_pin

.PHONY: all
all: tca4311a_test isis_antenna_test si446x_test ina22x_test tps382x_test

.PHONY: tca4311a_test
tca4311a_test: $(BUILD_DIR)/tca4311a.o $(BUILD_DIR)/tca4311a_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(TCA4311A_TEST_FLAGS) $(BUILD_DIR)/tca4311a.o $(BUILD_DIR)/tca4311a_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_TCA4311A) -lm -lcmocka

.PHONY: isis_antenna_test
isis_antenna_test: $(BUILD_DIR)/isis_antenna.o $(BUILD_DIR)/isis_antenna_delay.o $(BUILD_DIR)/isis_antenna_i2c.o $(BUILD_DIR)/isis_antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/tca4311a_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) $(BUILD_DIR)/isis_antenna.o $(BUILD_DIR)/isis_antenna_delay.o $(BUILD_DIR)/isis_antenna_i2c.o $(BUILD_DIR)/isis_antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/tca4311a_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_ISIS_ANTENNA) -lm -lcmocka

.PHONY: si446x_test
si446x_test: $(BUILD_DIR)/si446x.o $(BUILD_DIR)/si446x_delay.o $(BUILD_DIR)/si446x_gpio.o $(BUILD_DIR)/si446x_spi.o $(BUILD_DIR)/si446x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(SI446X_TEST_FLAGS) $(BUILD_DIR)/si446x.o $(BUILD_DIR)/si446x_delay.o $(BUILD_DIR)/si446x_gpio.o $(BUILD_DIR)/si446x_spi.o $(BUILD_DIR)/si446x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_SI446X) -lm -lcmocka

.PHONY: ina22x_test
ina22x_test: $(BUILD_DIR)/ina22x.o $(BUILD_DIR)/ina22x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o
	$(CC) $(INA22X_TEST_FLAGS) $(BUILD_DIR)/ina22x.o $(BUILD_DIR)/ina22x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o -o $(BUILD_DIR)/$(TARGET_INA22X) -lm -lcmocka

.PHONY: tps382x_test
tps382x_test: $(BUILD_DIR)/tps382x.o $(BUILD_DIR)/tps382x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(TPS382X_TEST_FLAGS) $(BUILD_DIR)/tps382x.o $(BUILD_DIR)/tps382x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_TPS382X) -lcmocka

# Drivers
$(BUILD_DIR)/tca4311a.o: ../../drivers/tca4311a/tca4311a.c
	$(CC) $(TCA4311A_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna.o: ../../drivers/isis_antenna/isis_antenna.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_delay.o: ../../drivers/isis_antenna/isis_antenna_delay.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_i2c.o: ../../drivers/isis_antenna/isis_antenna_i2c.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x.o: ../../drivers/si446x/si446x.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_delay.o: ../../drivers/si446x/si446x_delay.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_gpio.o: ../../drivers/si446x/si446x_gpio.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_spi.o: ../../drivers/si446x/si446x_spi.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/ina22x.o: ../../drivers/ina22x/ina22x.c
	$(CC) $(INA22X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x.o: ../../drivers/tps382x/tps382x.c
	$(CC) $(TPS382X_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/tca4311a_test.o: tca4311a_test.c
	$(CC) $(TCA4311A_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_test.o: isis_antenna_test.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_test.o: si446x_test.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/ina22x_test.o: ina22x_test.c
	$(CC) $(INA22X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x_test.o: tps382x_test.c
	$(CC) $(TPS382X_TEST_FLAGS) -c $< -o $@

# Mockups
$(BUILD_DIR)/sys_log_wrap.o: ../mockups/system/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/adc_wrap.o: ../mockups/drivers/adc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/gpio_wrap.o: ../mockups/drivers/gpio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/wdt_wrap.o: ../mockups/drivers/wdt_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/flash_wrap.o: ../mockups/drivers/flash_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/spi_wrap.o: ../mockups/drivers/spi_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/i2c_wrap.o: ../mockups/drivers/i2c_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/tca4311a_wrap.o: ../mockups/drivers/tca4311a_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/task.o: ../freertos_sim/task.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_TCA4311A) $(BUILD_DIR)/$(TARGET_ISIS_ANTENNA) $(BUILD_DIR)/$(TARGET_SI446X) $(BUILD_DIR)/$(TARGET_INA22X) $(BUILD_DIR)/$(TARGET_TPS382X) $(BUILD_DIR)*.o
