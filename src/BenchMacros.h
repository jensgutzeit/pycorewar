/* BenchMacros.h
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

#ifndef BENCH_MACROS__H
#define BENCH_MACROS__H

/* x = x + 1 mod coresize */
#define INCMOD(x)		if (++(x) == coresize) (x) = 0

/* x = x - 1 mod coresize */
#define DECMOD(x)		if ((x)-- == 0) (x) = coresize - 1

/* z = x + y mod coresize */
#define ADDMOD(z,x,y)		(z) = (x) + (y);			    \
				if ( (z) >= coresize ) (z) -= coresize

/* z = x - y mod coresize */
#define SUBMOD(z,x,y)		(z) = (x) - (y);			    \
				if ( (z) >= coresize ) (z) += coresize

/* Increase instruction pointer and make sure, that it wraps around. */
#define IPINCMOD(x)		if ( ++(x) == core_end ) (x) = core

#define ASSIGNADDR(addr,x,y)	addr = x + y;                             \
				if (addr >= core_end) addr -= coresize

#define ADDADDR(addr,incr)	addr += incr;                             \
				if (addr >= core_end) addr -= coresize

#endif /* BENCH_MACROS__H */
