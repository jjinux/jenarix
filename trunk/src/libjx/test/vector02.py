

def dot(v0,v1):
    if len(v0)!=len(v1) :
        print "Vector size mismatch"
    else:
        return map(lambda x,y:x+y, v0,v1)

print dot([1,2,3], [2,3,4])

len( dot( range(1000000), range(1000000)));


