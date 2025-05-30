# **E650 Dashboard Communication Protocol**
* baud rate  500kbps
* Motorola format
* Standard
### *data input*
* ID: 0x101

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 0 | RPM (HIGH Byte)|-15,000|1|rpm|
| 1 | RPM (LOW Byte)| | | |
| 5 | Power| | |%POWER|

* ID: 0x112

|Byte|Description|
|:-:|:-|
| 6 | [BIT-3] --> Motor overheating indication|
| 6 | [BIT-4] --> Motor fault indication|

* ID: 0x251

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 0 | Voltage gauge (HIGH Byte)| |0.01|V|
| 1 | Voltage gauge (LOW Byte)| | | |
| 2 | Current meter (HIGH Byte)|-500|0.1|A|
| 3 | Current meter (LOW Byte)| | | |
| 4 | Fuel gauge| | |%|

* ID: 0x226

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 0 | [BIT-1] --> ABS indication| | | |
| 1 | Speed  (HIGH Byte)| |0.05625|km/h|
| 2 | Speed  (LOW Byte)| | | |

* ID: 0x302

|Byte|Description|
|:-:|:-|
| 0 | [BIT-4] ----------> Right turn|
| 0 | [BIT-5] ----------> Left turn|
| 0 | [BIT-7] ----------> Rear fog lights|
| 1 | [BIT-4, 5, 6, 7] --> Door alarm indications|
| 5 | [BIT-5] ----------> Position lights|
| 5 | [BIT-7] ----------> High beam|

* ID: 0x390

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 3 | [BIT-6] --> System failure| | | |
| 4 | Range  (HIGH Byte)| | |km|
| 5 | Range  (LOW Byte)| | | |

* ID: 0x3C0

|Byte|Description|
|:-:|:-|
| 0 | [BIT-0] --> Run preparation (Ready)|
| 1 | [BIT-0] --> Power reduction |
| 5 | [BIT-7] --> Park brake|
| 6 | rd gear --> (0x00:R, 0x01:N, 0x02:D, 0x03:S)|

* ID: 0x3C3

|Byte|Description|
|:-:|:-|
| 3 |Charge status indicatoin --> ((0x10:Flashing, 0x20:Solid on)|
| 5 | [BIT-0] -------------------> Charging cable |
| 5 | [BIT-1] -------------------> Power battery failure|
| 5 | [BIT-2] -------------------> Indication of insulation failure|
| 5 | [BIT-3] -------------------> Battery cut-off |
| 6 | [BIT-0] -------------------> Power battery heating indication|

* ID: 0x3C6

|Byte|Description|
|:-:|:-|
| 0 | [BIT-0] --> EPS indications|

* ID: 0x5B3

|Byte|Description|
|:-:|:-|
| 5 |Auxiliary battery charge indication (DCDC_Fault)--> 0x07:Light up|

* ID: 0x228

|Byte|Description|
|:-:|:-|
| 0 |[BIT-2] --> Tire pressure monitoring status 1|
| 0 |[BIT-3] --> Tire pressure monitoring status 2|

* ID: 0x345

|Byte|Description|
|:-:|:-|
| 0 |[BIT-0] --> Tire pressure monitoring status 3|
| 0 |[BIT-3] --> Tire pressure monitoring status 4|

### *data input (Cycle 1000 ms)*
* ID: 0x623

|Byte|Description|
|:-:|:-|
| 0 |Accrued miles (HIGH Byte)|
| 1 |Accrued miles .......^..........|
| 2 |Accrued miles (LOW Byte)|
| 4 |[BIT-6] --> safety belt (0:Close, 1:Open)|
| 4 |[BIT-7] --> Brake failure (0:Normal, 1:Fault)|

* ID: 0x624

|Byte|Description|
|:-:|:-|
| 0 |Product code (HIGH Byte)|
| 1 |Product code .......^..........|
| 2 |Product code .......^..........|
| 3 |Product code (LOW Byte)|
| 4 |Hardware version (HIGH Byte)|
| 5 |Hardware version (LOW Byte)|
| 6 |Software version (HIGH Byte)|
| 7 |Software version (LOW Byte)|

## Function

* Set PIN CS in class CANBus
```js
#define SPI_CS_PIN  5
CANBus CAN(SPI_CS_PIN);
```

* class E650_JM_A
```js
E650_JM_A dash;
```

* Gauge
 ```js
dash.set_Gauge(Speed, RPM, Fuel, Voltage, Current, Range, Power);
 ```
#
* Gear
 ```js
dash.set_Gear(Gear('R', 'N', 'D', 'S'), Gear_effective_position, Run_Preparation);
 ```
#
* Turn Signal
 ```js
dash.set_TurnSignal(turnLeft, turnRight);
 ```
#
* Light
 ```js
dash.set_Light(position_light, high_beam, rear_fog);
 ```
#
* Door alarm
 ```js
dash.set_Door_alarm(Door1, Door2, Door3, Door4);
 ```
#
* Alarm
 ```js
dash.set_Alarm(Park brake, ESP, Power reduction, Battery cut-off, ABS, Charging cable, Charge_status(0:Off, 1:Flashing, 2:Solid On));
 ```
#
* Failure
 ```js
dash.set_Failure(System failure, Battery failure, Motor_fault, Motor_overheating, Auxiliary battery, Insulation failure);
 ```
#
* Tire_pressure
 ```js
dash.set_Tire_pressure(Wheel1, Wheel2, Wheel3, Wheel4);
 ```
#
* Send data
 ```js
dash.Send();
 ```

## Original source

​
This library is possible thanks to original [Longan Labs CAN FD Library](https://github.com/Longan-Labs/Longan_CANFD) library. Thank you, Longan Labs.
##
[Overview](https://github.com/AkeJakkrapong)
##
[Repositories](https://github.com/AkeJakkrapong/E650-JM-A_Instrument_Cluster)