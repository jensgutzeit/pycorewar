# PyCorewar

PyCorewar is a fast MARS with an easy to use Python interface. It is
released under The GNU General Public License, Version 2 (see the file
COPYING for details).

At the moment the following features are supported:

- ICWS '88 (with multiwarrior support)
- ICWS '94 draft (without P-Space)
- most features of pMARS' parser

# Requirements

You need Python 2 (!) and a recent version of GCC for compiling PyCorewar.

# How to build

  `python2 setup.py build`

# How to install

  `python2 setup.py install`

# Bug reports

Please send any bug reports to [jens@jgutzeit.de](mailto:jens@jgutzeit.de).

# Thanks

Writing PyCorewar would have been almost impossible without the help of the
Core War Community. Special thanks go to

  Sascha Zapf for endlessly running benchmarks and testsuites and providing
  lots of ideas.

  Albert Ma, Nandor Sieben, Stefan Strack and Mintardjo Wangsaw for writing
  pMARS

  M Joonas Pihlaja for writing exhaust, which made it very easy to understand
  the inner workings of a MARS

  Martin Ankerl for writing exhaust-ma, where I have found a lot of ideas
  for optimizing PyCorewar.

  Michal Janeczek for writing fmars, where I have found even more ideas for
  optimizing PyCorewar.

  Paul V-Khoung for several nice chats about optimizing PyCorewar
