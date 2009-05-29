

all:

include ../../.config

ext_check:
	if [ ! -e ../../../../ext/bin/lemon ];then exit 1;fi; # ERRROR: no lemon!
	if [ ! -e ../../../../ext/bin/re2c ];then exit 1 ;fi; # ERROR: no re2c!

jx_jxon_parse.c: jx_jxon_parse.lem
	make -f ../../rules/codegen.mk ext_check
	cp lempar-c lempar.c
	../../../../ext/bin/lemon jx_jxon_parse.lem
	/bin/rm lempar.c

jx_jxon_scan.c: jx_jxon_scan.re
	make -f ../../rules/codegen.mk ext_check
	../../../../ext/bin/re2c jx_jxon_scan.re > jx_jxon_scan.c

jxon: jx_jxon_parse.c jx_jxon_scan.c

jx_shell_parse.c: jx_shell_parse.lem
	make -f ../../rules/codegen.mk ext_check
	cp ../jxon/lempar-c lempar.c
	../../../../ext/bin/lemon jx_shell_parse.lem
	/bin/rm lempar.c

jx_shell_scan.c: jx_shell_scan.re
	make -f ../../rules/codegen.mk ext_check
	../../../../ext/bin/re2c jx_shell_scan.re > jx_shell_scan.c

shell: jx_shell_parse.c jx_shell_scan.c

jx_py_parse.c: jx_py_parse.lem
	make -f ../../rules/codegen.mk ext_check
	cp ../jxon/lempar-c lempar.c
	../../../../ext/bin/lemon jx_py_parse.lem
	/bin/rm lempar.c

jx_py_scan.c: jx_py_scan.re
	make -f ../../rules/codegen.mk ext_check
	../../../../ext/bin/re2c jx_py_scan.re > jx_py_scan.c

py: jx_py_parse.c jx_py_scan.c

jx_net_parse.c: jx_net_parse.lem
	make -f ../../rules/codegen.mk ext_check
	cp ../jxon/lempar-c lempar.c
	../../../../ext/bin/lemon jx_net_parse.lem
	/bin/rm lempar.c

jx_net_scan.c: jx_net_scan.re
	make -f ../../rules/codegen.mk ext_check
	../../../../ext/bin/re2c jx_net_scan.re > jx_net_scan.c

net: jx_net_parse.c jx_net_scan.c