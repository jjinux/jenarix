

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


/* STANDALONE DEFAULT CONFIG */


/* Are our native ints & floats 64-bits wide? */

#define notJX_64_BIT


/* How many characters do we want to be able to squish into an object
   so as to avoid touching the heap?  (can only be 6, 10, or 14 in
   32-bit mode, 12 or 20 in 64-bit mode, and note that if
   JX_TINY_STR_SIZE is less than JX_TINY_STR_MAX_SIZE, then
   JX_TINY_STR_SIZE will be redefined in jx_inline.h) */

#ifndef JX_TINY_STR_SIZE
#define JX_TINY_STR_SIZE 0
#endif


/* do we want fake threads or real threads? */

#define noJX_OS_FAKE_THREADS


/* DEBUG SETTINGS */


/* track heap objects in depth? (print atexit) */

#define JX_HEAP_TRACKER


/* no heap tracker, but still wrap malloc, etc.? (print atexit) */

#define noJX_MEM_WRAP


/* should we scramble released memory whenever possible? */

#define JX_HEAP_GRINDER


/* should we avoid re-using memory? */

#define noJX_HEAP_SINGLE_USE


/* let's not do this again... */
#define JX_CONFIGURED

#endif
#endif
