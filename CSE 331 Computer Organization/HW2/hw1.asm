.data

size: .word 10				# size of the arrays
numOfArrays: .word 6			# number of arrays below

array1: .word 3,10,7,9,4,11,8,6,12,13	# the arrays to find the sequence from
array2: .word 6,8,5,1,6,9,4,3,8,10
array3: .word 1,2,3,4,5,6,7,8,9,10
array4: .word 10,9,8,7,6,5,4,3,2,1
array5: .word 1,3,5,7,9,2,4,6,8,10
array6: .word 3,15,16,7,11,16,13,17,1,6


weight: .word 1,0,0,0,0,0,0,0,0,0	# weight array to record weights of numbers in array according to their indexes
longest: .word 0,0,0,0,0,0,0,0,0,0	# array to assign elements of the subsequence in decreasing order

lenght: .word 1				# lenght of subsequence. its final value will be the size
endIndex: .word 0			# index of the greatest element of subsequence in the array


whiteSpace: .asciiz " "			# string to print whitespace
sizeText: .asciiz " Size: "		# string to print Size: 
newLine: .asciiz "\n"			# string to print newline


.text

# 	MAIN
###############################################################################################################
main:
lw $s0, lenght		# s0 = lenght
lw $s1, endIndex	# s1 = endIndex
la $s6, array1		# s6 = &array[0]
la $s7, weight		# s7 = &weight[0]

li $s4, 0		# array counter to count how many arrays have been covered

# for(int i = 0; i < numOfArrays, i++)
mainloop:
la $s5, returnfor1	# address to return to after for1 is saved to s5
j for1			# enter for1
returnfor1: 		# continue after for1

jal assign_Longest	# enter assign_Longest procedure

jal print		# enter print procedure

li $s0, 1		# lenght set back to 1
li $s1, 0		# endIndex set back to 0
lw $t1, size		# t1 = size
sll $t1, $t1, 2		# t1 = 4*size
add $s6, $s6, $t1	# s6 is incremented by 4*size, which means it now points to the next array
addi $s4, $s4, 1	# array counter. 
lw $t2, numOfArrays	# t2 = numOfArrays
blt $s4, $t2, mainloop	# if array counter is less than numOfArrays, continue the main loop

j finish		# else, jump to finish
###############################################################################################################



#	FOR1 PROCEDURE IS THE OUTER FOR LOOP IN THE C CODE - for (int i = 1; i < N; i++)
###############################################################################################################
for1:
#	initial state before start of first for loop
move $t0, $s6		# t0 = &array[0], it will be iterated
la $t1, weight		# t1 = &weight[0], it will be iterated
li $s2, 4		# s2 = i*4, i will iterate arrays so that it is loaded as 4*i

forloop1:
add $t0, $s6, $s2	# t0 = &array[i], 
add $t1, $s7, $s2	# t1 = &weight[i]

li $t4, 1		# t4 = 1
sw $t4, ($t1)		# weight[i] = 1

subi $s3, $s2, 4	# s3 = 4*j = 4*i - 4, it is used for iteration as well
jal for2		# jump and link to nested for

lw $t4, ($t1)		# t4 = weight[i]
slt $t4, $s0, $t4	# if(weight[i] > lenght) t4 = 1
beq $t4, $zero, ifnot	# if t4 != 1, don't go in if block

# 	if(weight[i] > lenght) satisfied
move $s1, $s2		# endIndex = 4*i
srl $s1, $s1, 2		# endIndex = 4*i/4 = i
sw $s1, endIndex		# endIndex stored
lw $t4, ($t1)		# t4 = weight[i]
move $s0, $t4		# lenght = weight[i]
sw $s0, lenght	# lenght stored

ifnot:			# increment state of the for loop
addi $s2, $s2, 4	# i++
lw $t4, size		# t4 = N
sll $t4, $t4, 2		# t4 = 4*t4, it is made because it will be compared with 4*i
bne $t4, $s2, forloop1	# if(t4 != N), continue loop

jr $s5			# else, return to main			
###############################################################################################################



# 	THIS IS THE NESTED FOR LOOP IN THE C CODE - for (int j = i - 1; j >= 0; j--)
###############################################################################################################
for2:
add $t2, $s6, $s3	# t2 = &array[j]
add $t3, $s7, $s3	# t3 = &weight[j]

lw $t4, ($t3)		# t4 = weight[j]
#addi $t4, $t4, 1 	# t4 = weight[j] + 1

lw $t5, ($t1)		# t5 = weight[i]
sge  $t4, $t4, $t5	# if(weight[j] >= weight[i]) t4 = 1

lw $t5, ($t2)		# t5 = array[j]
lw $t6, ($t0)		# t6 = array[i]
slt $t5, $t5, $t6	# if (array[j] < array [i]) t5 = 1

and $t4, $t4, $t5	# (array[i] > array[j] && weight[j] >= weight[i]) - t4 && t5
beq $t4, $zero, ifnot4	# if t4 && t5 == 0 , jump ifnot4, if statement is not satisfied

# 	(weight[j] + 1 > weight[i] && array[j] < array[i]) satisfied
lw $t4, ($t3)		# t4 = weight[j]
addi $t4, $t4, 1	# t4 = weight[j] + 1
sw $t4, ($t1)		# weight[i] = weight[j] + 1

ifnot4:			# increment state of the nested for loop
subi $s3, $s3, 4	# j--
bge $s3, $zero, for2	# if(j >= 0), continue loop

jr $ra			# else, return to calling procedure, which is outer for loop
###############################################################################################################



#	THIS IS THE FOR LOOP WHERE ELEMENTS ARE STORED IN "longest" ARRAY - for(int i = endIndex; i >= 0 ; i--)
###############################################################################################################
assign_Longest:
#	initial state before start of assign_Longest loop
add $t0, $s6, $zero	# t0 = &array[0]
add $t1, $s7, $zero	# t1 = &weight[0]
la $t4, longest		# t4 = &longest[0]
li $s2, 0		# s2 = k
move $s3, $s1 		# s3 = i = endIndex
sll $s3, $s3, 2		# s3 = 4*i


assignLoop:
add $t1, $s7, $s3	# t1 = &weight[i]
lw $t5, ($t1)		# t5 = weight[i]

sub $t6, $s0, $s2	# t6 = maxLEngth - k
bne $t5, $t6, ifnot5	# if weight[i] != lenght - k, jump ifnot 3, condition not satisfied

add $t0, $s6, $s3	# t0 = &array[i]
lw $t5, ($t0)
sw $t5, ($t4)		# longest[k] = array[i]
addi $t4, $t4, 4	# t4 = longest[k+1]
addi $s2, $s2, 1	# k++


ifnot5:
addi $s3, $s3, -4	# s3 = 4*i - 4
bge  $s3, $zero, assignLoop	#if i >= 0, , loop continues

jr $ra			# else, return to main
###############################################################################################################



#	THIS IS THE FOR LOOP WHERE SUBSEQUENCE ELEMENTS ARE PRINTED IN THE C CODE - for(int i = k; i >= 0; i--)
###############################################################################################################
print:
#	initial state before start of print loop
subi $s2, $s2, 1	# s2 = k--
move $s3, $s2		# s3 = i = k, this time no need to use i for iteration so it isn't multiplied by 4
subi $t4, $t4, 4	# t4 = longest[i--]

printloop:
li $v0, 1		# print int service code for syscall
lw $a0, ($t4)		# a0 = longest[i]
syscall			# one element is printed

li $v0, 4		# print string service code for syscall
la $a0, whiteSpace	# a0 = &whitespace
syscall			# print whitespace

subi $t4, $t4, 4	# t4 = longest[i--]
subi $s3, $s3, 1	# i--
bge $s3, $zero, printloop	# if(i >= 0), continue loop

la $a0, sizeText	# a0 = &sizeText, service code is already 4 
syscall			# print " Size: "

li $v0, 1		# print int service code for syscall
move $a0, $s0		# a0 = lenght, lenght's final value is the size
syscall			# size is printed

li $v0, 4		# print string service code for syscall
la $a0, newLine		# a0 = &newLine
syscall			# print newLine

jr $ra
###############################################################################################################


finish:
