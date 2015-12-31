# Redcode.py - Test cases for functions in Corewar.Redcode
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
from Corewar.Redcode import *

class TestCase(unittest.TestCase):
    """Test cases for functions in Corewar.Redcode"""

    def test_fromString(self):
        """Creating instructions from strings"""

        MODES = (MODE_DIRECT, MODE_IMMEDIATE, MODE_A_INDIRECT,
                 MODE_A_PREDECREMENT, MODE_A_POSTINCREMENT, MODE_B_INDIRECT,
                 MODE_B_PREDECREMENT, MODE_B_POSTINCREMENT)

        insn = Instruction()
        for opcode in (OPCODE_DAT, OPCODE_MOV, OPCODE_ADD, OPCODE_SUB,
                       OPCODE_JMP, OPCODE_JMZ, OPCODE_JMN, OPCODE_DJN,
                       OPCODE_SPL, OPCODE_SLT, OPCODE_MUL, OPCODE_SEQ,
                       OPCODE_SNE, OPCODE_NOP, OPCODE_STP, OPCODE_LDP):
            for modifier in (MODIFIER_A, MODIFIER_B, MODIFIER_F, MODIFIER_I,
                             MODIFIER_X, MODIFIER_AB, MODIFIER_BA):
                for amode in MODES:
                    for bmode in MODES:
                        insn.opcode = opcode
                        insn.modifier = modifier
                        insn.amode = amode
                        insn.afield = random.randint(0, 7999)
                        insn.bmode = bmode
                        insn.bfield = random.randint(0, 7999)

                        s = str(insn)
                        insn2 = fromString(s)
                        self.assertEqual(insn, insn2)
