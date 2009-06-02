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

#include "jx_public.h"
#include "jx_main_tools.h"

#define JX_MODE_AUTOMATIC      0
#define JX_MODE_CONSOLE        1
#define JX_MODE_EVALUATE       2
#define JX_MODE_PARSE_ONLY     3
#define JX_MODE_TRANSLATE_ONLY 4
#define JX_MODE_COMPILE_ONLY   5
#define JX_MODE_EXECUTE        6

int main(int argc, char *argv[])
{
  FILE *input = stdin;
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
              fprintf(stderr,"usage: ./jxp -[cehtx] file\n");
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
        input = fopen(argv[arg],"rb");
        break;
      }
      arg++;
    }
  }
  if(input && jx_ok(jx_os_process_init(argc, argv))) {
    
    jx_ob names = jx_hash_new();
    jx_ob scanner = jx_py_scanner_new_with_file(input);
    jx_ob node = jx_hash_new();
    jx_bool console = jx_adapt_for_console(input);

    if(mode == JX_MODE_AUTOMATIC) {
      if(console)
        mode = JX_MODE_CONSOLE;
      else
        mode = JX_MODE_EXECUTE;
    }

    jx_code_expose_secure_builtins(names);

    jx_py_expose_python_builtins(names);

    if(console) printf("Jenarix Python-like Syntax (JXP):\n");

    {
      jx_ob tree = JX_OB_NULL;
      jx_status status;
      jx_bool done = JX_FALSE;
      while( !done ) {
        status = jx_py_scanner_next_ob(&tree, scanner);
        switch(status) {
        case JX_YES:
          if(mode == JX_MODE_PARSE_ONLY) {
            jx_ob jxon = jx_ob_to_jxon(tree);
            printf("%s;\n",jx_ob_as_str(&jxon));
            jx_ob_free(jxon);
          } else {
            if(mode == JX_MODE_CONSOLE) jx_jxon_dump(stdout, "# parsed", tree);
            {
              jx_ob source = jx_py_translate_with_tree( jx_ob_copy(tree));
            
              if(mode == JX_MODE_TRANSLATE_ONLY) {
                jx_ob jxon = jx_ob_to_jxon(source);
                printf("%s;\n",jx_ob_as_str(&jxon));
                jx_ob_free(jxon);
              } else {
                if(mode == JX_MODE_CONSOLE) 
                  jx_jxon_dump(stdout, "# source", source);
                {
                  jx_ob code = jx_code_bind_with_source(names, source);
                  source = jx_ob_from_null();
                  if(mode == JX_MODE_COMPILE_ONLY) {
                    jx_ob jxon = jx_ob_to_jxon(code);
                    printf("%s;\n",jx_ob_as_str(&jxon));
                    jx_ob_free(jxon);
                  
                  } else {
                    if(mode == JX_MODE_CONSOLE)
                    jx_jxon_dump(stdout, "#   eval", code);
                    {
                      jx_ob result = jx_code_eval(node,code);
                    
                      if(!jx_null_check(result)) {
                        /* swallow null values just like Python does */
                      
                        if(console) {
                          jx_jxon_dump(stdout, "# result", result);
                        } else if(mode == JX_MODE_EVALUATE) {
                          jx_ob jxon = jx_ob_to_jxon(result);
                          printf("%s;\n",jx_ob_as_str(&jxon));
                          jx_ob_free(jxon);
                        }
                      }
                      jx_ob_free(result);
                    }
                  }
                  jx_ob_free(code);
                }
              }
              jx_ob_free(source);
            }
          }
          break;
        case JX_STATUS_SYNTAX_ERROR: /* catch this error */
          {
            jx_ob message = jx_py_scanner_get_error_message(scanner);
            if(jx_str_check(message)) 
              printf("%s\n",jx_ob_as_str(&message));
            else
              printf("Error: invalid syntax\n");
            if(!console)
              done = JX_TRUE;
            else
              jx_py_scanner_purge_input(scanner);
            jx_ob_free(message);
          }
          break;
        default: 
          if(!jx_ok(status)) 
            done = JX_TRUE;
          break;
        }
      }
      jx_ob_free(tree);
    }
    jx_ob_free(node);
    jx_ob_free(scanner);
    jx_ob_free(names);

    jx_os_process_complete();
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

