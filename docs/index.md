## References
1. [specMech Communications Guide](https://docs.google.com/document/d/16dir_fut0HBtZR5ReOcYNGfNxy1jjHzGRPkaTaeYjt8/edit?usp=sharing)
2. [BOSS Spectrograph Technical Manual](https://docs.google.com/document/d/11UHDSXXC8-JnVtt-BcFyOa2BDeodfezanSv0PJa2Pw8/edit?usp=sharing)

## Software Overview
specMech source code is kept here. specMech handles the motion controls and sensors on the BOSS spectrographs. The CCD readouts are handled by a different system, the Archon controller.

The specMech target processor is an Atmel ATMega4809 as implemented on a Microchip [dm320115 Curiosity Nano](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320115#additional-summary) board. The programming IDE we use is Atmel Studio 7.0 although one suspects that Microchip's MPLAB X should work with few, if any, code modifications.

## Hardware Overview

The original SDSS spectrograph uses a Z-World Little Giant controller board. This board is no longer manufactured so we designed and built a replacment to fit. The design software used is EagleCAD with Fusion360 for layout visualization.
