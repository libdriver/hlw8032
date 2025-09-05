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
 * @file      driver_hlw8032_read_test.c
 * @brief     driver hlw8032 read test source file
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

#include "driver_hlw8032_read_test.h"

static hlw8032_handle_t gs_handle;        /**< hlw8032 handle */

/**
 * @brief     read test
 * @param[in] mode chip mode
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t hlw8032_read_test(hlw8032_mode_t mode, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    hlw8032_info_t info;
    
    /* link interface function */
    DRIVER_HLW8032_LINK_INIT(&gs_handle, hlw8032_handle_t);
    DRIVER_HLW8032_LINK_UART_INIT(&gs_handle, hlw8032_interface_uart_init);
    DRIVER_HLW8032_LINK_UART_DEINIT(&gs_handle, hlw8032_interface_uart_deinit);
    DRIVER_HLW8032_LINK_UART_READ(&gs_handle, hlw8032_interface_uart_read);
    DRIVER_HLW8032_LINK_UART_FLUSH(&gs_handle, hlw8032_interface_uart_flush);
    DRIVER_HLW8032_LINK_DELAY_MS(&gs_handle, hlw8032_interface_delay_ms);
    DRIVER_HLW8032_LINK_DEBUG_PRINT(&gs_handle, hlw8032_interface_debug_print);
    
    /* get hlw8032 information */
    res = hlw8032_info(&info);
    if (res != 0)
    {
        hlw8032_interface_debug_print("hlw8032: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        hlw8032_interface_debug_print("hlw8032: chip is %s.\n", info.chip_name);
        hlw8032_interface_debug_print("hlw8032: manufacturer is %s.\n", info.manufacturer_name);
        hlw8032_interface_debug_print("hlw8032: interface is %s.\n", info.interface);
        hlw8032_interface_debug_print("hlw8032: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        hlw8032_interface_debug_print("hlw8032: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        hlw8032_interface_debug_print("hlw8032: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        hlw8032_interface_debug_print("hlw8032: max current is %0.2fmA.\n", info.max_current_ma);
        hlw8032_interface_debug_print("hlw8032: max temperature is %0.1fC.\n", info.temperature_max);
        hlw8032_interface_debug_print("hlw8032: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
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
    
    /* start read test */
    hlw8032_interface_debug_print("hlw8032: start read test.\n");
    
    for (i = 0; i < times; i++)
    {
        int64_t quantity_electricity_counter;
        hlw8032_data_t data;
        
        /* delay 2000ms */
        hlw8032_interface_delay_ms(2000);
        
        /* read */
        res = hlw8032_read(&gs_handle, &data);
        if (res != 0)
        {
            hlw8032_interface_debug_print("hlw8032: read failed.\n");
            (void)hlw8032_deinit(&gs_handle);
            
            return 1;
        }
        
        /* quantity electricity export */
        res = hlw8032_quantity_electricity_export(&gs_handle, &quantity_electricity_counter);
        if (res != 0)
        {
            hlw8032_interface_debug_print("hlw8032: quantity electricity export failed.\n");
            (void)hlw8032_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        hlw8032_interface_debug_print("hlw8032: voltage parameter raw is 0x%08X.\n", data.voltage_parameter_raw);
        hlw8032_interface_debug_print("hlw8032: voltage raw is 0x%08X.\n", data.voltage_raw);
        hlw8032_interface_debug_print("hlw8032: current parameter raw is 0x%08X.\n", data.current_parameter_raw);
        hlw8032_interface_debug_print("hlw8032: current raw is 0x%08X.\n", data.current_raw);
        hlw8032_interface_debug_print("hlw8032: power parameter raw is 0x%08X.\n", data.power_parameter_raw);
        hlw8032_interface_debug_print("hlw8032: power raw is 0x%08X.\n", data.power_raw);
        hlw8032_interface_debug_print("hlw8032: update status is 0x%02X.\n", data.update_status);
        hlw8032_interface_debug_print("hlw8032: status is 0x%02X.\n", data.status);
        hlw8032_interface_debug_print("hlw8032: pf counter is %d.\n", data.pf_counter);
        hlw8032_interface_debug_print("hlw8032: effective voltage is %0.2fV.\n", data.effective_voltage_v);
        hlw8032_interface_debug_print("hlw8032: effective current is %0.2fA.\n", data.effective_current_a);
        hlw8032_interface_debug_print("hlw8032: active power is %0.2fW.\n", data.active_power_w);
        hlw8032_interface_debug_print("hlw8032: apparent power is %0.2fW.\n", data.apparent_power_w);
        hlw8032_interface_debug_print("hlw8032: power factor is %0.2f.\n", data.power_factor);
        hlw8032_interface_debug_print("hlw8032: quantity electricity is %0.2fKW·h.\n", data.quantity_electricity_kwh);
        hlw8032_interface_debug_print("hlw8032: quantity electricity counter is %lld.\n", quantity_electricity_counter);
    }
    
    /* finish read test */
    hlw8032_interface_debug_print("hlw8032: finish read test.\n");
    (void)hlw8032_deinit(&gs_handle);

    return 0;
}
