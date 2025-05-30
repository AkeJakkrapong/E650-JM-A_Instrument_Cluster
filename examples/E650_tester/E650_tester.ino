/*********************************************************************************
 * MCU                : ESP32 Dev Module
 * CANBUS             : MCP2518FD
 * Instrument Cluster : E650-JM-A (wuhan green instrument)
 * Original Library   : Logan Labs (https://github.com/Longan-Labs/Longan_CANFD)
 * @authors           : Longan Labs
 * Modified By        : AkeJakkrapong

 *********************************************************************************/

#include <E650_JM_A.h>

#define SPI_CS_PIN  5
CANBus CAN(SPI_CS_PIN);
E650_JM_A dash;

uint16_t cycle1 = 100;
uint32_t last_cycle1;


void setup() 
{
  Serial.begin(115200);
  delay(500);

  while (CAN_OK != CAN.begin(CAN_500KBPS, MCP2518FD_40MHz))
  {
    Serial.println("CAN init fail!");
    delay(100);
    while(1);
  }
  Serial.println("CAN init OK!");
  last_cycle1 = millis();
}


void loop() 
{
  if(millis() - last_cycle1 >= cycle1)
  {  
    last_cycle1 = millis();
    dash.set_Gauge(1.0, 9900, 19, 500.0, -500.0, 10, 100);    // Speed(0 to 199km/h), RPM(0 to 9900rpm), Fuel(0 to 100%), Voltage(0 to 500V), Current(-500 to 500A), Range(0 to 500km), Power(0 to 100%)
    dash.set_Gear('N', 1, 1);                                 // Gear('R', 'N', 'D', 'S'), Gear_effective_position, Run_Preparation
    dash.set_TurnSignal(1, 1);                                // turnLeft, turnRight
    dash.set_Light(1, 1, 1);                                  // position_light, high_beam, rear_fog
    dash.set_Door_alarm(1, 1, 1, 1);                          // Door1, Door2, Door3, Door4
    dash.set_Alarm(1, 1, 1, 1, 1, 1, 2);                      // Park brake, ESP, Power reduction, Battery cut-off, ABS, Charging cable, Charge_status(0:Off, 1:Flashing, 2:Solid On)
    dash.set_Failure(1, 1, 1, 1, 1, 1);                       // System failure, Battery failure, Motor_fault, Motor_overheating, Auxiliary battery, Insulation failure
    dash.set_Tire_pressure(1, 1, 1, 1);                       // Tire_pressure (Wheel1, Wheel2, Wheel3, Wheel4)
    dash.Send();
  }
}
