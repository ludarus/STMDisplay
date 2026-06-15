/*
 * display.c
 *
 *  Created on: Jun 14, 2026
 *      Author: Luke Fadel
 */
#include "main.h"
#include "image.h"

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

void writePixel(SPI_HandleTypeDef *spi, uint8_t x, uint8_t y, uint16_t colour) {
	// x coordinates command - colset
	LCD_CMD(spi, 0x2A);

	//setting the range to start at x and end at x
	uint8_t col[] = {

	(x >> 8) & 0xFF,   // x high byte
	x & 0xFF,           // x low byte
	(x >> 8) & 0xFF,   // end x high byte (same pixel)
	x & 0xFF            // end x low byte
	};

//sending data
	LCD_DATA(spi, col, 4);

// y coordinates command - pageset
	LCD_CMD(spi, 0x2B);

// setting the range to start at y and end at y
	uint8_t page[] = { (y >> 8) & 0xFF, y & 0xFF, (y >> 8) & 0xFF, y & 0xFF };

//sending data
	LCD_DATA(spi, page, 4);

// memory write command
	LCD_CMD(spi, 0x2C);

//sending pixel colour
	uint8_t pixel[] = { (colour >> 8) & 0xFF, colour & 0xFF };
	LCD_DATA(spi, pixel, 2);
}

void fillScreen(SPI_HandleTypeDef *spi, uint16_t colour) {
	// set column address: 0–239 (full 240 px width) - instruction 2Ah
	LCD_CMD(spi, 0x2A);      // CASET
	uint8_t caset[] = { 0x00, 0x00, 0x00, /*239*/0xEF };
	LCD_DATA(spi, caset, 4);

	// set row address: 0–319 (full 320 px height)
	LCD_CMD(spi, 0x2B);      // PASET

	uint8_t paset[] = { 0x00, 0x00,
	/*splitting 319 into 2 bits because it's bigger than 255*/
	0x01, 0x3F };
	LCD_DATA(spi, paset, 4);

	// memory write command
	LCD_CMD(spi, 0x2C);

	//transmitting all in one action for faster drawing
	//setting DC pin to write mode (high)
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);

	//selecting SPI device
	LCD_SELECT();

	//sending pixel colour
	uint8_t screen[] = { (colour >> 8) & 0xFF, colour & 0xFF };
	for (uint32_t i = 0; i < 240UL * 320UL; i++) {
		HAL_SPI_Transmit(spi, screen, 2, HAL_MAX_DELAY);
	}

	//deselecting
	LCD_DESELECT();

}

void displayImage(SPI_HandleTypeDef *spi) {
	// set column address: 0–239 (full 240 px width) - instruction 2Ah
	LCD_CMD(spi, 0x2A);      // CASET
	uint8_t caset[] = { 0x00, 0x00, 0x00, /*239*/0xEF };
	LCD_DATA(spi, caset, 4);

	// set row address: 0–319 (full 320 px height)
	LCD_CMD(spi, 0x2B);      // PASET

	uint8_t paset[] = { 0x00, 0x00,
	/*splitting 319 into 2 bits because it's bigger than 255*/
	0x01, 0x3F };
	LCD_DATA(spi, paset, 4);

	// memory write command
	LCD_CMD(spi, 0x2C);

	//transmitting all in one action for faster drawing
	//setting DC pin to write mode (high)
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);

	//selecting SPI device
	LCD_SELECT();

	//sending image data. chunking the data by column because the image size could be (and is) greater than 2^16

	for (uint16_t x = 0; x < 320; x++) {
		uint8_t * p = &imageData[x * 2 * 240];
		HAL_SPI_Transmit(spi, p, 240 * 2, HAL_MAX_DELAY);
//		HAL_SPI_Transmit_DMA(spi, p, 240 * 2);
	}

	//deselecting
	LCD_DESELECT();
}
