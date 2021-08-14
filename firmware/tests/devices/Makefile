TARGET_WATCHDOG=watchdog_unit_test
TARGET_TEMP_SENSOR=temp_sensor_unit_test
TARGET_ANTENNA=antenna_unit_test
TARGET_RADIO=radio_unit_test
TARGET_POWER_SENSOR=power_sensor_unit_test
TARGET_LEDS=leds_unit_test
TARGET_MEDIA=media_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -I../../config/ -I../../tests/freertos_sim/ -Wl,--wrap=sys_log_print_event_from_module,--wrap=sys_log_new_line,--wrap=sys_log_print_msg,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_float,--wrap=sys_log_print_hex

WATCHDOG_TEST_FLAGS=$(FLAGS),--wrap=wdt_init,--wrap=wdt_reset,--wrap=tps382x_init,--wrap=tps382x_trigger

TEMP_SENSOR_TEST_FLAGS=$(FLAGS),--wrap=adc_init,--wrap=adc_read,--wrap=adc_temp_get_mref,--wrap=adc_temp_get_nref

ANTENNA_TEST_FLAGS=$(FLAGS),--wrap=isis_antenna_init,--wrap=isis_antenna_arm,--wrap=isis_antenna_disarm,--wrap=isis_antenna_start_sequential_deploy,--wrap=isis_antenna_start_independent_deploy,--wrap=isis_antenna_read_deployment_status_code,--wrap=isis_antenna_read_deployment_status,--wrap=isis_antenna_get_antenna_status,--wrap=isis_antenna_get_antenna_timeout,--wrap=isis_antenna_get_burning,--wrap=isis_antenna_get_arming_status,--wrap=isis_antenna_get_raw_temperature,--wrap=isis_antenna_raw_to_temp_c,--wrap=isis_antenna_get_temperature_c,--wrap=isis_antenna_delay_s,--wrap=isis_antenna_delay_ms

RADIO_TEST_FLAGS=$(FLAGS),--wrap=si446x_reset,--wrap=si446x_power_up,--wrap=si446x_configuration_init,--wrap=si446x_part_info,--wrap=si446x_start_tx,--wrap=si446x_start_rx,--wrap=si446x_get_int_status,--wrap=si446x_gpio_pin_cfg,--wrap=si446x_set_property,--wrap=si446x_change_state,--wrap=si446x_nop,--wrap=si446x_fifo_info,--wrap=si446x_write_tx_fifo,--wrap=si446x_read_rx_fifo,--wrap=si446x_get_property,--wrap=si446x_func_info,--wrap=si446x_frr_a_read,--wrap=si446x_frr_b_read,--wrap=si446x_frr_c_read,--wrap=si446x_frr_d_read,--wrap=si446x_get_adc_reading,--wrap=si446x_get_packet_info,--wrap=si446x_get_ph_status,--wrap=si446x_get_modem_status,--wrap=si446x_get_chip_status,--wrap=si446x_ircal,--wrap=si446x_protocol_cfg,--wrap=si446x_request_device_state,--wrap=si446x_rx_hop,--wrap=si446x_shutdown,--wrap=si446x_power_on,--wrap=si446x_check_cts,--wrap=si446x_set_cmd,--wrap=si446x_get_cmd,--wrap=si446x_spi_init,--wrap=si446x_spi_transfer,--wrap=si446x_spi_write_byte,--wrap=si446x_spi_write,--wrap=si446x_spi_read,--wrap=si446x_gpio_init,--wrap=si446x_gpio_write_sdn,--wrap=si446x_gpio_read_nirq,--wrap=si446x_delay_ms

POWER_SENSOR_TEST_FLAGS=$(FLAGS),--wrap=ina22x_init,--wrap=ina22x_write_reg,--wrap=ina22x_read_reg,--wrap=ina22x_get_current,--wrap=ina22x_get_manufacturer_id,--wrap=ina22x_get_die_id

LEDS_TEST_FLAGS=$(FLAGS),--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

MEDIA_TEST_FLAGS=$(FLAGS),--wrap=flash_init,--wrap=flash_write,--wrap=flash_write_single,--wrap=flash_read_single,--wrap=flash_write_long,--wrap=flash_read_long,--wrap=flash_erase

.PHONY: all
all: watchdog_test temp_sensor_test antenna_test radio_test power_sensor_test leds_test media_test

.PHONY: watchdog_test
watchdog_test: $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o
	$(CC) $(WATCHDOG_TEST_FLAGS) $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o -o $(BUILD_DIR)/$(TARGET_WATCHDOG) -lcmocka

.PHONY: temp_sensor_test
temp_sensor_test: $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(TEMP_SENSOR_TEST_FLAGS) $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) -lcmocka

.PHONY: antenna_test
antenna_test: $(BUILD_DIR)/antenna.o $(BUILD_DIR)/antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/isis_antenna_wrap.o
	$(CC) $(ANTENNA_TEST_FLAGS) $(BUILD_DIR)/antenna.o $(BUILD_DIR)/antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/isis_antenna_wrap.o -o $(BUILD_DIR)/$(TARGET_ANTENNA) -lcmocka

.PHONY: radio_test
radio_test: $(BUILD_DIR)/radio.o $(BUILD_DIR)/radio_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/si446x_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(RADIO_TEST_FLAGS) $(BUILD_DIR)/radio.o $(BUILD_DIR)/radio_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/si446x_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_RADIO) -lcmocka -lm

.PHONY: power_sensor_test
power_sensor_test: $(BUILD_DIR)/power_sensor.o $(BUILD_DIR)/power_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/ina22x_wrap.o
	$(CC) $(POWER_SENSOR_TEST_FLAGS) $(BUILD_DIR)/power_sensor.o $(BUILD_DIR)/power_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/ina22x_wrap.o -o $(BUILD_DIR)/$(TARGET_POWER_SENSOR) -lcmocka

.PHONY: leds_test
leds_test: $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(LEDS_TEST_FLAGS) $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_LEDS) -lcmocka

.PHONY: media_test
media_test: $(BUILD_DIR)/media.o $(BUILD_DIR)/media_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/flash_wrap.o
	$(CC) $(MEDIA_TEST_FLAGS) $(BUILD_DIR)/media.o $(BUILD_DIR)/media_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/media_wrap.o -o $(BUILD_DIR)/$(TARGET_MEDIA) -lcmocka

# Devices
$(BUILD_DIR)/watchdog.o: ../../devices/watchdog/watchdog.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/temp_sensor.o: ../../devices/temp_sensor/temp_sensor.c
	$(CC) $(TEMP_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/antenna.o: ../../devices/antenna/antenna.c
	$(CC) $(ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/radio.o: ../../devices/radio/radio.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/power_sensor.o: ../../devices/power_sensor/power_sensor.c
	$(CC) $(POWER_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/leds.o: ../../devices/leds/leds.c
	$(CC) $(LEDS_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/media.o: ../../devices/media/media.c
	$(CC) $(MEDIA_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/watchdog_test.o: watchdog_test.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/temp_sensor_test.o: temp_sensor_test.c
	$(CC) $(TEMP_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/antenna_test.o: antenna_test.c
	$(CC) $(ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/radio_test.o: radio_test.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/power_sensor_test.o: power_sensor_test.c
	$(CC) $(POWER_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/leds_test.o: leds_test.c
	$(CC) $(LEDS_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/media_test.o: media_test.c
	$(CC) $(MEDIA_TEST_FLAGS) -c $< -o $@

# Mockups
$(BUILD_DIR)/sys_log_wrap.o: ../mockups/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/wdt_wrap.o: ../mockups/wdt_wrap.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x_wrap.o: ../mockups/tps382x_wrap.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/adc_wrap.o: ../mockups/adc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_wrap.o: ../mockups/isis_antenna_wrap.c
	$(CC) $(ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_wrap.o: ../mockups/si446x_wrap.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/ina22x_wrap.o: ../mockups/ina22x_wrap.c
	$(CC) $(POWER_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/gpio_wrap.o: ../mockups/gpio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/flash_wrap.o: ../mockups/flash_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/task.o: ../freertos_sim/task.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_WATCHDOG) $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) $(BUILD_DIR)/$(TARGET_ANTENNA) $(BUILD_DIR)/$(TARGET_RADIO) $(BUILD_DIR)/$(TARGET_POWER_SENSOR) $(BUILD_DIR)/$(TARGET_LEDS) $(BUILD_DIR)/$(TARGET_MEDIA) $(BUILD_DIR)/*.o
