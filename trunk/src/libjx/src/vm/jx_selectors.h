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

#define JX_SELECTOR_NOP        0
#define JX_SELECTOR_IF         1
#define JX_SELECTOR_WHILE      2
#define JX_SELECTOR_DO         3
#define JX_SELECTOR_FOR        4
#define JX_SELECTOR_QUOTE      5
#define JX_SELECTOR_DEF        6
#define JX_SELECTOR_CODE       7
#define JX_SELECTOR_APPLY      8
#define JX_SELECTOR_INVOKE     9

#define JX_SELECTOR_RESOLVE   11
#define JX_SELECTOR_MAP       12
#define JX_SELECTOR_TEST      13
#define JX_SELECTOR_SWITCH    14
#define JX_SELECTOR_DISPATCH  15
#define JX_SELECTOR_RAW       16
#define JX_SELECTOR_EVAL      17
#define JX_SELECTOR_EXEC      18
#define JX_SELECTOR_SELECT    19
#define JX_SELECTOR_DEFUN     20
#define JX_SELECTOR_LAMBDA    21
#define JX_SELECTOR_DEFMAC    22
#define JX_SELECTOR_REDUCE    23
#define JX_SELECTOR_FOREACH   24
#define JX_SELECTOR_DEBUG     25
#define JX_SELECTOR_BREAK     27
#define JX_SELECTOR_CONTINUE  28

#define JX_SELECTOR_PARMAP    29


/* the number below must be higher than the last special form 
   but lower than the first "safe" selector */

#define JX_SELECTOR_SPECIAL_FORMS_LIMIT 32

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

#define JX_SELECTOR_TAIL        47
#define JX_SELECTOR_RETURN      48

#define JX_SELECTOR_ENTITY      49
#define JX_SELECTOR_FILL        50
#define JX_SELECTOR_RANGE       61
#define JX_SELECTOR_SET         62
#define JX_SELECTOR_GET         63
#define JX_SELECTOR_BORROW      64
#define JX_SELECTOR_TAKE        65
#define JX_SELECTOR_DEL         66

#define JX_SELECTOR_IDENTICAL   67
#define JX_SELECTOR_EQ          68
#define JX_SELECTOR_LT          69
#define JX_SELECTOR_GT          70
#define JX_SELECTOR_LE          71
#define JX_SELECTOR_GE          72
#define JX_SELECTOR_NE          73
#define JX_SELECTOR_AND         74    
#define JX_SELECTOR_OR          75
#define JX_SELECTOR_BIT_AND     76 
#define JX_SELECTOR_BIT_OR      77
#define JX_SELECTOR_BIT_XOR     78
#define JX_SELECTOR_BIT_SHL     79
#define JX_SELECTOR_BIT_SHR     80

#define JX_SELECTOR_ADD         81
#define JX_SELECTOR_SUB         82
#define JX_SELECTOR_MUL         83
#define JX_SELECTOR_DIV         84
#define JX_SELECTOR_IDIV        85
#define JX_SELECTOR_MOD         86

#define JX_SELECTOR_BIT_NOT     87
#define JX_SELECTOR_NOT         88
#define JX_SELECTOR_NEG         89

#define JX_SELECTOR_OUTPUT      80
#define JX_SELECTOR_ERROR       91

#define JX_SELECTOR_SIZE        92
#define JX_SELECTOR_APPEND      93
#define JX_SELECTOR_PUSH        94

#define JX_SELECTOR_POP         95
#define JX_SELECTOR_SHIFT       96
#define JX_SELECTOR_UNSHIFT     97
#define JX_SELECTOR_INSERT      98
#define JX_SELECTOR_RESIZE      99
#define JX_SELECTOR_EXTEND     100
#define JX_SELECTOR_SLICE      101
#define JX_SELECTOR_CUTOUT     102
#define JX_SELECTOR_IMPL       103

#define JX_SELECTOR_INCR       104
#define JX_SELECTOR_DECR       105
#define JX_SELECTOR_SYMBOLS    106
#define JX_SELECTOR_HAS        107
#define JX_SELECTOR_SAME       108

#define JX_SELECTOR_STR        109
#define JX_SELECTOR_INT        110 
#define JX_SELECTOR_FLOAT      111
#define JX_SELECTOR_BOOL       112
#define JX_SELECTOR_SYNCHRONIZE  113
#define JX_SELECTOR_SYNCHRONIZED 114

#define JX_SELECTOR_REVERSE    115

#define JX_SELECTOR_SHARE      116
#define JX_SELECTOR_SHARED     117

#define JX_SELECTOR_POW        118

#endif
