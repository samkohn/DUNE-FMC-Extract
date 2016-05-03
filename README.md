DUNE Fast Monte Carlo Data Extractor
==========

This repository contains ROOT scripts (macros) for extracting all
sorts of useful data from the Fast Monte Carlo. In particular,
these scripts will create input files for my [DUNE FD spectrum
calculator](https://github.com/samkohn/DUNE-configs).

Required Inputs
--------

These scripts take as input Fast Monte Carlo output
files, GENIE splines, Fast Monte Carlo flux files
(provided by the flux group), and the Prob3++ library
`.so` file. For examples of where to find them, check out
[Configuration.C](https://github.com/samkohn/DUNE-FMC-Extract/blob/master/Configuration.C).

How to use
--------

The main scripts are:

 - ExtractBeamFlux.C
 - CreateOscillationVectors.C
 - ExtractCrossSection.C
 - ExtractDetectorResponseMatrix.C
 - ExtractEfficiency.C

To run all of these scripts, use the Extract.C file, which is run with
a command like

```
$ root -b -l -q "Extract.C(120, 0, 10)"
```

This command will run over all of the standard parametrizations of each
script (e.g. neutrino and antineutrino mode), dumping the data in 120
bins between 0 and 10 GeV.

Each script can also be run independently. Most of them (except for
CreateOscillationVectors.C) can be run directly from the command line
with a command like

```
$ root -b -l -q "ExtractBeamFlux.C(120, 0, 10, true)"
```

which will extract the beam flux in 120 bins from 0 to 10 GeV for
neutrino mode (`false` -> antineutrino mode).

The CreateOscillationVectors.C file relies on an oscillation probability
calculator from the Prob3++ library. The Prob3++ library `.so` file
must be available. On the FNAL cluster DUNE section, it is called
`libThreeProb_2.10.so` and its location can be found in the Extract.C
file's code. Consequently, before this script is executed, the required
library must be loaded and the script must be compiled in the following
way inside ROOT:

```
[] .L /path/to/lib3++.so
[] .L CreateOscillationVectors.C+
```

Note the trailing `+` character, which tells ROOT to compile that code
using the previously loaded library.
