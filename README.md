# tiny_mcp23017
This is a TinyWireM library for the MCP23017 I2C 16-bit bidirectional GPIO expander.

The library uses TinyWireM so that it can be used with ATtiny devices like the 85.

TODO:
* support more than one 23017.
* add a function to provide fast bit tests.  Right now to test a bit it reads both 8-bit sets of pins and then compares.  In theory you only want to read 16-bits once per loop then just test which bits are set.  
* Support interrupts/etc (though this might be impractical with 8-pin tiny chips)
