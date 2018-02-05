

#ifndef SOURCES_GPIO_GPIO_H
#define SOURCES_GPIO_GPIO_H

// set up all required GPIO pins. IMPORTANT function
void GPIO_init();

// sets red LED to provided state (1 = ON, 0 = OFF)
void redLED(int state);

// same, but with green
void greenLED(int state);

// same, but with blue
void blueLED(int state);

// returns the value read from SW3
int checkSW3();

// configure GPIO input pin to receive flags
void RFM69_DIO0_Init();

// read in value from DIO0 pin on RFM69
uint8_t RFM69_DIO0_Read();

#endif /* SOURCES_GPIO_GPIO_H */
