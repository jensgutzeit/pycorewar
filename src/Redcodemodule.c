/* Redcodemodule.c - Classes for representing Redcode instructions
 *
 * Copyright (C) 2005, 2006 Jens Gutzeit <jens@jgutzeit.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <Python.h>
#include "structmember.h"
#include <MyTypes.h>
#include <Instruction.h>
#include <string.h>

/*
 * ATTENTION! Be sure to use the same order as in Instruction.h.
 */

static char *INSTRUCTIONS88[] = {"DAT", "MOV", "ADD", "SUB", "JMP", "JMZ",
				"JMN", "DJN", "CMP", "SPL", "SLT"};
static char *MODES88 = "$#@<";

static char *INSTRUCTIONS[] = {"DAT", "MOV", "ADD", "SUB", "JMP", "JMZ",
			       "JMN", "DJN", "SEQ", "SPL", "SLT", "MUL",
			       "DIV", "MOD", "SNE", "NOP", "LDP", "STP"};
static char *MODIFIERS[] = {"F ", "A ", "B ", "AB", "BA", "X ", "I "};
static char *MODES = "$#@<>*{}";

typedef struct {
	PyObject_HEAD
	u32_t coresize; /* size of core */
	insn_t insn; /* the instruction itself */
} Instruction88;

typedef struct {
	PyObject_HEAD
	u32_t coresize; /* size of core */
	insn_t insn; /* the instruction itself */
} Instruction;

static PyTypeObject Instruction88Type;
static PyTypeObject InstructionType;

/*
 * class Instruction88
 */

PyDoc_STRVAR(Instruction88__doc__,
"Redcode instruction according to ICWS '88\n\n"\
"Methods:\n\n"\
"  copy() -- create a copy (clone) of the instruction\n"\
"  reinit() -- reset to default, i.e. DAT $ 0, $ 0\n\n"\
"Attributes:\n\n"\
"  opcode -- opcode of instruction\n"\
"  amode -- A-mode\n"\
"  afield -- A-field (value is always reduced mod CORESIZE)\n"\
"  bmode -- B-mode\n"\
"  bfield -- B-field (value is always reduced mod CORESIZE)");

static PyObject *
Instruction88_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Instruction88 *self;
	s32_t coresize = 8000;
	PyObject *state;

	/* Check for invocation from pickle with __getstate__ state. */
	if (PyTuple_GET_SIZE(args) == 4) {
		s32_t coresize;
		s32_t insn;
		s32_t a;
		s32_t b;

		/* Get and check coresize. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 0))) {
			coresize = PyInt_AsLong(state);
			if ((coresize < 1) || (coresize > MAX_CORESIZE)) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"coresize state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError, "Bad coresize state");
			return NULL;
		}

		/* Get and check opcode/A-mode/B-mode. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 1))) {
			insn = PyInt_AsLong(state);
			if ((insn < 0) ||
			    (insn > INSN88(0x0a, 0x03, 0x03))) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"instruction state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError,
					"Bad instruction state");
			return NULL;
		}

		/* Get and check A-field. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 2))) {
			a = PyInt_AsLong(state);
			if ((a < 0) || (a >= coresize)) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"A-field state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError, "Bad A-field state");
			return NULL;
		}

		/* Get and check B-field. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 3))) {
			b = PyInt_AsLong(state);
			if ((b < 0) || (b >= coresize)) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"B-field state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError, "Bad B-field state");
			return NULL;
		}

		/* Build instruction. */
		self = (Instruction88 *) type->tp_alloc(type, 0);
		if (self != NULL) {
			self->coresize = coresize;
			self->insn.insn = insn;
			self->insn.a = a;
			self->insn.b = b;
		}

		return (PyObject *) self;
	}

	/* Parse arguments. */
	static char *kwlist[] = {"coresize", NULL};
	PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &coresize);

	if ((coresize < 1) || (coresize > MAX_CORESIZE)) {
		PyErr_SetString(PyExc_ValueError, "Invalid coresize");
		return NULL;
	}

	self = (Instruction88 *) type->tp_alloc(type, 0);
	if (self != NULL) {
		self->coresize = coresize;
		self->insn.insn = INSN88(DAT, DIRECT, DIRECT);
		self->insn.a = 0;
		self->insn.b = 0;
	}

	return (PyObject *) self;
}

static void 
Instruction88_dealloc(Instruction88 *self)
{
	self->ob_type->tp_free((PyObject*) self);
}

PyDoc_STRVAR(opcode88__doc__,
"Opcode of the instruction");

static PyObject *
Instruction88_GetOpcode(Instruction88 *self, void *closure)
{
	PyObject *opcode = PyInt_FromLong(OPCODE88(self->insn.insn));

	if (opcode == NULL) {
		return NULL;
	}
	/* Py_INCREF(opcode); */

	return opcode;
}

static int
Instruction88_SetOpcode(Instruction88 *self, PyObject *value, void *closure)
{
	s32_t opcode;

	/* Plausibility check. */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Invalid opcode");
		return -1;
	}

	/* Get and check value. */
	opcode = PyLong_AsLong(value);
	if ((opcode < 0) || (opcode > 0x0a)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid opcode according to ICWS '88 rules");
		return -1;
	}

	self->insn.insn = SETOPCODE88(self->insn.insn, opcode);

	return 0;
}

PyDoc_STRVAR(amode88__doc__,
"A-mode of instruction");

static PyObject *
Instruction88_GetAMode(Instruction88 *self, void *closure)
{
	PyObject *amode = PyInt_FromLong(AMODE88(self->insn.insn));

	if (amode == NULL) {
		return NULL;
	}
	/* Py_INCREF(amode); */

	return amode;
}

static int
Instruction88_SetAMode(Instruction88 *self, PyObject *value, void *closure)
{
	s32_t amode;

	/* Plausibility check. */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Invalid A-mode");
		return -1;
	}

	/* Get and check value. */
	amode = PyLong_AsLong(value);
	if ((amode < 0x00) || (amode > 0x03)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid A-mode according to ICWS '88 rules");
		return -1;
	}

	self->insn.insn = SETAMODE88(self->insn.insn, amode);

	return 0;
}

PyDoc_STRVAR(bmode88__doc__,
"B-mode of instruction");

static PyObject *
Instruction88_GetBMode(Instruction88 *self, void *closure)
{
	PyObject *bmode = PyInt_FromLong(BMODE88(self->insn.insn));

	if (bmode == NULL) {
		return NULL;
	}
	/* Py_INCREF(bmode); */

	return bmode;
}

static int
Instruction88_SetBMode(Instruction88 *self, PyObject *value, void *closure)
{
	s32_t bmode;

	/* Plausibility check. */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Invalid B-mode");
		return -1;
	}

	/* Get and check value. */
	bmode = PyLong_AsLong(value);
	if ((bmode < 0x00) || (bmode > 0x03)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid B-mode according to ICWS '88 rules");
		return -1;
	}

	self->insn.insn = SETBMODE88(self->insn.insn, bmode);

	return 0;
}

PyDoc_STRVAR(afield88__doc__,
"A-field of instruction\n\n"\
"Values are reduced module CORESIZE!");

static PyObject *
Instruction88_GetAField(Instruction88 *self, void *closure)
{
	PyObject *afield = PyInt_FromLong(self->insn.a);

	if (afield == NULL) {
		return NULL;
	}
	/* Py_INCREF(afield); */

	return afield;
}

static int
Instruction88_SetAField(Instruction88 *self, PyObject *value, void *closure)
{
	s32_t tmp;

	/* Plausibility check. */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "A-field must be an integer");
		return -1;
	}

	/* Get value and reduce module CORESIZE. */
	tmp = PyInt_AsLong(value);
	if (tmp < 0) {
		tmp = self->coresize - (abs(tmp) % self->coresize);
	}
	self->insn.a = tmp % self->coresize;

	return 0;
}

PyDoc_STRVAR(bfield88__doc__,
"B-field of instruction\n\n"\
"Values are reduced module CORESIZE!");

static PyObject *
Instruction88_GetBField(Instruction88 *self, void *closure)
{
	PyObject *bfield = PyInt_FromLong(self->insn.b);

	if (bfield == NULL) {
		return NULL;
	}
	/* Py_INCREF(bfield); */

	return bfield;
}

static int
Instruction88_SetBField(Instruction88 *self, PyObject *value, void *closure)
{
	s32_t tmp;

	/* Plausibility check. */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "B-field must be an integer");
		return -1;
	}

	/* Get value and reduce modulo CORESIZE. */
	tmp = PyInt_AsLong(value);
	if (tmp < 0) {
		tmp = self->coresize - (abs(tmp) % self->coresize);
	}
	self->insn.b = tmp % self->coresize;

	return 0;
}

static PyObject *
Instruction88_richcompare(Instruction88 *obj1, Instruction88 *obj2, int op)
{
	u32_t isEqual;
	PyObject *result;

	isEqual = obj1->insn.insn == obj2->insn.insn &&
		  obj1->insn.a == obj2->insn.a &&
		  obj1->insn.b == obj2->insn.b;

	if (op == Py_EQ) {
		result = isEqual ? Py_True : Py_False;
		Py_INCREF(result);
		return result;
	}

	if (op == Py_NE) {
		result = isEqual ? Py_False : Py_True;
		Py_INCREF(result);
		return result;
	}

	/* Exception on all other comparisions. */
	PyErr_SetString(PyExc_NotImplementedError,
			"Can only test, whether an instruction is equal "\
			"to another or not.");

	return NULL;
}

static PyObject *
Instruction88_repr(Instruction88 *self)
{
	PyObject *result;
	char buffer[33];
	int afield;
	int bfield;

	afield = self->insn.a;
	if (afield > self->coresize/2) {
		afield -= self->coresize;
	}

	bfield = self->insn.b;
	if (bfield > self->coresize/2) {
		bfield -= self->coresize;
	}

	if (self->coresize < 2*100000) {
		snprintf(buffer, 33, "%s    %c%6d, %c%6d", 
			 INSTRUCTIONS88[OPCODE88(self->insn.insn)],
			 MODES88[AMODE88(self->insn.insn)], afield,
			 MODES88[BMODE88(self->insn.insn)], bfield);
	} else {
		snprintf(buffer, 33, "%s    %c%11d, %c%11d", 
			 INSTRUCTIONS88[OPCODE88(self->insn.insn)],
			 MODES88[AMODE88(self->insn.insn)], afield,
			 MODES88[BMODE88(self->insn.insn)], bfield);
	}

	/* Build result. */
	result = PyString_FromString(buffer);

	return result;
}

PyDoc_STRVAR(copy88__doc__,
"copy() -> Instruction88\n\n"\
"Return a copy (clone) of the instruction.");

static PyObject *
Instruction88_copy(Instruction88 *self)
{
	Instruction88 *insn_copy;

	insn_copy = PyObject_New(Instruction88, &Instruction88Type);
	if (insn_copy == NULL) {
		return NULL;
	}

	/* Copy values. */
	insn_copy->coresize = self->coresize;
	insn_copy->insn.insn = self->insn.insn;
	insn_copy->insn.a = self->insn.a;
	insn_copy->insn.b = self->insn.b;

	return (PyObject*) insn_copy;
}

PyDoc_STRVAR(reinit88__doc__,
"reinit() -> None\n\n"\
"Reset instruction to default values (DAT $ 0, $ 0).");

static PyObject *
Instruction88_reinit(Instruction88 *self)
{
	self->insn.insn = INSN88(DAT, DIRECT, DIRECT);
	self->insn.a = 0;
	self->insn.b = 0;

	Py_INCREF(Py_None);
	return Py_None;
}

static int
Instruction88_buffer_getreadbuf(Instruction88 *self, int index,
				const void **ptr)
{
	if (index != 0) {
		PyErr_SetString(PyExc_SystemError,
				"Accessing invalid segment.");
		return -1;
	}
	*ptr = (void *) &(self->insn);

	return sizeof(insn_t);
}

static int
Instruction88_buffer_getsegcount(Instruction88 *self, int *lenp)
{
	/* Store length of insn_t. */
	if (lenp) {
		*lenp = sizeof(insn_t);
	}
	return 1;
}

/* Pickle support. */

static PyObject *
Instruction88_getstate(Instruction88 *self)
{
	return Py_BuildValue("kkkk", self->coresize, self->insn.insn,
			     self->insn.a, self->insn.b);
}

static PyObject *
Instruction88_reduce(Instruction88 *self)
{
	return Py_BuildValue("(ON)", self->ob_type,
			     Instruction88_getstate(self));
}

static PyMemberDef Instruction88_members[] = {
	{NULL} /* sentinel */
};

static PyMethodDef Instruction88_methods[] = {
	{"copy", (PyCFunction)Instruction88_copy, METH_NOARGS, copy88__doc__},
	{"reinit", (PyCFunction)Instruction88_reinit, METH_NOARGS,
	 reinit88__doc__},
	{"__reduce__", (PyCFunction)Instruction88_reduce, METH_NOARGS,
	 PyDoc_STR("__reduce__() -> (cls, state)")},
	{NULL, NULL, 0, NULL} /* sentinel */
};

static PyGetSetDef Instruction88_getseters[] = {
	{"opcode", (getter)Instruction88_GetOpcode,
	 (setter)Instruction88_SetOpcode, opcode88__doc__, NULL},
	{"amode", (getter)Instruction88_GetAMode,
	 (setter)Instruction88_SetAMode, amode88__doc__, NULL},
	{"afield", (getter)Instruction88_GetAField,
	 (setter)Instruction88_SetAField, afield88__doc__, NULL},
	{"bmode", (getter)Instruction88_GetBMode,
	 (setter)Instruction88_SetBMode, bmode88__doc__, NULL},
	{"bfield", (getter)Instruction88_GetBField,
	 (setter)Instruction88_SetBField, bfield88__doc__, NULL},
	{NULL} /* sentinel */
};

static PyBufferProcs Instruction88_as_buffer = {
	(getreadbufferproc) Instruction88_buffer_getreadbuf,
	(getwritebufferproc) NULL, /* No writing to buffer supported. */
	(getsegcountproc) Instruction88_buffer_getsegcount,
	(getcharbufferproc) NULL,
};

static PyTypeObject Instruction88Type = {
	PyObject_HEAD_INIT(NULL) 0, 			/* ob_size */
	"Corewar.Redcode.Instruction88",		/* tp_name */
	sizeof(Instruction88),		 	  	/* tp_basicsize */
	0,                         			/* tp_itemsize */
	(destructor)Instruction88_dealloc,	  	/* tp_dealloc */
	0,						/* tp_print */
	0,						/* tp_getattr */
	0,						/* tp_setattr */
	0,						/* tp_compare */
	(reprfunc)Instruction88_repr,			/* tp_repr */
	0,						/* tp_as_number */
	0,						/* tp_as_sequence */
	0,						/* tp_as_mapping */
	0,						/* tp_hash */
	0,						/* tp_call */
	0,						/* tp_str */
	0,						/* tp_getattro */
	0,						/* tp_setattro */
	&Instruction88_as_buffer,			/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	Instruction88__doc__,				/* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	(richcmpfunc)Instruction88_richcompare,		/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	Instruction88_methods,				/* tp_methods */
	Instruction88_members,				/* tp_members */
	Instruction88_getseters,			/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	Instruction88_new,				/* tp_new */
};

/*
 * class Instruction
 */

PyDoc_STRVAR(Instruction__doc__,
"Redcode instruction according to ICWS '94 draft with pMARS extension\n\n"\
"Methods:\n\n"\
"  copy() -- create a copy (clone) of the instruction\n"\
"  reinit() -- reset to default, i.e. DAT.F $ 0, $ 0\n\n"\
"Attributes:\n\n"\
"  opcode -- opcode of instruction\n"\
"  modifier -- modifer of instruction\n"\
"  amode -- A-mode\n"\
"  afield -- A-field (value is always reduced mod CORESIZE)\n"\
"  bmode -- B-mode\n"\
"  bfield -- B-field (value is always reduced mod CORESIZE)");

static PyObject *
Instruction_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Instruction *self;
	s32_t coresize = 8000;
	PyObject *state;

	/* Check for invocation from pickle with __getstate__ state. */
	if (PyTuple_GET_SIZE(args) == 4) {
		s32_t coresize;
		s32_t insn;
		s32_t a;
		s32_t b;

		/* Get and check coresize. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 0))) {
			coresize = PyInt_AsLong(state);
			if ((coresize < 1) || (coresize > MAX_CORESIZE)) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"coresize state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError, "Bad coresize state");
			return NULL;
		}

		/* Get and check opcode/modifier/A-mode/B-mode. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 1))) {
			insn = PyInt_AsLong(state);
			if ((insn < 0) ||
			    (insn > INSN(0x11, 0x06, 0x07, 0x07))) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"instruction state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError,
					"Bad instruction state");
			return NULL;
		}

		/* Get and check A-field. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 2))) {
			a = PyInt_AsLong(state);
			if ((a < 0) || (a >= coresize)) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"A-field state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError, "Bad A-field state");
			return NULL;
		}

		/* Get and check B-field. */
		if (PyInt_Check(state = PyTuple_GET_ITEM(args, 3))) {
			b = PyInt_AsLong(state);
			if ((b < 0) || (b >= coresize)) {
				PyErr_SetString(PyExc_ValueError, "Bad "\
						"B-field state");
				return NULL;
			}
		} else {
			PyErr_SetString(PyExc_TypeError, "Bad B-field state");
			return NULL;
		}

		/* Build instruction. */
		self = (Instruction *) type->tp_alloc(type, 0);
		if (self != NULL) {
			self->coresize = coresize;
			self->insn.insn = insn;
			self->insn.a = a;
			self->insn.b = b;
		}

		return (PyObject *) self;
	}

	/* Get coresize. */
	static char *kwlist[] = {"coresize", NULL};
	PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &coresize);

	if ((coresize < 1) || (coresize > MAX_CORESIZE)) {
		PyErr_SetString(PyExc_ValueError, "Invalid coresize");
		return NULL;
	}

	self = (Instruction *) type->tp_alloc(type, 0);
	if (self != NULL) {
		self->coresize = coresize;
		self->insn.insn = INSN(DAT, MODIFIER_F, DIRECT, DIRECT);
		self->insn.a = 0;
		self->insn.b = 0;
	}

	return (PyObject *) self;
}

static void 
Instruction_dealloc(Instruction *self)
{
	self->ob_type->tp_free((PyObject*) self);
}

PyDoc_STRVAR(opcode__doc__,
"Opcode of the instruction");

static PyObject *
Instruction_GetOpcode(Instruction *self, void *closure)
{
	PyObject *opcode = PyInt_FromLong(OPCODE(self->insn.insn));

	if (opcode == NULL) {
		return NULL;
	}
	/* Py_INCREF(opcode); */

	return opcode;
}

static int
Instruction_SetOpcode(Instruction *self, PyObject *value, void *closure)
{
	s32_t opcode;

	/* plausibility checks */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Invalid opcode");
		return -1;
	}

	/* get value */
	opcode = PyLong_AsLong(value);

	if ((opcode < DAT) || (opcode > STP)) {
		PyErr_SetString(PyExc_ValueError,
				"Unknown opcode according to ICWS '94 "\
				"draft (with pMARS extension) rules");
		return -1;
	}

	self->insn.insn = SETOPCODE(self->insn.insn, opcode);

	return 0;
}

PyDoc_STRVAR(modifier__doc__,
"Opcode modifier of instruction");

static PyObject *
Instruction_GetModifier(Instruction *self, void *closure)
{
	PyObject *modifier = PyInt_FromLong(MODIFIER(self->insn.insn));

	if (modifier == NULL) {
		return NULL;
	}
	/* Py_INCREF(modifier); */

	return modifier;
}

static int
Instruction_SetModifier(Instruction *self, PyObject *value, void *closure)
{
	s32_t modifier;

	/* plausibility checks */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Invalid modifier");
		return -1;
	}

	/* get value */
	modifier = PyLong_AsLong(value);

	if ((modifier < MODIFIER_F) || (modifier > MODIFIER_I)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid or unknown modifier according "\
				"to ICWS '94 draft rules");
		return -1;
	}

	self->insn.insn = SETMODIFIER(self->insn.insn, modifier);

	return 0;
}

PyDoc_STRVAR(amode__doc__,
"A-mode of instruction");

static PyObject *
Instruction_GetAMode(Instruction88 *self, void *closure)
{
	PyObject *amode = PyInt_FromLong(AMODE(self->insn.insn));

	if (amode == NULL) {
		return NULL;
	}
	/* Py_INCREF(amode); */

	return amode;
}

static int
Instruction_SetAMode(Instruction *self, PyObject *value, void *closure)
{
	s32_t amode;

	/* plausibility checks */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Invalid A-mode");
		return -1;
	}

	/* get value */
	amode = PyLong_AsLong(value);

	if ((amode < DIRECT) || (amode > A_POSTINCREMENT)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid or unknown A-mode according "\
				"to ICWS '94 draft rules");
		return -1;
	}

	self->insn.insn = SETAMODE(self->insn.insn, amode);

	return 0;
}

PyDoc_STRVAR(bmode__doc__,
"B-mode of instruction");

static PyObject *
Instruction_GetBMode(Instruction *self, void *closure)
{
	PyObject *bmode = PyInt_FromLong(BMODE(self->insn.insn));

	if (bmode == NULL) {
		return NULL;
	}
	/* Py_INCREF(bmode); */

	return bmode;
}

static int
Instruction_SetBMode(Instruction *self, PyObject *value, void *closure)
{
	s32_t bmode;

	/* plausibility checks */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Invalid B-mode");
		return -1;
	}

	/* get value */
	bmode = PyLong_AsLong(value);

	if ((bmode < DIRECT) || (bmode > A_POSTINCREMENT)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid or unknown B-mode according "\
				"to ICWS '94 draft rules");
		return -1;
	}

	self->insn.insn = SETBMODE(self->insn.insn, bmode);

	return 0;
}

PyDoc_STRVAR(afield__doc__,
"A-field of instruction\n\n"\
"Values are reduced module CORESIZE!");

static PyObject *
Instruction_GetAField(Instruction *self, void *closure)
{
	PyObject *afield = PyInt_FromLong(self->insn.a);

	if (afield == NULL) {
		return NULL;
	}
	/* Py_INCREF(afield); */

	return afield;
}

static int
Instruction_SetAField(Instruction *self, PyObject *value, void *closure)
{
	s32_t tmp;

	/* plausibility checks */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "A-field must be an integer");
		return -1;
	}

	tmp = PyInt_AsLong(value);
	if (tmp < 0) {
		tmp = self->coresize - (abs(tmp) % self->coresize);
	}
	self->insn.a = tmp % self->coresize;

	return 0;
}

PyDoc_STRVAR(bfield__doc__,
"B-field of instruction\n\n"\
"Values are reduced module CORESIZE!");

static PyObject *
Instruction_GetBField(Instruction *self, void *closure)
{
	PyObject *bfield = PyInt_FromLong(self->insn.b);

	if (bfield == NULL) {
		return NULL;
	}
	/* Py_INCREF(bfield); */

	return bfield;
}

static int
Instruction_SetBField(Instruction *self, PyObject *value, void *closure)
{
	s32_t tmp;

	/* plausibility checks */
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "B-field must be an integer");
		return -1;
	}

	tmp = PyInt_AsLong(value);
	if (tmp < 0) {
		tmp = self->coresize - (abs(tmp) % self->coresize);
	}
	self->insn.b = tmp % self->coresize;

	return 0;
}

static PyObject *
Instruction_richcompare(Instruction *obj1, Instruction *obj2, int op)
{
	u32_t isEqual;
	PyObject *result;

	isEqual = obj1->insn.insn == obj2->insn.insn &&
		  obj1->insn.a == obj2->insn.a &&
		  obj1->insn.b == obj2->insn.b;

	if (op == Py_EQ) {
		result = isEqual ? Py_True : Py_False;
		Py_INCREF(result);
		return result;
	}

	if (op == Py_NE) {
		result = isEqual ? Py_False : Py_True;
		Py_INCREF(result);
		return result;
	}

	PyErr_SetString(PyExc_NotImplementedError,
			"Can only test, whether an instruction is equal "\
			"to another or not.");
	return NULL;
}

static PyObject *
Instruction_repr(Instruction *self)
{
	PyObject *result;
	char buffer[33];
	int afield;
	int bfield;

	afield = self->insn.a;
	if (afield > self->coresize/2) {
		afield -= self->coresize;
	}

	bfield = self->insn.b;
	if (bfield > self->coresize/2) {
		bfield -= self->coresize;
	}

	if (self->coresize < 2*100000) {
		snprintf(buffer, 33, "%s.%s %c%6d, %c%6d", 
			 INSTRUCTIONS[OPCODE(self->insn.insn)],
			 MODIFIERS[MODIFIER(self->insn.insn)],
			 MODES[AMODE(self->insn.insn)], afield,
			 MODES[BMODE(self->insn.insn)], bfield);
	} else {
		snprintf(buffer, 33, "%s.%s %c%11d, %c%11d", 
			 INSTRUCTIONS[OPCODE(self->insn.insn)],
			 MODIFIERS[MODIFIER(self->insn.insn)],
			 MODES[AMODE(self->insn.insn)], afield,
			 MODES[BMODE(self->insn.insn)], bfield);
	}

	/* build result */
	result = PyString_FromString(buffer);

	return result;
}

PyDoc_STRVAR(copy__doc__,
"copy() -> Instruction\n\n"\
"Return a copy (clone) of the instruction.");

static PyObject *
Instruction_copy(Instruction *self)
{
	Instruction *insn_copy;

	insn_copy = PyObject_New(Instruction, &InstructionType);
	if (insn_copy == NULL) {
		return NULL;
	}

	/* copy values */
	insn_copy->coresize = self->coresize;
	insn_copy->insn.insn = self->insn.insn;
	insn_copy->insn.a = self->insn.a;
	insn_copy->insn.b = self->insn.b;

	return (PyObject*) insn_copy;
}

PyDoc_STRVAR(reinit__doc__,
"reinit() -> None\n\n"\
"Reset instruction to default values (DAT.F $ 0, $ 0).");

static PyObject *
Instruction_reinit(Instruction88 *self)
{
	self->insn.insn = INSN(DAT, MODIFIER_F, DIRECT, DIRECT);
	self->insn.a = 0;
	self->insn.b = 0;

	Py_INCREF(Py_None);
	return Py_None;
}

static int
Instruction_buffer_getreadbuf(Instruction *self, int index, const void **ptr)
{
	if (index != 0) {
		PyErr_SetString(PyExc_SystemError,
				"Accessing invalid segment.");
		return -1;
	}
	*ptr = (void *) &(self->insn);

	return sizeof(insn_t);
}

static int
Instruction_buffer_getsegcount(Instruction *self, int *lenp)
{
	/* Store length of insn_t. */
	if (lenp) {
		*lenp = sizeof(insn_t);
	}
	return 1;
}

/* Pickle support. */

static PyObject *
Instruction_getstate(Instruction *self)
{
	return Py_BuildValue("kkkk", self->coresize, self->insn.insn,
			     self->insn.a, self->insn.b);
}

static PyObject *
Instruction_reduce(Instruction *self)
{
	return Py_BuildValue("(ON)", self->ob_type,
			     Instruction_getstate(self));
}

/*
 * Module functions.
 */

PyDoc_STRVAR(fromString__doc__,
"fromString(s, coresize, standard) -> Instruction88/Instruction\n\n"\
"Convert a string to a Redcode instruction.");

static PyObject *
Redcode_fromString(PyObject *self, PyObject *args, PyObject *kwds)
{
	const char *s;
	int	    len;
	s32_t	    coresize = 8000;
	s32_t	    standard = STANDARD_94;

	u32_t	    pos = 0; /* Position inside string. */
	s32_t	    opcode;
	s32_t	    modifier;
	s32_t	    amode;
	s32_t	    afield;
	s32_t	    bmode;
	s32_t	    bfield;

	s32_t	    sign;
	u32_t	    end_pos;
	u32_t	    need_bfield = 0;

	/* Parse arguments. */
	static char *kwlist[] = {"insn", "coresize", "standard", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "s#|ii", kwlist,
				         &s, &len, &coresize, &standard)) {
		return NULL;
	}

	/* Check arguments. */
	if (len < 6) {
		PyErr_SetString(PyExc_ValueError, "Instruction is too short.");
		return NULL;
	}
	if (len > 128) {
		PyErr_SetString(PyExc_ValueError, "Instruction is too long.");
		return NULL;
	}
	if ((coresize < 1) || (coresize > MAX_CORESIZE)) {
		PyErr_SetString(PyExc_ValueError, "Invalid size of core.");
		return NULL;
	}
	if ((standard < 0) || (standard > 2)) {
		PyErr_SetString(PyExc_ValueError, "Unknown Redcode standard.");
		return NULL;
	}

	/* Ignore whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}

	/* Parse opcode. */
	if (pos + 3 > len) {
		PyErr_SetString(PyExc_ValueError, "Instruction is too short.");
		return NULL;
	}
	if (strncasecmp(s + pos, "DAT", 3) == 0) {
		opcode = DAT;
	} else if (strncasecmp(s + pos, "MOV", 3) == 0) {
		opcode = MOV;
	} else if (strncasecmp(s + pos, "ADD", 3) == 0) {
		opcode = ADD;
	} else if (strncasecmp(s + pos, "SUB", 3) == 0) {
		opcode = SUB;
	} else if (strncasecmp(s + pos, "JMP", 3) == 0) {
		opcode = JMP;
	} else if (strncasecmp(s + pos, "JMZ", 3) == 0) {
		opcode = JMZ;
	} else if (strncasecmp(s + pos, "JMN", 3) == 0) {
		opcode = JMN;
	} else if (strncasecmp(s + pos, "DJN", 3) == 0) {
		opcode = DJN;
	} else if (strncasecmp(s + pos, "CMP", 3) == 0) {
		opcode = CMP;
	} else if (strncasecmp(s + pos, "SLT", 3) == 0) {
		opcode = SLT;
	} else if (strncasecmp(s + pos, "SPL", 3) == 0) {
		opcode = SPL;
	} else if (strncasecmp(s + pos, "MUL", 3) == 0) {
		opcode = MUL;
	} else if (strncasecmp(s + pos, "DIV", 3) == 0) {
		opcode = DIV;
	} else if (strncasecmp(s + pos, "MOD", 3) == 0) {
		opcode = MOD;
	} else if (strncasecmp(s + pos, "SEQ", 3) == 0) {
		opcode = SEQ;
	} else if (strncasecmp(s + pos, "SNE", 3) == 0) {
		opcode = SNE;
	} else if (strncasecmp(s + pos, "NOP", 3) == 0) {
		opcode = NOP;
	} else if (strncasecmp(s + pos, "LDP", 3) == 0) {
		opcode = LDP;
	} else if (strncasecmp(s + pos, "STP", 3) == 0) {
		opcode = STP;
	} else {
		PyErr_SetString(PyExc_ValueError, "Unknown opcode.");
		return NULL;
	}
	pos += 3;

	/* Check for valid ICWS '88 opcodes. */
	if ((standard == STANDARD_88) && (opcode > 0x0a)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid opcode under ICWS '88 rules.");
		return NULL;
	}

	/* Ignore whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}

	/* Parse (optional) modifier. */
	if (pos == len) {
		PyErr_SetString(PyExc_ValueError, "Instruction is too short.");
		return NULL;
	}
	modifier = 0xff; /* Some invalid value. */
	if (s[pos] == '.') {
		pos++;
		if (standard == STANDARD_88) {
			PyErr_SetString(PyExc_ValueError,
					"ICWS '88 does not support "\
					"opcode modifiers.");
			return NULL;
		}
		if (pos + 2 > len) {
			PyErr_SetString(PyExc_ValueError,
					"Instruction is too short.");
			return NULL;
		}
		if ((s[pos] == 'F') || (s[pos] == 'f')) {
			modifier = MODIFIER_F;
		} else if ((s[pos] == 'I') || (s[pos] == 'i')) {
			modifier = MODIFIER_I;
		} else if ((s[pos] == 'X') || (s[pos] == 'x')) {
			modifier = MODIFIER_X;
		} else if ((s[pos] == 'A') || (s[pos] == 'a')) {
			pos++;
			if ((s[pos] == 'B') || (s[pos] == 'b')) {
				modifier = MODIFIER_AB;
			} else {
				modifier = MODIFIER_A;
			}
		} else if ((s[pos] == 'B') || (s[pos] == 'b')) {
			pos++;
			if ((s[pos] == 'A') || (s[pos] == 'a')) {
				modifier = MODIFIER_BA;
			} else {
				modifier = MODIFIER_B;
			}
		} else {
			PyErr_SetString(PyExc_ValueError,
					"Missing or invalid modifier.");
			return NULL;
		}
		pos++;
	}

	/* Ignore whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}

	/* Parse (optional) A-mode. */
	if (pos >= len) {
		PyErr_SetString(PyExc_ValueError, "Incomplete instruction.");
		return NULL;
	}
	switch (s[pos]) {
	case '$':	amode = DIRECT; pos++; break;
	case '#':	amode = IMMEDIATE; pos++; break;
	case '@':	amode = INDIRECT; pos++; break;
	case '<':	amode = PREDECREMENT; pos++; break;
	case '>':	amode = B_POSTINCREMENT; pos++; break;
	case '*':	amode = A_INDIRECT; pos++; break;
	case '{':	amode = A_PREDECREMENT; pos++; break;
	case '}':	amode = A_POSTINCREMENT; pos++; break;
	default:	if ((s[pos] == '+') || (s[pos] == '-') ||
			    (s[pos] == ' ') || (s[pos] == '\t') ||
			    ((s[pos] >= '0') && (s[pos] <= '9'))) {
				/* Use default mode: $. */
				amode = DIRECT;
			} else {
				PyErr_SetString(PyExc_ValueError,
						"Invalid A-mode.");
				return NULL;
			}
	}

	/* Check A-mode under ICWS '88 rules, if necessary. */
	if ((standard == STANDARD_88) && (amode > 0x03)) {
		PyErr_SetString(PyExc_ValueError,
				"Invalid A-mode under ICWS '88 rules.");
		return NULL;
	}

	/* Ignore whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}
	if (pos + 3 > len) {
		PyErr_SetString(PyExc_ValueError, "Instruction is too short.");
		return NULL;
	}

	/* Parse A-field. */
	afield = 0;
	if (s[pos] == '+') {
		sign = 1;
		pos++;
	} else if (s[pos] == '-') {
		sign = -1;
		pos++;
	} else {
		sign = 1;
	}
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}
	if (pos == len) {
		PyErr_SetString(PyExc_ValueError, "Instruction is too short.");
		return NULL;
	}
	end_pos = pos;
	while ((end_pos < len) && (s[end_pos] >= '0') && (s[end_pos] <= '9')) {
		end_pos++;
	}
	afield = strtol(s + pos, NULL, 10);
	if (afield > (LONG_MAX >> 1)) {
		PyErr_SetString(PyExc_ValueError,
				"A-number exceeds supported range.");
		return NULL;
	}
	afield *= sign;
	pos = end_pos;

	/* Skip whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}

	/* Check for colon. */
	if (pos < len) {
		if (s[pos] == ',') {
			need_bfield = 1;
			pos++;
		} else {
			PyErr_SetString(PyExc_ValueError, "Missing B-field.");
			return NULL;
		}
	}

	/* Skip whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}

	/* Parse (optional) B-mode. */
	bmode = DIRECT;
	if (need_bfield && (pos < len)) {
		switch (s[pos]) {
		case '$':	bmode = DIRECT; pos++; break;
		case '#':	bmode = IMMEDIATE; pos++; break;
		case '@':	bmode = INDIRECT; pos++; break;
		case '<':	bmode = PREDECREMENT; pos++; break;
		case '>':	bmode = B_POSTINCREMENT; pos++; break;
		case '*':	bmode = A_INDIRECT; pos++; break;
		case '{':	bmode = A_PREDECREMENT; pos++; break;
		case '}':	bmode = A_POSTINCREMENT; pos++; break;
		default:	if ((s[pos] != '+') && (s[pos] != '-') &&
				    ((s[pos] < '0') || (s[pos] > '9'))) {
					PyErr_SetString(PyExc_ValueError,
							"Invalid B-mode.");
					return NULL;
				}
		}
	} else {
		PyErr_SetString(PyExc_ValueError, "Missing B-field.");
		return NULL;
	}

	/* Skip whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}
	
	/* Parse B-field. */
	bfield = 0;
	if (need_bfield && (pos < len)) {
		/* Get (optional) sign. */
		if (s[pos] == '+') {
			sign = 1;
			pos++;
		} else if (s[pos] == '-') {
			sign = -1;
			pos++;
		} else {
			sign = 1;
		}

		/* Skip possible whitespaces after sign. */
		while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
			pos++;
		}

		if (pos == len) {
			PyErr_SetString(PyExc_ValueError, "Missing B-field.");
			return NULL;
		}

		end_pos = pos;
		while ((end_pos < len) && (s[end_pos] >= '0') &&
		       (s[end_pos] <= '9')) {
			end_pos++;
		}
		bfield = strtol(s + pos, NULL, 10);
		if (bfield > (LONG_MAX >> 1)) {
			PyErr_SetString(PyExc_ValueError,
					"B-number exceeds supported range.");
			return NULL;
		}
		bfield *= sign;
		pos = end_pos;
	} else {
		PyErr_SetString(PyExc_ValueError, "Missing B-field.");
		return NULL;
	}

	/* Skip whitespaces. */
	while ((pos < len) && ((s[pos] == ' ') || (s[pos] == '\t'))) {
		pos++;
	}
	
	/* Check for end of instruction or start of comment. */
	if ((pos < len) && (s[pos] != ';')) {
		PyErr_SetString(PyExc_ValueError, "Invalid B-field.");
		return NULL;
	}

	/* "DAT $ x" --> "DAT $ 0, $ x" */
	if ((opcode == DAT) && !need_bfield) {
		bmode = amode;
		bfield = afield;
		amode = DIRECT;
		afield = 0;
	}

	/* Check ICWS '88 rules for instructions. */
	if (standard == STANDARD_88) {
		switch (opcode) {
		case DAT:
			if (((amode != IMMEDIATE) && 
			     (amode != PREDECREMENT)) ||
			    ((bmode != IMMEDIATE) &&
                             (bmode != PREDECREMENT))) {
				PyErr_SetString(PyExc_ValueError,
						"Invalid instruction under "\
						"ICWS '88 rules.");
				return NULL;
			}
			break;
		case MOV:
		case ADD:
		case SUB:
		case CMP:
		case SLT:
			if (bmode == IMMEDIATE) {
				PyErr_SetString(PyExc_ValueError,
						"Invalid instruction under "\
						"ICWS '88 rules.");
				return NULL;
			}
			break;
		case JMP:
		case JMZ:
		case JMN:
		case DJN:
		case SPL:
			if (amode == IMMEDIATE) {
				PyErr_SetString(PyExc_ValueError,
						"Invalid instruction under "\
						"ICWS '88 rules.");
				return NULL;
			}
			break;
		default:
			/* This shouldn't happen. */
			PyErr_SetString(PyExc_SystemError, "Parser bug!");
			return NULL;
		}
	}

	/* Fill in default modifier, if necessary. */
	if ((standard != STANDARD_88) && (modifier == 0xff)) {
		switch (opcode) {
		case DAT:
		case NOP:
			modifier = MODIFIER_F; break;
		case MOV:
		case SEQ:
		case SNE:
			if (amode == IMMEDIATE) {
				modifier = MODIFIER_AB;
			} else if (bmode == IMMEDIATE) {
				modifier = MODIFIER_B;
			} else {
				modifier = MODIFIER_I;
			}
			break;
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case MOD:
			if (amode == IMMEDIATE) {
				modifier = MODIFIER_AB;
			} else if (bmode == IMMEDIATE) {
				modifier = MODIFIER_B;
			} else {
				modifier = MODIFIER_F;
			}
			break;
		case JMP:
		case JMZ:
		case JMN:
		case DJN:
		case SPL:
			modifier = MODIFIER_B; break;
		case SLT:
		case LDP:
		case STP:
			if (amode == IMMEDIATE) {
				modifier = MODIFIER_AB;
			} else {
				modifier = MODIFIER_B;
			}
			break;
		default:
			/* This shouldn't happen. */
			PyErr_SetString(PyExc_SystemError, "Parser bug!");
			return NULL;
		}
	}

	/* Reduce field modulo CORESIZE. */
	if (afield < 0) {
		afield = coresize - (abs(afield) % coresize);
	}
	afield %= coresize;
	if (bfield < 0) {
		bfield = coresize - (abs(bfield) % coresize);
	}
	bfield %= coresize;

	/* Create instruction. */
	if (standard == STANDARD_88) {
		Instruction88 *insn;

		insn = (Instruction88 *)
		       Instruction88Type.tp_alloc(&Instruction88Type, 0);
		if (insn != NULL) {
			insn->coresize = coresize;
			insn->insn.insn = INSN88(opcode, amode, bmode);
			insn->insn.a = afield;
			insn->insn.b = bfield;
		}

		return (PyObject *) insn;
	} else {
		Instruction *insn;
		
		insn = (Instruction *)
			InstructionType.tp_alloc(&InstructionType, 0);
		if (insn != NULL) {
			insn->coresize = coresize;
			insn->insn.insn = INSN(opcode, modifier, amode, bmode);
			insn->insn.a = afield;
			insn->insn.b = bfield;
		}

		return (PyObject *) insn;
	}
}

static PyMemberDef Instruction_members[] = {
	{NULL} /* sentinel */
};

static PyMethodDef Instruction_methods[] = {
	{"copy", (PyCFunction)Instruction_copy, METH_NOARGS, copy__doc__},
	{"reinit", (PyCFunction)Instruction_reinit, METH_NOARGS,
	 reinit__doc__},
	{"__reduce__", (PyCFunction)Instruction_reduce, METH_NOARGS,
	 PyDoc_STR("__reduce__() -> (cls, state)")},
	{NULL, NULL, 0, NULL} /* sentinel */
};

static PyGetSetDef Instruction_getseters[] = {
	{"opcode", (getter)Instruction_GetOpcode,
	 (setter)Instruction_SetOpcode, opcode__doc__, NULL},
	{"modifier", (getter)Instruction_GetModifier,
	 (setter)Instruction_SetModifier, modifier__doc__, NULL},
	{"amode", (getter)Instruction_GetAMode,
	 (setter)Instruction_SetAMode, amode__doc__, NULL},
	{"afield", (getter)Instruction_GetAField,
	 (setter)Instruction_SetAField, afield__doc__, NULL},
	{"bmode", (getter)Instruction_GetBMode,
	 (setter)Instruction_SetBMode, bmode__doc__, NULL},
	{"bfield", (getter)Instruction_GetBField,
	 (setter)Instruction_SetBField, bfield__doc__, NULL},
	{NULL} /* sentinel */
};

static PyBufferProcs Instruction_as_buffer = {
	(getreadbufferproc) Instruction_buffer_getreadbuf,
	(getwritebufferproc) NULL, /* No writing to buffer supported. */
	(getsegcountproc) Instruction_buffer_getsegcount,
	(getcharbufferproc) NULL,
};

static PyTypeObject InstructionType = {
	PyObject_HEAD_INIT(NULL) 0, 			/*ob_size*/
	"Corewar.Redcode.Instruction", 			/*tp_name*/
	sizeof(Instruction),		 	  	/*tp_basicsize*/
	0,                         			/*tp_itemsize*/
	(destructor)Instruction_dealloc,	  	/*tp_dealloc*/
	0,						/*tp_print*/
	0,						/*tp_getattr*/
	0,						/*tp_setattr*/
	0,						/*tp_compare*/
	(reprfunc)Instruction_repr,			/*tp_repr*/
	0,						/*tp_as_number*/
	0,						/*tp_as_sequence*/
	0,						/*tp_as_mapping*/
	0,						/*tp_hash */
	0,						/*tp_call*/
	0,						/*tp_str*/
	0,						/*tp_getattro*/
	0,						/*tp_setattro*/
	&Instruction_as_buffer,				/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/*tp_flags*/
	Instruction__doc__,				/*tp_doc */
	0,						/*tp_traverse */
	0,						/*tp_clear */
	(richcmpfunc)Instruction_richcompare,		/*tp_richcompare */
	0,						/*tp_weaklistoffset */
	0,						/*tp_iter */
	0,						/*tp_iternext */
	Instruction_methods,				/*tp_methods */
	Instruction_members,				/*tp_members */
	Instruction_getseters,				/*tp_getset */
	0,						/*tp_base */
	0,						/*tp_dict */
	0,						/*tp_descr_get */
	0,						/*tp_descr_set */
	0,						/*tp_dictoffset */
	0,						/*tp_init */
	0,						/*tp_alloc */
	Instruction_new,				/*tp_new */
};

static PyMethodDef module_methods[] = {
	{"fromString", (PyCFunction)Redcode_fromString,
	 METH_VARARGS | METH_KEYWORDS, fromString__doc__},
	{NULL, NULL, 0, NULL}   /* sentinel */
};

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyDoc_STRVAR(module__doc__,
"Redcode instructions\n\n"\
"Classes:\n\n"\
"  Intruction88 -- Redcode instruction according to ICWS '88 rules\n"\
"  Instruction  -- Redcode instruction according to ICWS '94 (with pMARS\n"\
"                  extensions) rules\n\n"\
"Integer constants:\n\n"\
"  OPCODE_DAT, OPCODE_MOV, OPCODE_ADD, OPCODE_SUB, OPCODE_JMP,\n"\
"  OPCODE_JMZ, OPCODE_JMN, OPCODE_DJN, OPCODE_CMP, OPCODE_SPL,\n"\
"  OPCODE_SLT, OPCODE_MUL, OPCODE_DIV, OPCODE_MOD, OPCODE_SEQ,\n"\
"  OPCODE_SNE, OPCODE_NOP, OPCODE_LDP, OPCODE_STP  -- opcodes\n"\
"  MODE_DIRECT, MODE_IMMEDIATE, MODE_INDIRECT, MODE_PREDECREMENT,\n"\
"  MODE_B_INDIRECT, MODE_B_PREDECREMENT, MODE_B_POSTINCREMENT,\n"\
"  MODE_A_INDIRECT, MODE_A_PREDECREMENT, MODE_A_POSTINCREMENT\n"\
"   -- addressing modes");

PyMODINIT_FUNC
initRedcode(void)
{
	PyObject *m;
	PyObject *d;

	if (PyType_Ready(&Instruction88Type) < 0) {
		return;
	}
	if (PyType_Ready(&InstructionType) < 0) {
		return;
	}

	/* Initialize module. */	
	m = Py_InitModule3("Redcode", module_methods, module__doc__);
	if (m == NULL) {
		return;
	}

	/* Add classes Instruction88, Instruction */
	Py_INCREF(&Instruction88Type);
	PyModule_AddObject(m, "Instruction88",
			   (PyObject *) &Instruction88Type);
	Py_INCREF(&InstructionType);
	PyModule_AddObject(m, "Instruction", (PyObject *) &InstructionType);

	/* Get dict of module. */
	d = PyModule_GetDict(m);
	if (d == NULL) {
		return;
	}

	/* Add instruction set of ICWS '88. */
	PyModule_AddIntConstant(m, "OPCODE_DAT", DAT);
	PyModule_AddIntConstant(m, "OPCODE_MOV", MOV);
	PyModule_AddIntConstant(m, "OPCODE_ADD", ADD);
	PyModule_AddIntConstant(m, "OPCODE_SUB", SUB);
	PyModule_AddIntConstant(m, "OPCODE_JMP", JMP);
	PyModule_AddIntConstant(m, "OPCODE_JMZ", JMZ);
	PyModule_AddIntConstant(m, "OPCODE_JMN", JMN);
	PyModule_AddIntConstant(m, "OPCODE_DJN", DJN);
	PyModule_AddIntConstant(m, "OPCODE_CMP", CMP);
	PyModule_AddIntConstant(m, "OPCODE_SPL", SPL);
	PyModule_AddIntConstant(m, "OPCODE_SLT", SLT);

	/* Add instruction set of ICWS '94 draft (without p-space). */
	PyModule_AddIntConstant(m, "OPCODE_MUL", MUL);
	PyModule_AddIntConstant(m, "OPCODE_DIV", DIV);
	PyModule_AddIntConstant(m, "OPCODE_MOD", MOD);
	PyModule_AddIntConstant(m, "OPCODE_SEQ", SEQ);
	PyModule_AddIntConstant(m, "OPCODE_SNE", SNE);
	PyModule_AddIntConstant(m, "OPCODE_NOP", NOP);

	/* Add instruction set of ICWS '94 draft. */
	PyModule_AddIntConstant(m, "OPCODE_LDP", LDP);
	PyModule_AddIntConstant(m, "OPCODE_STP", STP);

	/* Add addressing modes of ICWS '88. */
	PyModule_AddIntConstant(m, "MODE_IMMEDIATE", IMMEDIATE);
	PyModule_AddIntConstant(m, "MODE_DIRECT", DIRECT);
	PyModule_AddIntConstant(m, "MODE_INDIRECT", INDIRECT);
	PyModule_AddIntConstant(m, "MODE_PREDECREMENT", PREDECREMENT);

	/* Add addressing modes of ICWS '94 draft. */
	PyModule_AddIntConstant(m, "MODE_B_INDIRECT", B_INDIRECT);
	PyModule_AddIntConstant(m, "MODE_B_PREDECREMENT", B_PREDECREMENT);
	PyModule_AddIntConstant(m, "MODE_B_POSTINCREMENT", B_POSTINCREMENT);
	PyModule_AddIntConstant(m, "MODE_A_INDIRECT", A_INDIRECT);
	PyModule_AddIntConstant(m, "MODE_A_PREDECREMENT", A_PREDECREMENT);
	PyModule_AddIntConstant(m, "MODE_A_POSTINCREMENT", A_POSTINCREMENT);

	/* Add opcode modifiers of ICWS '94 draft. */
	PyModule_AddIntConstant(m, "MODIFIER_A", MODIFIER_A);
	PyModule_AddIntConstant(m, "MODIFIER_B", MODIFIER_B);
	PyModule_AddIntConstant(m, "MODIFIER_F", MODIFIER_F);
	PyModule_AddIntConstant(m, "MODIFIER_I", MODIFIER_I);
	PyModule_AddIntConstant(m, "MODIFIER_X", MODIFIER_X);
	PyModule_AddIntConstant(m, "MODIFIER_AB", MODIFIER_AB);
	PyModule_AddIntConstant(m, "MODIFIER_BA", MODIFIER_BA);
}
