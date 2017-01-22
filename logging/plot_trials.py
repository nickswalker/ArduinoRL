import os
from typing import List

import numpy as np
import scipy as sp
import scipy.stats
import sys
from matplotlib import pyplot as pp
from matplotlib.backends.backend_pdf import PdfPages

from plotting_utilities import Plot


def main():
    trials_dir = sys.argv[1]
    trials = get_trials(trials_dir)
    rewards_by_step = extract_data(trials)

    means = []
    confidences = []
    for rewards in rewards_by_step:
        mean, confidence = mean_confidence_interval(rewards)
        means.append(mean)
        confidences.append(confidence)

    series = [i for i in range(0, len(means))]

    plot = Plot("Fast, Linear Initialization Performance")
    plot.plot_evaluations(series, means, confidences, "Sarsa")
    plot.save("figure1", ".")


def mean_confidence_interval(data, confidence=0.90):
    if len(data) == 1:
        return data[0], 0
    a = 1.0 * np.array(data)
    n = len(a)
    m, se = np.mean(a), scipy.stats.sem(a)
    h = se * sp.stats.t._ppf((1 + confidence) / 2., n - 1)
    return m, h * 2


def extract_data(paths: List[str]) -> List[List[float]]:
    rewards_by_episode = [[] for i in range(0, 200)]
    for path in paths:
        episodes, rewards = np.loadtxt(path, delimiter=",")
        i = 0
        for (steps, reward) in zip(episodes, rewards):
            rewards_by_episode[i].append(reward)
            i += 1

    rewards_by_episode = [episode for episode in rewards_by_episode if len(episode) > 0]
    return rewards_by_episode


def get_trials(dir: str) -> List[str]:
    return [os.path.join(dir, name) for name in os.listdir(dir) if
            os.path.isfile(os.path.join(dir, name)) and not name.startswith(".")]


if __name__ == "__main__":
    main()
