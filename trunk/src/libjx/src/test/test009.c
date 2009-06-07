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

#include "jx_public.h"

#define C1(ex,s1) printf("# %s (%s line %d,);\n",s1,__FILE__,__LINE__);
#define P1(ex,s1) printf(ex " || die('fail: %s line %d.');\n",s1,__FILE__,__LINE__);
#define P2(ex,s1,s2) printf(ex " || die('fail: %s line %d.');\n",s1,s2,__FILE__,__LINE__);

#define JS(st) \
{ \
  { \
    jx_ob ob = jx_ob_from_jxon_str(st); \
    jx_ob jxon = jx_ob_to_jxon(ob); \
    P2("'%s' == '%s'", st,jx_ob_as_str(&jxon)); \
    P1("0 == %d", jx_ob_free(jxon)); \
    P1("0 == %d", jx_ob_free(ob)); \
  } \
}

#define JF(st) \
{ \
  { \
    jx_ob ob = jx_ob_from_jxon_str(st); \
    jx_ob jxon = jx_ob_to_jxon(ob); \
    P1("'null' == '%s'", jx_ob_as_str(&jxon)); \
    P1("0 == %d", jx_ob_free(jxon)); \
    P1("0 == %d", jx_ob_free(ob)); \
  } \
}

unsigned int randmask(unsigned int mask)
{
  return ((unsigned int)random()) & mask;
}

void random_string(char *target,int mask)
{
  char *c = target;
  int len = randmask(mask);
  while(len--) {
    *(c++) = 65 + randmask(0xF);
  }
  *c = 0;
}

jx_int random_int(void)
{
  return random();
}

jx_float random_float(void) 
/* only return floats which are exact in both decimal and binary representations */
{
  jx_float result;
  jx_int expo = randmask(0x7);
  jx_int negative = randmask(0x1);
  jx_int inverse = randmask(0x1);
  jx_int value = 1;
  while(expo--) value = value * 2;
  if(negative)
    value = -value;
  if(inverse)
    result = 1.0/value;
  else
    result = value;
  return result;
}

#if 1
/* allow non-string primitives to be hash keys */
#define random_key random_primitive
#else
/* only string primitives as hash keys */
jx_ob random_key(void)
{
  char buffer[0xF+1];
  random_string(buffer,0xF);
  return jx_ob_from_str(buffer);
}
#endif

jx_ob random_primitive(void)
{
  jx_ob result = JX_OB_NULL;
  switch(randmask(0x7)) {
  case 0: 
    result = jx_ob_from_null();
    break;
  case 1: 
    result = jx_ob_from_bool(1);
    break;
  case 2:
    result = jx_ob_from_bool(0);
    break;
  case 3:
  case 4:
    result = jx_ob_from_int(random_int());
    break;
  case 5:
  case 6:
    result = jx_ob_from_float(random_float());
    break;
  case 7:
    {
      char buffer[0xF+1];
      random_string(buffer,0xF);
      result = jx_ob_from_str(buffer);
    }
    break;
  }
  return result;
}

jx_ob random_container(void)
{
  jx_ob result = JX_OB_NULL;

  if(randmask(0x1)) {
    result = jx_list_new();
  } else {
    result = jx_hash_new();
  }
  return result;
}

#define MAX_DEPTH 12
jx_ob random_tree(int count)
{
  jx_ob stack[MAX_DEPTH+1];
  int depth = 0;
  if(randmask(0x1)) {
    stack[depth] = jx_list_new();
  } else {
    stack[depth] = jx_hash_new();
  }
  while(count) {
    switch(randmask(0x3)) {
    case 0: /* exit */
      if(depth) {
        depth--;
      }
      break;
    case 1: /* nest */
      if(depth<MAX_DEPTH) {
        if(jx_list_check(stack[depth])) {
          stack[depth+1] = random_container();
          jx_list_append(stack[depth],stack[depth+1]);
          depth++;
        } else if(jx_hash_check(stack[depth])) {
          jx_ob key = random_key();
          stack[depth+1] = random_container();
          jx_hash_set(stack[depth], key, stack[depth+1]);
          depth++;
        }
      }
      break;
    default: /* extend */
      {
        jx_ob value = random_primitive();
        if(jx_list_check(stack[depth])) {
          jx_list_append(stack[depth], value);
        } else {
          jx_ob key = random_key();
          jx_hash_set(stack[depth], key,value);
        }
        count--;
      }
      break;
    }
  }
  return stack[0];
}

jx_int strsum(jx_char *c)
{
  jx_int result = 0;
  jx_char ch;
  while( (ch=*(c++)) ) result += ch;
  return result;
}

#define STOP_AFTER 100000

int main(int argc, char **argv)
{
  int complexity = 0;
  while(1) {
    jx_ob ob1 = random_tree( complexity );
    jx_ob jxon1 = jx_ob_to_jxon(ob1); 
    int len1 = strlen(jx_ob_as_str(&jxon1));
    int sum1 = strsum(jx_ob_as_str(&jxon1));

    jx_ob ob2 = jx_ob_from_jxon_str(jx_ob_as_str(&jxon1));
    jx_ob jxon2 = jx_ob_to_jxon(ob2); 
    int len2 = strlen(jx_ob_as_str(&jxon2));
    int sum2 = strsum(jx_ob_as_str(&jxon2));

    //    printf("# %s\n# %s\n",jx_ob_as_str(&jxon1), jx_ob_as_str(&jxon2));
    //    jx_ob_dump(stderr,"ob2",ob2);
    printf("# complexity = %d, JXON string length = %d\n",complexity, len1);
    P2("%d == %d",len1,len2);
    if(len1!=len2) {
      jx_jxon_dump(stdout,"1", jxon1);
      jx_jxon_dump(stdout,"2", jxon2);
      exit(1);
    }
    P2("%d == %d",sum1,sum2);
    P1("1 == %d",jx_ob_equal(ob1,ob2));

    if(complexity<100) {
      complexity++;
    } else {
      complexity += complexity;
    }

#if 0
    if(complexity>STOP_AFTER) {
      FILE *f = fopen("input.jxon","wb");
      fprintf(f,"%s\n",jx_ob_as_str(&jxon1));
      fclose(f);
      f = fopen("output.jxon","wb");
      fprintf(f,"%s\n",jx_ob_as_str(&jxon2));
      fclose(f);
    }
#endif

    jx_ob_free(jxon1);
    jx_ob_free(jxon2);
    jx_ob_free(ob1);
    jx_ob_free(ob2);

    if(complexity>STOP_AFTER)
      break;
  }
  
}
