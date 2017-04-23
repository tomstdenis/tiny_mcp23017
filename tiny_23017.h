#include <Arduino.h>
#include <TinyWireM.h>

void mcp23017_init(int address);

void mcp23017_refresh_mirror_con(void);
void mcp23017_refresh_mirror_pin(void);
void mcp23017_setmode(unsigned char pin, unsigned char mode);
void mcp23017_write(unsigned char pin, unsigned char data);
unsigned char mcp23017_read(unsigned char pin);

extern unsigned char mcp23017_address;
extern unsigned char mcp23017_iodira;
extern unsigned char mcp23017_iodirb;
extern unsigned char mcp23017_iopua;
extern unsigned char mcp23017_iopub;
extern unsigned char mcp23017_pina;
extern unsigned char mcp23017_pinb;

