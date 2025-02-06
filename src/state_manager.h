#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>

// Define different states
enum ControllerState
{
  STATE_IDLE,
  STATE_CONNECTING,
  STATE_CONNECTED,
  STATE_ERROR,
  STATE_WARNING,
  STATE_BOOTING,
  STATE_CUSTOM_PATTERN // You can add more states here
};

// Functions to update and retrieve state
void setControllerState(ControllerState newState);
ControllerState getControllerState();

// Initialize state manager
void initStateManager();

#endif // STATE_MANAGER_H
