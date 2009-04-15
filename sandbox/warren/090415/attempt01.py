
class BaseNode:
        
    def __init__(self, host):
        self._host = host
        self._network = []
        if host != self:
            self._host._connect( self.error, host.error) 
        
    def input(self, payload):
        self._enqueue( self.input, payload)
        
    def output(self, payload):
        self._host._enqueue( self.output, payload)

    def error(self, payload):
        self._host._enqueue( self.error, payload)

    def _connect(self, source, destination):
        self._network.append( (source,destination) )
        
    def _enqueue(self, source, payload):
        # will become asynchronous once we have concurrency
        for entry in self._network:
            if entry[0] == source:
                entry[1](payload)

    def _send(self, node, port, payload):
        # will become asynchronous once we have concurrency
        target_node = getattr(self,node)
        target_port = getattr(target_node, port)
        target_port(payload)
        
    def _call(self, node, port, payload): # synchronous
        target_node = getattr(self,node)
        target_port = getattr(target_node, port)
        return target_port(payload)

class AddsOne(BaseNode):

    def input(self, payload):
        self.output(payload + 1)
        self._host._call("nested","input",10)

class Negate(BaseNode):

    def input(self, payload):
        self.error("some error message")
        self.output(-payload)

class Doubles(BaseNode):

    def input(self, payload):
        self.output(payload * 2)

class NestedNetwork(BaseNode):

    def __init__(self,host):
        BaseNode.__init__(self,host)

        self.negate = Negate(self)
        self._connect( self.input, self.negate.input)
        self._connect( self.negate.output, self.output)

    def input(self,payload):
        BaseNode.input(self,payload)
        
class TopLevel(BaseNode):

    def __init__(self,host=None):
        BaseNode.__init__(self,self)
        
        self.addsOne = AddsOne(self)
        self.doubles = Doubles(self)
        self.nested = NestedNetwork(self)
        
        self._connect( self.input, self.addsOne.input )
        self._connect( self.input, self.doubles.input )        
        self._connect( self.addsOne.output, self.doubles.input )
        self._connect( self.doubles.output, self.nested.input )
        self._connect( self.nested.output, self.output )

    def output(self, payload):
        print "output:",payload

    def error(self, payload):
        print "error:",payload
        

toplevel = TopLevel()

import sys
while 1:
    number = int(sys.stdin.readline().strip())
    toplevel.input(number)
