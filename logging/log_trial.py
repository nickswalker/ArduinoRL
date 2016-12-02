import os
from typing import List, Tuple

import numpy as np
import serial

episode_end_marker = "***"
theta_marker = "["
trial_marker = "!!!"


class Trial:
    def __init__(self, episodes: List[Tuple[List, List]]):
        self.episodes = episodes
        self.episodes.append(([], []))

    def observe(self, step: int, reward: float):
        current_episode = self.episodes[len(self.episodes) - 1]
        current_episode[0].append(step)
        current_episode[1].append(reward)

    def episode_end(self):
        self.episodes.append(([], []))

    def last_observation(self):
        if len(self.episodes) < 1:
            return ""
        current_episode = self.episodes[len(self.episodes) - 1]
        if len(current_episode[0]) < 1:
            return ""
        last_step = current_episode[0][len(current_episode[0]) - 1]
        last_reward = current_episode[1][len(current_episode[1]) - 1]
        return str(last_step) + " " + str(last_reward)

    def get_per_episode_performance(self) -> Tuple[List[int], List[float]]:
        episode_performance = [[], []]
        for (steps, cumulative_rewards) in self.episodes:
            if len(cumulative_rewards) == 0:
                continue
            episode_reward = cumulative_rewards[len(cumulative_rewards) - 1]
            episode_performance[0].append(len(steps))
            episode_performance[1].append(episode_reward)
        return episode_performance


class Arduino:
    def __init__(self, port_name: str):
        self.serial = serial.Serial(port_name, 115200)

    def collect_trials(self, num_trials: int, max_num_steps: int = 100) -> List[Trial]:
        trials = []
        for i in range(num_trials):
            self.reset()
            self.wait_for_trial_mark()
            trial = self.collect_trial(max_num_steps)
            trials.append(trial)

    def collect_trial(self, max_num_steps: int) -> Trial:
        trial = Trial([])
        converged = False

        while not converged and len(trial.episodes) < max_num_steps:
            print(trial.last_observation())
            line = self.serial.readline()
            line_string = line.decode(errors="ignore")
            line_string = line_string.strip()

            if theta_marker in line_string or trial_marker == line_string:
                pass
            elif line_string == episode_end_marker:
                trial.episode_end()
            else:
                components = line_string.split(",")
                step = components[0]
                cumulative_reward = components[1]
                trial.observe(step, cumulative_reward)

        return trial

    def reset(self):
        self.serial.write([35, 35])

    def wait_for_trial_mark(self):
        line_string = ""
        while line_string != trial_marker:
            print(line_string)
            line = self.serial.readline()
            line_string = line.decode(errors="ignore")
            line_string = line_string.strip()
            self.reset()
        print("Saw mark")


def main():
    arduino = Arduino("/dev/cu.usbserial-A5020N7H")
    trials = arduino.collect_trials(10)
    for trial in trials:
        trial_num = count_trials()
        trial_performances = trial.get_per_episode_performance()
        np.savetxt("trials/" + str(trial_num) + ".csv", trial_performances, delimiter=",", fmt="%s")


def count_trials():
    return len([name for name in os.listdir('trials') if os.path.isfile("trials/" + name)])


if __name__ == "__main__":
    main()
