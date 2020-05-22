import matplotlib as mpl
mpl.use("pgf")

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
import matplotlib.pyplot as plt

import palettable
import numpy as np
import re
from collections import defaultdict

import ROOT
import rootpy.io
from rootpy.plotting import Canvas, Graph, Hist
from rootpy.plotting.style import get_style, set_style
import rootpy.plotting.root2matplotlib as rplt
