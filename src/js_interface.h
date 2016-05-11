#pragma once

#define KEY_REQUEST 0
#define KEY_TEMPERATURE 1
#define KEY_CONDITIONS 2

void js_interface_init(void);
void send_js_message(int id);
