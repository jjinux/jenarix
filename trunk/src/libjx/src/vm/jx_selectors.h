#ifndef _H_jx_selectors
#define _H_jx_selectors

/* 
Copyright (c) 2009, DeLano Scientific LLC, Palo Alto, California, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

* Neither the name of the DeLano Scientific LLC nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "jx_private.h"

/* special forms (built into "eval" for max. performance) */

#define JX_BUILTIN_NOP        0
#define JX_BUILTIN_IF         1
#define JX_BUILTIN_WHILE      2
#define JX_BUILTIN_DO         3
#define JX_BUILTIN_FOR        4
#define JX_BUILTIN_QUOTE      5
#define JX_BUILTIN_DEF        6
#define JX_BUILTIN_CODE       7
#define JX_BUILTIN_APPLY      8
#define JX_BUILTIN_INVOKE     9

#define JX_BUILTIN_RESOLVE   11
#define JX_BUILTIN_MAP       12
#define JX_BUILTIN_TEST      13
#define JX_BUILTIN_SWITCH    14
#define JX_BUILTIN_DISPATCH  15
#define JX_BUILTIN_RAW       16
#define JX_BUILTIN_EVAL      17
#define JX_BUILTIN_EXEC      18
#define JX_BUILTIN_SELECT    19
#define JX_BUILTIN_DEFUN     20
#define JX_BUILTIN_LAMBDA    21
#define JX_BUILTIN_DEFMAC    22
#define JX_BUILTIN_REDUCE    23
#define JX_BUILTIN_FOREACH   24

/* the number below must be higher than the last special form 
   but lower than the first "safe" selector */

#define JX_BUILTIN_SPECIAL_FORMS_LIMIT 25

/* The next batch of selectors (through 1024?) are reserved for "safe"
   functions which only involve internal actions performed within the
   interpreter and with containers (no access to filesystem, network,
   memory, etc.). 

   By convention, "safe" selector methods do not consumer their
   payload (that's done by eval instead)

   IMPORTANT: these functions must carefully prevent the propagation
   of weak references by making it impossible to store a weak
   reference into a container 
*/

/* symbols */

#define JX_BUILTIN_FILL        30
#define JX_BUILTIN_RANGE       31
#define JX_BUILTIN_SET         32
#define JX_BUILTIN_GET         33
#define JX_BUILTIN_BORROW      34
#define JX_BUILTIN_TAKE        35
#define JX_BUILTIN_DEL         36

#define JX_BUILTIN_IDENTICAL   37
#define JX_BUILTIN_EQ          38
#define JX_BUILTIN_LT          39
#define JX_BUILTIN_GT          40
#define JX_BUILTIN_LE          41
#define JX_BUILTIN_GE          42
#define JX_BUILTIN_NE          43
#define JX_BUILTIN_AND         44    
#define JX_BUILTIN_OR          45
#define JX_BUILTIN_BIT_AND     46 
#define JX_BUILTIN_BIT_OR      47
#define JX_BUILTIN_BIT_XOR     48
#define JX_BUILTIN_BIT_SHL     49
#define JX_BUILTIN_BIT_SHR     50

#define JX_BUILTIN_ADD         51
#define JX_BUILTIN_SUB         52
#define JX_BUILTIN_MUL         53
#define JX_BUILTIN_DIV         54
#define JX_BUILTIN_MOD         55

#define JX_BUILTIN_BIT_NOT     56
#define JX_BUILTIN_NOT         57
#define JX_BUILTIN_NEG         58

#define JX_BUILTIN_OUTPUT      59
#define JX_BUILTIN_ERROR       60

#define JX_BUILTIN_SIZE        61
#define JX_BUILTIN_APPEND      62
#define JX_BUILTIN_POP         63
#define JX_BUILTIN_SHIFT       64
#define JX_BUILTIN_UNSHIFT     65
#define JX_BUILTIN_INSERT      66
#define JX_BUILTIN_RESIZE      67
#define JX_BUILTIN_EXTEND      68
#define JX_BUILTIN_SLICE       69
#define JX_BUILTIN_CUTOUT      70
#define JX_BUILTIN_IMPL        71

#define JX_BUILTIN_INCR        72
#define JX_BUILTIN_DECR        73
#define JX_BUILTIN_SYMBOLS     74
#define JX_BUILTIN_HAS         75
#define JX_BUILTIN_SAME        76

#define JX_BUILTIN_STR         77
#define JX_BUILTIN_PUSH        78


#endif
