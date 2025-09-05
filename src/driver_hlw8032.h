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
 * @file      driver_hlw8032.h
 * @brief     driver hlw8032 header file
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

#ifndef DRIVER_HLW8032_H
#define DRIVER_HLW8032_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup hlw8032_driver hlw8032 driver function
 * @brief    hlw8032 driver modules
 * @{
 */

/**
 * @brief hlw8032 voltage coefficient definition
 */
#ifndef HLW8032_VOLTAGE_COEFFICIENT
    #define HLW8032_VOLTAGE_COEFFICIENT        1.88f        /**< 1.88f */
#endif

/**
 * @brief hlw8032 current coefficient definition
 */
#ifndef HLW8032_CURRENT_COEFFICIENT
    #define HLW8032_CURRENT_COEFFICIENT        1.0f        /**< 1.0f */
#endif

/**
 * @brief hlw8032 voltage k1 definition
 */
#ifndef HLW8032_VOLTAGE_K1
    #define HLW8032_VOLTAGE_K1        2.045f        /**< 2.045f */
#endif

/**
 * @brief hlw8032 current k2 definition
 */
#ifndef HLW8032_CURRENT_K2
    #define HLW8032_CURRENT_K2        1.0f        /**< 1.0f */
#endif

/**
 * @brief hlw8032 power k3 definition
 */
#ifndef HLW8032_POWER_K3
    #define HLW8032_POWER_K3        2.045f        /**< 2.045f */
#endif

/**
 * @addtogroup hlw8032_basic_driver
 * @{
 */

/**
 * @brief hlw8032 mode enumeration definition
 */
typedef enum
{
    HLW8032_MODE_SAMPLE      = 0,        /**< sample */
    HLW8032_MODE_TRANSFORMER = 1,        /**< transformer */
} hlw8032_mode_t;

/**
 * @brief hlw8032 status enumeration definition
 */
typedef enum
{
    HLW8032_STATUS_VOLTAGE_OVERFLOW = (1 << 3),        /**< voltage overflow */
    HLW8032_STATUS_CURRENT_OVERFLOW = (1 << 2),        /**< current overflow */
    HLW8032_STATUS_POWER_OVERFLOW   = (1 << 1),        /**< power overflow */
    HLW8032_STATUS_REGISTER_INVALID = (1 << 0),        /**< register invalid */
} hlw8032_status_t;

/**
 * @brief hlw8032 update status enumeration definition
 */
typedef enum
{
    HLW8032_UPDATE_STATUS_PF_OVERFLOW     = (1 << 7),        /**< pf overflow */
    HLW8032_UPDATE_STATUS_VOLTAGE_UPDATED = (1 << 6),        /**< voltage updated */
    HLW8032_UPDATE_STATUS_CURRENT_UPDATED = (1 << 5),        /**< current updated */
    HLW8032_UPDATE_STATUS_POWER_UPDATED   = (1 << 4),        /**< power updated */
} hlw8032_update_status_t;

/**
 * @brief hlw8032 data structure definition
 */
typedef struct hlw8032_data_s
{
    uint32_t voltage_parameter_raw;        /**< voltage parameter register raw */
    uint32_t voltage_raw;                  /**< voltage register raw */
    uint32_t current_parameter_raw;        /**< current parameter register raw */
    uint32_t current_raw;                  /**< current register raw */
    uint32_t power_parameter_raw;          /**< power parameter register raw */
    uint32_t power_raw;                    /**< power register raw */
    uint8_t update_status;                 /**< update status register */
    uint8_t status;                        /**< status register */
    uint16_t pf_counter;                   /**< pf counter register */
    float effective_voltage_v;             /**< effective voltage in voltage */
    float effective_current_a;             /**< effective current in ampere */
    float active_power_w;                  /**< active power in watt */
    float apparent_power_w;                /**< apparent power in watt */
    float power_factor;                    /**< power factor */
    float quantity_electricity_kwh;        /**< quantity electricity in KW·h */
} hlw8032_data_t;

/**
 * @brief hlw8032 handle structure definition
 */
typedef struct hlw8032_handle_s
{
    uint8_t (*uart_init)(void);                               /**< point to an uart_init function address */
    uint8_t (*uart_deinit)(void);                             /**< point to an uart_deinit function address */
    uint16_t (*uart_read)(uint8_t *buf, uint16_t len);        /**< point to an uart_read function address */
    uint8_t (*uart_flush)(void);                              /**< point to an uart_flush function address */
    void (*delay_ms)(uint32_t ms);                            /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);          /**< point to a debug_print function address */
    uint8_t inited;                                           /**< inited flag */
    uint8_t mode;                                             /**< mode */
    float voltage_coefficient;                                /**< voltage coefficient */
    float current_coefficient;                                /**< current coefficient */
    float voltage_kl;                                         /**< voltage kl */
    float current_k2;                                         /**< current k2 */
    float power_k3;                                           /**< power k3 */
    uint8_t is_first;                                         /**< first flag */
    int64_t pf_counter;                                       /**< pf counter */
    uint16_t pf_last_counter;                                 /**< pf last counter */
    uint8_t pf_last_ovf;                                      /**< pf last ovf */
} hlw8032_handle_t;

/**
 * @brief hlw8032 information structure definition
 */
typedef struct hlw8032_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} hlw8032_info_t;

/**
 * @}
 */

/**
 * @defgroup hlw8032_link_driver hlw8032 link driver function
 * @brief    hlw8032 link driver modules
 * @ingroup  hlw8032_driver
 * @{
 */

/**
 * @brief     initialize hlw8032_handle_t structure
 * @param[in] HANDLE pointer to a hlw8032 handle structure
 * @param[in] STRUCTURE hlw8032_handle_t
 * @note      none
 */
#define DRIVER_HLW8032_LINK_INIT(HANDLE, STRUCTURE)          memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link uart_init function
 * @param[in] HANDLE pointer to a hlw8032 handle structure
 * @param[in] FUC pointer to an uart_init function address
 * @note      none
 */
#define DRIVER_HLW8032_LINK_UART_INIT(HANDLE, FUC)          (HANDLE)->uart_init = FUC

/**
 * @brief     link uart_deinit function
 * @param[in] HANDLE pointer to a hlw8032 handle structure
 * @param[in] FUC pointer to an uart_deinit function address
 * @note      none
 */
#define DRIVER_HLW8032_LINK_UART_DEINIT(HANDLE, FUC)        (HANDLE)->uart_deinit = FUC

/**
 * @brief     link uart_read function
 * @param[in] HANDLE pointer to a hlw8032 handle structure
 * @param[in] FUC pointer to an uart_read function address
 * @note      none
 */
#define DRIVER_HLW8032_LINK_UART_READ(HANDLE, FUC)          (HANDLE)->uart_read = FUC

/**
 * @brief     link uart_flush function
 * @param[in] HANDLE pointer to a hlw8032 handle structure
 * @param[in] FUC pointer to an uart_flush function address
 * @note      none
 */
#define DRIVER_HLW8032_LINK_UART_FLUSH(HANDLE, FUC)         (HANDLE)->uart_flush = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a hlw8032 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_HLW8032_LINK_DELAY_MS(HANDLE, FUC)           (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a hlw8032 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_HLW8032_LINK_DEBUG_PRINT(HANDLE, FUC)        (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup hlw8032_basic_driver hlw8032 basic driver function
 * @brief    hlw8032 basic driver modules
 * @ingroup  hlw8032_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a hlw8032 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t hlw8032_info(hlw8032_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t hlw8032_init(hlw8032_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hlw8032_deinit(hlw8032_handle_t *handle);

/**
 * @brief      read the data
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *data pointer to a data structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 frame is error
 *             - 5 data is invalid
 * @note       none
 */
uint8_t hlw8032_read(hlw8032_handle_t *handle, hlw8032_data_t *data);

/**
 * @brief     set mode
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @param[in] mode chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hlw8032_set_mode(hlw8032_handle_t *handle, hlw8032_mode_t mode);

/**
 * @brief      get mode
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *mode pointer to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_get_mode(hlw8032_handle_t *handle, hlw8032_mode_t *mode);

/**
 * @brief      quantity electricity export
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *quantity_electricity_counter pointer to a quantity electricity counter buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_quantity_electricity_export(hlw8032_handle_t *handle, int64_t *quantity_electricity_counter);

/**
 * @brief     quantity electricity import
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @param[in] quantity_electricity_counter quantity electricity counter
 * @return    status code
 *            - 0 success
 *            - 1 import failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 frame is error
 * @note      none
 */
uint8_t hlw8032_quantity_electricity_import(hlw8032_handle_t *handle, int64_t quantity_electricity_counter);

/**
 * @brief     set sample voltage coefficient
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @param[in] coeff voltage coefficient
 * @return    status code
 *            - 0 success
 *            - 1 set sample voltage coefficient failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hlw8032_set_sample_voltage_coefficient(hlw8032_handle_t *handle, float coeff);

/**
 * @brief      get sample voltage coefficient
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *coeff pointer to a voltage coefficient buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sample voltage coefficient failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_get_sample_voltage_coefficient(hlw8032_handle_t *handle, float *coeff);

/**
 * @brief     set sample current coefficient
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @param[in] coeff current coefficient
 * @return    status code
 *            - 0 success
 *            - 1 set sample current coefficient failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hlw8032_set_sample_current_coefficient(hlw8032_handle_t *handle, float coeff);

/**
 * @brief      get sample current coefficient
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *coeff pointer to a current coefficient buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sample current coefficient failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_get_sample_current_coefficient(hlw8032_handle_t *handle, float *coeff);

/**
 * @brief     set transformer voltage k1
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @param[in] coeff voltage coefficient
 * @return    status code
 *            - 0 success
 *            - 1 set transformer voltage k1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hlw8032_set_transformer_voltage_k1(hlw8032_handle_t *handle, float coeff);

/**
 * @brief      get transformer voltage k1
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *coeff pointer to a voltage coefficient buffer
 * @return     status code
 *             - 0 success
 *             - 1 get transformer voltage k1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_get_transformer_voltage_k1(hlw8032_handle_t *handle, float *coeff);

/**
 * @brief     set transformer current k2
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @param[in] coeff current coefficient
 * @return    status code
 *            - 0 success
 *            - 1 set transformer current k2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hlw8032_set_transformer_current_k2(hlw8032_handle_t *handle, float coeff);

/**
 * @brief      get transformer current k2
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *coeff pointer to a current coefficient buffer
 * @return     status code
 *             - 0 success
 *             - 1 get transformer current k2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_get_transformer_current_k2(hlw8032_handle_t *handle, float *coeff);

/**
 * @brief     set transformer power k3
 * @param[in] *handle pointer to a hlw8032 handle structure
 * @param[in] coeff power coefficient
 * @return    status code
 *            - 0 success
 *            - 1 set transformer power k3 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hlw8032_set_transformer_power_k3(hlw8032_handle_t *handle, float coeff);

/**
 * @brief      get transformer power k3
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *coeff pointer to a power coefficient buffer
 * @return     status code
 *             - 0 success
 *             - 1 get transformer power k3 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_get_transformer_power_k3(hlw8032_handle_t *handle, float *coeff);

/**
 * @}
 */

/**
 * @defgroup hlw8032_extern_driver hlw8032 extern driver function
 * @brief    hlw8032 extern driver modules
 * @ingroup  hlw8032_driver
 * @{
 */

/**
 * @brief      get buffer
 * @param[in]  *handle pointer to a hlw8032 handle structure
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len buffer length
 * @param[in]  ms delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 get buf failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hlw8032_get_buffer(hlw8032_handle_t *handle, char *buf, uint16_t len, uint32_t ms);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
