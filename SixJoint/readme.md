# Six Joint Arm

## The platform

<a href="photos/eagle_small.jpg">
    <img src="photos/eagle_small.jpg?raw=true" width="400px" align="right" vspace="2px">
</a>

The agent gets to control a six degrees-of-freedom arm. The joints have different ranges of motion, as determined by the physical constraints of the arm. A current sensor provides instantaneous current consumption readings.

## Task

The agent must point the LED at the photoresistor in as few actions as possible. Each episode ends when the photocell reads above a threshold, and the agent is reset to a random start position. The agent is penalized for turning on the LED unnecessarily.

## Approach and Performance

To see the details on the implementation and approach, as well as the specification of the reward function, please see the [writeup](https://dl.dropboxusercontent.com/u/971295/SixJointArduinoRL_writeup.pdf). You can also watch [a video](https://www.youtube.com/watch?v=SCv1AomFDG0) of the agent in action.

## Photos

![](photos/arm_detail_small.jpg?raw=true)

![](photos/action_detail_small.jpg?raw=true)

![](photos/long_shutter_small.jpg?raw=true)
