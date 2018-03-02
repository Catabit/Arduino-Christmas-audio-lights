# Arduino-Christmas-audio-lights

This project was about making an old Christmas light strip light up based on the ambient sounds and/or music. I divided the project into 3 steps:

Hacking the old control unit:
I started by reverse engineering the circuit to find the main Sillicon Controlled Rectifiers (almost the same function as a Thyristor) and to determine if the 5V supplied by the Arduino would be enough to trigger them, which proved to be true. I figured out that most of the old circuit was working on mains voltage (220V) so I had to be careful not to create something too dangerous to use, hence the physical separation of the units (the old one and the arduino itself) and the decision to keep the old plastic housing but to cut open a small window to add the signal wires.

Controlling the unit:
Once i figured out how the old circuit works I added some basic code to test out the Arduino's capabilities which proved to be sufficent.

Adding sound detection and filtering:
Using a capacitive microphone and a 386D OP Amp I built a board which supplies the Arduino an amplified (gain @200) analog signal biased on 2.5V (0-5V range). Using some filtering software (Fast fourier transforms) I extracted the low frequency noises which give the rythm of the music and powered on/off the lights accordingly. The code could have been improved but the main issue was the low quality microphone which picked up lots of unfiltered noise.

I chose not to upload the code here, but it is based around the https://github.com/circuitar/Dimmer project which uses a Zero-Cross detection algorithm and the Arduino internal timers.

Video:

https://youtu.be/BRpwzS5ST4w - mostly following the beat of the music, but it gets false positives or misses steps sometimes

https://youtu.be/reowFETNMWA - the Arduino responding to finger snaps by lighting on/off an LED

https://youtu.be/ykY_GMRlUBM - controlling the fade speed of the lights with input from the keyboard

https://youtu.be/xFpXm2ftjNs - preliminary test of powering on the individual light colors.


