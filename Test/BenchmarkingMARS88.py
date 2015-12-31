# BenchmarkingMARS88 - Test cases for Corewar.Benchmarking.MARS_88
#
# Copyright (C) 2006 Jens Gutzeit <jens@jgutzeit.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import unittest
import Corewar
import Corewar.Benchmarking

class TestCase(unittest.TestCase):
    """Test cases for Corewar.Benchmarking.MARS_88."""

    def setUp(self):
        """Set up test case."""

        self.parser = Corewar.Parser(standard=Corewar.STANDARD_88)

    def test_default_values(self):
        """Default values of the Corewar.Benchmarking.MARS_88."""

        mars88 = Corewar.Benchmarking.MARS_88()
        self.assertEqual(mars88.coresize, 8000)
        self.assertEqual(mars88.maxprocesses, 8000)
        self.assertEqual(mars88.maxcycles, 80000)
        self.assertEqual(mars88.mindistance, 100)
        self.assertEqual(mars88.maxlength, 100)

    def test_values(self):
        """Setting values for Corwar.Benchmarking.MARS_88."""

        mars88 = Corewar.Benchmarking.MARS_88(coresize=80, maxprocesses=81,\
                                             maxcycles=800, mindistance=6,\
                                             maxlength=5)
        self.assertEqual(mars88.coresize, 80)
        self.assertEqual(mars88.maxprocesses, 81)
        self.assertEqual(mars88.maxcycles, 800)
        self.assertEqual(mars88.mindistance, 6)
        self.assertEqual(mars88.maxlength, 5)

    def test_invalid_insn(self):
        """Execution of invalid instruction in Corewar.Benchmarking.MARS_88"""

        parser = Corewar.Parser(standard=Corewar.STANDARD_94_NOP)
        mars88 = Corewar.Benchmarking.MARS_88()

        try:
            w1 = parser.parse_file('Test/warriors/invalid_insn_88.red')
        except:
            self.fail('Could not parse warrior.')

        try:
            result = mars88.run((w1,w1), rounds=1)
        except ValueError, e:
            pass
        else:
            self.fail()

    def test_fight_01(self):
        """Freight Train v0.2 vs. The Next Step '88"""

        parser = Corewar.Parser(standard=Corewar.STANDARD_88)
        mars88 = Corewar.Benchmarking.MARS_88()

        try:
            w1 = parser.parse_file('Test/warriors/freighttrainv02.red')
            w2 = parser.parse_file('Test/warriors/nextstep88.red')
        except:
            self.fail('Could not parse warriors.')

        result = mars88.run((w1, w2), 1000, 1234567)
        self.assertEqual(result, [[195, 262, 543], [262, 195, 543], 107775203])
