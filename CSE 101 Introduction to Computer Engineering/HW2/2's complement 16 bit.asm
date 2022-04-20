load R0,00 ;first 8 digit, 
load R1,00 ;second 8 digit, You can write your number in places given as 00
load R5,255 ;FF
load R6,01 ; number 1 to adding
xor R0,R0,R5 ;1's complement
xor R1,R1,R5 ;1's complement

move R7,R0 ; in this part, program will check the values in order to avoid from overflow because the whole number is located partly in two different registers
move R0,R1 ;program moves values because jmpEQ command only works for R0
jmpEQ R5=R0,c1 ; if last 8 digits of our binary number is 11111111, which means hex number located in register is FF, it will make the addition to the first 8 digits, it will jump to that command
addi R0,R0,R6 ; if not, there is no problem, it makes the addition to 1's complement and we find 2's complement
move R1,R0
move R0,R7
jmp hlt

c1: move R1,R0 ;in this part, program will check if the value was zero. If it was, it will change it back to zero because there isn't negative of zero.
move R0,R7
jmpEQ R5=R0,zerostate ;if first 8 digits are also FF, we had taken 1's complement of zero so program will change the values back to zero.
addi R0,R0,R6 ;if not, it makes the addition to 1's complement and we find 2's complement
jmp hlt
zerostate: load R0,00
load R1,00
hlt: halt

