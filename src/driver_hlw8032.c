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
 * @file      driver_hlw8032.c
 * @brief     driver hlw8032 source file
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

#include "driver_hlw8032.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "HLW TECHNOLOGY HLW8032"        /**< chip name */
#define MANUFACTURER_NAME         "HLW TECHNOLOGY"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        4.5f                            /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                            /**< chip max supply voltage */
#define MAX_CURRENT               4.0f                            /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                          /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                           /**< chip max operating temperature */
#define DRIVER_VERSION            1000                            /**< driver version */

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
uint8_t hlw8032_init(hlw8032_handle_t *handle)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->debug_print == NULL)                                   /* check debug_print */
    {
        return 3;                                                      /* return error */
    }
    if (handle->uart_init == NULL)                                     /* check uart_init */
    {
        handle->debug_print("hlw8032: uart_init is null.\n");          /* uart_init is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->uart_deinit == NULL)                                   /* check uart_deinit */
    {
        handle->debug_print("hlw8032: uart_deinit is null.\n");        /* uart_deinit is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->uart_read == NULL)                                     /* check uart_read */
    {
        handle->debug_print("hlw8032: uart_read is null.\n");          /* uart_read is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->uart_flush == NULL)                                    /* check uart_flush */
    {
        handle->debug_print("hlw8032: uart_flush is null.\n");         /* uart_flush is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->delay_ms == NULL)                                      /* check delay_ms */
    {
        handle->debug_print("hlw8032: delay_ms is null.\n");           /* delay_ms is null */
        
        return 3;                                                      /* return error */
    }

    if (handle->uart_init() != 0)                                      /* uart init */
    {
        handle->debug_print("hlw8032: uart init failed.\n");           /* uart init failed */
        
        return 1;                                                      /* return error */
    }
    handle->mode = 0;                                                  /* init 0 */
    handle->pf_counter = 0;                                            /* init 0 */
    handle->pf_last_counter = 0;                                       /* init 0 */
    handle->pf_last_ovf = 0;                                           /* init 0 */
    handle->is_first = 0;                                              /* init 0 */
    handle->voltage_coefficient = HLW8032_VOLTAGE_COEFFICIENT;         /* set voltage coefficient */
    handle->current_coefficient = HLW8032_CURRENT_COEFFICIENT;         /* set current coefficient */
    handle->voltage_k1 = HLW8032_VOLTAGE_K1;                           /* set voltage kl */
    handle->current_k2 = HLW8032_CURRENT_K2;                           /* set current k2 */
    handle->power_k3 = HLW8032_POWER_K3;                               /* set power k3 */
    handle->inited = 1;                                                /* flag finish initialization */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t hlw8032_deinit(hlw8032_handle_t *handle)
{
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (handle->uart_deinit() != 0)                                   /* uart deinit */
    {
        handle->debug_print("hlw8032: uart deinit failed.\n");        /* uart deinit failed */
        
        return 1;                                                     /* return error */
    }         
    handle->inited = 0;                                               /* flag close */
    
    return 0;                                                         /* success return 0 */ 
}

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
uint8_t hlw8032_read(hlw8032_handle_t *handle, hlw8032_data_t *data)
{
    uint8_t res;
    uint8_t i;
    uint8_t j;
    uint8_t offset;
    uint8_t done;
    uint16_t len;
    uint16_t check_sum;
    double cnt;
    uint8_t buf[48];
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    res = handle->uart_flush();                                                  /* uart flush */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("hlw8032: uart flush failed.\n");                    /* uart flush failed */
        
        return 1;                                                                /* return error */
    }
    handle->delay_ms(250);                                                       /* delay 250ms */
    len = handle->uart_read((uint8_t *)buf, 48);                                 /* uart read */
    if (len != 48)                                                               /* check result */
    {
        handle->debug_print("hlw8032: uart read failed.\n");                     /* uart read failed */
        
        return 1;                                                                /* return error */
    }
    offset = 0;                                                                  /* init 0 */
    done = 0;                                                                    /* init 0 */
    for (i = 0; i < 48; i++)                                                     /* loop all */
    {
        if ((i != 0) && (buf[i] == 0x5A) && ((i + 23) < 48))                     /* check frame */
        {
            check_sum = 0;                                                       /* init 0 */
            for (j = 0; j < 21; j++)                                             /* sum all */
            {
                check_sum += buf[i - 1 + 2 + j];                                 /* add */
            }
            if (buf[i - 1 + 23] != (check_sum & 0xFF))                           /* check frame trail */
            {
                continue;                                                        /* continue */
            }
            offset = i - 1;                                                      /* save index */
            done = 1;                                                            /* flag done */
            
            break;                                                               /* break */
        }
    }
    if (done != 1)                                                               /* check done */
    {
        handle->debug_print("hlw8032: frame is error.\n");                       /* frame is error */
        
        return 4;                                                                /* return error */
    }
    data->status = buf[offset + 0];                                              /* set status */
    data->voltage_parameter_raw = ((uint32_t)buf[offset + 2] << 16) |
                                  ((uint32_t)buf[offset + 3] << 8) |
                                  ((uint32_t)buf[offset + 4] << 0);              /* set voltage parameter raw */
    data->voltage_raw = ((uint32_t)buf[offset + 5] << 16) |
                        ((uint32_t)buf[offset + 6] << 8) |
                        ((uint32_t)buf[offset + 7] << 0);                        /* set voltage raw */
    data->current_parameter_raw = ((uint32_t)buf[offset + 8] << 16) |
                                  ((uint32_t)buf[offset + 9] << 8) |
                                  ((uint32_t)buf[offset + 10] << 0);             /* set current parameter raw */
    data->current_raw = ((uint32_t)buf[offset + 11] << 16) |
                        ((uint32_t)buf[offset + 12] << 8) |
                        ((uint32_t)buf[offset + 13] << 0);                       /* set current raw */
    data->power_parameter_raw = ((uint32_t)buf[offset + 14] << 16) |
                                ((uint32_t)buf[offset + 15] << 8) |
                                ((uint32_t)buf[offset + 16] << 0);               /* set power parameter raw */
    data->power_raw = ((uint32_t)buf[offset + 17] << 16) |
                      ((uint32_t)buf[offset + 18] << 8) |
                      ((uint32_t)buf[offset + 19] << 0);                         /* set power raw */
    data->update_status = buf[offset + 20];                                      /* set update status */
    data->pf_counter = ((uint16_t)buf[offset + 21] << 8) |
                       ((uint16_t)buf[offset + 22] << 0);                        /* set pf counter */
    if (data->status != 0x55)                                                    /* check status */
    {
        handle->debug_print("hlw8032: data is invalid.\n");                      /* data is invalid */
        
        return 5;                                                                /* return error */
    }
    if (handle->is_first != 0)                                                   /* check first */
    {
        uint8_t ovf;
        
        ovf = (data->update_status >> 7) & 0x01;                                 /* get ovf */
        if (ovf != handle->pf_last_ovf)                                          /* ovf */
        {
            handle->pf_counter += 65536;                                         /* add 65536 */
            handle->pf_counter += (int64_t)((int32_t)data->pf_counter - 
                                            (int32_t)handle->pf_last_counter);   /* add to total counter */
        }
        else
        {
            handle->pf_counter += (int64_t)((int32_t)data->pf_counter - 
                                            (int32_t)handle->pf_last_counter);   /* add to total counter */
        }
        handle->pf_last_ovf = ovf;                                               /* save to last ovf */
        handle->pf_last_counter = data->pf_counter;                              /* save to last counter */
    }
    else
    {
        handle->pf_last_counter = data->pf_counter;                              /* save to last counter */
        handle->pf_last_ovf = (data->update_status >> 7) & 0x01;                 /* save to last ovf */
        handle->is_first = 1;                                                    /* set fist */
    }
    if (handle->mode != 0)                                                       /* transformer */
    {
        if ((data->update_status & (1 << 6)) != 0)                               /* check voltage bit */
        {
            data->effective_voltage_v = (float)(data->voltage_parameter_raw) / 
                                        (float)(data->voltage_raw) * 
                                        (float)(handle->voltage_k1);             /* set effective voltage */
        }
        else
        {
            data->effective_voltage_v = 0.0f;                                    /* set 0.0 */
        }
        if ((data->update_status & (1 << 5)) != 0)                               /* check current bit */
        {
            data->effective_current_a = (float)(data->current_parameter_raw) / 
                                        (float)(data->current_raw) * 
                                        (float)(handle->current_k2);             /* set effective current */
        }
        else
        {
            data->effective_current_a = 0.0f;                                    /* set 0.0 */
        }
        if ((data->update_status & (1 << 4)) != 0)                               /* check power bit */
        {
            data->active_power_w = (float)(data->power_parameter_raw) / 
                                   (float)(data->power_raw) * 
                                   (float)(handle->power_k3);                    /* set active power */
        }
        else
        {
            data->active_power_w = 0.0f;                                         /* set 0.0 */
        }
        if ((data->update_status & (7 << 4)) == (7 << 4))                        /* check power bit */
        {
            data->apparent_power_w = (float)(data->effective_voltage_v) * 
                                     (float)(data->effective_current_a);         /* set apparent power */
        }
        else
        {
            data->apparent_power_w = 0.0f;                                       /* set 0.0 */
            data->effective_current_a = 0.0f;                                    /* force to 0.0 */
        }
        if ((data->update_status & (7 << 4)) == (7 << 4))                        /* check power bit */
        {
            data->power_factor = data->active_power_w / data->apparent_power_w;  /* set power factor */
            if (data->power_factor > 1.0f)                                       /* check range */
            {
                data->power_factor = 1.0f;                                       /* force to 1.0 */
            }
        }
        else
        {
            data->power_factor = 0.0f;                                           /* set 0.0 */
        }
        cnt = (1.0 / (double)(data->power_parameter_raw)) * 
              (1.0 / ((double)(handle->power_k3))) * 
              1000000000.0 * 3600.0;                                             /* get cnt */
        data->quantity_electricity_kwh = (float)((double)(handle->pf_counter) / 
                                                (double)(cnt));                  /* set quantity electricity */
    }
    else                                                                         /* sample */
    {
        if ((data->update_status & (1 << 6)) != 0)                               /* check voltage bit */
        {
            data->effective_voltage_v = (float)(data->voltage_parameter_raw) / 
                                        (float)(data->voltage_raw) * 
                                        (float)(handle->voltage_coefficient);    /* set effective voltage */
        }
        else
        {
            data->effective_voltage_v = 0.0f;                                    /* set 0.0 */
        }
        if ((data->update_status & (1 << 5)) != 0)                               /* check current bit */
        {
            data->effective_current_a = (float)(data->current_parameter_raw) / 
                                        (float)(data->current_raw) * 
                                        (float)(handle->current_coefficient);    /* set effective current */
        }
        else
        {
            data->effective_current_a = 0.0f;                                    /* set 0.0 */
        }
        if ((data->update_status & (1 << 4)) != 0)                               /* check power bit */
        {
            data->active_power_w = (float)(data->power_parameter_raw) / 
                                   (float)(data->power_raw) * 
                                   (float)(handle->voltage_coefficient) *
                                   (float)(handle->current_coefficient);         /* set active power */
        }
        else
        {
            data->active_power_w = 0.0f;                                         /* set 0.0 */
        }
        if ((data->update_status & (7 << 4)) == (7 << 4))                        /* check power bit */
        {
            data->apparent_power_w = (float)(data->effective_voltage_v) * 
                                     (float)(data->effective_current_a);         /* set apparent power */
        }
        else
        {
            data->apparent_power_w = 0.0f;                                       /* set 0.0 */
            data->effective_current_a = 0.0f;                                    /* force to 0.0 */
        }
        if ((data->update_status & (7 << 4)) == (7 << 4))                        /* check power bit */
        {
            data->power_factor = data->active_power_w / data->apparent_power_w;  /* set power factor */
            if (data->power_factor > 1.0f)                                       /* check range */
            {
                data->power_factor = 1.0f;                                       /* force to 1.0 */
            }
        }
        else
        {
            data->power_factor = 0.0f;                                           /* set 0.0 */
        }
        cnt = (1.0 / (double)(data->power_parameter_raw)) * 
              (1.0 / ((double)(handle->voltage_coefficient) * 
              (double)(handle->current_coefficient))) *
              1000000000.0 * 3600.0;                                             /* get cnt */
        data->quantity_electricity_kwh = (float)((double)(handle->pf_counter) / 
                                                (double)(cnt));                  /* set quantity electricity */
    }
    
    return 0;                                                                    /* success return 0 */ 
}

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
uint8_t hlw8032_quantity_electricity_export(hlw8032_handle_t *handle, int64_t *quantity_electricity_counter)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    if (handle->inited != 1)                                   /* check handle initialization */
    {
        return 3;                                              /* return error */
    }
    
    *quantity_electricity_counter = handle->pf_counter;        /* set quantity electricity counter */
    
    return 0;                                                  /* success return 0 */ 
}

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
uint8_t hlw8032_quantity_electricity_import(hlw8032_handle_t *handle, int64_t quantity_electricity_counter)
{
    uint8_t res;
    uint8_t i;
    uint8_t j;
    uint8_t offset;
    uint8_t done;
    uint8_t update_status;
    uint16_t len;
    uint16_t check_sum;
    uint16_t pf_counter;
    uint8_t buf[48];
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    
    res = handle->uart_flush();                                      /* uart flush */
    if (res != 0)                                                    /* check result */
    {
        handle->debug_print("hlw8032: uart flush failed.\n");        /* uart flush failed */
        
        return 1;                                                    /* return error */
    }
    handle->delay_ms(250);                                           /* delay 250ms */
    len = handle->uart_read((uint8_t *)buf, 48);                     /* uart read */
    if (len != 48)                                                   /* check result */
    {
        handle->debug_print("hlw8032: uart read failed.\n");         /* uart read failed */
        
        return 1;                                                    /* return error */
    }
    offset = 0;                                                      /* init 0 */
    done = 0;                                                        /* init 0 */
    for (i = 0; i < 48; i++)                                         /* loop all */
    {
        if ((i != 0) && (buf[i] == 0x5A) && ((i + 23) < 48))         /* check frame */
        {
            check_sum = 0;                                           /* init 0 */
            for (j = 0; j < 21; j++)                                 /* sum all */
            {
                check_sum += buf[i - 1 + 2 + j];                     /* add */
            }
            if (buf[i - 1 + 23] != (check_sum & 0xFF))               /* check frame trail */
            {
                continue;                                            /* continue */
            }
            offset = i - 1;                                          /* save index */
            done = 1;                                                /* flag done */
            
            break;                                                   /* break */
        }
    }
    if (done != 1)                                                   /* check done */
    {
        handle->debug_print("hlw8032: frame is error.\n");           /* frame is error */
        
        return 4;                                                    /* return error */
    }

    pf_counter = ((uint16_t)buf[offset + 21] << 8) |
                 ((uint16_t)buf[offset + 22] << 0);                  /* set pf counter */
    update_status = buf[offset + 20];                                /* set update status */
    handle->pf_last_counter = pf_counter;                            /* save to last counter */
    handle->pf_last_ovf = (update_status >> 7) & 0x01;               /* save to last ovf */
    handle->is_first = 1;                                            /* set fist */
    handle->pf_counter = quantity_electricity_counter;               /* set quantity electricity counter */
    
    return 0;                                                        /* success return 0 */ 
}

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
uint8_t hlw8032_set_mode(hlw8032_handle_t *handle, hlw8032_mode_t mode)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    handle->mode = (uint8_t)mode;        /* set mode */
    
    return 0;                            /* success return 0 */ 
}

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
uint8_t hlw8032_get_mode(hlw8032_handle_t *handle, hlw8032_mode_t *mode)
{
    if (handle == NULL)                            /* check handle */
    {
        return 2;                                  /* return error */
    }
    if (handle->inited != 1)                       /* check handle initialization */
    {
        return 3;                                  /* return error */
    }
    
    *mode = (hlw8032_mode_t)(handle->mode);        /* set mode */
    
    return 0;                                      /* success return 0 */ 
}

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
uint8_t hlw8032_set_sample_voltage_coefficient(hlw8032_handle_t *handle, float coeff)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }
    if (handle->inited != 1)                    /* check handle initialization */
    {
        return 3;                               /* return error */
    }
    
    handle->voltage_coefficient = coeff;        /* set voltage coefficient */
    
    return 0;                                   /* success return 0 */ 
}

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
uint8_t hlw8032_get_sample_voltage_coefficient(hlw8032_handle_t *handle, float *coeff)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }
    if (handle->inited != 1)                    /* check handle initialization */
    {
        return 3;                               /* return error */
    }
    
    *coeff = handle->voltage_coefficient;       /* set voltage coefficient */
    
    return 0;                                   /* success return 0 */ 
}

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
uint8_t hlw8032_set_sample_current_coefficient(hlw8032_handle_t *handle, float coeff)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }
    if (handle->inited != 1)                    /* check handle initialization */
    {
        return 3;                               /* return error */
    }
    
    handle->current_coefficient = coeff;        /* set current coefficient */
    
    return 0;                                   /* success return 0 */ 
}

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
uint8_t hlw8032_get_sample_current_coefficient(hlw8032_handle_t *handle, float *coeff)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }
    if (handle->inited != 1)                    /* check handle initialization */
    {
        return 3;                               /* return error */
    }
    
    *coeff = handle->current_coefficient;       /* set current coefficient */
    
    return 0;                                   /* success return 0 */ 
}

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
uint8_t hlw8032_set_transformer_voltage_k1(hlw8032_handle_t *handle, float coeff)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    handle->voltage_k1 = coeff;        /* set coefficient */
    
    return 0;                          /* success return 0 */ 
}

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
uint8_t hlw8032_get_transformer_voltage_k1(hlw8032_handle_t *handle, float *coeff)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *coeff = handle->voltage_k1;       /* set coefficient */
    
    return 0;                          /* success return 0 */ 
}

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
uint8_t hlw8032_set_transformer_current_k2(hlw8032_handle_t *handle, float coeff)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    handle->current_k2 = coeff;        /* set coefficient */
    
    return 0;                          /* success return 0 */ 
}

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
uint8_t hlw8032_get_transformer_current_k2(hlw8032_handle_t *handle, float *coeff)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *coeff = handle->current_k2;       /* set coefficient */
    
    return 0;                          /* success return 0 */ 
}

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
uint8_t hlw8032_set_transformer_power_k3(hlw8032_handle_t *handle, float coeff)
{
    if (handle == NULL)              /* check handle */
    {
        return 2;                    /* return error */
    }
    if (handle->inited != 1)         /* check handle initialization */
    {
        return 3;                    /* return error */
    }
    
    handle->power_k3 = coeff;        /* set coefficient */
    
    return 0;                        /* success return 0 */ 
}

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
uint8_t hlw8032_get_transformer_power_k3(hlw8032_handle_t *handle, float *coeff)
{
    if (handle == NULL)              /* check handle */
    {
        return 2;                    /* return error */
    }
    if (handle->inited != 1)         /* check handle initialization */
    {
        return 3;                    /* return error */
    }
    
    *coeff = handle->power_k3;       /* set coefficient */
    
    return 0;                        /* success return 0 */ 
}

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
uint8_t hlw8032_get_buffer(hlw8032_handle_t *handle, char *buf, uint16_t len, uint32_t ms)
{
    uint8_t res;
    uint16_t l;

    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    res = handle->uart_flush();                                     /* uart flush */ 
    if (res != 0)                                                   /* check result */
    {
        handle->debug_print("hlw8032: uart flush failed.\n");       /* uart flush failed */
       
        return 1;                                                   /* return error */
    }
    handle->delay_ms(ms);                                           /* delay */
    l = handle->uart_read((uint8_t *)buf, len);                     /* uart read */
    if (l != len)                                                   /* check result */
    {
        handle->debug_print("hlw8032: uart read failed.\n");        /* uart read failed */
       
        return 1;                                                   /* return error */
    }
    
    return 0;                                                       /* success return 0 */ 
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a hlw8032 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t hlw8032_info(hlw8032_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(hlw8032_info_t));                        /* initialize hlw8032 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "UART", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
