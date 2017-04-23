#include <tiny_23017.h>

// 23017 lib...
unsigned char mcp23017_address = 0x20;
unsigned char mcp23017_iodira  = 0x00;
unsigned char mcp23017_iodirb  = 0x00;
unsigned char mcp23017_iopua  = 0x00;
unsigned char mcp23017_iopub  = 0x00;
unsigned char mcp23017_pina   = 0x00;
unsigned char mcp23017_pinb   = 0x00;

static void mcp23017_write_reg(unsigned char reg, unsigned char value)
{
  TinyWireM.beginTransmission(mcp23017_address);
  TinyWireM.send(reg);
  TinyWireM.send(value);
  TinyWireM.endTransmission();  
}

// copies io direction/pullup to device
void mcp23017_refresh_mirror_con(void)
{
  mcp23017_write_reg(0, mcp23017_iodira);
  mcp23017_write_reg(1, mcp23017_iodirb);
  mcp23017_write_reg(0xc, mcp23017_iopua);
  mcp23017_write_reg(0xd, mcp23017_iopub);
}

// reads and then writes bits.
void mcp23017_refresh_mirror_pin(void)
{
  // read and mask off output bits
  TinyWireM.beginTransmission(mcp23017_address);
  TinyWireM.send(0x12);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(mcp23017_address, 1);
  mcp23017_pina = (mcp23017_pina & ~mcp23017_iodira) | (TinyWireM.read() & mcp23017_iodira);

  TinyWireM.beginTransmission(mcp23017_address);
  TinyWireM.send(0x13);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(mcp23017_address, 1);
  mcp23017_pinb = (mcp23017_pinb & ~mcp23017_iodirb) | (TinyWireM.read() & mcp23017_iodirb);

  // write values out
  mcp23017_write_reg(0x12, mcp23017_pina);
  mcp23017_write_reg(0x13, mcp23017_pinb);
}

// init library.  Address is lower three A0-A2 bits
void mcp23017_init(int address)
{
  TinyWireM.begin();
  mcp23017_address = 0x20 | address;
  mcp23017_refresh_mirror_con();
}

// set mode, pins are numbered 0-7 for A0-A7 and 8-15 for B0-B7
// modes can be INPUT, OUTPUT, INPUT_PULLUP
void mcp23017_setmode(unsigned char pin, unsigned char mode)
{
  // update mirrors
  if (pin < 8) {
    mcp23017_iodira &= ~(1 << pin);
    mcp23017_iopua &= ~(1 << pin);
    if (mode != OUTPUT) {
      mcp23017_iodira |= (1 << pin);
      if (mode == INPUT_PULLUP) {
        mcp23017_iopua |= (1 << pin);
      }
    }
  } else {
    pin -= 8;
    mcp23017_iodirb &= ~(1 << pin);
    mcp23017_iopub &= ~(1 << pin);
    if (mode != OUTPUT) {
      mcp23017_iodirb |= (1 << pin);
      if (mode == INPUT_PULLUP) {
        mcp23017_iopub |= (1 << pin);
      }
    }
  }
  mcp23017_refresh_mirror_con();
}

// write a bit to a pin, numbering like setmode
void mcp23017_write(unsigned char pin, unsigned char data)
{
  if (pin < 8) {
    mcp23017_pina &= ~(1 << pin);
    mcp23017_pina |= data << pin;
  } else {
    pin -= 8;
    mcp23017_pinb &= ~(1 << pin);
    mcp23017_pinb |= data << pin;
  }
  mcp23017_refresh_mirror_pin();
}

// read a bit from a pin
unsigned char mcp23017_read(unsigned char pin)
{
  mcp23017_refresh_mirror_pin();
  if (pin < 8) {
    return ((1 << pin) & mcp23017_pina) ? 1 : 0;
  } else {
    pin -= 8;
    return ((1 << pin) & mcp23017_pinb) ? 1 : 0;
  }
}
