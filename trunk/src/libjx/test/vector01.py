
def v_add(v0,v1):

    if len(v0)!=len(v1):
        print "Vector size mismatch"
    else:
        r = []
        while v0:
            r.append(v0.pop() + v1.pop())
        r.reverse()
        return r

print v_add([1,2,3], [2,3,4])

print len( v_add( range(1000000), range(1000000)))



