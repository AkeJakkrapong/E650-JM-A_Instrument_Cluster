/*************************************************************************************************************************
 CAN protocol: Motorola format, baud rate: 500kbps
 A3:  The seat belt is not fastened with the indication (12V-)
 A8:  CAN-H
 A9:  CAN-L
 A10: GND
 A11: BATT(12V+)
 A12: TGN (12V+)
 A20: Braking fault indication (12V-)
 ________________________________________________________________________________________________________________________

 Gauge Type
 -  0x226 Speed
 -  0x101 RPM
 -  0x251 Fuel gauge
 -  0x251 Voltage gauge
 -  0x251 Current meter
 -  0x390 Range
 -  0x101 Power

 Light Type (- Dial , * LCD screen)
 -  0x302 Right turn
 -  0x302 Left turn
 *  0x3C0 Run preparation (Ready)
 -  0x390 System failure
 *  0x3C3 Power battery heating indication
 -  0x3C3 Power battery failure
 -  0x3C3 Charging cable
 *  0x302 Door alarm indications
 -  (A3*Low level)  Seat belt indication
 -  0x302 rear fog lights
 -  0x302 High beam
 *  0x228, 0x345  Tire pressure monitoring status
 *  (*SOC <20%) Low battery indicator
 -  0x302 Position lights
 *  0x112 Motor overheating indication
 -  (*Low level) Brake fault indication
 -  0x3C0 Park brake
 -  0x3C6 EPS indications
 -  0x3C0 Power reduction
 -  0x3C3 Battery cut-off
 *  0x226 ABS indication
 -  0x112 motor fault indication
 -  0x3C3 Charge status indicatoin
 -  (No data) Airbag indications
 -  0x3C3 Indication of insulation failure
 -  0x5B3 DCDC indication
 -  0x3C0 rd gear (S, D, N, R)
  ________________________________________________________________________________________________________________________

 Meter data input (cycle <1500 ms)
 RPM                          : 0x101 -> Byte1:LowByte, Byte0:HighByte (Offset -15000) (0 to 9900rpm) Unit:rpm
 Power                        : 0x101 -> Byte5 (0 to 100%) Unit:%POWER

 Motor overheating indication : 0x112 -> Byte6, Bit3
 motor fault indication       : 0x112 -> Byte6, Bit4

 Voltage gauge                : 0x251 -> Byte1:LowByte, Byte0:HighByte (precision 0.01) (0 to 500V) Unit:V
 Current meter                : 0x251 -> Byte3:LowByte, Byte2:HighByte (precision 0.1, Offset -500) (-500 to 500A) Unit:A
 Fuel gauge                   : 0x251 -> Byte4  Unit:% (<20%, <40%, <60%, <80%, 80% upper)

 ABS indication               : 0x226 -> Byte0, Bit1
 Speed                        : 0x226 -> Byte2:LowByte, Byte1:HighByte (precision 0.05625) (0 to 199km/h) Unit:km/h

 Right turn                   : 0x302 -> Byte0, Bit4
 Left turn                    : 0x302 -> Byte0, Bit5
 rear fog lights              : 0x302 -> Byte0, Bit7
 Door alarm indications       : 0x302 -> Byte1, Bit(4, 5, 6, 7)
 Position lights              : 0x302 -> Byte5, Bit5
 High beam                    : 0x302 -> Byte5, Bit7

 System failure               : 0x390 -> Byte3, Bit6
 Range                        : 0x390 -> Byte5:LowByte, Byte4:HighByte  (0 to 500km) Unit:km

 Run preparation (Ready)      : 0x3C0 -> Byte0, Bit0
 Power reduction              : 0x3C0 -> Byte1, Bit0
 Park brake                   : 0x3C0 -> Byte5, Bit7
 rd gear                      : 0x3C0 -> Byte6, (0x00:R, 0x01:N, 0x02:D, 0x03:S)

 Charge status indicatoin           : 0x3C3 -> Byte3, (0x10:Flashing, 0x20:Solid on)
 Charging cable                     : 0x3C3 -> Byte5, Bit0
 Power battery failure              : 0x3C3 -> Byte5, Bit1
 Indication of insulation failure   : 0x3C3 -> Byte5, Bit2
 Battery cut-off                    : 0x3C3 -> Byte5, Bit3
 Power battery heating indication   : 0x3C3 -> Byte6, Bit0

 EPS indications                    : 0x3C6 -> Byte0, Bit0

 Auxiliary battery charge indication: 0x5B3 -> Byte5, (0x01 or 0x02 or 0x03 or 0x04 or 0x05:Light up) (DCDC_Fault)
 DCDC indication                    : 0x5B3 -> Byte5, (0x01 or 0x02 or 0x03 or 0x04 or 0x05 Light up)

 Tire pressure monitoring status    : 0x228 -> Byte0, Bit(2, 3)
                                    : 0x345 -> Byte0, Bit(0, 3)
 ________________________________________________________________________________________________________________________

 Meter data output (cycle 1000 ms)
 Accrued miles    : 0x623 -> Byte2:LowByte, Byte1, Byte0:HighByte Unit:km
 safety belt      : 0x623 -> Byte4, Bit6(0:Close, 1:Open)
 Brake failure    : 0x623 -> Byte4, Bit7(0:Normal, 1:Fault)

 Product code     : 0x624 -> Byte3:LowByte, Byte2, Byte1, Byte0:HighByte
 Hardware version : 0x624 -> Byte5:LowByte, Byte4:HighByte
 Software version : 0x624 -> Byte7:LowByte, Byte6:HighByte
 *************************************************************************************************************************/

#include "E650_JM_A.h"
extern CANBus CAN;

// Helper function to check if a data buffer is all zeros
bool isBufferEmpty(uint8_t *buf, size_t len)
{
  for (size_t i = 0; i < len; ++i)
  {
    if(buf[i] != 0x00)
    {
      return false;
    }
  }
  return true;
}

// Speed(0 to 199km/h), RPM(0 to 9900rpm), Fuel(0 to 100%), Voltage(0 to 500V), Current(-500 to 500A), Range(0 to 500km), Power(0 to 100%)
void E650_JM_A::set_Gauge(float Speed, uint16_t RPM, uint8_t Fuel, float Voltage, float Current, uint16_t Range, uint8_t Power)
{
  // limit parameter
  if(Speed <= 0){Speed = 0;} if(Speed >= 199){Speed = 199;}
  if(RPM <= 0){RPM = 0;} if(RPM >= 9900){RPM = 9900;}
  if(Fuel <= 0){Fuel = 0;} if(Fuel >= 100){Fuel = 100;}
  if(Voltage <= 0){Voltage = 0;} if(Voltage >= 500){Voltage = 500;}
  if(Current <= -500){Current = -500;} if(Current >= 500){Current = 500;}
  if(Range <= 0){Range = 0;} if(Range >= 500){Range = 500;}
  if(Power <= 0){Power = 0;} if(Power >= 100){Power = 100;}

  // calculated offset and resolution
  uint16_t pSpeed = Speed / 0.05625;
  RPM = RPM + 15000;
  uint16_t pVoltage = Voltage / 0.1;
  uint16_t pCurrent = (Current + 500) / 0.1;

  // Add RPM to 0x101
  dataBUF1[0] = RPM >> 8;
  dataBUF1[1] = RPM & 0xFF;
  // Add Power to 0x101
  dataBUF1[5] = Power;

  // Add Voltage to 0x251
  dataBUF3[0] = pVoltage >> 8;
  dataBUF3[1] = pVoltage & 0xFF;
  // Add Current to 0x251
  dataBUF3[2] = pCurrent >> 8;
  dataBUF3[3] = pCurrent & 0xFF;
  dataBUF3[4] = Fuel;

  // Add Speed to 0x226
  dataBUF4[1] = pSpeed >> 8;
  dataBUF4[2] = pSpeed & 0xFF;

  // Add Range to 0x390
  dataBUF12[4] = Range >> 8;
  dataBUF12[5] = Range;
}

// Gear('R', 'N', 'D', 'S'), Gear_effective_position, Run_Preparation
void E650_JM_A::set_Gear(char rd_gear, byte Gear_effective_position, byte Run_Preparation)
{
  switch (rd_gear)
  {
  case 'R':
    dataBUF6[6] = 0x00;
    break;
  case 'N':
    dataBUF6[6] = 0x01;
    break;
  case 'D':
    dataBUF6[6] = 0x02;
    break;
  case 'S':
    dataBUF6[6] = 0x03;
    break;
  }
  dataBUF6[0] = (Gear_effective_position << 4) | Run_Preparation;
}

// turnLeft, turnRight
void E650_JM_A::set_TurnSignal(byte turnLeft, byte turnRight)
{
  // Clear existing turn signal bits
  dataBUF5[0] &= ~(0x10 | 0x20); // Clear Bit4 (Right turn) and Bit5 (Left turn)
  // Set Left turn signal
  if(turnLeft == 1)
  {
    dataBUF5[0] |= 0x20; // 0x302, Bit5
  }
  // Set Right turn signal
  if(turnRight == 1)
  {
    dataBUF5[0] |= 0x10; // 0x302, Bit4
  }
}

// position_light, high_beam, rear_fog
void E650_JM_A::set_Light(byte position_light, byte high_beam, byte rear_fog)
{
  // Clear existing light bits in Byte5
  dataBUF5[5] &= ~(0x20 | 0x80); // Clear Bit5 (Position lights) and Bit7 (High beam)
  // Clear existing rear fog light bit in Byte0
  dataBUF5[0] &= ~0x80;
  // Set Position lights
  if(position_light == 1)
  {
    dataBUF5[5] |= 0x20; // 0x302, Bit5
  }
  // Set High beam
  if(high_beam == 1)
  {
    dataBUF5[5] |= 0x80; // 0x302, Bit7
  }
  // Set rear fog lights
  if(rear_fog == 1)
  {
    dataBUF5[0] |= 0x80; // 0x302, Bit7
  }
}

// Door1, Door2, Door3, Door4
void E650_JM_A::set_Door_alarm(byte door1, byte door2, byte door3, byte door4)
{
  // Clear all door alarm bits first
  dataBUF5[1] &= ~(0xF0); // Clear Bit4, Bit5, Bit6, Bit7
  // Set individual door alarm bits
  if(door1 == 1)
  {
    dataBUF5[1] |= 0x10; // 0x302, Bit4
  }
  if(door2 == 1)
  {
    dataBUF5[1] |= 0x20; // 0x302, Bit5
  }
  if(door3 == 1)
  {
    dataBUF5[1] |= 0x40; // 0x302, Bit6
  }
  if(door4 == 1)
  {
    dataBUF5[1] |= 0x80; // 0x302, Bit7
  }
}

// Park brake, ESP, Power reduction, Battery cut-off, ABS, Charging cable, Charge_status(0:Off, 1:Flashing, 2:Solid On)
void E650_JM_A::set_Alarm(byte Park_brake, byte ESP, byte Power_reduction, byte Battery_cutoff, byte ABS, byte Charging_cable, uint8_t Charge_status)
{
  // Park brake
  (Park_brake == 1) ? (dataBUF6[5] |= 0x80) : (dataBUF6[5] &= ~0x80);
  // ESP (EPS indications)
  dataBUF8[0] = (ESP == 1) ? 0x01 : 0x00;
  // Power reduction
  dataBUF6[1] = (Power_reduction == 1) ? 0x01 : 0x00;
  // Battery cut-off
  (Battery_cutoff == 1) ? (dataBUF7[5] |= 0x08) : (dataBUF7[5] &= ~0x08);
  // ABS
  dataBUF4[0] = (ABS == 1) ? 0x02 : 0x00;
  // Charging cable
  (Charging_cable == 1) ? (dataBUF7[5] |= 0x01) : (dataBUF7[5] &= ~0x01);
  // Charge status
  switch (Charge_status)
  {
  case 0:
    dataBUF7[3] = 0x00;
    break; // Off
  case 1:
    dataBUF7[3] = 0x10;
    break; // Flashing
  case 2:
    dataBUF7[3] = 0x20;
    break; // Solid on
  }
}

// System failure, Battery failure, Motor fault, Motor_overheating, Auxiliary battery, Insulation failure
void E650_JM_A::set_Failure(byte System_failure, byte Battery_failure, byte motor_fault, byte motor_overheating, byte Auxiliary_battery, byte insulation_failure)
{
  // System failure
  dataBUF12[3] = (System_failure == 1) ? 0x40 : 0x00;
  // Battery failure
  (Battery_failure == 1) ? (dataBUF7[5] |= 0x02) : (dataBUF7[5] &= ~0x02);
  // Motor fault
  (motor_fault == 1) ? (dataBUF2[6] |= 0x10) : (dataBUF2[6] &= ~0x10);
  // Motor overheating
  (motor_overheating == 1) ? (dataBUF2[6] |= 0x08) : (dataBUF2[6] &= ~0x08);
  // Auxiliary battery (DCDC_Fault)
  dataBUF9[5] = (Auxiliary_battery == 1) ? 0x07 : 0x00; // 0x07 for "light up"
  // Insulation failure
  (insulation_failure == 1) ? (dataBUF7[5] |= 0x04) : (dataBUF7[5] &= ~0x04);
}

void E650_JM_A::set_Tire_pressure(byte wheel1, byte wheel2, byte wheel3, byte wheel4)
{
  // Clear existing bits for all wheels
  dataBUF10[0] &= ~(0x04 | 0x08); // Clear Bit2 and Bit3 for 0x228
  dataBUF11[0] &= ~(0x01 | 0x08); // Clear Bit0 and Bit3 for 0x345
  // Set individual wheel status
  if(wheel1 == 1)
  {
    dataBUF10[0] |= 0x04; // 0x228, Bit2
  }
  if(wheel2 == 1)
  {
    dataBUF10[0] |= 0x08; // 0x228, Bit3
  }
  if(wheel3 == 1)
  {
    dataBUF11[0] |= 0x01; // 0x345, Bit0
  }
  if(wheel4 == 1)
  {
    dataBUF11[0] |= 0x08; // 0x345, Bit3
  }
}

void E650_JM_A::Send()
{
  if(!isBufferEmpty(dataBUF1, 8))
  {
    CAN.sendMsgBuf(0x101, 0, 8, dataBUF1);
  }
  if(!isBufferEmpty(dataBUF2, 8))
  {
    CAN.sendMsgBuf(0x112, 0, 8, dataBUF2);
  }
  if(!isBufferEmpty(dataBUF3, 8))
  {
    CAN.sendMsgBuf(0x251, 0, 8, dataBUF3);
  }
  if(!isBufferEmpty(dataBUF4, 8))
  {
    CAN.sendMsgBuf(0x226, 0, 8, dataBUF4);
  }
  if(!isBufferEmpty(dataBUF5, 8))
  {
    CAN.sendMsgBuf(0x302, 0, 8, dataBUF5);
  }
  if(!isBufferEmpty(dataBUF6, 8))
  {
    CAN.sendMsgBuf(0x3C0, 0, 8, dataBUF6);
  }
  if(!isBufferEmpty(dataBUF7, 8))
  {
    CAN.sendMsgBuf(0x3C3, 0, 8, dataBUF7);
  }

  CAN.sendMsgBuf(0x3C6, 0, 8, dataBUF8); // This line was uncommented and seemed to always send dataBUF8

  if(!isBufferEmpty(dataBUF9, 8))
  {
    CAN.sendMsgBuf(0x5B3, 0, 8, dataBUF9);
  }
  if(!isBufferEmpty(dataBUF10, 8))
  {
    CAN.sendMsgBuf(0x228, 0, 8, dataBUF10);
  }
  if(!isBufferEmpty(dataBUF11, 8))
  {
    CAN.sendMsgBuf(0x345, 0, 8, dataBUF11);
  }
  if(!isBufferEmpty(dataBUF12, 8))
  {
    CAN.sendMsgBuf(0x390, 0, 8, dataBUF12);
  }
}
