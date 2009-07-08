
# demonstrates how a large object can be shared (read only) across a number of threads 

# create 400 MB array

large_ob = range(100000000)

print len(large_ob)

#share(large_ob)


print pareval( [len(large_ob), 
                len(large_ob),
                len(large_ob),
                len(large_ob),
                len(large_ob),
                len(large_ob)] )

share(large_ob, false)
