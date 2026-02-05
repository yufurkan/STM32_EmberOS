/*
 * i2c_scanner.h
 *
 *  Created on: Dec 30, 2025
 *      Author: yufur
 */

#ifndef INC_I2C_SCANNER_H_
#define INC_I2C_SCANNER_H_

#include "stm32f4xx_hal.h"
#include "main.h"

//burada bu doysadaki tanımlı fonksiyonların kullanımı ile ilgili sıkıntı yaşadım bu yüzden burayı c derleyicisine  veriyorum

//Im having trouble using the functions defined in this file. so Im handing this over to the C compiler.
#ifdef __cplusplus
extern "C" {
#endif


// I2C hattını parametre olarak al -- Uart da uyarlanabilir
void I2C_Scanner_Baslat(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart, char *HatIsmi);
#ifdef __cplusplus
}
#endif
#endif /* INC_I2C_SCANNER_H_ */
