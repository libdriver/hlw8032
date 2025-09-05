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
 * @file      driver_hlw8032_basic.c
 * @brief     driver hlw8032 basic source file
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

#include "driver_hlw8032_basic.h"

static hlw8032_handle_t gs_handle;        /**< hlw8032 handle */

/**
 * @brief     basic example init
 * @param[in] mode chip mode
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t hlw8032_basic_init(hlw8032_mode_t mode)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_HLW8032_LINK_INIT(&gs_handle, hlw8032_handle_t);
    DRIVER_HLW8032_LINK_UART_INIT(&gs_handle, hlw8032_interface_uart_init);
    DRIVER_HLW8032_LINK_UART_DEINIT(&gs_handle, hlw8032_interface_uart_deinit);
    DRIVER_HLW8032_LINK_UART_READ(&gs_handle, hlw8032_interface_uart_read);
    DRIVER_HLW8032_LINK_UART_FLUSH(&gs_handle, hlw8032_interface_uart_flush);
    DRIVER_HLW8032_LINK_DELAY_MS(&gs_handle, hlw8032_interface_delay_ms);
    DRIVER_HLW8032_LINK_DEBUG_PRINT(&gs_handle, hlw8032_interface_debug_print);
    
    /* hlw8032 init */
    res = hlw8032_init(&gs_handle);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: init failed.\n");
        
        return 1;
    }
    
    /* set mode */
    res = hlw8032_set_mode(&gs_handle, mode);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: set mode failed.\n");
        (void)hlw8032_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default sample voltage coefficient */
    res = hlw8032_set_sample_voltage_coefficient(&gs_handle, HLW8032_BASIC_DEFAULT_SAMPLE_VOLTAGE_COEFFICIENT);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: set sample voltage coefficient failed.\n");
        (void)hlw8032_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default sample current coefficient */
    res = hlw8032_set_sample_current_coefficient(&gs_handle, HLW8032_BASIC_DEFAULT_SAMPLE_CURRENT_COEFFICIENT);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: set sample current coefficient failed.\n");
        (void)hlw8032_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default transformer voltage k1 */
    res = hlw8032_set_transformer_voltage_k1(&gs_handle, HLW8032_BASIC_DEFAULT_TRANSFORMER_VOLTAGE_K1);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: set transformer voltage k1 failed.\n");
        (void)hlw8032_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default transformer current k2 */
    res = hlw8032_set_transformer_current_k2(&gs_handle, HLW8032_BASIC_DEFAULT_TRANSFORMER_CURRENT_K2);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: set transformer current k2 failed.\n");
        (void)hlw8032_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default transformer power k3 */
    res = hlw8032_set_transformer_power_k3(&gs_handle, HLW8032_BASIC_DEFAULT_TRANSFORMER_POWER_K3);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: set transformer power k3 failed.\n");
        (void)hlw8032_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example quantity electricity export
 * @param[out] *quantity_electricity_counter pointer to a quantity electricity counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 quantity electricity export failed
 * @note       none
 */
uint8_t hlw8032_basic_quantity_electricity_export(int64_t *quantity_electricity_counter)
{
    uint8_t res;
    
    /* export */
    res = hlw8032_quantity_electricity_export(&gs_handle, quantity_electricity_counter);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example quantity electricity import
 * @param[in] quantity_electricity_counter quantity electricity counter
 * @return    status code
 *            - 0 success
 *            - 1 quantity electricity import failed
 * @note      none
 */
uint8_t hlw8032_basic_quantity_electricity_import(int64_t quantity_electricity_counter)
{
    uint8_t res;
    
    /* import */
    res = hlw8032_quantity_electricity_import(&gs_handle, quantity_electricity_counter);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *data pointer to a data structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hlw8032_basic_read(hlw8032_data_t *data)
{
    uint8_t res;
    
    /* read */
    res = hlw8032_read(&gs_handle, data);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t hlw8032_basic_deinit(void)
{
    /* deinit hlw8032 */
    if (hlw8032_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
