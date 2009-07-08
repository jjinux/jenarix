

# inner functions (NOTE: outer function symbols are statically closed upon by inner functions)

def fn1(a):

    c = 4
    list = [1,2,3]

    def fn2(b):
        print list
        return b+1+c  

    # closes on 4

    list = [3,4,5]
    c = 6

    return fn2(1+a)

if not __builtins__:
    print 1


print fn1(3)


   
