Automello
=========

Originally form the Feb, 2012 Music Hack Day in San Francisco

Forked and taken forward Oct 2019, jowolf & aaronw0lff

Files
=====

- setup.sh - installs python venv & dependencies
- requirements.txt - python dependencies
- sampler - turns a directory of sound files into a midi sample player
- get_samples - converts one directory of files into a playable one, recognizing pitch, etc
- monophonic_tonality_sorter.py - support functions for get_samples
- utility.py - support functions

Dependencies
============

- pyo - Python DSP server - https://github.com/belangeo/pyo http://ajaxsoundstudio.com/software/pyo/
- numpy - numerical / math / matrix / array library - https://numpy.org/
- pyechonest - Python client for Echo Nest audio DSP library - https://github.com/echonest
- wxPython (Not currently installed by setup) - https://www.wxpython.org/pages/overview/

Notes
=====

- Currently python2
- Plugins require C/C++ compiler(s)

Todo
====

- python3 update
- get wxPython gui working
- enhancements for DSP options for conversions / recognition
- compiling and instructions for plugins, Roli Juce https://juce.com/
- update docs and this README accordingly
- update icon for Gimp

JJW & AEW Oct 2019
