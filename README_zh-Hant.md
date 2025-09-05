[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HLW8032

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hlw8032/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

HLW8032是一款高精度的電能計量IC，它採用CMOS制造技術，主要用於單相應用。 它能够量測線電壓和電流，並能計算有功功率，視在功率和功率因素。

LibDriver HLW8032是LibDriver推出的HLW8032全功能驅動，該驅動提供有效電壓讀取，有效電流讀取，有功功率讀取，視在功率讀取，功率因數讀取，電量讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver HLW8032的源文件。

/interface目錄包含了LibDriver HLW8032與平台無關的UART總線模板。

/test目錄包含了LibDriver HLW8032驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver HLW8032編程範例。

/doc目錄包含了LibDriver HLW8032離線文檔。

/datasheet目錄包含了HLW8032數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的UART總線模板，完成指定平台的UART總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/hlw8032/index.html](https://www.libdriver.com/docs/hlw8032/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
