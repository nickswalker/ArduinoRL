# Arduino RL

A gradient descent Sarsa agent that controls a custom two degrees-of-freedom arm.

* Low memory footprint update implementation
* Logging utilities (written in Python) to parse data sent over serial
* Plotting utilities

## Why?

Reinforcement learning is a powerful and flexible approach to learning from interaction. Embedded reinforcement learning agents could be a key component to creating engaging, interactive experiences with everyday objects. However, RL methods have not typically been designed with memory constraints in mind. To investigate the issues embedded agents face, I wanted to see how a common learning algorithm would work in the 2kb of SRAM available on an Atmel 328p (Arduino Uno/Pro Mini).

## The platform

<a href="/photos/eagle_small.jpg">
    <img src="/photos/eagle_small.jpg?raw=true" width="400px" align="right" vspace="2px">
</a>

The agent gets to control a two degrees-of-freedom arm. The joints have 155 degrees of rotation. The elbow joint controls a rod tipped with an LED which the agent can toggle on and off. A photo resistor on the surface can detect whether the agent is pointing at it.

## Task

The agent must point the LED at the photoresistor in as few actions as possible. Each episode ends when the photocell reads above a threshold, and the agent is reset to a random start position. The agent is penalized for turning on the LED uneccesarily.

## Approach and Performance

To see the details on the implementation and approach, as well as the specification of the reward function, please see the [writeup](https://dl.dropboxusercontent.com/u/971295/ArduinoRL_writeup.pdf). You can also watch [a video](https://www.youtube.com/watch?v=SCv1AomFDG0) of the agent in action.

## Photos

![](/photos/arm_detail_small.jpg?raw=true)

![](/photos/action_detail_small.jpg?raw=true)

![](/photos/long_shutter_small.jpg?raw=true)
