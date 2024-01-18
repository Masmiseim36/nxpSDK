# Random number generator
## Overview
The RNG module is part of the framework used for random number generation. It uses hardware RNG peripherals and a software pseudo-random number generation algorithm. If no hardware acceleration is present, the RNG module uses a software algorithm. On devices that have the SIM_UID registers, the UIDL is used as the initial seed for the random number generator.