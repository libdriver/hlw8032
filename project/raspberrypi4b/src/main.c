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
 * @file      main.c
 * @brief     main source file
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
#include "driver_hlw8032_read_test.h"
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief     hlw8032 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t hlw8032(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"times", required_argument, NULL, 1},
        {"mode", required_argument, NULL, 2},
        {"counter", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    int64_t pf_counter = 0;
    hlw8032_mode_t mode = HLW8032_MODE_SAMPLE;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* times */
            case 1 :
            {
                /* set times */
                times = atoi(optarg);
                
                break;
            }
            
            /* mode */
            case 2 :
            {
                if (strcmp("SAMPLE", optarg) == 0)
                {
                    mode = HLW8032_MODE_SAMPLE;
                }
                else if (strcmp("TRANSFORMER", optarg) == 0)
                {
                    mode = HLW8032_MODE_TRANSFORMER;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* counter */
            case 3 :
            {
                /* set counter */
                pf_counter = atol(optarg);
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (hlw8032_read_test(mode, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = hlw8032_basic_init(mode);
        if (res != 0)
        {
            return 1;
        }
        
        /* import quantity electricity */
        res = hlw8032_basic_quantity_electricity_import(pf_counter);
        if (res != 0)
        {
            (void)hlw8032_basic_deinit();
            
            return 1;
        }
        
        for (i = 0; i < times; i++)
        {
            int64_t quantity_electricity_counter;
            hlw8032_data_t data;
            
            /* delay 1000ms */
            hlw8032_interface_delay_ms(1000);
            
            /* read data */
            res = hlw8032_basic_read(&data);
            if (res != 0)
            {
                (void)hlw8032_basic_deinit();
                
                return 1;
            }
            
            /* quantity electricity export */
            res = hlw8032_basic_quantity_electricity_export(&quantity_electricity_counter);
            if (res != 0)
            {
                (void)hlw8032_basic_deinit();
                
                return 1;
            }
            
            /* output */
            hlw8032_interface_debug_print("hlw8032: %d/%d.\n", i + 1, times);
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
        
        /* deinit */
        (void)hlw8032_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        hlw8032_interface_debug_print("Usage:\n");
        hlw8032_interface_debug_print("  hlw8032 (-i | --information)\n");
        hlw8032_interface_debug_print("  hlw8032 (-h | --help)\n");
        hlw8032_interface_debug_print("  hlw8032 (-p | --port)\n");
        hlw8032_interface_debug_print("  hlw8032 (-t read | --test=read) [--mode=<SAMPLE | TRANSFORMER>] [--times=<num>]\n");
        hlw8032_interface_debug_print("  hlw8032 (-e read | --example=read) [--mode=<SAMPLE | TRANSFORMER>] [--counter=<cnt>] ");
        hlw8032_interface_debug_print("[--times=<num>]\n");
        hlw8032_interface_debug_print("\n");
        hlw8032_interface_debug_print("Options:\n");
        hlw8032_interface_debug_print("      --counter=<cnt>                Set quantity electricity counter.([default: 0])\n");
        hlw8032_interface_debug_print("  -e <read>, --example=<read>        Run the driver example.\n");
        hlw8032_interface_debug_print("  -h, --help                         Show the help.\n");
        hlw8032_interface_debug_print("  -i, --information                  Show the chip information.\n");
        hlw8032_interface_debug_print("      --mode=<SAMPLE | TRANSFORMER>  Set the chip mode.([default: SAMPLE])\n");
        hlw8032_interface_debug_print("  -p, --port                         Display the pins used by this device to connect the chip.\n");
        hlw8032_interface_debug_print("  -t <read>, --test=<read>           Run the driver test.\n");
        hlw8032_interface_debug_print("      --times=<num>                  Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        hlw8032_info_t info;
        
        /* print hlw8032 info */
        hlw8032_info(&info);
        hlw8032_interface_debug_print("hlw8032: chip is %s.\n", info.chip_name);
        hlw8032_interface_debug_print("hlw8032: manufacturer is %s.\n", info.manufacturer_name);
        hlw8032_interface_debug_print("hlw8032: interface is %s.\n", info.interface);
        hlw8032_interface_debug_print("hlw8032: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        hlw8032_interface_debug_print("hlw8032: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        hlw8032_interface_debug_print("hlw8032: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        hlw8032_interface_debug_print("hlw8032: max current is %0.2fmA.\n", info.max_current_ma);
        hlw8032_interface_debug_print("hlw8032: max temperature is %0.1fC.\n", info.temperature_max);
        hlw8032_interface_debug_print("hlw8032: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        hlw8032_interface_debug_print("hlw8032: TX connected to GPIO15(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = hlw8032(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        hlw8032_interface_debug_print("hlw8032: run failed.\n");
    }
    else if (res == 5)
    {
        hlw8032_interface_debug_print("hlw8032: param is invalid.\n");
    }
    else
    {
        hlw8032_interface_debug_print("hlw8032: unknown status code.\n");
    }

    return 0;
}
