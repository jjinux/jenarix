'''
Created on May 12, 2009

@author: D. Potts
'''

# Copyright (c) 2009, DeLano Scientific LLC, Palo Alto, California, USA.
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#  notice, this list of conditions and the following disclaimer in the
#  documentation and/or other materials provided with the distribution.
#
# * Neither the name of the DeLano Scientific LLC nor the names of its
#  contributors may be used to endorse or promote products derived from
#  this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


import sys

class Symbol(object):
    names_dict = {}

    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return "<%s>" % self.name

    @staticmethod
    def new(name):
        if name in Symbol.names_dict:
            return Symbol.names_dict[name]
        new_symbol = Symbol(name)
        Symbol.names_dict[name] = new_symbol
        return new_symbol

keyword_list = ["IMPORT", "FROM", "NODE", "PORT", "EOF"]
Symbol.import_ = Symbol.new("IMPORT")
Symbol.from_ = Symbol.new("FROM")
Symbol.node = Symbol.new("NODE")
Symbol.port = Symbol.new("PORT")
Symbol.identifier = Symbol.new("Identifier")
Symbol.number = Symbol.new("Number")
Symbol.operator = Symbol.new("Operator")

Symbol.EOF = Symbol.new("EOF") # To be removed once reading true EOF in files


class cradle():
    '''
    classdocs
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.Look = None  # Lookahead character
        self.LCount = 0   # Label counter
        self.Token = None # Current token (uses Symbol.*)
        self.Value = None # String token of self.Look
        self.get_char()   # Prime the pump, so to speak

    def error(self, text):
        "Report an Error"
        sys.stdout.write("\nError: %s." % (text))

    def abort(self, text):
        "Report Error and Halt"
        self.error(text)
        sys.exit(1)

    def expected(self, text):
        "Report What Was Expected"
        self.abort(text + " Expected")

    def emit_line(self, text):
        "Output a String with Tab and Newline"
        sys.stdout.write("\t" + text + "\n")

    def get_char(self):
        "Read New Character From Input Stream"
        self.Look = sys.stdin.read(1)

    def match(self, character):
        "Match a Specific Input Character"
        if self.Look == character:
            self.get_char()
        else:
            self.expected("'%c'" % character)

    def is_addop(self, character):
        "Recognize an Addop"
        return character in ["+", "-"]

    def is_op(self, character):
        "Recognize Any Operator"
        return character in ["|", "+", "-", "*", "/", "="]

    def is_white(self, character):
        "Recognize White Space"
        return character.isspace() and character != "\n"

    def skip_white(self):
        "Skip Over Leading White Space"
        while self.is_white(self.Look):
            self.get_char()

    def skip_comma(self):
        "Skip Over a Comma"
        self.skip_white()
        if self.Look == ",":
            self.get_char()
            self.skip_white()

    def fin(self):
        "Skip a \n"
        if self.Look == "\n":
            self.get_char()
        self.skip_white()

    def get_name(self):
        "Get an Identifier"
        self.Value = ""
        if not self.Look.isalpha():
            self.expected("Name")
        while self.Look.isalnum() or self.Look in ["_"]:
            self.Value = self.Value + self.Look.upper()
            self.get_char()
        if self.Value in keyword_list:
            self.Token = Symbol.new(self.Value)
        else:
            self.Token = Symbol.identifier

    def get_num(self):
        "Get a Number"
        self.Value = ""
        if not self.Look.isdigit():
            self.expected("Integer")
        while self.Look.isdigit():
            self.Value = self.Value + self.Look
            self.get_char()
        self.Token = Symbol.number

    def get_op(self):
        self.Value = ""
        if not self.is_op(self.Look):
            self.expected("Operator")
        while self.is_op(self.Look):
            self.Value = self.Value + self.Look
            self.get_char()
        self.Token = Symbol.operator

    def ident(self):
        "Parse and Translate an Identifier"
        name = self.get_name()
        if self.Look == "(":
            self.match("(")
            self.match(")")
            self.emit_line("BSR " + name + " [call routine]")
        else:
            self.emit_line("MOVE " + name + "(PC),D0 [load variable]")

    def factor(self):
        "Parse and Translate a Math Factor"
        if self.Look == "(":
            self.match("(")
            self.expression()
            self.match(")")
        elif self.Look.isalpha():
            self.ident()
        else:
            self.emit_line("MOVE #" + self.get_num() + ",D0")

    def multiply(self):
        "Recognize and Translate a Multiply"
        self.match("*")
        self.factor()
        self.emit_line("MULS (SP)+,D0 [pop stack]")

    def divide(self):
        "Recognize and Translate a Divide"
        self.match("/")
        self.factor()
        self.emit_line("MOVE (SP)+,D1 [pop stack]")
        self.emit_line("DIVS D1,D0")

    def add(self):
        "Recognize and Translate an Add"
        self.match("+")
        self.term()
        self.emit_line("ADD (SP)+,D0 [pop stack]")

    def subtract(self):
        "Recognize and Translate a Subtract"
        self.match("-")
        self.term()
        self.emit_line("SUB (SP)+,D0 [pop stack]")
        self.emit_line("NEG D0")

    def term(self):
        "Parse and Translate a Math Term"
        self.factor()
        while self.Look in ["*", "/"]:
            self.emit_line("MOVE D0,-(SP) [push stack]")
            if self.Look == "*":
                self.multiply()
            elif self.Look == "/":
                self.divide()

    def expression(self):
        "Parse and Translate an Expression"
        if self.is_addop(self.Look):
            self.emit_line("CLR D0")
        else:
            self.term()
        while self.is_addop(self.Look):
            self.emit_line("MOVE D0,-(SP) [push stack]")
            if self.Look == "+":
                self.add()
            elif self.Look == "-":
                self.subtract()

    def assignment(self):
        "Parse and Translate an Assignment Statement"
        name = self.get_name()
        self.match("=")
        self.expression()
        self.emit_line("LEA " + name + "(PC),A0 [assign to variable]")
        self.emit_line("MOVE D0,(A0)")

    def other(self):
        "Recognize and Translate an 'Other'"
        self.emit_line(self.get_name())

    def block(self):
        "Recognize and Translate a Statement Block"
        while self.Look not in ["e", "l"]:
            if self.Look == "i":
                self.do_if()
            else:
                self.other()

    def do_program(self):
        "Parse and Translate a Program"
        self.prolog()
        self.block()
        if self.Look != "e":
            self.expected("End")
        self.epilog()
        self.emit_line("END")

    def gen_new_label(self):
        "Generate a Unique Label"
        retval = "L" + str(self.LCount)
        self.LCount += 1
        return retval

    def post_label(self, label):
        "Post a Label To Output"
        sys.stdout.write(label + ":\n")

    def do_if(self):
        "Recognize and Translate an IF Construct"
        self.match("i")
        self.condition()
        L1 = self.gen_new_label()
        L2 = L1
        self.emit_line("BEQ " + L1 + " [branch/skip to label if true]")
        self.block()
        if self.Look == "l":
            self.match("l")
            L2 = self.gen_new_label()
            self.emit_line("BRA " + L2 + " [branch/skip to label]")
            self.post_label(L1)
            self.block()
        self.match("e")
        self.post_label(L2)

    def condition(self):
        self.emit_line("<condition>")

    def scan(self):
        "Lexical Scanner"
        while self.Look == "\n":
            self.fin()
        if self.Look.isalpha():
            self.get_name()
        elif self.Look.isdigit():
            self.get_num()
        elif self.is_op(self.Look):
            self.get_op()
        else:
            self.Value = self.Look
            self.Token = Symbol.operator
            self.get_char()
        self.skip_white()

    def main(self):
        while self.Token != Symbol.EOF:
            self.scan()
            if self.Token in \
                [Symbol.identifier, Symbol.number, Symbol.operator]:
                sys.stdout.write(str(self.Token) + " ")
            elif self.Token in \
                [Symbol.import_, Symbol.from_, Symbol.node, Symbol.port]:
                sys.stdout.write("<Keyword> ")
            sys.stdout.write(self.Value + "\n")
