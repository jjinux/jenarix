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

#include "jx_public.h"


int main(int argc, char **argv)
{
  jx_ob list = jx_list_new();

  printf("0 == %d\n",jx_list_get_size(list));

  printf("0 == %d\n",jx_list_append(list, jx_ob_from_int(9)));
  printf("1 == %d\n",jx_list_get_size(list));
  printf("9 == %d\n",jx_ob_as_int(jx_list_borrow(list,0)));
  printf("0 == %d\n",jx_list_append(list, jx_ob_from_int(8)));
  printf("2 == %d\n",jx_list_get_size(list));
  printf("8 == %d\n",jx_ob_as_int(jx_list_borrow(list,1)));

  printf("0 == %d\n",jx_list_append(list, jx_ob_from_float(2.0F)));
  printf("3 == %d\n",jx_list_get_size(list));
  printf("9 == %d\n",jx_ob_as_int(jx_list_borrow(list,0)));
  printf("8 == %d\n",jx_ob_as_int(jx_list_borrow(list,1)));
  printf("2.0 == %f\n",jx_ob_as_float(jx_list_borrow(list,2)));
  jx_ob_free(list);
}

