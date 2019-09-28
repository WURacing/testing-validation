#include <asf.h>
#include "util.h"

volatile uint32_t g_ul_ms_ticks = 0;
void SysTick_Handler(void)
{
	g_ul_ms_ticks++;
}

bool ledstate;

void shift_array_left(char *arr, const size_t size, const size_t bits) {
	const size_t chunks = bits / (8);

	if (chunks >= size) {
		return;
	}

	if (chunks) {
		memmove(arr, arr + chunks, size - chunks);
	}

	const size_t left = bits % (8);

	// If we have non directly addressable bits left we need to move the whole thing one by one.
	if (left) {
		const size_t right = (8) - left;
		const size_t l = size - chunks - 1;
		for (size_t i = 0; i < l; i++) {
			arr[i] = ((arr[i] << left) & ~left) | (arr[i+1] >> right);
		}
		arr[l] = (arr[l] << left) & ~left;
	}
}

int main (void)
{
	system_init();
	delay_init();
	
	
	struct rtc_calendar_time now;

	
	/* Insert application code here, after the board has been initialized. */
	configure_usart_cdc();
	
	port_pin_set_output_level(LED_0_PIN, 1);
	
	configure_can();
	
	system_interrupt_enable_global();

	
	configure_i2c();
	initialize_rtc_calendar();
	SysTick_Config(48000000 / 1000);
	
	irq_initialize_vectors();
	cpu_irq_enable();

	
	


	char test_file_name[50];
	Ctrl_status status;
	FRESULT res;
	FATFS fs;
	FIL file_object;
	FILINFO file_stat;
	char line[550];
	int logno = 0;
	int onetwentyeighths = 0;


	/* Initialize SD MMC stack */
	sd_mmc_init();

	while (1) {
		printf("Please plug an SD, MMC or SDIO card in slot.\n\r");

		/* Wait card present and ready */
		do {
			status = sd_mmc_test_unit_ready(0);
			if (CTRL_FAIL == status) {
				printf("Failed to initialize SD card [%d], please re-insert the card.\r\n", status);
				while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
				}
			}
		} while (CTRL_GOOD != status);

		memset(&fs, 0, sizeof(FATFS));
		res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
		if (FR_INVALID_DRIVE == res) {
			printf("Failed to mount FAT32 filesystem on SD card [%d], please check that\r\n", res);
			goto main_end_of_test;
		}

		do {
			sprintf(test_file_name, "0:LOG%05d.CSV", logno++);
			bzero(&file_stat, sizeof(file_stat));
			res = f_stat(test_file_name, &file_stat);
		} while (res == FR_OK);
		
		if (res != FR_NO_FILE) {
			printf("Failed to find new file on card [%d]\r\n", res);
			goto main_end_of_test;
		}
		
		test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
		res = f_open(&file_object, (char const *)test_file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (res != FR_OK) {
			printf("Failed to create file on card [%d]\r\n", res);
			goto main_end_of_test;
		}

		printf("Starting data logging...\r\n");
		res = f_puts("year,month,day,hour,min,sec,ms,id,data\n", &file_object);
		if (res == -1) goto sd_cleanup;
		
		port_pin_set_output_level(LED_0_PIN, 0);
		
		delay_ms(5000);
		
		port_pin_set_output_level(LED_0_PIN, 1);
		
		long count = 0;
		
		sprintf(line, "12345678901234567890");
		
		// write a total of 3 mb
		while (count < 3000) {
				count++;
				// 50 bytes
				sprintf(line + 20, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
				sprintf(line + 120, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
				sprintf(line + 220, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
				sprintf(line + 320, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
				sprintf(line + 420, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
				sprintf(line + 520, "123456789012");
				// Write line
				f_write(&file_object, line, 512, NULL);
				// Flush
				f_sync(&file_object);
				
				shift_array_left(line, 550, 512 * 8);
		}
		
		sd_cleanup:
		f_close(&file_object);
		
		port_pin_set_output_level(LED_0_PIN, 0);

		delay_s(10);

		main_end_of_test:
		printf("Please unplug the card.\r\n");
		port_pin_set_output_level(LED_0_PIN, 0);
		while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
		}
	}

}
