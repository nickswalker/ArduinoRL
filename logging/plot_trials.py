import os
from typing import List

import numpy as np
import scipy as sp
import scipy.stats
from matplotlib import pyplot as pp
from matplotlib.backends.backend_pdf import PdfPages


def main():
    trials = get_trials()
    rewards_by_step = extract_data(trials)

    means = []
    confidences = []
    for rewards in rewards_by_step:
        mean, confidence = mean_confidence_interval(rewards)
        means.append(mean)
        confidences.append(confidence)

    series = [i for i in range(0, len(means))]

    plot = Plot("Mean reward per episode, p=0.10")
    plot.plot_evaluations(series, means, confidences, "Sarsa")
    plot.save("figure0")


def mean_confidence_interval(data, confidence=0.90):
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


def get_trials() -> List[str]:
    return ["trials/" + name for name in os.listdir('trials') if
            os.path.isfile("trials/" + name) and not name.startswith(".")]


class Plot:
    def __init__(self, title):
        self.ax = pp.axes()
        self.ax.set_title(title, fontsize=14, fontweight='bold')
        self.ax.set_xlabel('Episodes')
        self.ax.set_ylabel('Reward')
        self.markers = {"o", "D", "^", "8", "h", "s"}

    def plot_evaluations(self, series, means, confidences,
                         label):
        pp.errorbar(series, means, yerr=confidences, label=label)

    def save(self, name):
        # pp.legend(loc="lower right")
        pdf = PdfPages(str(name) + '.pdf')

        pdf.savefig()

        pdf.close()


if __name__ == "__main__":
    main()
