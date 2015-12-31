/* BenchMARS94nop.c 
 *
 * Copyright (C) 2006 Jens Gutzeit <jens@jgutzeit.de>
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

/* ATTENTION!
 *
 * This file was generated with the Optimizer for PyCorewar.
 *
 * Please edit with caution!
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <MyTypes.h>
#include <Instruction.h>
#include <BenchWarrior.h>
#include <BenchProcessQueue.h>
#include <BenchPositioning.h>
#include <BenchMacros.h>
#include <BenchMARS.h>
#include <BenchMARS94nop.h>

/* Run a two warrior fight. Parameters are NOT checked for plausibility!
 * Return the result after the fights or NULL on error. */
u64_t *
run_94nop(insn_t *core, warrior_t *warriors, u32_t coresize,
	  u32_t maxprocesses, u32_t maxcycles, u32_t mindistance,
	  u32_t numrounds, u32_t seed)
{
	u32_t round; 		/* Number of the current round. */
	u64_t *results; 	/* List with the results, format: w l t. */
	u64_t insn_counter = 0; /* Counter of executed instructions. */
	u32_t pos2; 		/* Position of second warrior in the core. */
	u32_t cycle; 		/* Number of current cycle. */
	pq_info_t pq_info1; 	/* Information about processes queue of the
				 * first warrior. */
	pq_info_t pq_info2; 	/* Information about processes queue of the
				 * second warrior. */
	pq_info_t *pqinfo;
	insn_t **pqueue; 	/* Memory for the process queues. */

	/* Available space in the core for second warrior. */
	u32_t const space = coresize - 2 * mindistance + 1; 

	/* Pointer to end of core. */
	insn_t *core_end = core + coresize;

	/* Allocate memory for the process queues. */
	pqueue = calloc(2 * (maxcycles + maxprocesses), sizeof(insn_t *));

	/* Allocate and initialized memory for results. */
	results = calloc(5, sizeof(u64_t));

	/* Check allocated memory. */
	if ((pqueue == NULL) | (results == NULL)) {
		free (results);
		free (pqueue);
		return NULL;
	}

	/* Adjust seed to make the second warrior be at position seed in
	 * the first round. */
	seed -= mindistance;

	/* No need to use field 'prev'. */
	pq_info1.next = &pq_info2;
	pq_info2.next = &pq_info1;

	/* Run the given number of rounds. */
	for (round = 0; round < numrounds; round++) {
		/* One head is enough, because both processes queues are
		 * interleaved. */
		insn_t **head;

		/* Calculate position of second warrior. */
		pos2 = mindistance + seed % space;
		seed = rng(seed);
		
		/* Set up core. */
		memset(core, 0, coresize * sizeof(insn_t));

		/* Load warriors. */
		memcpy(core, warriors[0].insns,
		       warriors[0].length * sizeof(insn_t));
		memcpy(core + pos2, warriors[1].insns,
		       warriors[1].length * sizeof(insn_t));

		/* Set execution order and initialize both process queues. */
		if (round % 2 == 0) {
			pqueue[0] = core + warriors[0].start;
			pq_info1.tail = pqueue + 2;
			pq_info1.numprocesses = 1;

			pqueue[1] = core + pos2 + warriors[1].start;
			pq_info2.tail = pqueue + 3;
			pq_info2.numprocesses = 1;

			pqinfo = &pq_info1;
		} else {
			pqueue[1] = core + warriors[0].start;
			pq_info1.tail = pqueue + 3;
			pq_info1.numprocesses = 1;

			pqueue[0] = core + pos2 + warriors[1].start;
			pq_info2.tail = pqueue + 2;
			pq_info2.numprocesses = 1;

			pqinfo = &pq_info2;
		}
		head = pqueue;

		/* Run one round. */
		cycle = 2 * maxcycles;

		/* Update instruction counter with preliminary number of
		 * executed instructions. */
		insn_counter += 2 * maxcycles;
		do {
			insn_t *ip;    /* Pointer to the currently executed
				        * instruction. */
			field_t ra_a;  /* A register values */
			field_t ra_b;
			field_t rb_a;  /* B register values */
			field_t rb_b;
			insn_t *aAddr; /* Pointer to address specified by
					* A operand. */
			insn_t *bAddr; /* Pointer to address specified by
					* B operand. */

			field_t *tmp_field;

			/* Get instruction pointer. */
			PQ_BI_NEXT(ip);

			/* Optimized instructions. */
			switch(ip->insn) {
			case INSN(MOV, MODIFIER_I, IMMEDIATE, DIRECT):
				/* Evaluate A-operand. */
				/* Nothing to do. */

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *ip;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, DIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, A_INDIRECT):
				/* Evaluate A-operand. */
				/* Nothing to do. */

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);

				/* Copy complete instruction. */
				*bAddr = *ip;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, DIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DAT, MODIFIER_F, DIRECT, DIRECT):
				/* No need to evaluate A-operand. */

				/* No need to evaluate B-operand. */

				/* Current processes dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = ip->insn;
				bAddr->a = ip->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ra_a = ip->a;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = ip->insn;
				bAddr->a = ra_a;
				bAddr->b = ip->b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ra_a = ip->a;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);

				/* Copy complete instruction. */
				bAddr->insn = ip->insn;
				bAddr->a = ra_a;
				bAddr->b = ip->b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, A_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, IMMEDIATE):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DAT, MODIFIER_F, B_PREDECREMENT, DIRECT):
				/* Partially evaluate A-operand. */
				/* Use ip instead of aAddr. */
				ADDADDR(ip, ip->a);
				DECMOD(ip->b);

				/* No need to evaluate B-operand. */

				/* Current processes dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, B_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, DIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = ra_a;
				bAddr->b = aAddr->b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, B_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, A_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, A_POSTINCREMENT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = ra_a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_B, DIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_b = bAddr->b;

				DECMOD(bAddr->b);
				if (rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				INCMOD(bAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(JMP, MODIFIER_B, DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND(aAddr);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, B_INDIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(JMP, MODIFIER_B, DIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND(aAddr);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_B, DIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_b = ip->b;

				DECMOD(bAddr->b);
				if (rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DAT, MODIFIER_F, B_POSTINCREMENT, B_POSTINCREMENT):
				/* Save value, because it might be
				 * changed by evaluation of A-operand. */
				rb_b = ip->b;

				/* Partially evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				INCMOD(aAddr->b);

				/* Partially evaluate B-operand. */
				/* Use ip instead of bAddr. */
				ADDADDR(ip, rb_b);
				INCMOD(ip->b);

				/* Current processes dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, DIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(ADD, MODIFIER_F, DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Execute instruction. */
				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			}

			/* Evaluate A- and B-operand. */
			switch (GET_MODES(ip->insn)) {
			case MODES(IMMEDIATE, IMMEDIATE):
				/* Evaluate A- and B-operand. */
				aAddr = bAddr = ip;
				ra_a = rb_a = ip->a;
				ra_b = rb_b = ip->b;
				break;
			case MODES(IMMEDIATE, DIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, B_INDIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, B_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(IMMEDIATE, A_INDIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, A_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(DIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				rb_b = ip->b;
				break;
			case MODES(DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(DIRECT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_INDIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				rb_b = ip->b;
				break;
			case MODES(B_INDIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_INDIRECT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);							DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_PREDECREMENT, IMMEDIATE):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				break;
			case MODES(B_PREDECREMENT, DIRECT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, B_INDIRECT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, B_PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, B_POSTINCREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_PREDECREMENT, A_INDIRECT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, A_PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, A_POSTINCREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_POSTINCREMENT, IMMEDIATE):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				break;
			case MODES(B_POSTINCREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);
	
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, B_INDIRECT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, B_PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, B_POSTINCREMENT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_POSTINCREMENT, A_INDIRECT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, A_PREDECREMENT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, A_POSTINCREMENT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_INDIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				rb_b = ip->b;
				break;
			case MODES(A_INDIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_INDIRECT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_PREDECREMENT, IMMEDIATE):
				rb_a = ip->a;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_b = ip->b;
				break;
			case MODES(A_PREDECREMENT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_PREDECREMENT,A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_POSTINCREMENT, IMMEDIATE):
				rb_a = ip->a;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_b = ip->b;
				break;
			case MODES(A_POSTINCREMENT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_POSTINCREMENT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			default:
				/* Set values to prevent compiler warnings
				 * about uninitialized variables. */
				ra_a = ra_b = rb_a = rb_b = 0;
				aAddr = bAddr = NULL;

				/* This shouldn't happen. */
				printf ("Found invalid modes.");
			}
	
			/* Switch on opcode/modifier. Cases are roughly
			 * sorted according to the frequency of the opcodes. */
			switch (GET_OP_MOD(ip->insn)) {
			/*
			 * MOV
			 */
			case OP_MOD(MOV, MODIFIER_A):
				bAddr->a = ra_a;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_F):
				bAddr->a = ra_a;
			case OP_MOD(MOV, MODIFIER_B):
				bAddr->b = ra_b;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_AB):
				bAddr->b = ra_a;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_X):
				bAddr->b = ra_a;
			case OP_MOD(MOV, MODIFIER_BA):
				bAddr->a = ra_b;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_I):
				bAddr->insn = aAddr->insn;
				bAddr->a = ra_a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * DAT
			 */
			case OP_MOD(DAT, MODIFIER_A):
			case OP_MOD(DAT, MODIFIER_B):
			case OP_MOD(DAT, MODIFIER_F):
			case OP_MOD(DAT, MODIFIER_I):
			case OP_MOD(DAT, MODIFIER_X):
			case OP_MOD(DAT, MODIFIER_AB):
			case OP_MOD(DAT, MODIFIER_BA):
				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				break;
			/*
			 * SPL
			 */
			case OP_MOD(SPL, MODIFIER_A):
			case OP_MOD(SPL, MODIFIER_B):
			case OP_MOD(SPL, MODIFIER_F):
			case OP_MOD(SPL, MODIFIER_I):
			case OP_MOD(SPL, MODIFIER_X):
			case OP_MOD(SPL, MODIFIER_AB):
			case OP_MOD(SPL, MODIFIER_BA):
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}
				break;
			/*
			 * DJN
			 */
			case OP_MOD(DJN, MODIFIER_BA):
			case OP_MOD(DJN, MODIFIER_A):
				DECMOD(bAddr->a);
				if (rb_a == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}
				break;
			case OP_MOD(DJN, MODIFIER_AB):
			case OP_MOD(DJN, MODIFIER_B):
				DECMOD(bAddr->b);
				if (rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}
				break;
			case OP_MOD(DJN, MODIFIER_X):
			case OP_MOD(DJN, MODIFIER_I):
			case OP_MOD(DJN, MODIFIER_F):
				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if (rb_a == 1 && rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}
				break;
			/*
			 * SUB
			 */
			case OP_MOD(SUB, MODIFIER_I):
			case OP_MOD(SUB, MODIFIER_F):
				SUBMOD(bAddr->b, rb_b, ra_b);
			case OP_MOD(SUB, MODIFIER_A):
				SUBMOD(bAddr->a, rb_a, ra_a);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SUB, MODIFIER_B):
				SUBMOD(bAddr->b, rb_b, ra_b);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SUB, MODIFIER_X):
				SUBMOD(bAddr->a, rb_a, ra_b);
			case OP_MOD(SUB, MODIFIER_AB):
				SUBMOD(bAddr->b, rb_b, ra_a);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SUB, MODIFIER_BA):
				SUBMOD(bAddr->a, rb_a, ra_b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * JMZ
			 */
			case OP_MOD(JMZ, MODIFIER_BA):
			case OP_MOD(JMZ, MODIFIER_A):
				if (rb_a == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMZ, MODIFIER_AB):
			case OP_MOD(JMZ, MODIFIER_B):
				if (rb_b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMZ, MODIFIER_X):
			case OP_MOD(JMZ, MODIFIER_F):
			case OP_MOD(JMZ, MODIFIER_I):
				if ((rb_a == 0) && (rb_b == 0)) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			/*
			 * ADD
			 */
			case OP_MOD(ADD, MODIFIER_I):
			case OP_MOD(ADD, MODIFIER_F):
				ADDMOD(bAddr->b, ra_b, rb_b);
			case OP_MOD(ADD, MODIFIER_A):
				ADDMOD(bAddr->a, ra_a, rb_a);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(ADD, MODIFIER_B):
				ADDMOD(bAddr->b, ra_b, rb_b);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(ADD, MODIFIER_X):
				ADDMOD(bAddr->a, ra_b, rb_a);
			case OP_MOD(ADD, MODIFIER_AB):
				ADDMOD(bAddr->b, ra_a, rb_b);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(ADD, MODIFIER_BA):
				ADDMOD(bAddr->a, ra_b, rb_a);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * JMN
			 */
			case OP_MOD(JMN, MODIFIER_BA):
			case OP_MOD(JMN, MODIFIER_A):
				if (rb_a != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMN, MODIFIER_AB):
			case OP_MOD(JMN, MODIFIER_B):
				if (rb_b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMN, MODIFIER_X):
			case OP_MOD(JMN, MODIFIER_F):
			case OP_MOD(JMN, MODIFIER_I):
				if ((rb_a != 0) || (rb_b != 0)) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			/*
			 * SNE
			 */
			case OP_MOD(SNE, MODIFIER_A):
				if (ra_a != rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_B):
				if (ra_b != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_AB):
				if (ra_a != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_BA):
				if (ra_b != rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_I):
				if (aAddr->insn != bAddr->insn ||
				    ra_a != rb_a || ra_b != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_F):
				if (ra_a != rb_a || ra_b != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_X):
				if (ra_a != rb_b || ra_b != rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * JMP
			 */
			case OP_MOD(JMP, MODIFIER_A):
			case OP_MOD(JMP, MODIFIER_B):
			case OP_MOD(JMP, MODIFIER_F):
			case OP_MOD(JMP, MODIFIER_I):
			case OP_MOD(JMP, MODIFIER_X):
			case OP_MOD(JMP, MODIFIER_AB):
			case OP_MOD(JMP, MODIFIER_BA):
				/* Queue jump destination. */
				PQ_BI_APPEND(aAddr);
				break;
			/*
			 * SEQ
			 */
			case OP_MOD(SEQ, MODIFIER_A):
				if (ra_a == rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_B):
				if (ra_b == rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_AB):
				if (ra_a == rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_BA):
				if (ra_b == rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_I):
				if (ra_a == rb_a && ra_b == rb_b &&
				    aAddr->insn == bAddr->insn) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_F):
				if (ra_a == rb_a && ra_b == rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_X):
				if (ra_a == rb_b && ra_b == rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * SLT
			 */
			case OP_MOD(SLT, MODIFIER_A):
				if (ra_a < rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_B):
				if (ra_b < rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_AB):
				if (ra_a < rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_BA):
				if (ra_b < rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_I):
			case OP_MOD(SLT, MODIFIER_F):
				if (ra_a < rb_a && ra_b < rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_X):
				if (ra_a < rb_b && ra_b < rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;				
			/*
			 * MUL
			 */
			case OP_MOD(MUL, MODIFIER_I):
			case OP_MOD(MUL, MODIFIER_F):
				bAddr->b = (rb_b * ra_b) % coresize;
			case OP_MOD(MUL, MODIFIER_A):
				bAddr->a = (rb_a * ra_a) % coresize;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MUL, MODIFIER_B):
				bAddr->b = (rb_b * ra_b) % coresize;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MUL, MODIFIER_X):
				bAddr->a = (rb_a * ra_b) % coresize;
			case OP_MOD(MUL, MODIFIER_AB):
				bAddr->b = (rb_b * ra_a) % coresize;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MUL, MODIFIER_BA):
				bAddr->a = (rb_a * ra_b) % coresize;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * DIV
			 */
			case OP_MOD(DIV, MODIFIER_I):
			case OP_MOD(DIV, MODIFIER_F):
				if (ra_a) bAddr->a = rb_a / ra_a;
				if (ra_b) bAddr->b = rb_b / ra_b;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_X):
				if (ra_b) bAddr->a = rb_a / ra_b;
				if (ra_a) bAddr->b = rb_b / ra_a;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_A):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a / ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_B):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b / ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_AB):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b / ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_BA):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a / ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;

			/*
			 * NOP
			 */
			case OP_MOD(NOP, MODIFIER_A):
			case OP_MOD(NOP, MODIFIER_B):
			case OP_MOD(NOP, MODIFIER_F):
			case OP_MOD(NOP, MODIFIER_I):
			case OP_MOD(NOP, MODIFIER_X):
			case OP_MOD(NOP, MODIFIER_AB):
			case OP_MOD(NOP, MODIFIER_BA):
				/* Nothing to do. */
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * MOD
			 */
			case OP_MOD(MOD, MODIFIER_I):
			case OP_MOD(MOD, MODIFIER_F):
				if (ra_a) bAddr->a = rb_a % ra_a;
				if (ra_b) bAddr->b = rb_b % ra_b;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_X):
				if (ra_b) bAddr->a = rb_a % ra_b;
				if (ra_a) bAddr->b = rb_b % ra_a;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_A):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a % ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_B):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b % ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_AB):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b % ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_BA):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a % ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;			
			default:
				/* This shouldn't happen. */
				printf("Invalid instruction found. "\
				       "You have a problem!\n");
			}

			/* Switch to next warriors. */
			pqinfo = pqinfo->next;
		} while (--cycle);

		/* Accumulate results. */
		if (pq_info1.numprocesses == 0) {
			results[1]++; /* First warrior has lost. */
		} else if (pq_info2.numprocesses == 0) {
			results[0]++; /* First warrior has won. */
		} else {
			results[2]++; /* Tie. */
		}
	}

	/* Save number of executed instructions. */
	results[3] = insn_counter;

	/* Clean up. */
	free(pqueue);

	return results;
}

/* Run a two warrior fight with all possible start positions. Parameters
 * are NOT checked for plausibility! Return the result after the fights
 * or NULL on error. */
u64_t *
p_run_94nop(insn_t *core, warrior_t *warriors, u32_t coresize,
	  u32_t maxprocesses, u32_t maxcycles, u32_t mindistance)
{
	u32_t round; 		/* Number of the current round. */
	u64_t *results; 	/* List with the results, format: w l t. */
	u64_t insn_counter = 0; /* Counter of executed instructions. */
	u32_t cycle; 		/* Number of current cycle. */
	pq_info_t pq_info1; 	/* Information about processes queue of the
				 * first warrior. */
	pq_info_t pq_info2; 	/* Information about processes queue of the
				 * second warrior. */
	pq_info_t *pqinfo;
	insn_t **pqueue; 	/* Memory for the process queues. */

	/* Pointer to end of core. */
	insn_t *core_end = core + coresize;

	/* Allocate memory for the process queues. */
	pqueue = calloc(2 * (maxcycles + maxprocesses), sizeof(insn_t *));

	/* Allocate and initialized memory for results. */
	results = calloc(4, sizeof(u64_t));

	/* Check allocated memory. */
	if ((pqueue == NULL) | (results == NULL)) {
		free (results);
		free (pqueue);
		return NULL;
	}

	/* No need to use field 'prev'. */
	pq_info1.next = &pq_info2;
	pq_info2.next = &pq_info1;

	/* Run the given number of rounds. */
	for (round = 0; round < 2 * (coresize-2*mindistance+1); round++) { 
		/* One head is enough, because both processes queues are
		 * interleaved. */
		insn_t **head;

		/* Calculate position of second warrior. */
		u32_t pos2 = mindistance + round/2;
		
		/* Set up core. */
		memset(core, 0, coresize * sizeof(insn_t));

		/* Load warriors. */
		memcpy(core, warriors[0].insns,
		       warriors[0].length * sizeof(insn_t));
		memcpy(core + pos2, warriors[1].insns,
		       warriors[1].length * sizeof(insn_t));

		/* Set execution order and initialize both process queues. */
		if (round % 2 == 0) {
			pqueue[0] = core + warriors[0].start;
			pq_info1.tail = pqueue + 2;
			pq_info1.numprocesses = 1;

			pqueue[1] = core + pos2 + warriors[1].start;
			pq_info2.tail = pqueue + 3;
			pq_info2.numprocesses = 1;

			pqinfo = &pq_info1;
		} else {
			pqueue[1] = core + warriors[0].start;
			pq_info1.tail = pqueue + 3;
			pq_info1.numprocesses = 1;

			pqueue[0] = core + pos2 + warriors[1].start;
			pq_info2.tail = pqueue + 2;
			pq_info2.numprocesses = 1;

			pqinfo = &pq_info2;
		}
		head = pqueue;

		/* Run one round. */
		cycle = 2 * maxcycles;

		/* Update instruction counter with preliminary number of
		 * executed instructions. */
		insn_counter += 2 * maxcycles;
		do {
			insn_t *ip;    /* Pointer to the currently executed
				        * instruction. */
			field_t ra_a;  /* A register values */
			field_t ra_b;
			field_t rb_a;  /* B register values */
			field_t rb_b;
			insn_t *aAddr; /* Pointer to address specified by
					* A operand. */
			insn_t *bAddr; /* Pointer to address specified by
					* B operand. */

			field_t *tmp_field;

			/* Get instruction pointer. */
			PQ_BI_NEXT(ip);

			/* Optimized instructions. */
			switch(ip->insn) {
			case INSN(MOV, MODIFIER_I, IMMEDIATE, DIRECT):
				/* Evaluate A-operand. */
				/* Nothing to do. */

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *ip;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, DIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, A_INDIRECT):
				/* Evaluate A-operand. */
				/* Nothing to do. */

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);

				/* Copy complete instruction. */
				*bAddr = *ip;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, DIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DAT, MODIFIER_F, DIRECT, DIRECT):
				/* No need to evaluate A-operand. */

				/* No need to evaluate B-operand. */

				/* Current processes dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = ip->insn;
				bAddr->a = ip->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ra_a = ip->a;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = ip->insn;
				bAddr->a = ra_a;
				bAddr->b = ip->b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, IMMEDIATE, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ra_a = ip->a;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);

				/* Copy complete instruction. */
				bAddr->insn = ip->insn;
				bAddr->a = ra_a;
				bAddr->b = ip->b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, A_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, IMMEDIATE):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DAT, MODIFIER_F, B_PREDECREMENT, DIRECT):
				/* Partially evaluate A-operand. */
				/* Use ip instead of aAddr. */
				ADDADDR(ip, ip->a);
				DECMOD(ip->b);

				/* No need to evaluate B-operand. */

				/* Current processes dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, B_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, DIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = ra_a;
				bAddr->b = aAddr->b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, B_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, A_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, A_POSTINCREMENT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				INCMOD(*tmp_field);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = ra_a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_B, DIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_b = bAddr->b;

				DECMOD(bAddr->b);
				if (rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, IMMEDIATE, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				INCMOD(bAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(JMP, MODIFIER_B, DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				/* No need to save ra_a or ra_b. */

				/* No need to evaluate B-operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND(aAddr);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(SPL, MODIFIER_B, B_INDIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(JMP, MODIFIER_B, DIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				/* No need to save ra_a or ra_b. */

				/* Partially evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND(aAddr);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_B, DIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_b = ip->b;

				DECMOD(bAddr->b);
				if (rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DAT, MODIFIER_F, B_POSTINCREMENT, B_POSTINCREMENT):
				/* Save value, because it might be
				 * changed by evaluation of A-operand. */
				rb_b = ip->b;

				/* Partially evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				INCMOD(aAddr->b);

				/* Partially evaluate B-operand. */
				/* Use ip instead of bAddr. */
				ADDADDR(ip, rb_b);
				INCMOD(ip->b);

				/* Current processes dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(DJN, MODIFIER_F, DIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;

				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if ((rb_a == 1) && (rb_b == 1)) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(MOV, MODIFIER_I, DIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			case INSN(ADD, MODIFIER_F, DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Execute instruction. */
				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Switch to next warrior. */
				pqinfo = pqinfo->next;
				continue;
			}

			/* Evaluate A- and B-operand. */
			switch (GET_MODES(ip->insn)) {
			case MODES(IMMEDIATE, IMMEDIATE):
				/* Evaluate A- and B-operand. */
				aAddr = bAddr = ip;
				ra_a = rb_a = ip->a;
				ra_b = rb_b = ip->b;
				break;
			case MODES(IMMEDIATE, DIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, B_INDIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, B_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(IMMEDIATE, A_INDIRECT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, A_PREDECREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(IMMEDIATE, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				aAddr = ip;
				ra_a = ip->a;
				ra_b = ip->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(DIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				rb_b = ip->b;
				break;
			case MODES(DIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(DIRECT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(DIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_INDIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				rb_b = ip->b;
				break;
			case MODES(B_INDIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_INDIRECT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);							DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_INDIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_PREDECREMENT, IMMEDIATE):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				break;
			case MODES(B_PREDECREMENT, DIRECT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, B_INDIRECT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, B_PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, B_POSTINCREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_PREDECREMENT, A_INDIRECT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, A_PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_PREDECREMENT, A_POSTINCREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_POSTINCREMENT, IMMEDIATE):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				break;
			case MODES(B_POSTINCREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);
	
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, B_INDIRECT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, B_PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, B_POSTINCREMENT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(B_POSTINCREMENT, A_INDIRECT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, A_PREDECREMENT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);
				
				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(B_POSTINCREMENT, A_POSTINCREMENT):
				rb_b = ip->b;
				
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_INDIRECT, IMMEDIATE):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_a = ip->a;
				rb_b = ip->b;
				break;
			case MODES(A_INDIRECT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_INDIRECT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_INDIRECT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_PREDECREMENT, IMMEDIATE):
				rb_a = ip->a;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_b = ip->b;
				break;
			case MODES(A_PREDECREMENT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_PREDECREMENT,A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_PREDECREMENT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_POSTINCREMENT, IMMEDIATE):
				rb_a = ip->a;

				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				bAddr = ip;
				rb_b = ip->b;
				break;
			case MODES(A_POSTINCREMENT, DIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, B_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, B_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, B_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			case MODES(A_POSTINCREMENT, A_INDIRECT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, A_PREDECREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				break;
			case MODES(A_POSTINCREMENT, A_POSTINCREMENT):
				/* Evaluate A-operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				tmp_field = &(aAddr->a);
				ADDADDR(aAddr, aAddr->a);
				ra_a = aAddr->a;
				ra_b = aAddr->b;
				INCMOD(*tmp_field);

				/* Evaluate B-operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				tmp_field = &(bAddr->a);
				ADDADDR(bAddr, bAddr->a);
				rb_a = bAddr->a;
				rb_b = bAddr->b;
				INCMOD(*tmp_field);
				break;
			default:
				/* Set values to prevent compiler warnings
				 * about uninitialized variables. */
				ra_a = ra_b = rb_a = rb_b = 0;
				aAddr = bAddr = NULL;

				/* This shouldn't happen. */
				printf ("Found invalid modes.");
			}
	
			/* Switch on opcode/modifier. Cases are roughly
			 * sorted according to the frequency of the opcodes. */
			switch (GET_OP_MOD(ip->insn)) {
			/*
			 * MOV
			 */
			case OP_MOD(MOV, MODIFIER_A):
				bAddr->a = ra_a;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_F):
				bAddr->a = ra_a;
			case OP_MOD(MOV, MODIFIER_B):
				bAddr->b = ra_b;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_AB):
				bAddr->b = ra_a;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_X):
				bAddr->b = ra_a;
			case OP_MOD(MOV, MODIFIER_BA):
				bAddr->a = ra_b;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MOV, MODIFIER_I):
				bAddr->insn = aAddr->insn;
				bAddr->a = ra_a;
				bAddr->b = ra_b;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * DAT
			 */
			case OP_MOD(DAT, MODIFIER_A):
			case OP_MOD(DAT, MODIFIER_B):
			case OP_MOD(DAT, MODIFIER_F):
			case OP_MOD(DAT, MODIFIER_I):
			case OP_MOD(DAT, MODIFIER_X):
			case OP_MOD(DAT, MODIFIER_AB):
			case OP_MOD(DAT, MODIFIER_BA):
				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--pqinfo->numprocesses == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				break;
			/*
			 * SPL
			 */
			case OP_MOD(SPL, MODIFIER_A):
			case OP_MOD(SPL, MODIFIER_B):
			case OP_MOD(SPL, MODIFIER_F):
			case OP_MOD(SPL, MODIFIER_I):
			case OP_MOD(SPL, MODIFIER_X):
			case OP_MOD(SPL, MODIFIER_AB):
			case OP_MOD(SPL, MODIFIER_BA):
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BI_APPEND(aAddr);
				}
				break;
			/*
			 * DJN
			 */
			case OP_MOD(DJN, MODIFIER_BA):
			case OP_MOD(DJN, MODIFIER_A):
				DECMOD(bAddr->a);
				if (rb_a == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}
				break;
			case OP_MOD(DJN, MODIFIER_AB):
			case OP_MOD(DJN, MODIFIER_B):
				DECMOD(bAddr->b);
				if (rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}
				break;
			case OP_MOD(DJN, MODIFIER_X):
			case OP_MOD(DJN, MODIFIER_I):
			case OP_MOD(DJN, MODIFIER_F):
				DECMOD(bAddr->a);
				DECMOD(bAddr->b);
				if (rb_a == 1 && rb_b == 1) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				}
				break;
			/*
			 * SUB
			 */
			case OP_MOD(SUB, MODIFIER_I):
			case OP_MOD(SUB, MODIFIER_F):
				SUBMOD(bAddr->b, rb_b, ra_b);
			case OP_MOD(SUB, MODIFIER_A):
				SUBMOD(bAddr->a, rb_a, ra_a);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SUB, MODIFIER_B):
				SUBMOD(bAddr->b, rb_b, ra_b);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SUB, MODIFIER_X):
				SUBMOD(bAddr->a, rb_a, ra_b);
			case OP_MOD(SUB, MODIFIER_AB):
				SUBMOD(bAddr->b, rb_b, ra_a);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SUB, MODIFIER_BA):
				SUBMOD(bAddr->a, rb_a, ra_b);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * JMZ
			 */
			case OP_MOD(JMZ, MODIFIER_BA):
			case OP_MOD(JMZ, MODIFIER_A):
				if (rb_a == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMZ, MODIFIER_AB):
			case OP_MOD(JMZ, MODIFIER_B):
				if (rb_b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMZ, MODIFIER_X):
			case OP_MOD(JMZ, MODIFIER_F):
			case OP_MOD(JMZ, MODIFIER_I):
				if ((rb_a == 0) && (rb_b == 0)) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			/*
			 * ADD
			 */
			case OP_MOD(ADD, MODIFIER_I):
			case OP_MOD(ADD, MODIFIER_F):
				ADDMOD(bAddr->b, ra_b, rb_b);
			case OP_MOD(ADD, MODIFIER_A):
				ADDMOD(bAddr->a, ra_a, rb_a);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(ADD, MODIFIER_B):
				ADDMOD(bAddr->b, ra_b, rb_b);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(ADD, MODIFIER_X):
				ADDMOD(bAddr->a, ra_b, rb_a);
			case OP_MOD(ADD, MODIFIER_AB):
				ADDMOD(bAddr->b, ra_a, rb_b);
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(ADD, MODIFIER_BA):
				ADDMOD(bAddr->a, ra_b, rb_a);

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * JMN
			 */
			case OP_MOD(JMN, MODIFIER_BA):
			case OP_MOD(JMN, MODIFIER_A):
				if (rb_a != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMN, MODIFIER_AB):
			case OP_MOD(JMN, MODIFIER_B):
				if (rb_b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(JMN, MODIFIER_X):
			case OP_MOD(JMN, MODIFIER_F):
			case OP_MOD(JMN, MODIFIER_I):
				if ((rb_a != 0) || (rb_b != 0)) {
					/* Queue jump destination. */
					PQ_BI_APPEND(aAddr);
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			/*
			 * SNE
			 */
			case OP_MOD(SNE, MODIFIER_A):
				if (ra_a != rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_B):
				if (ra_b != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_AB):
				if (ra_a != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_BA):
				if (ra_b != rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_I):
				if (aAddr->insn != bAddr->insn ||
				    ra_a != rb_a || ra_b != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_F):
				if (ra_a != rb_a || ra_b != rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SNE, MODIFIER_X):
				if (ra_a != rb_b || ra_b != rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * JMP
			 */
			case OP_MOD(JMP, MODIFIER_A):
			case OP_MOD(JMP, MODIFIER_B):
			case OP_MOD(JMP, MODIFIER_F):
			case OP_MOD(JMP, MODIFIER_I):
			case OP_MOD(JMP, MODIFIER_X):
			case OP_MOD(JMP, MODIFIER_AB):
			case OP_MOD(JMP, MODIFIER_BA):
				/* Queue jump destination. */
				PQ_BI_APPEND(aAddr);
				break;
			/*
			 * SEQ
			 */
			case OP_MOD(SEQ, MODIFIER_A):
				if (ra_a == rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_B):
				if (ra_b == rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_AB):
				if (ra_a == rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_BA):
				if (ra_b == rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_I):
				if (ra_a == rb_a && ra_b == rb_b &&
				    aAddr->insn == bAddr->insn) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_F):
				if (ra_a == rb_a && ra_b == rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SEQ, MODIFIER_X):
				if (ra_a == rb_b && ra_b == rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * SLT
			 */
			case OP_MOD(SLT, MODIFIER_A):
				if (ra_a < rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_B):
				if (ra_b < rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_AB):
				if (ra_a < rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_BA):
				if (ra_b < rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_I):
			case OP_MOD(SLT, MODIFIER_F):
				if (ra_a < rb_a && ra_b < rb_b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(SLT, MODIFIER_X):
				if (ra_a < rb_b && ra_b < rb_a) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;				
			/*
			 * MUL
			 */
			case OP_MOD(MUL, MODIFIER_I):
			case OP_MOD(MUL, MODIFIER_F):
				bAddr->b = (rb_b * ra_b) % coresize;
			case OP_MOD(MUL, MODIFIER_A):
				bAddr->a = (rb_a * ra_a) % coresize;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MUL, MODIFIER_B):
				bAddr->b = (rb_b * ra_b) % coresize;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MUL, MODIFIER_X):
				bAddr->a = (rb_a * ra_b) % coresize;
			case OP_MOD(MUL, MODIFIER_AB):
				bAddr->b = (rb_b * ra_a) % coresize;
				
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			case OP_MOD(MUL, MODIFIER_BA):
				bAddr->a = (rb_a * ra_b) % coresize;

				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * DIV
			 */
			case OP_MOD(DIV, MODIFIER_I):
			case OP_MOD(DIV, MODIFIER_F):
				if (ra_a) bAddr->a = rb_a / ra_a;
				if (ra_b) bAddr->b = rb_b / ra_b;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_X):
				if (ra_b) bAddr->a = rb_a / ra_b;
				if (ra_a) bAddr->b = rb_b / ra_a;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_A):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a / ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_B):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b / ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_AB):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b / ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(DIV, MODIFIER_BA):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a / ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;

			/*
			 * NOP
			 */
			case OP_MOD(NOP, MODIFIER_A):
			case OP_MOD(NOP, MODIFIER_B):
			case OP_MOD(NOP, MODIFIER_F):
			case OP_MOD(NOP, MODIFIER_I):
			case OP_MOD(NOP, MODIFIER_X):
			case OP_MOD(NOP, MODIFIER_AB):
			case OP_MOD(NOP, MODIFIER_BA):
				/* Nothing to do. */
				/* Queue next instruction. */
				IPINCMOD(ip);
				PQ_BI_APPEND(ip);
				break;
			/*
			 * MOD
			 */
			case OP_MOD(MOD, MODIFIER_I):
			case OP_MOD(MOD, MODIFIER_F):
				if (ra_a) bAddr->a = rb_a % ra_a;
				if (ra_b) bAddr->b = rb_b % ra_b;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_X):
				if (ra_b) bAddr->a = rb_a % ra_b;
				if (ra_a) bAddr->b = rb_b % ra_a;
				if (!ra_a || !ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_A):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a % ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_B):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b % ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_AB):
				if (!ra_a) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->b = rb_b % ra_a;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;
			case OP_MOD(MOD, MODIFIER_BA):
				if (!ra_b) {
					/* Current process dies. Finish this
					 * round, if warrior is dead. */
					if (--pqinfo->numprocesses == 0) {
						/* Update instruction counter.
						 */
						insn_counter -= cycle - 1;

						cycle = 1;
					}
				} else {
					bAddr->a = rb_a % ra_b;
					
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND(ip);
				}
				break;			
			default:
				/* This shouldn't happen. */
				printf("Invalid instruction found. "\
				       "You have a problem!\n");
			}

			/* Switch to next warriors. */
			pqinfo = pqinfo->next;
		} while (--cycle);

		/* Accumulate results. */
		if (pq_info1.numprocesses == 0) {
			results[1]++; /* First warrior has lost. */
		} else if (pq_info2.numprocesses == 0) {
			results[0]++; /* First warrior has won. */
		} else {
			results[2]++; /* Tie. */
		}
	}

	/* Save number of executed instructions. */
	results[3] = insn_counter;

	/* Clean up. */
	free(pqueue);

	return results;
}

u64_t *mw_run_94nop(insn_t *core, u32_t numwarriors, warrior_t *warriors,
	            u32_t coresize, u32_t maxprocesses, u32_t maxcycles,
	            u32_t mindistance, u32_t numrounds, u32_t seed)
{
	return NULL;
}
