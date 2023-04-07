#ifndef ps2_keyboard_h_
#define ps2_keyboard_h_

#include <SDL_keycode.h>
#include <unordered_map>
#include "devices.h"
#include <memory>


class ps2_kbd_t
{
public:
  ps2_kbd_t();
  void attach_port(altera_ps2_t port);
  int send_keypress(SDL_KeyCode keycode, bool isMakeCode);
  int send_response(uint8_t byte);

private:
  std::shared_ptr<altera_ps2_t> ps2_port;
  std::unordered_map<SDL_KeyCode, uint8_t> keycode_map; 
};


#endif