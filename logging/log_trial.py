from typing import List, Tuple

import numpy as np
import serial

episode_end_marker = "***"
theta_marker = "["
good_performance_threshold = 20
times_seen_threshold = 9
max_num_episodes = 30


def main():
    episodes = collect_data()
    episode_performance = convert_to_performance(episodes)

    np.savetxt("trial.csv", episode_performance, delimiter=",", fmt="%s")


def convert_to_performance(episodes: List[Tuple[List, List]]) -> List[
    List[float]]:
    episode_performance = [[], []]
    for (steps, cumulative_rewards) in episodes:
        episode_reward = cumulative_rewards[len(cumulative_rewards) - 1]
        episode_performance[0].append(len(steps))
        episode_performance[1].append(episode_reward)
    return episode_performance


def collect_data() -> List[Tuple[List, List]]:
    arduino = serial.Serial("/dev/cu.usbserial-A5020N7H", 115200)

    current_episode = ([], [])
    episodes = []
    converged = False
    times_seen = 0

    while not converged and len(episodes) < max_num_episodes:
        line = arduino.readline()
        lineString = line.decode(errors="ignore")
        lineString = lineString.strip()

        if theta_marker in lineString:
            ()
        elif lineString == episode_end_marker:
            last_step = current_episode[0][len(current_episode[0]) - 1]
            last_reward = current_episode[1][len(current_episode[1]) - 1]
            print(str(last_step) + " " + str(last_reward))
            episodes.append(current_episode)
            if len(current_episode[0]) < good_performance_threshold:
                times_seen += 1
            if times_seen > times_seen_threshold:
                converged = True
            current_episode = ([], [])
        else:
            components = lineString.split(",")
            step = components[0]
            cumulative_reward = components[1]
            current_episode[0].append(step)
            current_episode[1].append(cumulative_reward)

    return episodes


if __name__ == "__main__":
    main()
