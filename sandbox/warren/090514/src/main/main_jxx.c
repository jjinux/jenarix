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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "jx_public.h"

#ifndef WIN32
#include<fcntl.h>
#else
#if defined(MS_WINDOWS) || defined(__CYGWIN__)
#include <fcntl.h>
#endif
#endif

static jx_bool jx_adapt_for_console(void)
{
  if (isatty((int)fileno(stdin))) {
#if defined(MS_WINDOWS) || defined(__CYGWIN__)
    _setmode(fileno(stdin), O_BINARY);
    _setmode(fileno(stdout), O_BINARY);
#endif
#ifdef WIN32
#if 0
    {
      unsigned long on = 1;
      ioctlsocket(0, FIONBIO, &on);
    }
#endif
#endif
    setvbuf(stdin,  (char *)NULL, _IONBF, BUFSIZ);
    setvbuf(stdout, (char *)NULL, _IONBF, BUFSIZ);
    setvbuf(stderr, (char *)NULL, _IONBF, BUFSIZ);
#if 0
    fcntl(0,F_SETFL,O_NONBLOCK);
#endif
    return JX_TRUE;
  } else
    return JX_FALSE;
}

int main(int argc, char **argv)
{
  jx_ob namespace = jx_hash_new();
  jx_ob scanner = jx_jxon_scanner_new_with_file(stdin);
  jx_ob node = jx_hash_new();
  jx_bool console = jx_adapt_for_console();

  jx_code_expose_secure_builtins(namespace);

  {
    jx_ob source = JX_OB_NULL;
    jx_status status;
    jx_bool done = JX_FALSE;
    while( !done ) {
      status = jx_jxon_scanner_next_ob(&source, scanner);
      switch(status) {
      case JX_YES:
        if(console) jx_jxon_dump(stdout, "# source", source);
        {
          jx_ob code = jx_code_bind_with_source(namespace, source);
          source = jx_ob_from_null();
          
          jx_jxon_dump(stdout, "# exec", code);
          {
            jx_ob result = jx_code_exec(node,code);
            
            if(console) 
              jx_jxon_dump(stdout, "# result", result);
            else {
              jx_ob jxon = jx_ob_to_jxon(result);
              printf("%s;\n",jx_ob_as_str(&jxon));
              jx_ob_free(jxon);
            }
            jx_ob_free(result);
          }
          jx_ob_free(code);
        }
        break;
      case JX_STATUS_SYNTAX_ERROR: /* catch this error */
        {
          jx_ob message = jx_jxon_scanner_get_error_message(scanner);
          printf("Error: invalid syntax\n");
          if(jx_str_check(message)) 
            printf("%s\n",jx_ob_as_str(&message));
          if(!console)
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
  jx_ob_free(node);
  jx_ob_free(scanner);
  jx_ob_free(namespace);
}

