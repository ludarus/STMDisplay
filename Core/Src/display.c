/*
 * display.c
 *
 *  Created on: Jun 14, 2026
 *      Author: Luke Fadel
 */
#include "main.h"

// userguide description = Reset active low. Mapped to pin PA1
void LCD_RESET(void) {
	//setting the reset pin to low to signal a reset
	HAL_GPIO_WritePin(DISP_RESET_GPIO_Port, DISP_RESET_Pin, GPIO_PIN_RESET);

	//small delay
	HAL_Delay(10);

	//setting the pin to high (default state)
	HAL_GPIO_WritePin(DISP_RESET_GPIO_Port, DISP_RESET_Pin, GPIO_PIN_SET);

	HAL_Delay(100);
}

//userguide description = SPI chip select active high
// update: there must be an inverter within the architecture, as the cs pin is actually active low.
void LCD_SELECT(void) {
	//setting the select pin to low
	HAL_GPIO_WritePin(LCD_NC_GPIO_Port, LCD_NC_Pin, GPIO_PIN_RESET);
}

void LCD_DESELECT(void) {
	//setting the select pin to high
	HAL_GPIO_WritePin(LCD_NC_GPIO_Port, LCD_NC_Pin, GPIO_PIN_SET);
}

//SPI SETTINGS: 8 bit data size
void LCD_DATA(SPI_HandleTypeDef *spi, uint8_t *data, uint16_t size) {
	//setting DC pin to write mode (high)
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);

	//selecting SPI device
	LCD_SELECT();

	//using SPI to transmit data
	HAL_SPI_Transmit(spi, data, size, HAL_MAX_DELAY);

	//deselecting SPI device
	LCD_DESELECT();
}

void LCD_CMD(SPI_HandleTypeDef *spi, uint8_t cmd) {
	//setting DC pin to command mode (low)
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);

	//selecting SPI device
	LCD_SELECT();

	//using SPI to transmit data
	HAL_SPI_Transmit(spi, &cmd, 1, HAL_MAX_DELAY);

	//deselecting SPI device
	LCD_DESELECT();
}
