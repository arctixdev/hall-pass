#include "led_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <map>

// LED array
CRGB leds[NUM_LEDS];

// Lookup table for LED behavior based on state
std::map<ControllerState, LedPattern> statePatternMap = {
    {STATE_IDLE, {CRGB::Blue, 0, 0, false}},            // Solid Blue
    {STATE_CONNECTING, {CRGB::Yellow, 500, 500, true}}, // Blinking Yellow
    {STATE_CONNECTED, {CRGB::Green, 0, 0, false}},      // Solid Green
    {STATE_ERROR, {CRGB::Red, 200, 200, true}},         // Fast Blinking Red
    {STATE_WARNING, {CRGB::Orange, 1000, 500, true}},   // Slow Blinking Orange
    {STATE_BOOTING, {CRGB::Purple, 100, 100, true}},    // Fast Blinking Purple
    {STATE_CUSTOM_PATTERN, {CRGB::White, 0, 0, false}}  // Custom pattern
};

// Current LED pattern
LedPattern currentPattern = statePatternMap[STATE_IDLE];

void setupLEDs()
{
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

// Function to set a custom pattern dynamically
void setCustomPattern(CRGB color, int onTime, int offTime, bool isBlinking)
{
  currentPattern = {color, onTime, offTime, isBlinking};
  setControllerState(STATE_CUSTOM_PATTERN);
}

// LED Task (runs continuously in background)
void ledTask(void *pvParameters)
{
  bool ledState = true;
  while (true)
  {
    ControllerState state = getControllerState();

    // Get the pattern based on the state
    if (statePatternMap.find(state) != statePatternMap.end() && state != STATE_CUSTOM_PATTERN)
    {
      currentPattern = statePatternMap[state];
    }

    // Apply LED pattern
    if (currentPattern.isBlinking)
    {
      leds[0] = ledState ? currentPattern.color : CRGB::Black;
      ledState = !ledState;
      FastLED.show();
      vTaskDelay(pdMS_TO_TICKS(ledState ? currentPattern.onTime : currentPattern.offTime));
    }
    else
    {
      leds[0] = currentPattern.color;
      FastLED.show();
      vTaskDelay(pdMS_TO_TICKS(100)); // Keep checking state every 100ms
    }
  }
}

// Start LED FreeRTOS task
void startLEDTask()
{
  xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, NULL, 1);
}
