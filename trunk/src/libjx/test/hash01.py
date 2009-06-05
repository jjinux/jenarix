

def main():
    n = 1000000
    X = {}
    for i in xrange(1,n+1):
        X[str(i)] = i
    c = 0
    for i in xrange(1,n+1):
        c += X.has_key(str(i))
    print c

main()
