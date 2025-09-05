/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_hlw8032_basic.h
 * @brief     driver hlw8032 basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-09-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/09/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_HLW8032_BASIC_H
#define DRIVER_HLW8032_BASIC_H

#include "driver_hlw8032_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup hlw8032_example_driver hlw8032 example driver function
 * @brief    hlw8032 example driver modules
 * @ingroup  hlw8032_driver
 * @{
 */

/**
 * @brief hlw8032 basic example default definition
 */
#define HLW8032_BASIC_DEFAULT_SAMPLE_VOLTAGE_COEFFICIENT        1.88f       /**< 1.88f */
#define HLW8032_BASIC_DEFAULT_SAMPLE_CURRENT_COEFFICIENT        1.0f        /**< 1.0f */
#define HLW8032_BASIC_DEFAULT_TRANSFORMER_VOLTAGE_K1            2.045f      /**< 2.045f */
#define HLW8032_BASIC_DEFAULT_TRANSFORMER_CURRENT_K2            1.0f        /**< 1.0f */
#define HLW8032_BASIC_DEFAULT_TRANSFORMER_POWER_K3              2.045f      /**< 2.045f */

/**
 * @brief     basic example init
 * @param[in] mode chip mode
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t hlw8032_basic_init(hlw8032_mode_t mode);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t hlw8032_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *data pointer to a data structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hlw8032_basic_read(hlw8032_data_t *data);

/**
 * @brief      basic example quantity electricity export
 * @param[out] *quantity_electricity_counter pointer to a quantity electricity counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 quantity electricity export failed
 * @note       none
 */
uint8_t hlw8032_basic_quantity_electricity_export(int64_t *quantity_electricity_counter);

/**
 * @brief     basic example quantity electricity import
 * @param[in] quantity_electricity_counter quantity electricity counter
 * @return    status code
 *            - 0 success
 *            - 1 quantity electricity import failed
 * @note      none
 */
uint8_t hlw8032_basic_quantity_electricity_import(int64_t quantity_electricity_counter);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
