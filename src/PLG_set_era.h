

#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "e09f2f31-63cc-490a-b2b8-0b3f40511f90"
// /* Define setting button */
#define BUTTON_PIN 0

#if defined(BUTTON_PIN)
// Active low (false), Active high (true)
#define BUTTON_INVERT false
#define BUTTON_HOLD_TIMEOUT 5000UL

// This directive is used to specify whether the configuration should be erased.
// If it's set to true, the configuration will be erased.
#define ERA_ERASE_CONFIG false
#endif

#include <ERa.hpp>

const char ssid[] = "PLG.mcu";
const char pass[] = "ok11111111";

WiFiClient mbTcpClient;

#include <Time/ERaEspTime.hpp>

#include <Time/ERaBaseTime.hpp>

#if defined(BUTTON_PIN)
#include <ERa/ERaButton.hpp>

ERaButton button;

#if ERA_VERSION_NUMBER >= ERA_VERSION_VAL(1, 6, 0)
static void eventButton(uint16_t pin, ButtonEventT event)
{
    if (event != ButtonEventT::BUTTON_ON_HOLD)
    {
        return;
    }
    ERa.switchToConfig(ERA_ERASE_CONFIG);
    (void)pin;
}
#elif ERA_VERSION_NUMBER >= ERA_VERSION_VAL(1, 2, 0)
static void eventButton(uint8_t pin, ButtonEventT event)
{
    if (event != ButtonEventT::BUTTON_ON_HOLD)
    {
        return;
    }
    ERa.switchToConfig(ERA_ERASE_CONFIG);
    (void)pin;
}
#else
static void eventButton(ButtonEventT event)
{
    if (event != ButtonEventT::BUTTON_ON_HOLD)
    {
        return;
    }
    ERa.switchToConfig(ERA_ERASE_CONFIG);
}
#endif

#if defined(ESP32)
#include <pthread.h>

pthread_t pthreadButton;

static void *handlerButton(void *args)
{
    for (;;)
    {
        button.run();
        ERaDelay(10);
    }
    pthread_exit(NULL);
}

void initButton()
{
    pinMode(BUTTON_PIN, INPUT);
    button.setButton(BUTTON_PIN, digitalRead, eventButton,
                     BUTTON_INVERT)
        .onHold(BUTTON_HOLD_TIMEOUT);
    pthread_create(&pthreadButton, NULL, handlerButton, NULL);
}
#elif defined(ESP8266)
#include <Ticker.h>

Ticker ticker;

static void handlerButton()
{
    button.run();
}

void initButton()
{
    pinMode(BUTTON_PIN, INPUT);
    button.setButton(BUTTON_PIN, digitalRead, eventButton,
                     BUTTON_INVERT)
        .onHold(BUTTON_HOLD_TIMEOUT);
    ticker.attach_ms(100, handlerButton);
}
#elif defined(ARDUINO_AMEBA)
#include <GTimer.h>

const uint32_t timerIdButton{0};

static void handlerButton(uint32_t data)
{
    button.run();
    (void)data;
}

void initButton()
{
    pinMode(BUTTON_PIN, INPUT);
    button.setButton(BUTTON_PIN, digitalReadArduino, eventButton,
                     BUTTON_INVERT)
        .onHold(BUTTON_HOLD_TIMEOUT);
    GTimer.begin(timerIdButton, (100 * 1000), handlerButton);
}
#endif
#endif

/* This function will run every time ERa is connected */
ERA_CONNECTED()
{
    ERA_LOG(ERA_PSTR("ERa"), ERA_PSTR("ERa connected!"));
}

/* This function will run every time ERa is disconnected */
ERA_DISCONNECTED()
{
    ERA_LOG(ERA_PSTR("ERa"), ERA_PSTR("ERa disconnected!"));
}

/* This function print uptime every second */
void timerEvent()
{
    ERA_LOG(ERA_PSTR("Timer"), ERA_PSTR("Uptime: %d"), ERaMillis() / 1000L);
}

#if defined(USE_BASE_TIME)
unsigned long getTimeCallback()
{
    // Please implement your own function
    // to get the current time in seconds.
    return 0;
}
#endif