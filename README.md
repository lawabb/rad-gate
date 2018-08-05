# rad-gate
Arduino powered BMX gate controller with spoken cadence and random start

Main changes from dfrencham/rad-gate project (Thank you Danny for sharing your code) are:

1. "Reaction time" function. This produces a voice message and readout of time from 
gate drop to front wheel tripping the reaction time sensor. ( An IR light beam in my case)
 mp3 files are provided for the announcements.
2. Basic low battery voltage monitor and alarm
3. 4 Digit seven segement LED display for display of "reaction time" and voltage display on low voltage. See 
https://tronixlabs.com.au/display/led/numeric/four-digit-seven-segment-display-module-and-enclosure-australia/
for more information on the display
3. Option to configure gate activation polarity, and option to provide a short pulse for gate activation eg as is required
by a solenoid rather than an electromagnet
4. Some rewrite of timing code to incorporate the above and adhere to UCI timing document.
5. Alter "abort" code to indicate and prevent progress of sequence asap
6. Change to FastLED library to simpify options of LightTree
7. Added 'randomness' to prevent same random sequence on each start up
8. A 'setup' mode to aid positioning of the reaction time sensor. See addtional information below.

Major Hardware Components.

1. Arduino UNO R3 (Others should work)
2. Arduino UNO sheild - or some circuit board to mount everything on
3. 4 Digit 7 segment LED display (communicates directly with 74HC595s) (eBay)
4. Audio amplfier eg PAM8610 module (with volume)  (AliExpress)
5. MP3 player JQ6500 (16P)  (AliExpress)
  Or Adafruit Soundboard  (untested)
6. Solenoid - KingWay 50 lb (AliExpress)
7. Light beam transmitter/receiver E30-R4NA (AliExpress)
8. Optional RF receiver- can parallel contacts with "Go" switch
9. Options for Light Tree - Serial eg Neopixel or similar  (search WS2812 Aliexpress) or Individual (4) outputs


Additional information on using E30-R4NA sensor (for reaction time measurement) .
The E30-R4NA Infra Red Tx/Rx unit is relatively cheap and works up to a range of about 2.5m (from my tests). It does however require some additional hardware to effectively narrow the beam so that a narrow bmx tyre will interrupt the beam sufficiently to trigger the receive circuit. This can be done by mounting the module in a 170mm length of 40mm diameter PVC water pipe as shown below.

![IR Sensor stands](doc/IR_sensor_stands.jpg)

To setup the sensor,  the Tx/Rx unit must be accurately pointed and aligned with a reflector unit. To make this easier you can change the normal mode of operation of the controller to 'setup mode' by closing a switch connected to D12 of the Arduino. Once the setup mode is started, a continuous tone will be heard. Move the reflector and/or the tx/rx unit until the lower tone of the two tones is reliably heard. Test that interupting the beam with a sufficiently narrow object such as a broom handle causes the tone to change to a higher pitched tone during the interrption period. When satisfied with the alignment and position return the 'setup mode' switch to the normal position. The controller will then reset back to normal operation.

[Video of Controller](https://youtu.be/LeO_OOp3VB0)
