# Code in Jupyter Notebook as Python file.

# Default values.
# Input files.
FILE1 = "booktexts/tarih1Out1.txt"
FILE2 = "booktexts/psikoloji1Out2.txt"

# Match and gap values. 
MATCH = 3
GAP = 2

#Threshold value for finding paths with minimum length of. In default, it is 1.
THRESHOLD = 1*MATCH

#maxOf function to fin max of 4 integers.
def maxOf(a,b,c,d):
    if(a >= b and a >= c and a >= d):
        return a
    if(b >= a and b >= c and b >= d):
        return b
    if(c >= a and c >= b and c >= d):
        return c
    return d

# Traceback function checks the difference between our main cell (M) and the three cells coming before it. (a,b,c)
# If their differences are equal to corresponding MATCH or GAP values, their condition is true.
# The function checks these conditions and decides which cell to go. 
# If more than one conditions are true, it chooses the cell with the highest value in order to find the longest path.
def maxTraceBack(M, a, b, c):
    conda = abs(M - a) == GAP
    condb = abs(M - b) == GAP
    condc = M - c == MATCH
    condd = M - c == -MATCH
    if(conda and condb and (condc or condd)):
        theMax = maxOf(a,b,c,0)
        if(theMax == a):
            return 1
        elif(theMax == b):
            return 2
        elif(theMax == c):
            if(condc):
                return 3
            return 4
    if(conda and condb):
        theMax = maxOf(a,b,0,0)
        if(theMax == a):
            return 1
        return 2
    
    if(conda and (condc or condd)):
        theMax = maxOf(a,c,0,0)
        if(theMax == a):
            return 1
        elif(condc):
            return 3
        return 4

    if(condb and (condc or condd)):
        theMax = maxOf(b,c,0,0)
        if(theMax == b):
            return 2
        elif(condc):
            return 3
        return 4

    if(conda):
        return 1
    elif(condb):
        return 2
    elif(condc):
        return 3
    else:
        return 4

# Input files are read, matrix is created.

fp1 = open(FILE1)
data1 = fp1.read()
lines1 = data1.split('\n')

fp2 = open(FILE2)
data2 = fp2.read()
lines2 = data2.split('\n')

while("" in lines1):
    lines1.remove("")

while("" in lines2):
    lines2.remove("")

rows, cols = (len(lines1) + 1, len(lines2) + 1)
matrix = [[0 for i in range(cols + 1)] for j in range(rows + 1)]

# Smith-Waterman algorithm part. Matrix is filled. 

for i in range (1,len(lines1) + 1):
    for j in range (1,len(lines2) + 1):
        if(lines1[i-1] == lines2[j-1]):
            match = MATCH
        else:
            match = -MATCH
        val = maxOf(matrix[i-1][j] - GAP, matrix[i][j-1] - GAP, matrix[i-1][j-1] + match, 0)
        matrix[i][j] = val
    

# In this part, the starting index candidates of every path in the matrix are found. 
# A cell is (probably) starting point of a path if its value is higher than the cells surrounding it.

iind = []
jind = []
for i in range (1,len(lines1) + 1):
    for j in range (1,len(lines2) + 1):
        if(matrix[i][j] > matrix[i-1][j] and matrix[i][j] > matrix[i][j-1] and matrix[i][j] > matrix[i-1][j-1] and
        matrix[i][j] > matrix[i+1][j] and matrix[i][j] > matrix[i][j+1] and matrix[i][j] > matrix[i+1][j+1] and
        matrix[i][j] > matrix[i-1][j+1] and matrix[i][j] > matrix[i+1][j-1] and matrix[i][j] >= THRESHOLD):
            iind.append(i)
            jind.append(j)
    
# Due to GAP2, some starting indexes might not be starting points of paths. 
# In this part, these indexes are found and removed by doing traceback.

i = -1
while(True):
    i += 1
    if(i >= len(iind)):
        break
    try:
        tempi = iind[i]
        tempj = jind[i]
    except:
        break
    while(True):
        if(matrix[tempi][tempj] == 0):
            break
        trace = maxTraceBack(matrix[tempi][tempj], matrix[tempi-1][tempj], matrix[tempi][tempj-1], matrix[tempi-1][tempj-1])
        if(trace == 1):
            tempi -= 1
        elif(trace == 2):
            tempj -= 1
        else:
            tempi -= 1
            tempj -= 1
        try:
            if(iind.index(tempi) == jind.index(tempj)):
                iind.remove(tempi)
                jind.remove(tempj)
                i -= 1
        except:
            continue

#In this part, the final traceback is completed and the output is printed.

print("Document 1: " + FILE1 + "\nDocument 2: " + FILE2)
for i in range (len(iind)):
    print("-----------------------\n" + str(i+1) + ". Alignment:")
    pathlength = 0
    out = []
    outi = []
    outj = []
    while(True):
        if(matrix[iind[i]][jind[i]] == 0):
            break
        pathlength += 1
        trace = maxTraceBack(matrix[iind[i]][jind[i]], matrix[iind[i]-1][jind[i]], matrix[iind[i]][jind[i]-1], matrix[iind[i]-1][jind[i]-1])
        outi.append(iind[i])
        outj.append(jind[i])
        if(trace == 1):
            out.append("---------GAP1---------")
            iind[i] -= 1
        elif(trace == 2):
            out.append("---------GAP1---------")
            jind[i] -= 1
        elif(trace == 3):
            out.append(lines1[iind[i]-1])
            iind[i] -= 1
            jind[i] -= 1
        elif(trace == 4):
            out.append("---------GAP2--------")
            iind[i] -= 1
            jind[i] -= 1

    print("Path length: " + str(pathlength))
    for j in range (len(out) - 1, -1, -1):
        print(str(outi[j]) + " - " + str(outj[j]) + ": " + out[j])

if(len(iind) == 0):
    print("There isn't any matching lines in these two document.")


