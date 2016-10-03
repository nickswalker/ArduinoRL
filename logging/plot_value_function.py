import re
from enum import Enum
from typing import List

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import serial
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D

base_range = (20, 160)
elbow_range = (20, 160)


class ArmState:
    def __init__(self, base: float, elbow: float, led_on: bool):
        self.base = base
        self.elbow = elbow
        self.led_on = led_on


class JointAction(Enum):
    Stay = 0
    Left = 1
    Right = 2


class ArmAction:
    def __init__(self, base_movement: JointAction, elbow_movement: JointAction, led_on: bool):
        self.base_movement = base_movement
        self.elbow_movement = elbow_movement
        self.led_on = led_on


class ValueFunction:
    def __init__(self, theta):
        self.theta = theta

    def value(self, state: ArmState, action: ArmAction) -> float:
        phi = self.extract_features(state, action)
        assert len(phi) == len(self.theta)
        return np.dot(self.theta, phi)

    def extract_features(self, state: ArmState, action: ArmAction) -> List[float]:

        base_direction = 0
        elbow_direction = 0
        if action.base_movement == JointAction.Left:
            base_direction = -1
        elif action.base_movement == JointAction.Right:
            base_direction = 1

        if action.elbow_movement == JointAction.Left:
            elbow_direction = -1
        elif action.elbow_movement == JointAction.Right:
            elbow_direction = 1

        phi = []
        phi += self.square_bin_binary_augment(state.base, state.elbow, state.led_on, 30, 6)
        phi.append(elbow_direction)
        phi.append(base_direction)
        return phi

    def square_bin_binary_augment(self, x_value: float, y_value: float, binary_value: bool, bin_by: float,
                                  side_length: float) -> List[float]:
        max_index = side_length * side_length * 2
        result = [0 for _ in range(0, max_index)]
        xBin = int(x_value / bin_by)
        yBin = int(y_value / bin_by)

        bin_index = side_length * yBin + xBin

        if binary_value:
            bin_index += side_length * side_length

        for i in range(0, max_index):
            if i == bin_index:
                result[i] = 1.0
            else:
                result[i] = 0.0
        return result


def main():
    arduino = serial.Serial("/dev/cu.usbserial-A5020N7H", 115200)
    test = Axes3D(plt.gcf())
    matplotlib.interactive(True)
    plot = SurfacePlot(base_range, elbow_range)
    first = True
    plt.show()
    plt.ion()
    while True:
        if not first:
            break

        line = arduino.readline()
        lineString = line.decode(errors="ignore")
        lineString = lineString.strip()
        if "[" in lineString:
            weights = re.findall(r"[-\d.]+", lineString)
            if len(weights) != 74:
                continue
            weight_floats = [float(weight) for weight in weights]
            value_function = ValueFunction(weight_floats)
            plot.draw_now(value_function)
            plt.ioff()


class SurfacePlot():
    def __init__(self, x_range, y_range):
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.ax.set_zlim3d(-100, 100)

        x_resolution = (x_range[1] - x_range[0]) / 10.0
        y_resolution = (y_range[1] - y_range[0]) / 10.0

        x = np.arange(x_range[0], x_range[1], x_resolution)
        y = np.arange(y_range[0], y_range[1], y_resolution)
        X, Y = np.meshgrid(x, y)

        self.x = X
        self.y = Y

        z = np.zeros(self.x.shape)
        self.surf = self.ax.plot_surface(
            self.x, self.y, z, rstride=1, cstride=1,
            cmap=cm.jet, linewidth=0, antialiased=False)

        self.ax.set_xlabel('Base angle')
        self.ax.set_ylabel('Elbow angle')
        self.ax.set_zlabel('Value')

        plt.draw()

    def draw_now(self, value_function: ValueFunction):
        self.surf.remove()
        zs = np.array(
            [value_function.value(ArmState(x, y, False), ArmAction(JointAction.Stay, JointAction.Stay, True)) for x, y
             in
             zip(np.ravel(self.x), np.ravel(self.y))])
        z = zs.reshape(self.x.shape)

        self.surf = self.ax.plot_surface(
            self.x, self.y, z, rstride=1, cstride=1,
            cmap=cm.viridis, linewidth=0, antialiased=False)
        plt.draw()  # redraw the canvas
        plt.pause(0.0001)


if __name__ == "__main__":
    main()
