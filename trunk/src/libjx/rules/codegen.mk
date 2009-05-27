

all:

include ../../.config

jx_jxon_parse.c: jx_jxon_parse.lem
	cp lempar-c lempar.c
	../../../../ext/bin/lemon jx_jxon_parse.lem
	/bin/rm lempar.c

jx_jxon_scan.c: jx_jxon_scan.re
	../../../../ext/bin/re2c jx_jxon_scan.re > jx_jxon_scan.c

jxon: jx_jxon_parse.c jx_jxon_scan.c


jx_shell_parse.c: jx_shell_parse.lem
	cp lempar-c lempar.c
	../../../../ext/bin/lemon jx_shell_parse.lem
	/bin/rm lempar.c

jx_shell_scan.c: jx_shell_scan.re
	../../../../ext/bin/re2c jx_shell_scan.re > jx_shell_scan.c

shell: jx_shell_parse.c jx_shell_scan.c