/*
 * display.h
 *
 *  Created on: Jun 14, 2026
 *      Author: joe
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

void LCD_RESET(void);
void LCD_SELECT(void);
void LCD_DESELECT(void);
void LCD_DATA(SPI_HandleTypeDef *spi, uint8_t *data, uint16_t size);
void LCD_CMD(SPI_HandleTypeDef *spi, uint8_t cmd);
void writePixel(SPI_HandleTypeDef *spi, uint8_t x, uint8_t y, uint16_t colour);
void fillScreen(SPI_HandleTypeDef *spi, uint16_t colour);

#endif /* INC_DISPLAY_H_ */
