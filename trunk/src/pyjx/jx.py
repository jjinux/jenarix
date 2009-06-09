#!/usr/bin/env python

# A pure Python-based Jenarix runtime implementation, intended to
# serve as a cross-check for the C-based implementation.

import sys

if len(sys.argv)>1:
    input = open(sys.argv[1],'rb')
else:
    input = sys.stdin

# status codes

JX_TRUE = True
JX_FALSE = False

JX_YES = JX_TRUE
JX_NO  = JX_FALSE
JX_SUCCESS = JX_FALSE
JX_FAILURE = -1

# special forms selectors

JX_SELECTOR_RESOLVE = 1L
JX_SELECTOR_GET = 2L
JX_SELECTOR_SET = 3L

JX_SELECTOR_SPECIAL_FORMS_LIMIT = 32L

# normal selectors

JX_SELECTOR_ADD = 32L
JX_SELECTOR_OUTPUT = 33L

# jx_ob is simply a native Python object with a native Python
# BooleanType, IntType, FloatType, StringType, ListType, or
# DictType. 

# Jenarix null is NoneType, and Jenarix identifiers are
# implemented as strings wrapped inside of length-1 TupleTypes

# Jenarix builtins selectors are indicated using LongType

class jx_ob:
    def __init__(self,data):
        self.ob = jx_ob_from_py_ob(data)

# note: temporary bogus attempt to recognize and transform identifiers
# without using a real parser.

import re
ident_re = re.compile('([\[{,]\s*)([a-zA-Z_][a-zA-Z0-9]*)(\s*[,}\]])')

class jx_jxon_scanner:

    def __init__(self,file):
        self.file = file

    def next_ob(self):
        while 1:
            text = self.file.readline()
            if not len(text):
                sys.exit(0)
            text = text.strip()
            if len(text):
                break
        last = ''
        while text!=last:
            last = text
            text = ident_re.sub(r"\1('\2',)\3",text)
        if text[-1:]==';':
            text = text[:-1] # strip semicolon
        return (JX_YES,eval(text))

def jx_ob(status):
    return status>=0

import types

def jx_list_size(ob):
    if jx_list_check(ob):
        return len(ob)
    else:
        return 0

def jx_list_check(ob):
    return isinstance(ob,types.ListType)

def jx_hash_check(ob):
    return isinstance(ob,types.DictType)

def jx_ident_check(ob):
    return isinstance(ob,types.TupleType)

def jx_builtin_selector_check(ob):
    return isinstance(ob,types.LongType)


def jx__jxon_dump(file,prompt,ob,builtins):
    if prompt:
        file.write(prompt)
        file.write(": ")
    if jx_list_check(ob):
        file.write("[")
        sep = ""
        for entry in ob:
            file.write(sep)
            jx__jxon_dump(file,None,entry,builtins) 
            sep = ","
        file.write("]")
    elif jx_builtin_selector_check(ob):
        for (key,value) in builtins.iteritems():
            if value == ob:
                file.write("`"+key[0])
                return
        file.write(str(ob))
    else:
        file.write(str(ob))

def jx_jxon_dump(file,prompt,ob,builtins):
    jx__jxon_dump(file,prompt,ob,builtins)
    file.write("\n")

def jx_code_expose_secure_builtins(names):
    names[('add',)] = JX_SELECTOR_ADD
    names[('set',)] = JX_SELECTOR_SET
    names[('get',)] = JX_SELECTOR_GET
    names[('output',)] = JX_SELECTOR_OUTPUT

def jx_code_bind_with_source(names,source):
    if jx_list_check(source):
        if jx_list_size(source)<1:
            return source
        code = []
        unresolved = 0
        if jx_ident_check(source[0]):
            if names.has_key(source[0]):
                source[0] = names[source[0]]
                unresolved = {
                    JX_SELECTOR_SET : 2,
                    JX_SELECTOR_GET : 2,
                }.get(source[0],0)
        for entry in source:
            if jx_ident_check(entry):
                if not unresolved:
                    code.append([JX_SELECTOR_RESOLVE,entry])
                else:
                    code.append(entry)
                    unresolved = unresolved - 1
            else:
                code.append(jx_code_bind_with_source(names,entry))
        return code
    else:
        return source

def jx_code_eval_to_weak(node, expr): 
    if jx_hash_check(hash):
        # to do
        return None

    elif not jx_list_check(expr):
        return expr

    elif jx_list_check(expr):

        # handle special forms

        if jx_builtin_selector_check( expr[0] ):
            if expr[0] < JX_SELECTOR_SPECIAL_FORMS_LIMIT:
                return {
                    JX_SELECTOR_RESOLVE : lambda : node.get(expr[1],expr[1]),
                    JX_SELECTOR_SET : lambda : node.__setitem__(expr[1],jx_code_eval(node,expr[2])),
                    JX_SELECTOR_GET : lambda : node.__getitem__(expr[1]),
                    }[expr[0]]()

        # evaluate sub-expressions

        result = [jx_code_eval_to_weak(node,i) for i in expr]
        
        # apply callable (if present)

        if jx_builtin_selector_check( result[0] ):
            return {
                JX_SELECTOR_ADD : lambda : result[1]+result[2],
                JX_SELECTOR_OUTPUT : lambda : sys.stdout.write(repr(result[1])+";\n")
                }[result[0]]()
        
        # otherwise, simply return

        return result

import copy
def jx_code_eval(node, expr):

    # enforce copy semantics

    return copy.deepcopy( jx_code_eval_to_weak(node, expr) )
    
names = {}
scanner = jx_jxon_scanner(input)
node = {}

jx_code_expose_secure_builtins(names)

done = False
while not done:
    (status,source) = scanner.next_ob()
    if status == JX_YES:
        jx_jxon_dump(sys.stdout,"# source",source,names)

        code = jx_code_bind_with_source(names,source)
        jx_jxon_dump(sys.stdout,"#   eval",code,names)

        result = jx_code_eval(node,code)
        jx_jxon_dump(sys.stdout,"# result",result,names)

    elif not jx_ok(status):
        break;
        


