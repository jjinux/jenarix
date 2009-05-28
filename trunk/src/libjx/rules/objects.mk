
# core infrastructure: owned types, objects, builtins

OOO_CORE = 	\
	$(JX_PREFIX1)_private.o

# OS-level stuff (memory, threads, locks, etc.)

OOO_OS = \
	$(JX_PREFIX1)_os_process.o \
	$(JX_PREFIX1)_os_io.o \
	$(JX_PREFIX1)_heap.o \
	$(JX_PREFIX1)_mem_wrap.o 

# virtual machine / jenarix interpreter 

OOO_VM = \
	$(JX_PREFIX1)_code.o \
	$(JX_PREFIX1)_safe.o 

# JXON IO output / scanner / parser

OOO_JXON = \
	$(JX_PREFIX1)_jxon_out.o \
	$(JX_PREFIX1)_jxon_parse.o \
	$(JX_PREFIX1)_jxon_scan.o

# Shell-like scanner / parser

OOO_SHELL = \
	$(JX_PREFIX1)_shell_parse.o \
	$(JX_PREFIX1)_shell_scan.o

# Python-like scanner / parser

OOO_PY = \
	$(JX_PREFIX1)_py_parse.o \
	$(JX_PREFIX1)_py_scan.o \
	$(JX_PREFIX1)_py_trans.o 

# "mains" for the various read-eval-print executables

OOO_MAIN = $(JX_PREFIX1)_shell.o \
	$(JX_PREFIX1)_lisp.o \
	$(JX_PREFIX1)_py.o 

#	$(JX_PREFIX1)_python.o \
#	$(JX_PREFIX1)_java.o \
#	$(JX_PREFIX1)_network.o 

OOO_JXON = \
	$(JX_PREFIX1)_jxon_out.o \
	$(JX_PREFIX1)_jxon_parse.o \
	$(JX_PREFIX1)_jxon_scan.o

OOO_LIB = $(OOO_CORE) $(OOO_OS) $(OOO_JXON) $(OOO_VM) $(OOO_SHELL) $(OOO_PY)
OOO = $(OOO_LIB) $(OOO_MAIN)


