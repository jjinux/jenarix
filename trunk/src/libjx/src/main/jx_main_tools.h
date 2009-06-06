#ifndef _H_jx_main_tools
#define _H_jx_main_tools

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

/* stuff to help us build "main" apps */

#include "jx_public.h"

#ifndef WIN32
#include<fcntl.h>
#else
#if defined(MS_WINDOWS) || defined(__CYGWIN__)
#include <fcntl.h>
#endif
#endif

JX_INLINE jx_bool jx_adapt_for_console(FILE *input)
{
  if (isatty((int)fileno(input))) {
#if defined(MS_WINDOWS) || defined(__CYGWIN__)
    _setmode(fileno(input), O_BINARY);
    _setmode(fileno(stdout), O_BINARY);
#endif
#ifdef WIN32
#endif
    setvbuf(input,  (char *)NULL, _IONBF, BUFSIZ);
    setvbuf(stdout, (char *)NULL, _IONBF, BUFSIZ);
    setvbuf(stderr, (char *)NULL, _IONBF, BUFSIZ);
    return JX_TRUE;
  } else
    return JX_FALSE;
}


#define JX_MODE_AUTOMATIC      0
#define JX_MODE_CONSOLE        1
#define JX_MODE_EVALUATE       2
#define JX_MODE_PARSE_ONLY     3
#define JX_MODE_TRANSLATE_ONLY 4
#define JX_MODE_COMPILE_ONLY   5
#define JX_MODE_EXECUTE        6

JX_INLINE jx_int jx_main_parse_mode(FILE **input, int argc, char *argv[])
{
  jx_int mode = JX_MODE_AUTOMATIC;
  {
    jx_int arg = 1;
    while(arg<argc) {
      switch(argv[arg][0]) {
      case '-':
        {
          char *ch = argv[arg]+1;
          while(*ch) {
            switch(*(ch++)) {
            case 'p':
              mode = JX_MODE_PARSE_ONLY;
              break;
            case 't':
              mode = JX_MODE_TRANSLATE_ONLY;
              break;
            case 'c':
              mode = JX_MODE_COMPILE_ONLY;
              break;
            case 'e':
              mode = JX_MODE_EVALUATE;
              break;
            case 'x':
              mode = JX_MODE_EXECUTE;
              break;
            case 'n':
              mode = JX_MODE_CONSOLE;
              break;
            case 'h':
              fprintf(stderr,"usage: ./jx[p] -[cehtx] file\n");
              fprintf(stderr," -c stop after compilation\n");
              fprintf(stderr," -e evaluate and print return values\n");
              fprintf(stderr," -h print help\n");
              fprintf(stderr," -n console mode\n");
              fprintf(stderr," -t stop after parsing\n");
              fprintf(stderr," -t stop after translation\n");
              fprintf(stderr," -x just execute (suppress return values)\n");
              exit(-1);
              break;
            }
          }
        }
        break;  
      default:
        *input = fopen(argv[arg],"rb");
        break;
      }
      arg++;
    }
  }
  return mode;
}
#endif
