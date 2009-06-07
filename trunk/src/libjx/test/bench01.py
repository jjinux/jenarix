size = 30
iter = 100

def mkmatrix(rows, cols):
    count = 1
    mx = [ None ] * rows
    for i in range(rows):
        mx[i] = [0] * cols
        for j in range(cols):
            mx[i][j] = count
            count += 1
    return mx

def mmult(rows, cols, m1, m2):
    m3 = [ None ] * rows
    for i in range( rows ):
        m3[i] = [0] * cols
        for j in range( cols ):
            val = 0
            for k in range( cols ):
                val += m1[i][k] * m2[k][j]
            m3[i][j] = val

    return m3

def main():
    m1 = mkmatrix(size, size)
    m2 = mkmatrix(size, size)
    for i in range(iter):
        mm = mmult(size, size, m1, m2)
    print mm[0][0], mm[2][3], mm[3][2], mm[4][4]

main()

