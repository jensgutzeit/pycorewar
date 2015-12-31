/* BenchProcessQueue.h 
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

#ifndef COREWAR_BENCH_PROCESS_QUEUE__H
#define COREWAR_BENCH_PROCESS_QUEUE__H

/*  Old version. This will be removed soon! Do not use! */
typedef struct _pqueue_info_t {
	insn_t **head;			/* Next processes to run. */
	insn_t **tail;			/* Next free location for queueing
					 * a new process. */
	u32_t numprocesses;		/* Number of processes for this
					 * warrior. */
	struct _pqueue_info_t *next; 	/* Pointer to process queue of next
					 * living warrior. */
	struct _pqueue_info_t *prev; 	/* Pointer to process queue of previous
					 * living warrior. */
} pqueue_info_t;

/* Non-interleaved version. */
typedef struct _pq_ni_info_t {
	insn_t **head;			/* Next processes to run. */
	insn_t **tail;			/* Next free location for queueing
					 * a new process. */
	u32_t numprocesses;		/* Number of processes for this
					 * warrior. */
	struct _pq_ni_info_t *next; 	/* Pointer to process queue of next
					 * living warrior. */
	struct _pq_ni_info_t *prev; 	/* Pointer to process queue of previous
					 * living warrior. */
} pq_ni_info_t;

/* Interleaved version. */
typedef struct _pq_i_info_t {
	insn_t **tail;			/* Next free location for queueing
					 * a new process. */
	u32_t numprocesses;		/* Number of processes for this
					 * warrior. */
	struct _pq_i_info_t *next; 	/* Pointer to process queue of next
					 * living warrior. */
	struct _pq_i_info_t *prev; 	/* Pointer to process queue of previous
					 * living warrior. */
} pq_i_info_t;

/* Use only this type in your source code. */
#define pq_info_t	pq_ni_info_t

/* Macros for allocating memory and initializing the processes queue. */

/* Append a process to the processes queue without checking the size of
 * the queue. */

/* Old version. This will be removed soon! Do not use! */
#define PQUEUE_APPEND(x)	*pqinfo->tail++ = (x);			\
				if (pqinfo->tail == pqueue_end)		\
				             	pqinfo->tail = pqueue

/* Small non-interleaved version. */
#define PQ_SNI_APPEND(x)	*pqinfo->tail++ = (x);			\
				if (pqinfo->tail == pqueue_end)		\
					pqinfo->tail = pqueue

/* Small non-interleaved version for partially unrolled loops. */
#define PQ_SNI_APPEND1(x)	*tail1++ = (x);				\
				if (tail1 == pqueue_end) tail1 = pqueue

#define PQ_SNI_APPEND2(x)	*tail2++ = (x);				\
				if (tail2 == pqueue_end) tail2 = pqueue

/* Small interleaved version. */
#define PQ_SI_APPEND(x)		*pqinfo->tail = (x);			\
				pqinfo->tail += 2;			\
				if (pqinfo->tail >= pqueue_end)		\
					pqinfo->tail -= 2 * maxprocesses + 2

/* Small interleaved version for partially unrolled loops. */
#define PQ_SI_APPEND1(x)	*tail1 = (x);				\
				tail1 += 2;				\
				if (tail1 >= pqueue_end)		\
					tail1 -= 2 * maxprocesses + 2

#define PQ_SI_APPEND_88_STD1(x)	*tail1 = (x);				\
				tail1 += 2;				\
				if (tail1 >= pqueue_end)		\
					tail1 -= 2 * MARS88_STD_MAXPROCESSES + 2

#define PQ_SI_APPEND2(x)	*tail2 = (x);				\
				tail2 += 2;				\
				if (tail2 >= pqueue_end)		\
					tail2 -= 2 * maxprocesses + 2

#define PQ_SI_APPEND_88_STD2(x)	*tail2 = (x);				\
				tail2 += 2;				\
				if (tail2 >= pqueue_end)		\
					tail2 -= 2 * MARS88_STD_MAXPROCESSES + 2

/* Big non-interleaved version. */
#define PQ_BNI_APPEND(x)	*pqinfo->tail++ = (x)

/* Big non-interleaved version for partially unrolled loops. */
#define PQ_BNI_APPEND1(x)	*tail1++ = (x)

#define PQ_BNI_APPEND2(x)	*tail2++ = (x)

/* Big interleaved version. */
#define PQ_BI_APPEND(x)		*pqinfo->tail = (x);			\
				pqinfo->tail += 2

/* Big interleaved version for partially unrolled loops. */
#define PQ_BI_APPEND1(x)	*tail1 = (x);				\
				tail1 += 2

#define PQ_BI_APPEND2(x)	*tail2 = (x);				\
				tail2 += 2

/* Use only these macros in your source code. */
#define PQ_APPEND(x)		PQ_SNI_APPEND(x)

#define PQ_APPEND1(x)		PQ_SNI_APPEND1(x)

#define PQ_APPEND2(x)		PQ_SNI_APPEND2(x)

/* Return the next process and remove it from the process queue. */

/* Old version. This will be removed soon! Do not use! */
#define PQUEUE_NEXT()		*pqinfo->head++;			\
				if (pqinfo->head == pqueue_end) 	\
						pqinfo->head = pqueue

/* Small non-interleaved version. */
#define PQ_SNI_NEXT(ip)		ip = *pqinfo->head++;			\
				if (pqinfo->head == pqueue_end)		\
					pqinfo->head = pqueue

/* Small non-interleaved version for partially unrolled loops. */
#define PQ_SNI_NEXT1(ip)	ip = *head1++;				\
				if (head1 == pqueue_end) head1 = pqueue

#define PQ_SNI_NEXT2(ip)	ip = *head2++;				\
				if (head2 == pqueue_end) head2 = pqueue

/* Small interleaved version. */
#define PQ_SI_NEXT(ip)		ip = *head++;				\
				if (head == pqueue_end)	head = pqueue

/* Small interleaved version for partially unrolled loops. */
#define PQ_SI_NEXT1(ip)		PQ_SI_NEXT(ip)	

#define PQ_SI_NEXT2(ip)		PQ_SI_NEXT(ip)

/* Big non-interleaved version. */
#define PQ_BNI_NEXT(ip)		ip = *pqinfo->head++

/* Big non-interleaved version for partially unrolled loops. */
#define PQ_BNI_NEXT1(ip)	ip = *head1++

#define PQ_BNI_NEXT2(ip)	ip = *head2++

/* Big interleaved version. */
#define PQ_BI_NEXT(ip)		ip = *head++

/* Big interleaved version for partially unrolled loops. */
#define PQ_BI_NEXT1(ip)		PQ_BI_NEXT(ip)

#define PQ_BI_NEXT2(ip)		PQ_BI_NEXT(ip)

/* Use only these macro in your source code. */
#define PQ_NEXT(ip)		PQ_SNI_NEXT(ip)

#define PQ_NEXT1(ip)		PQ_SNI_NEXT1(ip)

#define PQ_NEXT2(ip)		PQ_SNI_NEXT2(ip)

#endif /* COREWAR_BENCH_PROCESS_QUEUE_H */
