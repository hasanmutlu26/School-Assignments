module _32bit_ADD(result,A,B,C0);
input [31:0] A, B;
input C0;
output [31:0] result;
wire C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15, C16, C17, C18, C19, C20, C21, C22, C23, C24, C25, C26, C27, C28, C29, C30, C31, C32;

//Inresulttantiate the full addder
full_adder FA0(result[0], C1, A[0], B[0], C0);
full_adder FA1(result[1], C2, A[1], B[1], C1);
full_adder FA2(result[2], C3, A[2], B[2], C2);
full_adder FA3(result[3], C4, A[3], B[3], C3);
full_adder FA4(result[4], C5, A[4], B[4], C4);
full_adder FA5(result[5], C6, A[5], B[5], C5);
full_adder FA6(result[6], C7, A[6], B[6], C6);
full_adder FA7(result[7], C8, A[7], B[7], C7);
full_adder FA8(result[8], C9, A[8], B[8], C8);
full_adder FA9(result[9], C10, A[9], B[9], C9);
full_adder FA10(result[10], C11, A[10], B[10], C10);
full_adder FA11(result[11], C12, A[11], B[11], C11);
full_adder FA12(result[12], C13, A[12], B[12], C12);
full_adder FA13(result[13], C14, A[13], B[13], C13);
full_adder FA14(result[14], C15, A[14], B[14], C14);
full_adder FA15(result[15], C16, A[15], B[15], C15);
full_adder FA16(result[16], C17, A[16], B[16], C16);
full_adder FA17(result[17], C18, A[17], B[17], C17);
full_adder FA18(result[18], C19, A[18], B[18], C18);
full_adder FA19(result[19], C20, A[19], B[19], C19);
full_adder FA20(result[20], C21, A[20], B[20], C20);
full_adder FA21(result[21], C22, A[21], B[21], C21);
full_adder FA22(result[22], C23, A[22], B[22], C22);
full_adder FA23(result[23], C24, A[23], B[23], C23);
full_adder FA24(result[24], C25, A[24], B[24], C24);
full_adder FA25(result[25], C26, A[25], B[25], C25);
full_adder FA26(result[26], C27, A[26], B[26], C26);
full_adder FA27(result[27], C28, A[27], B[27], C27);
full_adder FA28(result[28], C29, A[28], B[28], C28);
full_adder FA29(result[29], C30, A[29], B[29], C29);
full_adder FA30(result[30], C31, A[30], B[30], C30);
full_adder FA31(result[31], C32, A[31], B[31], C31);
				
endmodule
 
