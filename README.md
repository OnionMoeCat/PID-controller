# **Udacity Self-Driving Car Enginer Nanodegree - *PID Control Project***

## Basic Build Instructions

1. ./install-%platform%.sh
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## Introduction

The purpose of this project was to "build a PID controller and tune the PID hyperparameters by applying the general processing flow as described in the lessons," and to "test your solution on the simulator!" The simulator provides cross-track error (CTE), speed, and steering angle data. The PID (proportional/integral/differential) controller must respond with steering and throttle commands to drive the car reliably around the simulator track.

## Rubric Discussion Points

- *Describe the effect each of the P, I, D components had in your implementation.*

The P, or "proportional". It causes the car to steer proportional (and opposite) to the car's distance from the lane center (which is the CTE) - if the car is far to the right it steers hard to the left, if it's slightly to the left it steers slightly to the right.

The D, or "differential", counteracts the P component's tendency to overshoot the center line. A properly tuned D parameter will cause the car to approach the center line smoothly.

The I, or "integral", counteracts a bias in the CTE which prevents the P-D controller from reaching the center line. This bias can take several forms, such as a steering drift (as in the Control unit lessons).

The final PID controller implementation performed much like in the following video.

[Final Parameters](https://www.youtube.com/watch?v=2YeIQ_7hgSk)

The difference in performance when the I component is removed from the controller is subtle. Notice that the center line is not followed as closely around curves.
Removing the D component from the controller is disastrous. It begins to ring back and forth across the center line until finally leaving the track.

- *Describe how the final hyperparameters were chosen.*

Hyperparameters were tuned manually at first. This was necessary because the narrow track left little room for error, and when attempting to automate parameter optimization (such as Twiddle) it was very common for the car to leave the track, thus invalidating the optimization. 

I also implemented a PID controller for the throttle. The throttle PID controller is fed the magnitude of the CTE because it doesn't make sense to throttle up for right-side CTE and down for left-side CTE, for example. For this reason the throttle controller doesn't include an I component, which would only grow indefinitely.

