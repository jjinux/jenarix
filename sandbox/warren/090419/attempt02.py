

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

# we're using Python for the following:
# * portable threads, locks, events, and scheduling
# * memory management of queued payloads
# * per node/network namespaces
# * exception handling

# within a given node/network, what must be mutually exclusive?
# * builtin nodes: all actions
# * network nodes: any read / write of network structure

import threading, Queue

class BaseNode:
        
    def __init__(self, host):
        self._host = host
        self._network = []
        self._rlock = threading.RLock()
        if host != self:
            self._host._connect( self.error, host.error)
            self._queue = host._queue
        else: # top level
            self._queue = Queue.Queue()
            
    def input(self, payload): # port
        self._enqueue( self.input, payload)
        
    def output(self, payload): # port
        self._host._enqueue( self.output, payload)

    def error(self, payload): # port
        self._host._enqueue( self.error, payload)

    def _connect(self, source, destination):
        self._rlock.acquire()
        self._network.append( (source,destination) )
        self._rlock.release()
        
    def _mutexed_call(self, port, payload, blocking=1):
        if self._rlock.acquire(blocking=1):
            result = port(payload)
            self._rlock.release()
            return (True,result)
        else:
            return (False,None)

    def _enqueue(self, source, payload):
        # host method, only called by guests
        self._rlock.acquire()
        for entry in self._network:
            if entry[0] == source:
                self._queue.put( (entry[1].im_self, entry[1], payload) ) 
        self._rlock.release()
        
    def _send(self, node, port, payload):
        # asynchronous message
        self._rlock.acquire()
        target_node = getattr(self,node)
        target_port = getattr(target_node, port)
        self._rlock.release()
        self._queue.put( (target_node, target_port, payload) )
        
    def _call(self, node, port, payload):
        # synchronous RPC with return value
        self._rlock.acquire()
        target_node = getattr(self, node)
        target_port = getattr(target_node, port)
        self._rlock.release()
        return target_node._mutexed_call(target_port,payload)[1]

import time,random

class AddsOne(BaseNode):

    def input(self, payload):
        time.sleep(random.random()/10)
        self._host._send("nested","input",11)
        self._host._call("nested","input",10)
        self.output(payload + 1)

class Negate(BaseNode):

    def input(self, payload):
        time.sleep(random.random()/10)
        self.error("some error message")
        self.output(-payload)

class Doubles(BaseNode):

    def input(self, payload):
        time.sleep(random.random()/10)
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

import Queue
import threading

toplevel = TopLevel()

# per-thread runloop

def _runloop(queue):
    while 1:
        q = queue.get()
        result = q[0]._mutexed_call(q[1], q[2], blocking=0)
        if not result[0]:
            queue.put( q ) # unable to call (busy), so put back on queue

# create thread pool

for a in range(10):
    thrd = threading.Thread(target=_runloop, args=(toplevel._queue,))
    thrd.setDaemon(1)
    thrd.start()

import sys
while 1:
    number = int(sys.stdin.readline().strip())
    toplevel.input(number)
