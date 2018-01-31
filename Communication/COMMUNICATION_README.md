# Communication Overview

The CubeSat communication system must be low power and able to transmit at least 400 km.

# VAF: Adaptive Spread Spectrum
The location of the satellite will directly affect the received power at the ground station. This will likely be the limiting factor in the system. The ground station can transmit 4W EIRP which will easily be able to be picked up by the satellite. On the contrary, the satellite will only be able to transmit 20 dBm, 21 is the antenna directivity is accounted for. 

In order to improve the data rate the system may use adaptive spread spectrum to improve the data rate. The system will likely be using Barker Codes to improve the received signal at the ground station using post processing. The data rate will be directly reduced by a factor of the data rate over the sequence lenght. Reducing the sequence length by from 13 to 11 will improve the effective data rate from 23 kbps to 27 kbps allowing the images to be transmitted faster. 

Based on the received signal strength estimates for the closest part of the satellite orbit, the removal of the sequence all together may be a possibility thus increasing the data rate to the maximum of 300 kbps.
