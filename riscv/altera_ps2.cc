#include "devices.h"
#include <mqueue.h>

#define ALTERAPS2_SIZE 8

altera_ps2_t::altera_ps2_t(abstract_interrupt_controller_t *intctrl, uint32_t interrupt_id)
  : intctrl(intctrl), interrupt_id(interrupt_id)
{
  printf("Starting to init ps2\n");
  tx_mqd = mq_open ("/spike-ps2kbdrx", O_WRONLY);
  if (tx_mqd < 0) {
    perror("mq_open() tx_mqd");
  }
  rx_mqd = mq_open ("/spike-ps2kbdtx", O_RDONLY | O_CREAT | O_NONBLOCK);
  if (rx_mqd < 0) {
    perror("mq_open() rx_mqd");
  }
  printf("Init ps2\n");

}

bool altera_ps2_t::load(reg_t addr, size_t len, uint8_t* bytes)
{
  // Enforcing 32 bit accesses to the device because I can't seem to find any docs on what's
  // supposed to happen for partial reads of the data register and all the drivers I could find
  // only do word/dword reads
  if (len != 4)
    return false;
  
  if (addr == 0){
    intctrl->set_interrupt_level(interrupt_id, 0);
    if (rx_queue.empty()) {
      memset(bytes, 0, 4);
    } else {
      bytes[0] = rx_queue.front();
      bytes[1] = 0x80;
      bytes[2] = rx_queue.size() & 0xFF;
      bytes[3] = rx_queue.size() >> 8;

      //printf("ps2 read %X\n", *((int*) bytes));

      rx_queue.pop();
    }
  }

  //Again ignoring PS2 Errors for now
  else if (addr == 4) {
    bytes[0] = irq_enable ? 1 : 0;
    bytes[1] = irq_pending ? 1 : 0;
  }
  return true;
}

bool altera_ps2_t::store(reg_t addr, size_t len, const uint8_t* bytes)
{
  if (len != 4)
    return false;
  if (addr == 0) {
    //printf("sending %X\n", bytes[0]);
    mq_send (tx_mqd, (char*) bytes, 1, 0);
    return true;
  }

  //Ignoring PS2 Errors for now
  else if (addr == 4) {
    irq_enable = (bytes[0] & 0x1);
    return true;
  }
  return false;
}

void altera_ps2_t::tick()
{
  char recieved_byte;

  if (mq_receive (rx_mqd, &recieved_byte, 1, NULL) > -1) {
    //printf("Recieving %X\n", recieved_byte);
    rx_queue.push(recieved_byte);
    if (irq_enable) {
      irq_pending = true;
      intctrl->set_interrupt_level(interrupt_id, 1);
    }
  }
}