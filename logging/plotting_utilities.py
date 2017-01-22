import os

import numpy as np
from matplotlib import pyplot as pp
from matplotlib.backends.backend_pdf import PdfPages

SPINE_COLOR = 'gray'


class Plot:
    def __init__(self, title, for_print: bool = True):
        self.for_print = for_print
        self.ax = pp.axes()
        pp.gcf().patch.set_alpha(0.5)

        self.ax.set_xlabel('Iterations', fontsize=14)
        self.ax.set_ylabel('Reward', fontsize=14)

        self.markers = {"o", "D", "^", "8", "h", "s"}
        title_string = title
        if self.for_print:
            self.latexify()
            title_string = r"\textbf{%s}" % title

        self.ax.set_title(title_string, fontsize=18, fontweight='bold', y=1.05)
        self.ax = self.format_axes(self.ax)

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

        assert (columns in [1, 2])

        if fig_width is None:
            fig_width = 3.39 if columns == 1 else 6.9  # width in inches

        if fig_height is None:
            golden_mean = (np.sqrt(5) - 1.0) / 2.0  # Aesthetic ratio
            fig_height = int(fig_width * golden_mean)  # height in inches

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
                  'figure.figsize': [fig_width, fig_height],
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
            ax.spines[spine].set_visible(False)

        ax.xaxis.set_ticks_position('bottom')
        ax.yaxis.set_ticks_position('left')
        ax.grid(which="both", color="white", linestyle="-", linewidth=1)
        ax.set_axisbelow(True)
        ax.set_axis_bgcolor("#EEEEEE")

        for axis in [ax.xaxis, ax.yaxis]:
            axis.set_tick_params(direction='out', color="white")

        return ax

    def plot_evaluations(self, series, means, confidences,
                         label):
        pp.errorbar(series, means, label=label)

    def save(self, name: str, path: str):
        # pp.legend(loc="lower right")

        pp.tight_layout()
        full_out = os.path.join(path, name + ".pdf")
        pdf = PdfPages(full_out)

        pdf.savefig()

        pdf.close()