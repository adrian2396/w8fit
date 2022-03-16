#ifndef DEBUG_H 
#define DEBUG_H

// Debug directives. Use printf() syntax
// Commnent next line to disable debug messages
#define DEBUG_ESP_PORT Serial
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ ) 
#else
#define DEBUG_MSG(...)
#endif
#define DEBUG_BAUD_RATE 115200 // Terminal baud rate

inline void set_debug_port()
{
    DEBUG_ESP_PORT.begin(DEBUG_BAUD_RATE);
}

#endif