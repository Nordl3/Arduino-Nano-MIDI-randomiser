# Arduino-Nano-MIDI-randomiser
Arduino Nano MIDI randomiser for the Behringer Syncussion SY-1. MIDI clock in from an RD-8, 2 buttons, random 8-step pitch + trigger sequences for left and right drums, MIDI out to SY-1 in Mode 2.

https://youtu.be/OfJEdvcUYO0

RD-8 sends the MIDI clock. Nano listens for clock/start/stop, then spits out random 8-step pitch + trigger sequences to the left and right SY-1 drums. Two buttons only: one randomises the left drum, one randomises the right.

The MIDI IN side uses a 6N137 opto. Powered the Nano from a 9V wart into Vin and fed the opto from the Nano 5V rail.

