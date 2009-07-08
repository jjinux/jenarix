
# demonstrates use of inner classes

# compatiblity kludge(s)

if __builtins__:
    exec("def new(*arg): return arg[0](*arg[1:])") # support for "new"
 
class Outer:
        
    class Inner:

        def __init__(self, mulval):
            print "inner __init__ called"
            self.mulval = mulval

        def mult(self,x):
            return [x,self.mulval,self.mulval * x]
   
    def __init__(self):
        print "outer __init__ called"
        self.inner = new(self.Inner,3)

    def doMulti(self, value):
        return self.inner.mult(5)

obj = new(Outer)

print obj.doMulti(2)





