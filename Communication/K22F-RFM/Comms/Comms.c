/* High Level Communication Functions
 *
 * Alex Colpitts
 *
 * Feb 19, 2018
 */

#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../SPI0/SPI0_driver.h"
#include "../RFM69/RFM69registers.h"
#include "../RFM69/RFM69_driver.h"
#include "../GPIO/gpio.h"
#include "../FTM/FTM_driver.h" // needed for timeout capability
#include "../GPIO/gpio.h" // included in RFM69 driver


////////////////////////////////// GroundStation Functions //////////////////////////////////

// request packet number for satellite to transmit
void packetRequest(uint8_t *p, int block);

// get satellite to start transmitting
void txStart(uint8_t *p);


//////////////////////////////////// Satellite Functions ////////////////////////////////////

// transmit block n
void transmitPacket(uint8_t *p, int block);

// transmit image size in blocks
void imageSize(uint8_t *p);

