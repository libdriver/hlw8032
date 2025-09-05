[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HLW8032
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hlw8032/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

HLW8032 ist ein hochpräziser IC zur Messung elektrischer Energie, der im CMOS-Verfahren hergestellt wird und hauptsächlich für einphasige Anwendungen eingesetzt wird. Er misst Netzspannung und -strom und berechnet Wirkleistung, Scheinleistung und Leistungsfaktor.

LibDriver HLW8032 ist ein vollfunktionaler Treiber für HLW8032, der von LibDriver eingeführt wurde. Er bietet die Anzeige von Effektivspannung, Effektivstrom, Wirkleistung, Scheinleistung, Leistungsfaktor, Strommenge und weiteren Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver HLW8032-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver HLW8032 UART.

/test enthält den Testcode des LibDriver HLW8032-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver HLW8032-Beispielcode.

/doc enthält das LibDriver HLW8032-Offlinedokument.

/Datenblatt enthält HLW8032-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige UART-Schnittstellenvorlage und stellen Sie Ihren Plattform-UART-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/hlw8032/index.html](https://www.libdriver.com/docs/hlw8032/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.