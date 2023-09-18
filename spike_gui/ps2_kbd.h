#ifndef ps2_keyboard_h_
#define ps2_keyboard_h_

#include <SDL2/SDL_keycode.h>
#include <unordered_map>
#include <memory>

#define PS2_SET_LEDS 0xED
#define PS2_ECHO 0xEE
#define PS2_GETSET_SCANCODES 0xF0
#define PS2_ID_KEYBOARD 0xF2
#define PS2_SET_TYPEMATIC 0xF3
#define PS2_ENABLE_SCANNING 0xF4
#define PS2_DISABLE_SCANNING 0xF5
#define PS2_SET_DEFAULT_PARAMS 0xF6
#define PS2_RESEND 0xFE
#define PS2_RESET 0xFF

#define PS2_ACK 0xFA
#define PS2_NAK 0xFE
#define PS2_AOK 0xAA

#define PS2_SETCODE_2 0x41

enum Keyboard_State {PS2_STATE_CMD, PS2_STATE_SET_SAMPLE_RATE, PS2_STATE_SET_SCANCODE_SET, PS2_STATE_SET_LEDS};

typedef struct ps2_scancode {
  uint8_t bytes[4];
  int length;
} ps2_scancode;

class ps2_kbd_t
{
public:
  ps2_kbd_t();
  int send_keypress(SDL_Keycode keycode, bool isMakeCode);
  uint8_t poll_response();

private:
  int send_byte(uint8_t byte);
  void process_command(uint8_t command);
  void reset_keyboard_params();
  int tx_mqd;
  int rx_mqd;

  Keyboard_State kbd_state = PS2_STATE_CMD;
  bool send_scancodes = false;

  std::unordered_map<SDL_Keycode, ps2_scancode> keycode_map = {
    {SDLK_a,          ps2_scancode {{0x1C}, 1}},
    {SDLK_b,          ps2_scancode {{0x32}, 1}},
    {SDLK_c,          ps2_scancode {{0x21}, 1}},
    {SDLK_d,          ps2_scancode {{0x23}, 1}},
    {SDLK_e,          ps2_scancode {{0x24}, 1}},
    {SDLK_f,          ps2_scancode {{0x2B}, 1}},
    {SDLK_g,          ps2_scancode {{0x34}, 1}},
    {SDLK_h,          ps2_scancode {{0x33}, 1}},
    {SDLK_i,          ps2_scancode {{0x43}, 1}},
    {SDLK_j,          ps2_scancode {{0x3B}, 1}},
    {SDLK_k,          ps2_scancode {{0x42}, 1}},
    {SDLK_l,          ps2_scancode {{0x4B}, 1}},
    {SDLK_m,          ps2_scancode {{0x3A}, 1}},
    {SDLK_n,          ps2_scancode {{0x31}, 1}},
    {SDLK_o,          ps2_scancode {{0x44}, 1}},
    {SDLK_p,          ps2_scancode {{0x4D}, 1}},
    {SDLK_q,          ps2_scancode {{0x15}, 1}},
    {SDLK_r,          ps2_scancode {{0x2D}, 1}},
    {SDLK_s,          ps2_scancode {{0x1B}, 1}},
    {SDLK_t,          ps2_scancode {{0x2C}, 1}},
    {SDLK_u,          ps2_scancode {{0x3C}, 1}},
    {SDLK_v,          ps2_scancode {{0x2A}, 1}},
    {SDLK_w,          ps2_scancode {{0x1D}, 1}},
    {SDLK_x,          ps2_scancode {{0x22}, 1}},
    {SDLK_y,          ps2_scancode {{0x35}, 1}},
    {SDLK_z,          ps2_scancode {{0x1A}, 1}},
    {SDLK_0,          ps2_scancode {{0x45}, 1}},
    {SDLK_1,          ps2_scancode {{0x16}, 1}},
    {SDLK_2,          ps2_scancode {{0x1E}, 1}},
    {SDLK_3,          ps2_scancode {{0x26}, 1}},
    {SDLK_4,          ps2_scancode {{0x25}, 1}},
    {SDLK_5,          ps2_scancode {{0x2E}, 1}},
    {SDLK_6,          ps2_scancode {{0x36}, 1}},
    {SDLK_7,          ps2_scancode {{0x3D}, 1}},
    {SDLK_8,          ps2_scancode {{0x3E}, 1}},
    {SDLK_9,          ps2_scancode {{0x46}, 1}},
    {SDLK_RETURN,     ps2_scancode {{0x5A}, 1}},
    {SDLK_ESCAPE,     ps2_scancode {{0x76}, 1}},
    {SDLK_BACKSPACE,  ps2_scancode {{0x66}, 1}},
    {SDLK_TAB,        ps2_scancode {{0x0D}, 1}},
    {SDLK_SPACE,      ps2_scancode {{0x29}, 1}},
    {SDLK_MINUS,      ps2_scancode {{0x4E}, 1}},
    {SDLK_EQUALS,     ps2_scancode {{0x55}, 1}},
    {SDLK_LEFTBRACKET, ps2_scancode {{0x54}, 1}},
    {SDLK_RIGHTBRACKET, ps2_scancode {{0x5B}, 1}},
    {SDLK_BACKSLASH,  ps2_scancode {{0x5D}, 1}},
    {SDLK_SEMICOLON,  ps2_scancode {{0x4C}, 1}},
    {SDLK_QUOTE,      ps2_scancode {{0x52}, 1}},
    {SDLK_BACKQUOTE,  ps2_scancode {{0x0E}, 1}},
    {SDLK_LSHIFT,     ps2_scancode {{0x12}, 1}},
    {SDLK_RSHIFT,     ps2_scancode {{0x59}, 1}},
    {SDLK_LCTRL,      ps2_scancode {{0x14}, 1}},
    {SDLK_PERIOD,     ps2_scancode {{0x49}, 1}},
    {SDLK_SLASH,      ps2_scancode {{0x4A}, 1}},
    {SDLK_MINUS,      ps2_scancode {{0x4E}, 1}},
    {SDLK_EQUALS,     ps2_scancode {{0x55}, 1}},
    {SDLK_LEFT,       ps2_scancode {{0xE0, 0x6B}, 2}},
    {SDLK_RIGHT,      ps2_scancode {{0xE0, 0x74}, 2}},
    {SDLK_UP,         ps2_scancode {{0xE0, 0x75}, 2}},
    {SDLK_DOWN,       ps2_scancode {{0xE0, 0x72}, 2}},
  };
};

#endif