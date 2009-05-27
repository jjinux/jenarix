

all:

include ../../.config

jx_jxon_parse.c: jx_jxon_parse.lem
	if [ ! -e ../../../../ext/bin/lemon ];then exit 1;fi; # ERRROR: no lemon!
	cp lempar-c lempar.c
	../../../../ext/bin/lemon jx_jxon_parse.lem
	/bin/rm lempar.c

jx_jxon_scan.c: jx_jxon_scan.re
	if [ ! -e ../../../../ext/bin/re2c ];then exit 1 ;fi; # ERROR: no re2c!
	../../../../ext/bin/re2c jx_jxon_scan.re > jx_jxon_scan.c

jxon: jx_jxon_parse.c jx_jxon_scan.c


jx_shell_parse.c: jx_shell_parse.lem
	if [ ! -e ../../../../ext/bin/lemon ];then exit 1;fi; # ERROR: no lemon!
	cp lempar-c lempar.c
	../../../../ext/bin/lemon jx_shell_parse.lem
	/bin/rm lempar.c

jx_shell_scan.c: jx_shell_scan.re
	if [ ! -e ../../../../ext/bin/re2c ];then exit 1;fi; # ERROR: no re2c!
	../../../../ext/bin/re2c jx_shell_scan.re > jx_shell_scan.c

shell: jx_shell_parse.c jx_shell_scan.c