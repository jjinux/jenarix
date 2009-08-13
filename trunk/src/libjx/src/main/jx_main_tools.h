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

JX_INLINE jx_bool jx_adapt_for_console(FILE * input)
{
  if(isatty((int) fileno(input))) {
#if defined(MS_WINDOWS) || defined(__CYGWIN__)
    _setmode(fileno(input), O_BINARY);
    _setmode(fileno(stdout), O_BINARY);
#endif
#ifdef WIN32
#endif
    setvbuf(input, (char *) NULL, _IONBF, BUFSIZ);
    setvbuf(stdout, (char *) NULL, _IONBF, BUFSIZ);
    setvbuf(stderr, (char *) NULL, _IONBF, BUFSIZ);
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
#define JX_MODE_UNIT_TESTING   7

JX_INLINE jx_status jx_main_parse_mode(jx_int * mode, FILE ** input, int argc,
                                       char *argv[])
{
  jx_status status = JX_SUCCESS;
  jx_int arg = 1;
  *mode = JX_MODE_AUTOMATIC;
  while(arg < argc) {
    switch (argv[arg][0]) {
    case '-':
      {
        char *ch = argv[arg] + 1;
        while(*ch) {
          switch (*(ch++)) {
          case 'p':
            *mode = JX_MODE_PARSE_ONLY;
            break;
          case 't':
            *mode = JX_MODE_TRANSLATE_ONLY;
            break;
          case 'c':
            *mode = JX_MODE_COMPILE_ONLY;
            break;
          case 'e':
            *mode = JX_MODE_EVALUATE;
            break;
          case 'x':
            *mode = JX_MODE_EXECUTE;
            break;
          case 'n':
            *mode = JX_MODE_CONSOLE;
            break;
          case 'u':
            *mode = JX_MODE_UNIT_TESTING;
            break;
          case 'h':
            fprintf(stderr, "usage: ./jx[p] -[cehtx] file\n");
            fprintf(stderr, " -c stop after compilation\n");
            fprintf(stderr, " -e evaluate and print return values\n");
            fprintf(stderr, " -h print help\n");
            fprintf(stderr, " -n console mode\n");
            fprintf(stderr, " -t stop after parsing\n");
            fprintf(stderr, " -t stop after translation\n");
            fprintf(stderr, " -u run in unit test mode\n");
            fprintf(stderr, " -x just execute (suppress return values)\n");
            exit(-1);
            break;
          }
        }
      }
      break;
    default:
      *input = fopen(argv[arg], "rb");
      if(!*input) {
        printf("Error: unable to open '%s'\n", argv[arg]);
        status = JX_FAILURE;
      }
      break;
    }
    arg++;
  }
  return status;
}

JX_INLINE jx_status jx_main_expose_builtins_in_node(jx_ob node)
{
  jx_status status = JX_SUCCESS;
  jx_ob builtins = jx_hash_new_with_flags(JX_HASH_FLAG_BIDIRECTIONAL);
    
  status = jx_code_expose_secure_builtins(builtins);
  
  if(jx_ok(status)) {
    status = jx_hash_set(node, jx_builtins(), builtins);
  }

  return status;
}

JX_INLINE jx_status jx_main_exec_in_node(int argc, char *argv[], jx_ob node)
{
  jx_status status = JX_SUCCESS;
  FILE *input = stdin;
  jx_int mode = 0;

  if(jx_ok(status = jx_main_parse_mode(&mode, &input, argc, argv))) {

    jx_bool console = jx_adapt_for_console(input);
    jx_ob scanner = jx_jxon_scanner_new_with_file
      (input, (mode == JX_MODE_UNIT_TESTING) ? JX_SCANNER_FLAG_ECHO_COMMENTS : 0);

    if(mode == JX_MODE_AUTOMATIC) {
      if(console)
        mode = JX_MODE_CONSOLE;
      else
        mode = JX_MODE_EXECUTE;
    }


    if(mode == JX_MODE_CONSOLE)
      printf
        ("Jenarix VM Syntax (JXON) [%d-byte numbers, %d-byte tiny strings, %d-byte jx_ob]\n",
         (int) sizeof(jx_int), JX_TINY_STR_SIZE, (int) sizeof(jx_ob));

    {
      jx_ob source = jx_ob_from_null();
      jx_status status;
      jx_bool done = JX_FALSE;
      while(!done) {
        status = jx_jxon_scanner_next_ob(&source, scanner);
        switch (status) {
        case JX_YES:
          if((mode == JX_MODE_PARSE_ONLY) || (mode == JX_MODE_TRANSLATE_ONLY)) {
            jx_ob jxon = jx_ob_to_jxon_in_node(node, source);
            printf("%s;\n", jx_ob_as_str(&jxon));
            jx_ob_free(jxon);
          } else {
            if((mode == JX_MODE_CONSOLE) || (mode == JX_MODE_UNIT_TESTING))
              jx_jxon_dump_in_node(stdout, "# source", node, source);
            {
              jx_ob code =
                jx_code_bind_with_source(jx_hash_borrow(node, jx_builtins()), source);
              source = jx_ob_from_null();

              if(mode == JX_MODE_COMPILE_ONLY) {
                jx_ob jxon = jx_ob_to_jxon_in_node(node, code);
                printf("%s;\n", jx_ob_as_str(&jxon));
                jx_ob_free(jxon);
              } else {
                if((mode == JX_MODE_CONSOLE) || (mode == JX_MODE_UNIT_TESTING))
                  jx_jxon_dump_in_node(stdout, "#   eval", node, code);
                {
                  jx_ob result = jx_code_eval(node, code);
                  if(jx_opcode_check(result) &&
                     (jx_ob_identical(result, jx_ob_from_opcode(JX_OPCODE_RETURN, 0)))) {
                    done = JX_TRUE;
                  } else if(mode == JX_MODE_CONSOLE) {
                    jx_jxon_dump_in_node(stdout, "# result", node, result);
                  } else if((mode == JX_MODE_EVALUATE) || (mode == JX_MODE_UNIT_TESTING)) {
                    jx_ob jxon = jx_ob_to_jxon_in_node(node, result);
                    printf("%s;\n", jx_ob_as_str(&jxon));
                    jx_ob_free(jxon);
                    if(mode == JX_MODE_UNIT_TESTING) {  /* all unit test exprs must equal true */
                      if(!jx_ob_identical(jx_ob_from_bool(1), result)) {
                        jx_ob message = jx_jxon_scanner_get_error_message(scanner);
                        if(jx_str_check(message))
                          printf("%s\n", jx_ob_as_str(&message));
                        jx_ob_free(message);
                        status = JX_FAILURE;
                        done = JX_TRUE;
                      }
                      printf("\n");     /* add newline after test in output */
                    }
                  }
                  //jx_jxon_dump_in_node(stdout,"node",node,node);
                  jx_ob_free(result);
                }
              }
              jx_ob_free(code);
            }
          }
          break;
        case JX_NO:            /* keep going */
          break;
        case JX_STATUS_SYNTAX_ERROR:   /* catch this error */
          {
            jx_ob message = jx_jxon_scanner_get_error_message(scanner);
            if(jx_str_check(message))
              printf("%s\n", jx_ob_as_str(&message));
            else
              printf("Error: invalid syntax\n");

            if(!mode == JX_MODE_CONSOLE)
              done = JX_TRUE;
            else
              jx_jxon_scanner_purge_input(scanner);
            jx_ob_free(message);
          }
          break;
        default:               /* abort on all other errors */
          done = JX_TRUE;
          break;
        }
      }
      jx_ob_free(source);
    }
    jx_ob_free(scanner);
  }
  return status;
}

#endif
