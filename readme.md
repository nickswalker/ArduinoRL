# ArduinoRL

A couple of projects implementing reinforcement learning approaches to problems in arm control.

Also includes:

* Logging utilities (written in Python) to parse data sent over serial
* Plotting utilities

## Why?

Reinforcement learning is a powerful and flexible approach to learning from interaction. Embedded reinforcement learning agents could be a key component to creating engaging, interactive experiences with everyday objects. However, RL methods have not typically been designed with memory constraints in mind. To investigate the issues embedded agents face, I wanted to see how common learning algorithms would work in the 2kb of SRAM available on an Atmel 328p (Arduino Uno/Pro Mini) or the 32kb available on an ARM M0(Teensy 3.2).
