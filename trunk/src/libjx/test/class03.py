

# compatiblity kludge(s)

if __builtins__:
    exec("def new(*arg): return arg[0](*arg[1:])") # support for "new"
 
class Secondary:

    def __init__(self, mulval):
        print "secondary __init__ called"
        self.mulval = mulval
 
    def mult(self,x):
        return [x,self.mulval,self.mulval * x]
   
class Primary:
        
    def __init__(self):
        print "primary __init__ called"
        self.secondary = new(Secondary,3)

    def doMulti(self, value):
        return self.secondary.mult(5)

obj = new(Primary)

print obj.doMulti(2)




