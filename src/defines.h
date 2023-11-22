#define DEBUG_MODE 1 // Or 0 if you dont want to debug

#ifdef DEBUG_MODE
#define debug(x) Serial.print(x);
#define debugln(x) Serial.println(x);
#else
#define debug(x)
#define debugln(x)
#endif

// pin definitions for mcp23017
// https://registry.platformio.org/libraries/adafruit/Adafruit%20MCP23017%20Arduino%20Library
#define GPA0 0
#define GPA1 1
#define GPA2 2
#define GPA3 3
#define GPA4 4
#define GPA5 5
#define GPA6 6
#define GPA7 7
#define GPB0 8
#define GPB1 9
#define GPB2 10
#define GPB3 11
#define GPB4 12
#define GPB5 13
#define GPB6 14
#define GPB7 15

#define BTN01 13
#define BTN02 15
