#ifndef _E650_JM_A_H_
#define _E650_JM_A_H_

#include <Arduino.h>
#include "libs/Longan_CANFD/src/mcp2518fd_can.h"

class CANBus : public mcp2518fd
{
public:
    CANBus(int _pin) : mcp2518fd(_pin)
    {
    }
};

class E650_JM_A
{
private:
    /* data */
    uint8_t dataBUF1[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x101
    uint8_t dataBUF2[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x112
    uint8_t dataBUF3[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x251
    uint8_t dataBUF4[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x226
    uint8_t dataBUF5[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x302
    uint8_t dataBUF6[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x3C0
    uint8_t dataBUF7[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x3C3
    uint8_t dataBUF8[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x3C6
    uint8_t dataBUF9[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // ID:0x5B3
    uint8_t dataBUF10[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // ID:0x228
    uint8_t dataBUF11[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // ID:0x345
    uint8_t dataBUF12[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // ID:0x390

public:
    void set_Gauge(float Speed, uint16_t RPM, uint8_t Fuel, float Voltage, float Current, uint16_t Range, uint8_t Power);
    void set_Gear(char rd_gear, byte Gear_effective_position, byte Run_Preparation);
    void set_TurnSignal(byte turnLeft, byte turnRight);
    void set_Light(byte position_light, byte high_beam, byte rear_fog);
    void set_Door_alarm(byte door1, byte door2, byte door3, byte door4);
    void set_Alarm(byte Park_brake, byte ESP, byte Power_reduction, byte Battery_cutoff, byte ABS, byte Charging_cable, uint8_t Charge_status);
    void set_Failure(byte System_failure, byte Battery_failure, byte motor_fault, byte motor_overheating, byte Auxiliary_battery, byte insulation_failure);
    void set_Tire_pressure(byte wheel1, byte wheel2, byte wheel3, byte wheel4);
    void Send();
};
#endif