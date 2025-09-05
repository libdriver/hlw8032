[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HLW8032

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hlw8032/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

HLW8032 is a high-precision electric energy metering IC which is manufactured using CMOS process and is mainly applied to single-phase application. It is able to measure line voltage and current and calculate active power, apparent power and power factor. 

LibDriver HLW8032 is a full-featured driver for HLW8032, launched by LibDriver. It provides effective voltage reading, effective current reading, active power reading, apparent power reading, power factor reading, quantity electricity reading and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver HLW8032 source files.

/interface includes LibDriver HLW8032 UART platform independent template.

/test includes LibDriver HLW8032 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver HLW8032 sample code.

/doc includes LibDriver HLW8032 offline document.

/datasheet includes HLW8032 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface UART platform independent template and finish your platform UART driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_hlw8032_basic.h"

uint8_t res;
uint32_t i;
int64_t pf_counter = 0;

/* init */
res = hlw8032_basic_init(HLW8032_MODE_SAMPLE);
if (res != 0)
{
    return 1;
}

....
    
/* import quantity electricity */
res = hlw8032_basic_quantity_electricity_import(pf_counter);
if (res != 0)
{
    (void)hlw8032_basic_deinit();

    return 1;
}

...
    
for (i = 0; i < 3; i++)
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
    hlw8032_interface_debug_print("hlw8032: %d/%d.\n", i + 1, 3);
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
    
    ...
}

...
    
/* deinit */
(void)hlw8032_basic_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/hlw8032/index.html](https://www.libdriver.com/docs/hlw8032/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.