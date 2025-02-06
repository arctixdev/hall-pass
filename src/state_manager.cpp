#include "state_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static ControllerState controllerState = STATE_IDLE;
static SemaphoreHandle_t stateMutex;

void setControllerState(ControllerState newState)
{
  if (xSemaphoreTake(stateMutex, portMAX_DELAY))
  {
    controllerState = newState;
    xSemaphoreGive(stateMutex);
  }
}

ControllerState getControllerState()
{
  ControllerState tempState;
  if (xSemaphoreTake(stateMutex, portMAX_DELAY))
  {
    tempState = controllerState;
    xSemaphoreGive(stateMutex);
  }
  return tempState;
}

void initStateManager()
{
  stateMutex = xSemaphoreCreateMutex();
}
