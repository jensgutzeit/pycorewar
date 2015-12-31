# Instruction88.py - Test cases for Corewar.Redcode.Instruction88
#
# Copyright (C) 2005, 2006 Jens Gutzeit <jens@jgutzeit.de>
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
import random
import cStringIO
import pickle
from Corewar.Redcode import *

class TestCase(unittest.TestCase):
    """Test cases for Corewar.Redcode.Instruction88"""

    def test_default_values(self):
        """Default values of Corewar.Redcode.Instruction88"""

        insn = Instruction88()
        
        self.assertEqual(insn.opcode, OPCODE_DAT)
        self.assertEqual(insn.amode, MODE_DIRECT)
        self.assertEqual(insn.afield, 0)
        self.assertEqual(insn.bmode, MODE_DIRECT)
        self.assertEqual(insn.bfield, 0)
        self.assertEqual(str(insn), 'DAT    $     0, $     0')

    def test_reinitialization(self):
        """Corewar.Redcode.Instruction88/reinit()"""

        insn = Instruction88()
        insn.opcode = OPCODE_SPL
        insn.reinit()

        self.assertEqual(insn.opcode, OPCODE_DAT)
        self.assertEqual(insn.amode, MODE_DIRECT)
        self.assertEqual(insn.afield, 0)
        self.assertEqual(insn.bmode, MODE_DIRECT)
        self.assertEqual(insn.bfield, 0)

    def test_reduction_module_coresize(self):
        """Corewar.Redcode.Instruction88: Reduction of values mod CORESIZE"""

        insn = Instruction88(coresize=57013)
        insn.afield = 12345678
        insn.bfield = 73737373

        self.assertEqual(insn.afield, 12345678 % 57013)
        self.assertEqual(insn.bfield, 73737373 % 57013)

    def test_values(self):
        """Getting and setting values for Corewar.Redcode.Instruction88"""

        insn = Instruction88()
        random.seed(13821503)

        for opcode in (OPCODE_DAT, OPCODE_MOV, OPCODE_ADD, OPCODE_SUB,\
                       OPCODE_JMP, OPCODE_JMZ, OPCODE_JMN, OPCODE_DJN,\
                       OPCODE_CMP, OPCODE_SPL, OPCODE_SLT):
            for amode in (MODE_DIRECT, MODE_IMMEDIATE, MODE_INDIRECT,\
                          MODE_PREDECREMENT):
                for bmode in (MODE_DIRECT, MODE_IMMEDIATE, MODE_INDIRECT,\
                              MODE_PREDECREMENT):
                    afield = random.randint(0, 7999)
                    bfield = random.randint(0, 7999)

                    insn.opcode = opcode
                    insn.amode = amode
                    insn.afield = afield
                    insn.bmode = bmode
                    insn.bfield = bfield

                    self.assertEqual(insn.opcode, opcode)
                    self.assertEqual(insn.amode, amode)
                    self.assertEqual(insn.afield, afield)
                    self.assertEqual(insn.bmode, bmode)
                    self.assertEqual(insn.bfield, bfield)
                    
    def test_comparison(self):
        """Comparison of Corewar.Redcode.Instruction88"""

        i1 = Instruction88()
        i2 = Instruction88()

        self.assertEqual(i1 == i2, True)
        i2.afield = 1
        self.assertEqual(i1 == i2, False)
        self.assertEqual(i1 != i2, True)

    def test_str(self):
        """String representation of Corewar.Redcode.Instruction88"""

        insn = Instruction88()
        insn.opcode = OPCODE_ADD
        insn.afield = 1234
        insn.bfield = 5678
        self.assertEqual(str(insn), "ADD    $  1234, $ -2322")

        insn = Instruction88(1000000)
        insn.opcode = OPCODE_ADD
        insn.afield = 123457
        insn.bfield = 890123
        self.assertEqual(str(insn), "ADD    $     123457, $    -10987")

        insn = Instruction88(1000000000)
        insn.opcode = OPCODE_ADD
        insn.afield = 123456789
        insn.bfield = 987654321
        self.assertEqual(str(insn), "ADD    $  123456789, $  -1234567")

    def test_pickle(self):
        """(Un-)Pickling of Corewar.Redcode.Instruction88"""

        f = cStringIO.StringIO()

        insn = Instruction88(12345)
        insn.opcode = OPCODE_SUB
        insn.amode = MODE_INDIRECT
        insn.afield = 11001
        insn.bmode = MODE_PREDECREMENT
        insn.bfield = 103

        try:
            pickle.dump(insn, f)
            f.seek(0)
            pickled_insn = pickle.load(f)
            self.assertEqual(insn, pickled_insn)
        except Exception, e:
            self.fail(str(e))

    def test_bugfix_0001(self):
        """Bugfix for setting modes of Corewar.Redcode.Instruction88"""

        insn = Instruction88()
        insn.opcode = OPCODE_MOV
        insn.amode = MODE_DIRECT
        insn.afield = 0
        insn.bmode = MODE_DIRECT
        insn.bfield = 1
        self.assertEqual(str(insn), "MOV    $     0, $     1")
