import matplotlib as mpl
from matplotlib.figure import Figure
from matplotlib.backends.backend_pgf import FigureCanvas

import palettable
import numpy as np
import re
from collections import defaultdict

# https://matplotlib.org/users/customizing.html
mpl.rcParams.update({
    "pgf.texsystem": "lualatex",
    "pgf.preamble": [
        r"\usepackage{fontspec, unicode-math, isotope}",
        r"\setmainfont{Libertinus Serif}",
        r"\setsansfont{Libertinus Sans}",
        r"\setmonofont[Scale=MatchLowercase]{Source Code Pro}",
        r"\setmathfont{Libertinus Math}",
    ],
    "text.usetex": True,
    "font.family": "serif",
    "font.serif": "Libertinus Serif",
    "font.sans-serif": "Libertinus Sans",
    "font.monospace": "Source Code Pro",
    "axes.spines.left": True,
    "axes.spines.bottom": True,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "legend.fancybox": False,
    "legend.edgecolor": "#FFFFFF",
})

fig = Figure(figsize=(6.224, (6.224 / 1.618 * 0.75)))
FigureCanvas(fig)
