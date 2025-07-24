/*********
 * PLG Project LoRa Sender
 *
 * This code is for sending LoRa packets using an ESP32 and a LoRa transceiver module.
 * It initializes the LoRa module, sets the frequency, and sends packets with a counter value.
 * The packets alternate between "even" and "odd" based on the counter value.
 * * The code also includes a commented-out section for receiving packets, which can be enabled if needed.
 *
 * This code is part of the PLG Project for the PLG Contest 2025.
 */
#define ERA_DEBUG
#define SEN
// #define ERA_DEBUG
#define PLG_MASTER

#include <Arduino.h>

/**************************************************************
 *                PLG Project LoRa Sender
 ***************************************************************/
#if defined(PLG_MASTER)
#include <PLG_master.h>

#else
#include <PLG_slave.h>
#endif

void setup()
{
#if defined(PLG_MASTER)
  Setup_master(); // Call setup function for master device

#else
  Setup_slave(); // Call setup function for slave device
#endif
}
void loop()
{
#if defined(PLG_MASTER)
  PLG_master_loop(); // Call loop function for master device

#else
   PLG_slave_loop(); // Call loop function for slave device
 
#endif

  // You can adjust the delay as needed for your application
}
