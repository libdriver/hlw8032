### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

UART Pin: RX GPIO15.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

Enable serial port.

Disable serial console.

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(hlw8032  REQUIRED)
```
### 3. HLW8032

#### 3.1 Command Instruction

1. Show hlw8032 chip and driver information.

   ```shell
   hlw8032 (-i | --information)
   ```

2. Show hlw8032 help.

   ```shell
   hlw8032 (-h | --help)
   ```

3. Show hlw8032 pin connections of the current board.

   ```shell
   hlw8032 (-p | --port)
   ```

4. Run hlw8032 read test, num is the test times.

   ```shell
   hlw8032 (-t read | --test=read) [--mode=<SAMPLE | TRANSFORMER>] [--times=<num>]
   ```


5. Run hlw8032 read function, cnt is the quantity electricity counter, num is the test times.

   ```shell
   hlw8032 (-e read | --example=read) [--mode=<SAMPLE | TRANSFORMER>] [--counter=<cnt>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
./hlw8032 -i

hlw8032: chip is HLW TECHNOLOGY HLW8032.
hlw8032: manufacturer is HLW TECHNOLOGY.
hlw8032: interface is UART.
hlw8032: driver version is 1.0.
hlw8032: min supply voltage is 4.5V.
hlw8032: max supply voltage is 5.5V.
hlw8032: max current is 4.00mA.
hlw8032: max temperature is 85.0C.
hlw8032: min temperature is -40.0C.
```

```shell
./hlw8032 -p

hlw8032: TX connected to GPIO15(BCM).
```

```shell
./hlw8032 -t read --mode=SAMPLE --times=3

hlw8032: chip is HLW TECHNOLOGY HLW8032.
hlw8032: manufacturer is HLW TECHNOLOGY.
hlw8032: interface is UART.
hlw8032: driver version is 1.0.
hlw8032: min supply voltage is 4.5V.
hlw8032: max supply voltage is 5.5V.
hlw8032: max current is 4.00mA.
hlw8032: max temperature is 85.0C.
hlw8032: min temperature is -40.0C.
hlw8032: start read test.
hlw8032: voltage parameter raw is 0x00026D38.
hlw8032: voltage raw is 0x0000063E.
hlw8032: current parameter raw is 0x00003E41.
hlw8032: current raw is 0x00003F7E.
hlw8032: power parameter raw is 0x00503F60.
hlw8032: power raw is 0x0001624D.
hlw8032: update status is 0x71.
hlw8032: status is 0x55.
hlw8032: pf counter is 18046.
hlw8032: effective voltage is 187.10V.
hlw8032: effective current is 0.98A.
hlw8032: active power is 109.01W.
hlw8032: apparent power is 183.45W.
hlw8032: power factor is 0.59.
hlw8032: quantity electricity is 0.00KW·h.
hlw8032: quantity electricity counter is 0.
hlw8032: voltage parameter raw is 0x00026D38.
hlw8032: voltage raw is 0x0000063E.
hlw8032: current parameter raw is 0x00003E41.
hlw8032: current raw is 0x00003B69.
hlw8032: power parameter raw is 0x00503F60.
hlw8032: power raw is 0x00014C7B.
hlw8032: update status is 0x71.
hlw8032: status is 0x55.
hlw8032: pf counter is 18202.
hlw8032: effective voltage is 187.10V.
hlw8032: effective current is 1.05A.
hlw8032: active power is 116.16W.
hlw8032: apparent power is 196.05W.
hlw8032: power factor is 0.59.
hlw8032: quantity electricity is 0.00KW·h.
hlw8032: quantity electricity counter is 156.
hlw8032: voltage parameter raw is 0x00026D38.
hlw8032: voltage raw is 0x00000650.
hlw8032: current parameter raw is 0x00003E41.
hlw8032: current raw is 0x00006135.
hlw8032: power parameter raw is 0x00503F60.
hlw8032: power raw is 0x0001472E.
hlw8032: update status is 0x71.
hlw8032: status is 0x55.
hlw8032: pf counter is 18229.
hlw8032: effective voltage is 185.01V.
hlw8032: effective current is 0.64A.
hlw8032: active power is 118.04W.
hlw8032: apparent power is 118.49W.
hlw8032: power factor is 1.00.
hlw8032: quantity electricity is 0.00KW·h.
hlw8032: quantity electricity counter is 183.
hlw8032: finish read test.
```

```shell
./hlw8032 -e read --mode=SAMPLE --counter=0 --times=3

hlw8032: 1/3.
hlw8032: voltage parameter raw is 0x00026D38.
hlw8032: voltage raw is 0x00000639.
hlw8032: current parameter raw is 0x00003E41.
hlw8032: current raw is 0x00004A12.
hlw8032: power parameter raw is 0x00503F60.
hlw8032: power raw is 0x0001542C.
hlw8032: update status is 0x71.
hlw8032: status is 0x55.
hlw8032: pf counter is 19581.
hlw8032: effective voltage is 187.68V.
hlw8032: effective current is 0.84A.
hlw8032: active power is 113.54W.
hlw8032: apparent power is 157.74W.
hlw8032: power factor is 0.72.
hlw8032: quantity electricity is 0.00KW·h.
hlw8032: quantity electricity counter is 12.
hlw8032: 2/3.
hlw8032: voltage parameter raw is 0x00026D38.
hlw8032: voltage raw is 0x0000063F.
hlw8032: current parameter raw is 0x00003E41.
hlw8032: current raw is 0x00004A12.
hlw8032: power parameter raw is 0x00503F60.
hlw8032: power raw is 0x00016339.
hlw8032: update status is 0x71.
hlw8032: status is 0x55.
hlw8032: pf counter is 19467.
hlw8032: effective voltage is 186.98V.
hlw8032: effective current is 0.84A.
hlw8032: active power is 108.72W.
hlw8032: apparent power is 157.15W.
hlw8032: power factor is 0.69.
hlw8032: quantity electricity is 0.00KW·h.
hlw8032: quantity electricity counter is 12.
hlw8032: 3/3.
hlw8032: voltage parameter raw is 0x00026D38.
hlw8032: voltage raw is 0x0000063F.
hlw8032: current parameter raw is 0x00003E41.
hlw8032: current raw is 0x00004E67.
hlw8032: power parameter raw is 0x00503F60.
hlw8032: power raw is 0x0001785A.
hlw8032: update status is 0x71.
hlw8032: status is 0x55.
hlw8032: pf counter is 19479.
hlw8032: effective voltage is 186.98V.
hlw8032: effective current is 0.79A.
hlw8032: active power is 102.62W.
hlw8032: apparent power is 148.47W.
hlw8032: power factor is 0.69.
hlw8032: quantity electricity is 0.00KW·h.
hlw8032: quantity electricity counter is 12.
```

```shell
./hlw8032 -h

Usage:
  hlw8032 (-i | --information)
  hlw8032 (-h | --help)
  hlw8032 (-p | --port)
  hlw8032 (-t read | --test=read) [--mode=<SAMPLE | TRANSFORMER>] [--times=<num>]
  hlw8032 (-e read | --example=read) [--mode=<SAMPLE | TRANSFORMER>] [--counter=<cnt>] [--times=<num>]

Options:
      --counter=<cnt>                Set quantity electricity counter.([default: 0])
  -e <read>, --example=<read>        Run the driver example.
  -h, --help                         Show the help.
  -i, --information                  Show the chip information.
      --mode=<SAMPLE | TRANSFORMER>  Set the chip mode.([default: SAMPLE])
  -p, --port                         Display the pins used by this device to connect the chip.
  -t <read>, --test=<read>           Run the driver test.
      --times=<num>                  Set the running times.([default: 3])
```
