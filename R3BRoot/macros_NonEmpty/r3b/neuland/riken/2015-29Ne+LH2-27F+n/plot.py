#!/usr/bin/env python
import sys
sys.path.append('../../plotconf')
from plotconfroot import *


def convert_th1d_to_np(hist):
    nbins = hist.GetNbinsX()
    xs = np.fromiter((hist.GetBinLowEdge(i + 1) for i in range(nbins + 1)), float)
    ys = np.fromiter((hist.GetBinContent(i + 1) for i in range(nbins + 1)), float)
    return xs, ys


def rebin(x, bin):
    return np.array([a.mean() for a in np.array_split(x, bin)])


def plot_energy(expfile, simfile, ax, col):
    exphist = rootpy.ROOT.TH1D(expfile.hneutidq_exp.ProjectionY('pye', 1, 400))
    simhist = simfile.NeulandHitMon.hE
    simhist.Scale(exphist.Integral(20, 30) / simhist.Integral(20, 30))

    ax.set_xlabel(r'Energy $E$ [MeV]', horizontalalignment='right', x=1)
    ax.set_ylabel(r'Counts', horizontalalignment='right', y=1)
    ax.set_xlim(0, 125)
    ax.set_ylim(0, 8000)
    ax.text(0.05, 0.95, r'a) Hit Energy', horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)

    rplt.step(exphist, axes=ax, color=col[0])
    rplt.step(simhist, axes=ax, color=col[1])


def plot_time(expfile, simfile, ax, col):
    exphist = rootpy.ROOT.TH1D(expfile.hneutidt_exp.ProjectionY('pyt', 1, 400))
    simhist = simfile.NeulandHitMon.hTime
    simhist.Scale(exphist.Integral(0, 1000) / simhist.Integral(0, 1000) * 0.93)

    ax.set_xlabel(r'Time $t$ [ns]', horizontalalignment='right', x=1)
    ax.set_xlim(50, 100)
    ax.set_ylim(0, 17000)
    ax.text(0.05, 0.95, r'b) Hit Time', horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)

    rplt.step(exphist, axes=ax, color=col[0], label='Experiment')
    rplt.step(simhist, axes=ax, color=col[1], label='Simulation')


def plot_x(expfile, simfile, ax, col):
    exphist = rootpy.ROOT.TH1D(expfile.hneutidx_exp.ProjectionY('pyx', 1, 400))
    ex, ey = convert_th1d_to_np(exphist)
    simhist = simfile.NeulandHitMon.hX
    sx, sy = convert_th1d_to_np(simhist)

    ex /= 10
    ex += 7.5

    sx = rebin(sx, 80)
    sy = rebin(sy, 80)
    sy *= np.sum(ey) / np.sum(sy) * 0.95

    ax.set_xlabel(r'Position $x$ [cm]', horizontalalignment='right', x=1)
    ax.set_xlim(-125, 125)
    ax.set_ylim(0, 8000)
    ax.text(0.05, 0.95, r'c) Hit Horizontal Position', horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)

    ax.step(ex, ey, color=col[0])
    ax.step(sx, sy, color=col[1])


def plot_y(expfile, simfile, ax, col):
    exphist = rootpy.ROOT.TH1D(expfile.hneutidy_exp.ProjectionY('pyy', 1, 400))
    ex, ey = convert_th1d_to_np(exphist)
    simhist = simfile.NeulandHitMon.hY
    sx, sy = convert_th1d_to_np(simhist)

    ex /= 10
    ex += 5

    sx = rebin(sx, 80)
    sy = rebin(sy, 80)
    sy *= np.sum(ey) / np.sum(sy)

    ax.set_xlabel(r'Position $y$ [cm]', horizontalalignment='right', x=1)
    ax.set_xlim(-125, 125)
    ax.set_ylim(0, 10000)
    ax.text(0.05, 0.95, r'd) Hit Vertical Position', horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)

    ax.step(ex, ey, color=col[0])
    ax.step(sx, sy, color=col[1])


exp = rootpy.io.root_open('hist_29ne_27fn.20180313.root', 'read')
sim = rootpy.io.root_open('output/digi.root', 'read')
col = palettable.colorbrewer.qualitative.Set1_3.mpl_colors

fig = plt.figure(figsize=(6.224, 6.224))
plot_y(exp, sim, fig.add_subplot(224), col)
plot_x(exp, sim, fig.add_subplot(223), col)
plot_energy(exp, sim, fig.add_subplot(221), col)
plot_time(exp, sim, fig.add_subplot(222), col)
plt.legend(bbox_to_anchor=(1, .85))
fig.tight_layout(pad=0, w_pad=0.1, h_pad=0.1)
plt.savefig('plot.pdf', bbox_inches='tight')
