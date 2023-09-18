#include "ps2_kbd.h"
#include <unistd.h>
#include <stdio.h>
#include <SDL2/SDL_keycode.h>
#include <mqueue.h>


ps2_kbd_t::ps2_kbd_t() {
  struct mq_attr attr;

  attr.mq_flags = O_NONBLOCK;
  attr.mq_maxmsg = 64;
  attr.mq_msgsize = 1;
  attr.mq_curmsgs = 0;

  tx_mqd = mq_open ("/spike-ps2kbdtx", O_WRONLY | O_CREAT, 0660, &attr);
  if (tx_mqd < 0) {
    perror("mq_open() tx_mqd");
  }
  rx_mqd = mq_open ("/spike-ps2kbdrx", O_RDONLY | O_CREAT | O_NONBLOCK, 0660, &attr);
  if (rx_mqd < 0) {
    perror("mq_open() rx_mqd");
  }
}

int ps2_kbd_t::send_keypress(SDL_Keycode keycode, bool isMakeCode) {
  ps2_scancode scancode = keycode_map[keycode];
  if (scancode.length == 1) {
    if (!isMakeCode)
      ps2_kbd_t::send_byte(0xF0);
    ps2_kbd_t::send_byte(scancode.bytes[0]);
    return 0;
  }
  else {
    ps2_kbd_t::send_byte(scancode.bytes[0]);
    if (!isMakeCode)
      ps2_kbd_t::send_byte(0xF0);
    ps2_kbd_t::send_byte(scancode.bytes[1]);
    return 0;
  }
}

int ps2_kbd_t::send_byte(uint8_t byte) {
  mq_send (tx_mqd, (char*) &byte, 1, 0);
  usleep(1000);
  return 0;
}

void ps2_kbd_t::process_command(uint8_t command) {

  uint8_t response_length = 1;
  uint8_t response_byte[] = {PS2_ACK, 0, 0};

  switch(command) {
    case PS2_RESET:
      reset_keyboard_params();
      response_byte[1] = PS2_AOK;
      response_length = 2;
      break;

    case PS2_SET_LEDS:
      kbd_state = PS2_STATE_SET_LEDS;
      break;

    case PS2_ECHO:
      response_byte[0] = PS2_ECHO;
      break;

    case PS2_GETSET_SCANCODES:
      kbd_state = PS2_STATE_SET_SCANCODE_SET;
      break;

    case PS2_SET_TYPEMATIC:
      kbd_state = PS2_STATE_SET_SAMPLE_RATE;
      break;

    case PS2_DISABLE_SCANNING:
      send_scancodes = false;
      reset_keyboard_params();
      break;

    case PS2_ENABLE_SCANNING:
      send_scancodes = true;
      break;

    case PS2_ID_KEYBOARD:
      response_byte[1] = 0xAB;
      response_byte[2] = 0x83;
      response_length = 3;
      break;
    
    case PS2_SET_DEFAULT_PARAMS:
      reset_keyboard_params();
      break;

    case PS2_RESEND:
      printf("Unimplemented!\n");
      break;

    default:
      response_byte[0] = PS2_NAK;
    }

    int i;
    for (i = 0; i < response_length; i++) {
      printf("Sending Byte:%X\n", response_byte[i]);
      send_byte(response_byte[i]);
    }
}

uint8_t ps2_kbd_t::poll_response() {
  uint8_t recieved_byte;
  if (mq_receive (rx_mqd, (char*) &recieved_byte, 1, NULL) > -1) {
    printf("Received Byte:%X\n", recieved_byte);

    switch(kbd_state) {
      case PS2_STATE_CMD:
        process_command(recieved_byte);
        break;

      case PS2_STATE_SET_SAMPLE_RATE:
        send_byte(PS2_ACK);
        kbd_state = PS2_STATE_CMD;
        break;

      case PS2_STATE_SET_SCANCODE_SET:
        switch(recieved_byte) {
          case 0:
            send_byte(PS2_ACK);
            send_byte(2);
            break;

          case 2:
            send_byte(PS2_ACK);
            break;

          default:
          send_byte(PS2_NAK);
        }

        kbd_state = PS2_STATE_CMD;
        break;

      case PS2_STATE_SET_LEDS:
        send_byte(PS2_ACK);
        kbd_state = PS2_STATE_CMD;
    }
  }
  return 0;
}

void ps2_kbd_t::reset_keyboard_params() {
  kbd_state = PS2_STATE_CMD;
}