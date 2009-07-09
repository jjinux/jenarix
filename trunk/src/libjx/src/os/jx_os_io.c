
/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#include "jx_private.h"

#ifndef WIN32
#include<fcntl.h>
#endif

jx_status jx_os_unblock_stdin(void)
{
  jx_status status = JX_STATUS_SUCCESS;
#ifdef WIN32

#error to test!
  unsigned long on = 1;
  if(ioctlsocket(0, FIONBIO, &on) == SOCKET_ERROR) {
    status = JX_STATUS_FAILURE;
  }
#else
  if(fcntl(0, F_SETFL, O_NONBLOCK) < 0)
    status = JX_STATUS_FAILURE;
#endif
  return status;
}

jx_status jx_os_fprint(JX_OS_FILE * f, jx_ob ob)
{
  jx_status status;
  if(jx_str_check(ob)) {
    status = fprintf(jx_os_stdout, "%s", jx_ob_as_str(&ob));
    if(status < 0)
      status = JX_STATUS_FAILURE;
  } else {
    status = JX_STATUS_SUCCESS;
  }
  return status;
}
