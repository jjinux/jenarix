/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#ifndef _H_jx_config
#define _H_jx_config

/* Jenarix configuration */

#ifndef JX_CONFIGURED

/* STANDALONE DEFAULT / DEVELOPMENT CONFIG */

/* Are our native ints & floats 64-bits wide? */

#define notJX_64_BIT

/* How many characters do we want to be able to squish into an object
   so as to avoid touching the heap? (can only be 6, 10, or 18) */

#define JX_TINY_STR_SIZE 10

/* do we want fake threads or real threads? */

#define noJX_OS_FAKE_THREADS 

/* DEBUG SETTINGS */

/* wrap malloc, calloc, realloc, and free? (tripwire, etc.) */

#define JX_MEM_WRAP

/* track heap objects in depth? (with print-out) */

#define noJX_HEAP_TRACKER

/* should we scramble released memory whenever possible? */

#define noJX_HEAP_GRINDER

/* should we avoid re-using memory? */

#define noJX_HEAP_SINGLE_USE

/* let's not do this again... */
#define JX_CONFIGURED

#endif
#endif
