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

void get_symbol_from_node(jx_ob node, jx_char *name)
{
  jx_ob tmp = jx_ob_from_str(name);
  symbol = jx_hash_get(node,tmp);
  jx_ob_free(tmp);
  return symbol;
}

typedef {
  jx_ob menuBarEntity;
  jx_ob openGLEntity;
  jx_ob hSplitter;
  jx_ob vSplitter;
} Knowns;

void process_splitter ()
{
  /* what is it? */
  if(jx_ob_identical(current, known.menuBarEntity)) {

  } else if(jx_ob_identical(current, known.openGLEntity)) {
    
  } else { /* treat asgeneric */
  }
}

void traverse_gui(jx_ob gui_node, Knowns *known)
{
  jx_ob current = jx_list_shift(gui_node);
  if(jx_ob_identical(current, known.hSplitter)) {
    process_splitter(current, known, HORIZ);
  } else if(jx_ob_identical(current, known.vSplitter)) {
    process_splitter(current, known);    
  }
}

void process_gui(jx_ob node)
{
  Knowns known;

  /* get entity handles for the things we know about and expect */

  known.menuBarEntity = get_symbol_from_node("MenuBar");
  known.openGLEntity = get_symbol_from_node("OpenGL");
  
  /* get the gui */

  jx_ob gui = get_symbol_from_node("gui");

  /* traverse the gui */
  traverse_gui(gui,&known);

}



int main(int argc, char *argv[])
{
  int exit_status = EXIT_FAILURE;
  FILE *input = stdin;
  jx_int mode = jx_main_parse_mode(&input, argc, argv);

  if(input && jx_ok(jx_os_process_init(argc, argv))) {
    jx_bool console = jx_adapt_for_console(input);
    jx_ob node = jx_hash_new();
    jx_ob names = jx_hash_new_with_flags(JX_HASH_FLAG_BIDIRECTIONAL);
    jx_ob scanner = jx_jxon_scanner_new_with_file
      (input, (mode==JX_MODE_UNIT_TESTING) ? JX_SCANNER_FLAG_ECHO_COMMENTS : 0);
    exit_status = EXIT_SUCCESS;

    if(mode == JX_MODE_AUTOMATIC) {
      if(console)
        mode = JX_MODE_CONSOLE;
      else
        mode = JX_MODE_EXECUTE;
    }

    jx_code_expose_secure_builtins(names);

    jx_hash_set(node, jx_builtins(), jx_ob_copy(names));

    if(mode == JX_MODE_CONSOLE) 
      printf("Jenarix VM Syntax (JXON) [%d-byte numbers, %d-byte tiny strings, %d-byte jx_ob]\n",
             (int)sizeof(jx_int),JX_TINY_STR_SIZE,(int)sizeof(jx_ob));

    {
      jx_ob source = jx_ob_from_null();
      jx_status status;
      jx_bool done = JX_FALSE;
      while( !done ) {
        status = jx_jxon_scanner_next_ob(&source, scanner);
        switch(status) {
        case JX_YES:
          if((mode == JX_MODE_PARSE_ONLY) || (mode == JX_MODE_TRANSLATE_ONLY)) {
            jx_ob jxon = jx_ob_to_jxon_in_node(node,source);
            printf("%s;\n",jx_ob_as_str(&jxon));
            jx_ob_free(jxon);
          } else {
            if((mode == JX_MODE_CONSOLE) || (mode == JX_MODE_UNIT_TESTING))
              jx_jxon_dump_in_node(stdout, "# source", node, source);
            {
              jx_ob code = jx_code_bind_with_source(names, source);
              source = jx_ob_from_null();

              if(mode == JX_MODE_COMPILE_ONLY) {
                jx_ob jxon = jx_ob_to_jxon_in_node(node,code);
                printf("%s;\n",jx_ob_as_str(&jxon));
                jx_ob_free(jxon);
              } else {
                if((mode == JX_MODE_CONSOLE) || (mode == JX_MODE_UNIT_TESTING))
                  jx_jxon_dump_in_node(stdout, "#   eval", node, code);
                {
                  jx_ob result = jx_code_eval(node,code);
                  
                  if(jx_opcode_check(result) && 
                     (jx_ob_identical(result,jx_ob_from_opcode(JX_OPCODE_RETURN,0)))) {
                    done = JX_TRUE;
                  } else if(mode == JX_MODE_CONSOLE)  {
                    jx_jxon_dump_in_node(stdout, "# result", node, result);
                  } else if((mode == JX_MODE_EVALUATE)||(mode == JX_MODE_UNIT_TESTING)) {
                    jx_ob jxon = jx_ob_to_jxon_in_node(node,result);
                    printf("%s;\n",jx_ob_as_str(&jxon));
                    jx_ob_free(jxon);
                    if(mode == JX_MODE_UNIT_TESTING) { /* all unit test exprs must equal true */
                      if(!jx_ob_identical(jx_ob_from_bool(1),result)) {
                        jx_ob message = jx_jxon_scanner_get_error_message(scanner);
                        if(jx_str_check(message)) 
                          printf("%s\n",jx_ob_as_str(&message));
                        jx_ob_free(message);
                        exit_status = EXIT_FAILURE;
                        done = JX_TRUE;
                      }
                      printf("\n"); /* add newline after test in output */
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
        case JX_NO: /* keep going */
          break;
        case JX_STATUS_SYNTAX_ERROR: /* catch this error */
          {
            jx_ob message = jx_jxon_scanner_get_error_message(scanner);
            if(jx_str_check(message)) 
              printf("%s\n",jx_ob_as_str(&message));
            else
              printf("Error: invalid syntax\n");

            if(!mode == JX_MODE_CONSOLE)
              done = JX_TRUE;
            else
              jx_jxon_scanner_purge_input(scanner);
            jx_ob_free(message);
          }
          break;
        default: /* about on all other errors */
          done = JX_TRUE;
          break;
        }
      }
      jx_ob_free(source);
    }

    jx_jxon_dump_in_node(stdout,"node",node,node);

    process_gui(node);

    jx_ob_free(node);
    jx_ob_free(scanner);
    jx_ob_free(names);

    jx_os_process_complete();
  }
  return exit_status;
}

