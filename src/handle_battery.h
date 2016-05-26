#pragma once

void init_battery_layer(Layer *window_layer);
void battery_callback(BatteryChargeState state); 
void deinit_battery_layer(void);