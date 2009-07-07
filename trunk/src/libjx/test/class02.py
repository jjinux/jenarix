
# this example shows how the constructor can take arguments

# compatiblity kludge(s)

if __builtins__:
    exec("def new(*arg): return arg[0](*arg[1:])") # support for "new"
    
class SquareMatrix:

    def __init__(self, size):
        self.dim = [size, size]
        data = []
        for a in range(size):
            for b in range(size):
                if a==b:
                    data.append(1.0)
                else:
                    data.append(0.0)
        self.data = data
        print "Initialized with "+str(len(self.data))+" values."

m1 = new(SquareMatrix,300)
print m1.dim
print len(m1.data)


# with a value of 3000...

#  jxp:    8.48 sec (optimized 4-6-8 build, w/o heap tracker, on mac pro)
#  python: 3.57 sec
#  -> jxp is 2.4x slower

# note: unoptimized jxp w/ heap tracker takes ~22 seconds.




