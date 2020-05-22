#!/usr/bin/env python
import sys
sys.path.append('../../plotconf')
from plotconf import *

import csv

colors = palettable.colorbrewer.qualitative.Set1_9.mpl_colors

simdata = defaultdict(lambda: defaultdict(list))
with open('sim.dat') as file:
    reader = csv.reader(file, delimiter=' ')
    for pl, en, th, ef in reader:
        simdata[pl][int(en)].append([float(th), float(ef)])


pls = [
    {
        'name': 'QGSP_BERT_HP',
        'label': r'\textsc{Geant4} QGSP_BERT_HP',
        'color': colors[0],
    },
    {
        'name': 'QGSP_BIC_HP',
        'label': r'\textsc{Geant4} QGSP_BIC_HP',
        'color': colors[1],
    },
    {
        'name': 'QGSP_INCLXX_HP',
        'label': r'\textsc{Geant4} QGSP_INCLXX_HP',
        'color': colors[2],
    },
   {
       'name': 'Geant3',
       'label': r'\textsc{Geant3} GCALOR',
       'color': colors[3],
   },
]


def plot_eff(energy, ax):
    expdata = np.loadtxt('eff_exp_%d.dat' % energy)
    kwargs = {'fmt': '+', 'markersize': 3, 'linewidth': 1, 'capsize': 2}
    ax.errorbar(expdata[:, 0], expdata[:, 1], yerr=expdata[:, 2], **kwargs, color='black', label='Experiment')

    ax.text(1.2, 3, r'$E_n =$ %d\,MeV' % energy)

    for pl in pls:
        data = np.array(sorted(simdata[pl['name']][energy], key=lambda x: x[0]))
        ax.plot(data[:, 0], data[:, 1]*100, label=pl['label'], color=pl['color'], linewidth=1)


fig = Figure(figsize=(6.224, (6.224 / 1.618)))
FigureCanvas(fig)
ax1 = fig.add_subplot(211)
ax1.set_ylim(0, 39)
ax1.set_ylabel(r"Efficiency [\%]", horizontalalignment='right', y=1)
plot_eff(110, ax1)
ax1.tick_params(labelbottom=False)
ax1.legend(prop={'size': 'x-small'})
ax2 = fig.add_subplot(212, sharex=ax1)
ax2.set_ylim(0, 35)
ax2.set_xlabel(r"Threshold [MeV]", horizontalalignment='right', x=1)
plot_eff(250, ax2)

fig.savefig('plot.pdf', bbox_inches='tight')
