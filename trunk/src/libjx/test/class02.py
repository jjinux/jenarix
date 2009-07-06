
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

m1 = new(SquareMatrix,3000)

print m1.dim

#  jxp:    8.15 sec (optimized 4-6-8 build, w/o heap tracker, on mac pro)
#  python: 3.52 sec
#  -> jxp is 2.3x slower

# note: unoptimized jxp w/ heap tracker takes 21.7 seconds.




