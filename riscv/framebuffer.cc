#include "devices.h"
#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

fb_device_t::fb_device_t()
{
  fd = shm_open("/spike-fb", O_CREAT | O_RDWR, 0600);
  if (fd < 0) {
    printf("Could not create spike-fb shared memory - Did you exit the program correctly?\n");
  }

  ftruncate(fd, FB_SIZE);
  data = (char *) mmap(0, FB_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  memset(data, 0, FB_SIZE);
  printf("fd value: %d\nData Pointer: %p\n", fd, data);
}

fb_device_t::~fb_device_t()
{
  munmap(data, FB_SIZE);
  close(fd);
}
bool fb_device_t::load(reg_t addr, size_t len, uint8_t* bytes)
{
  if (addr + len > FB_SIZE)
    return false;
  memcpy(bytes, &data[addr], len);
  return true;
}

bool fb_device_t::store(reg_t addr, size_t len, const uint8_t* bytes)
{
  if (addr + len > FB_SIZE)
    return false;
  memcpy(&data[addr], bytes, len);
  return true;
}
