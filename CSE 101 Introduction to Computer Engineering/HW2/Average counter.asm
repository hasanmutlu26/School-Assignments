load R0,00
load R1,number1
load R2,othernumbers
load R3,16
load R4,01
load RF,[R1]

adding: load RE,[R2] ;First, program adds given 16 numbers.
jmpEQ RE=R0,div
addi RF,RE,RF
addi R2,R4,R2
jmp adding

div: addi R0,R3,R0 ;After adding is over, programs starts dividing. It adds 16's to 16's with a timer running at the same time and when additions get bigger than the value in FF, our division's result is shown on timer, R5.
addi R5,R4,R5
jmpEQ RF=R0,sending
jmpLE RF<=R0,sending
jmp div

sending: move R7,R0 ;At this point, program will check if it calculated one more than the actual value.
move R8,R5

check: load R0,00
load R9,255
addi R7,R9,R7
jmpEQ R7=R0,c2
addi RF,R9,RF
c1: jmpEQ RF=R0,extraction ;If added 16's and the numbers' additions don't reach to 0 at the same time, means we calculated one more than the actual value, so we will jump to substraction
c2: jmpEQ RF=R0,print ;Added 16's and the numbers' additions  will be substracted. If they come to 0 at the same time, the number is being divided to 16 fully so we don't need to worry, we can print the result.
jmp check

extraction: addi R8,R9,R8 ;We substract 1 from the result and print it.
move R0,R8
jmp hlt 

print: move R0,R5
hlt: halt




number1: db 8 ;This part is for numbers. And to be honest I could not prevent overflow so If you add numbers which their addition is not more than 255, program will work.
othernumbers: db 20
db 15
db 4
db 2
db 6
db 7
db 6
db 4 
db 9 
db 5
db 8
db 2
db 5
db 5
db 7
