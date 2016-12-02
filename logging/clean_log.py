import sys

import numpy as np

from log_trial import Trial, theta_marker, trial_marker, episode_end_marker


def main():
    file_name = sys.argv[1]
    trial = Trial([])

    with open(file_name, errors="ignore") as f:
        lines = f.readlines()
        for line in lines:
            line_string = line.strip()
            if theta_marker in line or trial_marker == line_string:
                pass
            elif line_string == episode_end_marker:
                trial.episode_end()
            elif len(line_string) > 0:
                components = line_string.split(",")
                step = int(components[0])
                cumulative_reward = float(components[1])
                trial.observe(step, cumulative_reward)

    trial_performances = trial.get_per_episode_performance()
    np.savetxt(file_name + "clean.csv", trial_performances, delimiter=",", fmt="%s")


if __name__ == "__main__":
    main()
