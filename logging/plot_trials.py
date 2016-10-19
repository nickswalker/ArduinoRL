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

SPINE_COLOR = 'gray'
class Plot:
    def __init__(self, title):
        self.ax = pp.axes()
        self.ax.set_title(r"\textbf{%s}" % title, fontsize=18, fontweight='bold', y=1.05)
        self.ax.set_xlabel('Episodes', fontsize=14)
        self.ax.set_ylabel('Reward', fontsize=14)
        self.markers = {"o", "D", "^", "8", "h", "s"}
        self.latexify()

    def latexify(self, fig_width=None, fig_height=None, columns=1):
        """Set up matplotlib's RC params for LaTeX plotting.
        Call this before plotting a figure.

        Parameters
        ----------
        fig_width : float, optional, inches
        fig_height : float,  optional, inches
        columns : {1, 2}
        """

        # code adapted from http://www.scipy.org/Cookbook/Matplotlib/LaTeX_Examples

        # Width and max height in inches for IEEE journals taken from
        # computer.org/cms/Computer.org/Journal%20templates/transactions_art_guide.pdf

        assert(columns in [1,2])

        if fig_width is None:
            fig_width = 3.39 if columns == 1 else 6.9 # width in inches

        if fig_height is None:
            golden_mean = (np.sqrt(5) - 1.0) / 2.0    # Aesthetic ratio
            fig_height = int(fig_width*golden_mean) # height in inches

        MAX_HEIGHT_INCHES = 8.0
        if fig_height > MAX_HEIGHT_INCHES:
            print("WARNING: fig_height too large:" + str(fig_height) +
                  "so will reduce to" + str(MAX_HEIGHT_INCHES) + "inches.")
            fig_height = MAX_HEIGHT_INCHES

        params = {'backend': 'ps',
                  'text.latex.preamble': [r'\usepackage{gensymb}'],
                  'axes.labelsize': 2,
                  'font.size': 14,
                  'font.weight': "bold",
                  'legend.fontsize': 6,
                  'xtick.labelsize': 6,
                  'ytick.labelsize': 6,
                  'text.usetex': True,
                  'figure.figsize': [fig_width,fig_height],
                  'figure.titleweight': "bold",
                  'font.family': 'serif'
        }

        # We change the fontsize of minor ticks label
        pp.tick_params(axis='both', which='major', labelsize=12, pad=-2)

        pp.rcParams.update(params)
        pp.rc('font', family="serif", serif="palatino")


    def format_axes(self, ax):

        for spine in ['top', 'right']:
            ax.spines[spine].set_visible(False)

        for spine in ['left', 'bottom']:
            ax.spines[spine].set_color(SPINE_COLOR)
            ax.spines[spine].set_linewidth(0.5)

        ax.xaxis.set_ticks_position('bottom')
        ax.yaxis.set_ticks_position('left')
        ax.grid(which="both")

        for axis in [ax.xaxis, ax.yaxis]:
            axis.set_tick_params(direction='out', color=SPINE_COLOR)

        return ax

    def plot_evaluations(self, series, means, confidences,
                         label):
        pp.errorbar(series, means, yerr=confidences, label=label)

    def save(self, name):
        # pp.legend(loc="lower right")
        self.ax = self.format_axes(self.ax)
        pp.tight_layout()
        pdf = PdfPages("../report/" + str(name) + '.pdf')

        pdf.savefig()

        pdf.close()


if __name__ == "__main__":
    main()
