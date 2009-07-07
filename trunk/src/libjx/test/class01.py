
# add "new" to python (compatibility kludge)

if __builtins__:
    def new(klass): return klass()

# the following code runs under both jxp and python (hooray!)

class myClass:

    def __init__(self):
        print "constructed"
        self.attr1 = 100
        self.attr2 = "just what are we thinking?"
#        print symbols()

    def methodA(self):
        print "methodA called"
        print symbols()
        return [1, 2, 3, self.attr1, self.attr2]

    def methodB(self,limit):
        print "methodB called"
        for b in range(limit):
            print b

#print symbols(false)

a = new(myClass)

print a

print a.methodA()

a.methodB(5)

a = "instance destroyed!"

print a



