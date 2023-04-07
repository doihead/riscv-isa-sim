#include "devices.h"

#define ALTERAPS2_SIZE 8

altera_ps2_t::altera_ps2_t(abstract_interrupt_controller_t *intctrl, uint32_t interrupt_id)
  : intctrl(intctrl), interrupt_id(interrupt_id), cntrl_regs(0)
{
  printf("Initialized PS/2 Controller");
}

bool altera_ps2_t::load(reg_t addr, size_t len, uint8_t* bytes)
{
  // Enforcing 32 bit accesses to the device because I can't seem to find any docs on what's
  // supposed to happen for partial reads of the data register and all the drivers I could find
  // only do word/dword reads
  if (addr > ALTERAPS2_SIZE || len != 4)
    return false;
  
  if (addr < 4){
    if (rx_queue.empty()) {
      memset(bytes, 0, 4);
    } else {
      bytes[0] = rx_queue.size() >> 8;
      bytes[1] = rx_queue.size() & 0xFF;
      bytes[2] = 0x80;
      bytes[3] = rx_queue.front();

      rx_queue.pop();
    }
  } else {
    memcpy(bytes, &cntrl_regs, 4);
  }
  return true;
}

bool altera_ps2_t::store(reg_t addr, size_t len, const uint8_t* bytes)
{
  if (addr + len > ALTERAPS2_SIZE)
    return false;
  if (addr == 0) {
    printf("Trying to send: %x\n", bytes[0]);
    return true;
  }

  else if (addr == 4) {
    cntrl_regs |= (bytes[0] & 0x1);
    return true;
  }
  return false;
}

void altera_ps2_t::send_byte(uint8_t byte)
{
  rx_queue.push(byte);
  if (cntrl_regs & 1) {
    intctrl->set_interrupt_level(interrupt_id, 1);
  }
}