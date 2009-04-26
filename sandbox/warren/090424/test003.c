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

#define P1(ex,s1) printf(ex " || die('fail: %s line %d.');\n",s1,__FILE__,__LINE__);
#define P2(ex,s1,s2) printf(ex " || die('fail: %s line %d.');\n",s1,s2,__FILE__,__LINE__);

int main(int argc, char **argv)
{
  {
    jx_ob list = jx_list_new();
    
    P1( "0 == %d", jx_list_size(list));
    
    P1( "0 == %d", jx_list_append(list, jx_ob_from_int(9)));
    P1( "1 == %d", jx_list_size(list));
    P1( "9 == %d", jx_ob_as_int(jx_list_borrow(list,0)));
    P1( "0 == %d", jx_list_append(list, jx_ob_from_int(8)));
    P1( "2 == %d", jx_list_size(list));
    P1( "8 == %d", jx_ob_as_int(jx_list_borrow(list,1))); 
    
    P1( "0 == %d", jx_list_append(list, jx_ob_from_float(2.0F)));
    P1( "3 == %d", jx_list_size(list));
    P1( "9 == %d", jx_ob_as_int(jx_list_borrow(list,0)));
    P1( "8 == %d", jx_ob_as_int(jx_list_borrow(list,1)));
    P1( "2.0 == %f",jx_ob_as_float(jx_list_borrow(list,2)));

    P1( "0 == %d", jx_ob_free(list));
  }
}

