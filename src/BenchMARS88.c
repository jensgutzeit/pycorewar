/* BenchMARS88.c 
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
#include <BenchMARS88.h>

/* Run a two warrior fight. Parameters are NOT checked for plausibility!
 * Return the result after the fights or NULL on error. */
u64_t *
run_88(insn_t *core, warrior_t *warriors, u32_t coresize, u32_t maxprocesses,
       u32_t maxcycles, u32_t mindistance, u32_t numrounds, u32_t seed)
{
	u32_t round; 		/* Number of the current round. */
	u64_t *results; 	/* List with the results, format: w l t. */
	u64_t insn_counter = 0; /* Counter of executed instructions. */
	u32_t pos2; 		/* Position of second warrior in the core. */
	u32_t cycle; 		/* Number of current cycle. */
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

	/* Run the given number of rounds. */
	for (round = 0; round < numrounds; round++) {
		/* One head is enough, because both processes queues are
		 * interleaved. */
		insn_t **head;
		
		insn_t **tail1;
		insn_t **tail2;

		u32_t numprocesses1;
		u32_t numprocesses2;

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
			tail1 = pqueue + 2;

			pqueue[1] = core + pos2 + warriors[1].start;
			tail2 = pqueue + 3;
		} else {
			pqueue[1] = core + warriors[0].start;
			tail2 = pqueue + 3;

			pqueue[0] = core + pos2 + warriors[1].start;
			tail1 = pqueue + 2;
		}
		numprocesses1 = numprocesses2 = 1;
		head = pqueue;

		/* Run one round. */
		cycle = 2 * maxcycles;

		/* Update instruction counter with preliminary number of
		 * executed instructions. */
		insn_counter += 2 * maxcycles;
		do {
			insn_t *ip;    /* Pointer to the currently executed
				        * instruction. */
			field_t ra_b;  /* A register value */
			field_t rb_b;  /* B register value */
			insn_t *aAddr; /* Pointer to address specified by
					* A operand. */
			insn_t *bAddr; /* Pointer to address specified by
					* B operand. */

			/* Get instruction pointer. */
			PQ_BI_NEXT1(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue1;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue1;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue1;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue1;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue1;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue1;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue1;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue1;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue1;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto no_queue1;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue1:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND1(ip);

no_queue1:		/* Do nothing. Process queue1 is handled by
			 * instruction. */

			if (--cycle == 0) goto round_done;

			/* Get instruction pointer. */
			PQ_BI_NEXT2(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue2;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue2;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue2;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue2;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue2;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue2;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue2;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue2;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue2;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto no_queue2;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue2:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND2(ip);

no_queue2:		/* Do nothing. Process queue2 is handled by
			 * instruction. */

			if (--cycle == 0) goto round_done;
			/* Get instruction pointer. */
			PQ_BI_NEXT1(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue3;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue3;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue3;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue3;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue3;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue3;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue3;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue3;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue3;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto no_queue3;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue3:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND1(ip);

no_queue3:		/* Do nothing. Process queue3 is handled by
			 * instruction. */

			if (--cycle == 0) goto round_done;

			/* Get instruction pointer. */
			PQ_BI_NEXT2(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue4;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue4;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue4;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue4;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue4;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue4;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue4;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue4;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue4;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto no_queue4;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue4:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND2(ip);

no_queue4:		/* Do nothing. Process queue4 is handled by
			 * instruction. */
			if (0);
		} while (--cycle);

round_done:	/* Accumulate results. */
		if (round % 2 == 0) {
			if (numprocesses1 == 0) {
				results[1]++; /* First warrior has lost. */
			} else if (numprocesses2 == 0) {
				results[0]++; /* First warrior has won. */
			} else {
				results[2]++; /* Tie. */
			}
		} else {
			if (numprocesses1 == 0) {
				results[0]++; /* Second warrior has lost. */
			} else if (numprocesses2 == 0) {
				results[1]++; /* Second warrior has won. */
			} else {
				results[2]++; /* Tie. */
			}
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
p_run_88(insn_t *core, warrior_t *warriors, u32_t coresize, u32_t maxprocesses,
       u32_t maxcycles, u32_t mindistance)
{
	u32_t round; 		/* Number of the current round. */
	u64_t *results; 	/* List with the results, format: w l t. */
	u64_t insn_counter = 0; /* Counter of executed instructions. */
	u32_t cycle; 		/* Number of current cycle. */
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

	/* Run the given number of rounds. */
	for (round = 0; round < 2 * (coresize-2*mindistance+1); round++) {
		/* One head is enough, because both processes queues are
		 * interleaved. */
		insn_t **head;
		
		insn_t **tail1;
		insn_t **tail2;

		u32_t numprocesses1;
		u32_t numprocesses2;

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
			tail1 = pqueue + 2;

			pqueue[1] = core + pos2 + warriors[1].start;
			tail2 = pqueue + 3;
		} else {
			pqueue[1] = core + warriors[0].start;
			tail2 = pqueue + 3;

			pqueue[0] = core + pos2 + warriors[1].start;
			tail1 = pqueue + 2;
		}
		numprocesses1 = numprocesses2 = 1;
		head = pqueue;

		/* Run one round. */
		cycle = 2 * maxcycles;

		/* Update instruction counter with preliminary number of
		 * executed instructions. */
		insn_counter += 2 * maxcycles;
		do {
			insn_t *ip;    /* Pointer to the currently executed
				        * instruction. */
			field_t ra_b;  /* A register value */
			field_t rb_b;  /* B register value */
			insn_t *aAddr; /* Pointer to address specified by
					* A operand. */
			insn_t *bAddr; /* Pointer to address specified by
					* B operand. */

			/* Get instruction pointer. */
			PQ_BI_NEXT1(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue1;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue1;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue1;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue1;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue1;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue1;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue1;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue1;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue1;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue1;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue1;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue1;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue1;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue1;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue1;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto no_queue1;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue1;
				}
				goto queue1;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue1;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue1:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND1(ip);

no_queue1:		/* Do nothing. Process queue1 is handled by
			 * instruction. */

			if (--cycle == 0) goto round_done;

			/* Get instruction pointer. */
			PQ_BI_NEXT2(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue2;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue2;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue2;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue2;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue2;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue2;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue2;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue2;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue2;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue2;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue2;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue2;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue2;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue2;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue2;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto no_queue2;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue2;
				}
				goto queue2;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue2;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue2:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND2(ip);

no_queue2:		/* Do nothing. Process queue2 is handled by
			 * instruction. */

			if (--cycle == 0) goto round_done;
			/* Get instruction pointer. */
			PQ_BI_NEXT1(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue3;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue3;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue3;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue3;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue3;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue3;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue3;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND1(ip);

				/* Queue new process, if possible. */
				if (numprocesses1 < maxprocesses) {
					numprocesses1++;
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue3;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue3;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses1 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue3;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND1(aAddr);
				goto no_queue3;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND1(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
				}
				goto no_queue3;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue3;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue3;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue3;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto no_queue3;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND1(aAddr);
					goto no_queue3;
				}
				goto queue3;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue3;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue3:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND1(ip);

no_queue3:		/* Do nothing. Process queue3 is handled by
			 * instruction. */

			if (--cycle == 0) goto round_done;

			/* Get instruction pointer. */
			PQ_BI_NEXT2(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue4;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue4;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue4;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue4;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue4;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue4;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue4;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BI_APPEND2(ip);

				/* Queue new process, if possible. */
				if (numprocesses2 < maxprocesses) {
					numprocesses2++;
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. Finish this						 * round, if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue4;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue4;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. Finish this round,
				 * if warrior is dead. */
				if (--numprocesses2 == 0) {
					/* Update instruction counter. */
					insn_counter -= cycle - 1;

					cycle = 1;
				}
				goto no_queue4;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BI_APPEND2(aAddr);
				goto no_queue4;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BI_APPEND2(ip);
				} else {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
				}
				goto no_queue4;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue4;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue4;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue4;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto no_queue4;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BI_APPEND2(aAddr);
					goto no_queue4;
				}
				goto queue4;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue4;
			default:
				/* Found invalid instruction */
				results[4] = MARS_88_INVALID_INSN;
				cycle = 1;
			}

queue4:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BI_APPEND2(ip);

no_queue4:		/* Do nothing. Process queue4 is handled by
			 * instruction. */
			if (0);
		} while (--cycle);

round_done:	/* Accumulate results. */
		if (round % 2 == 0) {
			if (numprocesses1 == 0) {
				results[1]++; /* First warrior has lost. */
			} else if (numprocesses2 == 0) {
				results[0]++; /* First warrior has won. */
			} else {
				results[2]++; /* Tie. */
			}
		} else {
			if (numprocesses1 == 0) {
				results[0]++; /* Second warrior has lost. */
			} else if (numprocesses2 == 0) {
				results[1]++; /* Second warrior has won. */
			} else {
				results[2]++; /* Tie. */
			}
		}
	}

	/* Save number of executed instructions. */
	results[3] = insn_counter;
	
	/* Clean up. */
	free(pqueue);

	return results;
}

/* Run a multiwarrior fight. Parameters are NOT checked for plausibility!
 * Return the result after the fights or NULL on error. */
u64_t *
mw_run_88(insn_t *core, u32_t numwarriors, warrior_t *warriors, u32_t coresize,
	  u32_t maxprocesses, u32_t maxcycles, u32_t mindistance,
	  u32_t numrounds, u32_t seed)
{
	u32_t round; 		/* Number of the current round. */
	u64_t *results; 	/* List with the results, format: w l t. */
	u64_t insn_counter = 0; /* Counter of executed instructions. */
	u32_t *positions; 	/* Position of all warriors in the core. */
	u32_t numalive;		/* Number of currently living warriors. */
	u32_t cycle; 		/* Number of current cycle. */
	pq_info_t *pq_infos; 	/* Information about processes queue of
				 * all warriors. */
	pq_info_t *pqinfo;
	insn_t **pqueue; 	/* Memory for the process queues. */
	u32_t i;
	u32_t adj;

	/* Pointer to end of core. */
	insn_t *core_end = core + coresize;

	/* Allocate memory for the process queues. */
	pqueue = calloc(numwarriors * (maxcycles + maxprocesses),
			sizeof(insn_t *));

	/* Allocate memory for information about the process queues. */
	pq_infos = calloc(numwarriors, sizeof(pq_info_t));

	/* Allocate and initialized memory for results. */
	results = calloc(numwarriors * (numwarriors + 1) + 1, sizeof(u64_t));

	/* Allocate memory for positions of all warriors. */
	positions = calloc(numwarriors, sizeof(s32_t));

	/* Check allocated memory. */
	if ((pqueue == NULL) | (pq_infos == NULL) | (results == NULL)) {
		free(positions);
		free(results);
		free(pq_infos);
		free(pqueue);
		return NULL;
	}

	/* Adjust seed to make the second warrior be at position seed in
	 * the first round. */
	seed -= mindistance;

	/* "First" warrior is always at positions 0. */
	positions[0] = 0;

	/* Run the given number of rounds. */
	for (round = 0; round < numrounds; round++) {
		/* Calculate positions of remaining warriors. */
		if (calc_pos(positions, numwarriors, coresize, mindistance,
			     &seed)) {
			calc_pos_backup(positions, numwarriors, coresize,
					mindistance, &seed);
		}
	
		/* Set up core. */
		memset(core, 0, coresize * sizeof(insn_t));

		/* Load warriors into core and setup all process queues. */
		insn_t **tmp_pqueue = pqueue;
		for (i = 0; i < numwarriors; i++) {
			/* Load warrior. */
			memcpy(core + positions[i], warriors[i].insns,
			       warriors[i].length * sizeof(insn_t));

			/* Set up process queue. */
			*tmp_pqueue = core + positions[i] + warriors[i].start;
			pq_infos[i].head = tmp_pqueue;
			pq_infos[i].tail = tmp_pqueue + 1;
			pq_infos[i].numprocesses = 1;

			/* Set next and previous warrior. */
			pq_infos[i].next = &pq_infos[(i + 1) % numwarriors];
			pq_infos[i].prev = &pq_infos[(i + numwarriors - 1) %
						     numwarriors];
			tmp_pqueue += maxprocesses + 1;
		}

		/* Set first warrior to execute. */
		pqinfo = &pq_infos[round % numwarriors];

		/* Run one round. */
		cycle = numwarriors * maxcycles;
		numalive = numwarriors;
		insn_counter += numwarriors * maxcycles;
		do {
			insn_t *ip;    /* Pointer to the currently executed
				        * instruction. */
			field_t ra_b;  /* A register value */
			field_t rb_b;  /* B register value */
			insn_t *aAddr; /* Pointer to address specified by
					* A operand. */
			insn_t *bAddr; /* Pointer to address specified by
					* B operand. */

			/* Get instruction pointer. */
			PQ_BNI_NEXT(ip);

			/* Switch on opcode/amode/bmode. */
			switch (ip->insn) {
			/*
	 		 * MOV
	 		 */
			case INSN88(MOV, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue;
			case INSN88(MOV, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue;
			case INSN88(MOV, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue;
			case INSN88(MOV, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* Save value, because it might
					       * be changed by A operand. */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);	

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue;
			case INSN88(MOV, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);	
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue;
			case INSN88(MOV, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);	

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue;
			case INSN88(MOV, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue;
			case INSN88(MOV, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				/* Copy complete instruction. */
				*bAddr = *aAddr;
		
				goto queue;
			case INSN88(MOV, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b;
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				/* Copy complete instruction. */
				bAddr->insn = aAddr->insn;
				bAddr->a = aAddr->a;
				bAddr->b = ra_b;
		
				goto queue;
			case INSN88(MOV, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue;
			case INSN88(MOV, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue;
			case INSN88(MOV, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);
						
				/* Copy A field to B field. */
				bAddr->b = ip->a;
		
				goto queue;
			/*
	 		 * SPL
	 		 */
			case INSN88(SPL, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);	
			case INSN88(SPL, DIRECT, IMMEDIATE):
			case INSN88(SPL, DIRECT, DIRECT):
			case INSN88(SPL, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BNI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(SPL, INDIRECT, IMMEDIATE):
			case INSN88(SPL, INDIRECT, DIRECT):
			case INSN88(SPL, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BNI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(SPL, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BNI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(SPL, PREDECREMENT, IMMEDIATE):
			case INSN88(SPL, PREDECREMENT, DIRECT):
			case INSN88(SPL, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BNI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(SPL, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
		
				/* Queue next instruction */
				IPINCMOD(ip);
				PQ_BNI_APPEND(ip);

				/* Queue new process, if possible. */
				if (pqinfo->numprocesses < maxprocesses) {
					pqinfo->numprocesses++;
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			/*
			 * DAT
			 */
			case INSN88(DAT, IMMEDIATE, PREDECREMENT):
				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(DAT, IMMEDIATE, IMMEDIATE):
			case INSN88(DAT, DIRECT, DIRECT):
				/* DAT $ x, $ y isn't a valid instruction,
				 * but can be executed nonetheless! */

				/* Current process dies. */ 
				if (--pqinfo->numprocesses == 0) {
					/* Adjust list of living warriors. */
					pqinfo->prev->next = pqinfo->next;
					pqinfo->next->prev = pqinfo->prev;

					/* Update instruction counter. */
					adj = 1 + (cycle - 1) / numalive;
					insn_counter -= adj;

					/* Adjust number of remaining cycles.
					 * and living warriors. */
					cycle -= adj;

					numalive--;
				}
				goto no_queue;
			case INSN88(DAT, PREDECREMENT, IMMEDIATE):
				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Current process dies. */
				if (--pqinfo->numprocesses == 0) {
					/* Adjust list of living warriors. */
					pqinfo->prev->next = pqinfo->next;
					pqinfo->next->prev = pqinfo->prev;

					/* Update instruction counter. */
					adj = 1 + (cycle - 1) / numalive;
					insn_counter -= adj;

					/* Adjust number of remaining cycles.
					 * and living warriors. */
					cycle -= adj;

					numalive--;
				}
				goto no_queue;
			case INSN88(DAT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Partially evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);

				/* Partially evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Current process dies. */
				if (--pqinfo->numprocesses == 0) {
					/* Adjust list of living warriors. */
					pqinfo->prev->next = pqinfo->next;
					pqinfo->next->prev = pqinfo->prev;

					/* Update instruction counter. */
					adj = 1 + (cycle - 1) / numalive;
					insn_counter -= adj;

					/* Adjust number of remaining cycles.
					 * and living warriors. */
					cycle -= adj;

					numalive--;
				}
				goto no_queue;
			/*
			 * JMP
			 */
			case INSN88(JMP, DIRECT, PREDECREMENT):
				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
			case INSN88(JMP, DIRECT, IMMEDIATE):
			case INSN88(JMP, DIRECT, DIRECT):
			case INSN88(JMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* No need to evaluate B operand. */
	
				/* Queue jump destination. */
				PQ_BNI_APPEND(aAddr);
				goto no_queue;
			case INSN88(JMP, INDIRECT, IMMEDIATE):
			case INSN88(JMP, INDIRECT, DIRECT):
			case INSN88(JMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BNI_APPEND(aAddr);
				goto no_queue;
			case INSN88(JMP, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BNI_APPEND(aAddr);
				goto no_queue;
			case INSN88(JMP, PREDECREMENT, IMMEDIATE):
			case INSN88(JMP, PREDECREMENT, DIRECT):
			case INSN88(JMP, PREDECREMENT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* No need to evaluate B operand. */

				/* Queue jump destination. */
				PQ_BNI_APPEND(aAddr);
				goto no_queue;
			case INSN88(JMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Partially evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);

				/* Queue jump destination. */
				PQ_BNI_APPEND(aAddr);
				goto no_queue;
			/*
			 * DJN
			 */
			case INSN88(DJN, DIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto  no_queue;
			case INSN88(DJN, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, INDIRECT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, PREDECREMENT, IMMEDIATE):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				/* Nothing to do. */

				DECMOD(ip->b);
				if (ip->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			case INSN88(DJN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				DECMOD(bAddr->b);
				if (bAddr->b == 0) {
					/* Queue next instruction. */
					IPINCMOD(ip);
					PQ_BNI_APPEND(ip);
				} else {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
				}
				goto no_queue;
			/*
			 * ADD
			 */
			case INSN88(ADD, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue;
			case INSN88(ADD, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue;
			case INSN88(ADD, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->b, bAddr->b, ip->a);

				goto queue;
			case INSN88(ADD, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(ADD, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(ADD, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue;
			case INSN88(ADD, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(ADD, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(ADD, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue;
			case INSN88(ADD, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(ADD, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(ADD, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				ADDMOD(bAddr->a, bAddr->a, aAddr->a);
				ADDMOD(bAddr->b, bAddr->b, ra_b);

				goto queue;
			/*
			 * SUB
			 */
			case INSN88(SUB, IMMEDIATE, DIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue;
			case INSN88(SUB, IMMEDIATE, INDIRECT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue;
			case INSN88(SUB, IMMEDIATE, PREDECREMENT):
				/* Evaluate A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->b, bAddr->b, ip->a);

				goto queue;
			case INSN88(SUB, DIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(SUB, DIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(SUB, DIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue;
			case INSN88(SUB, INDIRECT, DIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(SUB, INDIRECT, INDIRECT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(SUB, INDIRECT, PREDECREMENT):
				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue;
			case INSN88(SUB, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(SUB, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, aAddr->b);

				goto queue;
			case INSN88(SUB, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evaluate A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				SUBMOD(bAddr->a, bAddr->a, aAddr->a);
				SUBMOD(bAddr->b, bAddr->b, ra_b);

				goto queue;
			/*
			 * JMZ
			 */
			case INSN88(JMZ, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b == 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMZ, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto no_queue;
			case INSN88(JMZ, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b == 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			/*
			 * JMN
			 */
			case INSN88(JMN, DIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, DIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, DIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, DIRECT, PREDECREMENT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, INDIRECT, IMMEDIATE):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				/* Nothing to do. */

				if (ip->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, INDIRECT, DIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, INDIRECT, INDIRECT):
				/* Evaluate B operand first, because in this
				 * case no harm can be done. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Evalute A operand. */
					ASSIGNADDR(aAddr, ip, ip->a);
					ADDADDR(aAddr, aAddr->b);

					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, PREDECREMENT, IMMEDIATE):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				if (ip->b != 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, PREDECREMENT, DIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, PREDECREMENT, INDIRECT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			case INSN88(JMN, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b; /* == rb_b !!! */

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (bAddr->b != 0) {
					/* Queue jump destination. */
					PQ_BNI_APPEND(aAddr);
					goto no_queue;
				}
				goto queue;
			/*
			 * CMP
			 */
			case INSN88(CMP, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
		   		    aAddr->a    == bAddr->a &&
				    aAddr->b    == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(CMP, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */


				/* Evaluate B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->insn == bAddr->insn &&
				    aAddr->a    == bAddr->a &&
				    ra_b        == bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			/*
			 * SLT
			 */
			case INSN88(SLT, IMMEDIATE, DIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, IMMEDIATE, INDIRECT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, IMMEDIATE, PREDECREMENT):
				/* Evalute A operand. */
				/* Nothing to do. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ip->a < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, DIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, DIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);
				
				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, DIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, INDIRECT, DIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, INDIRECT, INDIRECT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, INDIRECT, PREDECREMENT):
				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, ip->b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, PREDECREMENT, DIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);

				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, PREDECREMENT, INDIRECT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				ADDADDR(bAddr, bAddr->b);

				if (aAddr->b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			case INSN88(SLT, PREDECREMENT, PREDECREMENT):
				rb_b = ip->b;

				/* Evalute A operand. */
				ASSIGNADDR(aAddr, ip, ip->a);
				DECMOD(aAddr->b);
				ADDADDR(aAddr, aAddr->b);
				ra_b = aAddr->b; /* Save value, because it
						  * might be changed by
						  * B operand. */
		
				/* Evalute B operand. */
				ASSIGNADDR(bAddr, ip, rb_b);
				DECMOD(bAddr->b);
				ADDADDR(bAddr, bAddr->b);

				if (ra_b < bAddr->b) {
					/* Skip next instruction. */
					IPINCMOD(ip);
				}
				goto queue;
			default:
				/* This shouldn't happen. */
				printf("Invalid instruction found. "\
				       "You have a problem!\n");
			}

queue:			/* Append next instruction. */
			IPINCMOD(ip);
			PQ_BNI_APPEND(ip);

no_queue:		/* Switch to next warrior. */
			pqinfo = pqinfo->next;
		} while ((--cycle > 0) & (numalive > 1));

		/* Update instruction counter. */
		insn_counter -= cycle;

		/* Accumulate results. */
		for (i = 0; i < numwarriors; i++) {
			if (pq_infos[i].numprocesses == 0) {
				/* Warrior died this round. */
				results[i*(numwarriors + 1) + numwarriors]++;
			} else {
				/* Warrior survived/won this round. */
				results[i*(numwarriors + 1) + numalive - 1]++;
			}
		}
	}

	/* Save number of executed instructions. */
	results[numwarriors * (numwarriors + 1)] = insn_counter;

	/* Clean up. */
	free(positions);
	free(pq_infos);
	free(pqueue);

	return results;
}
