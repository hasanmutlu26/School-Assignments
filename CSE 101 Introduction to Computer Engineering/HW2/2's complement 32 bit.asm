load R0,00 ;first 8 digit, 
load R1,00 ;second 8 digit, 
load R2,00 ;third 8 digit
load R3,00 ;fourth 8 digit , You can write your number in places given as 00
load R5,255 ;FF
load R6,01 ; number 1 to adding
xor R0,R0,R5 ;1's complement
xor R1,R1,R5 ;1's complement
xor R2,R2,R5 ;1's complement
xor R3,R3,R5 ;1's complement

move R7,R0 ; in this part, program will check the values in order to avoid from overflow because the whole number is located partly in two different registers
move R0,R3 ; program moves values because jmpEQ command only works for R0
jmpEQ R5=R0,c1 ; if last 8 digits of our binary number is all l, which means hex number is FF, it will make the addition to the next 8 digits, it will jump to that command
addi R0,R0,R6 ; if not, there is no problem, it makes the addition to 1's complement and we find 2's complement
move R3,R0
move R0,R7
jmp hlt

c1: move R7,R0 ; in this part, program will check the values in order to avoid from overflow because the whole number is located partly in two different registers
move R0,R2 ; program moves values because jmpEQ command only works for R0
jmpEQ R5=R0,c2 ; if next 8 digits  of our binary number are all one, which means this hex number is also FF, it will make the addition to the next 8 digits, it will jump to that command
addi R0,R0,R6 ; if not, there is no problem, it makes the addition to 1's complement and we find 2's complement
move R2,R0
move R0,R7
jmp hlt

c2: move R7,R0 ; in this part, program will check the values in order to avoid from overflow because the whole number is located partly in two different registers
move R0,R1 ; program moves values because jmpEQ command only works for R0
jmpEQ R5=R0,c3 ; if next 8 digits  of our binary number are all one, which means this hex number is also FF, it will make the addition to the first 8 digits, it will jump to that command
addi R0,R0,R6 ; if not, there is no problem, it makes the addition to 1's complement and we find 2's complement
move R1,R0
move R0,R7
jmp hlt

c3: move R1,R0 ;in this part, program will check if the value was zero. If it was, it will change it back to zero because there isn't negative of zero.
move R0,R7
jmpEQ R5=R0,zerostate ; if first 8 digits are also FF, we had taken 1's complement of zero so program will change the values back to zero.
addi R0,R0,R6 ;if not, it makes the addition to 1's complement and we find 2's complement
jmp hlt
zerostate: load R0,00 ; if we are trying to take 2's complemet of 0, program will not do that operation because it is wrong mathematically. Program changes the values back to zero.
load R1,00
load R2,00
load R3,00
hlt: halt

