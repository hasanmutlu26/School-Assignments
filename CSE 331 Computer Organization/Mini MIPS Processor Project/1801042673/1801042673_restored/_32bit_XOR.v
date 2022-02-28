module _32bit_XOR(result, A, B);
input [31:0] A, B;
output [31:0] result;

xor (result[0], A[0], B[0]);
xor (result[1], A[1], B[1]);
xor (result[2], A[2], B[2]);
xor (result[3], A[3], B[3]);
xor (result[4], A[4], B[4]);
xor (result[5], A[5], B[5]);
xor (result[6], A[6], B[6]);
xor (result[7], A[7], B[7]);
xor (result[8], A[8], B[8]);
xor (result[9], A[9], B[9]);
xor (result[10], A[10], B[10]);
xor (result[11], A[11], B[11]);
xor (result[12], A[12], B[12]);
xor (result[13], A[13], B[13]);
xor (result[14], A[14], B[14]);
xor (result[15], A[15], B[15]);
xor (result[16], A[16], B[16]);
xor (result[17], A[17], B[17]);
xor (result[18], A[18], B[18]);
xor (result[19], A[19], B[19]);
xor (result[20], A[20], B[20]);
xor (result[21], A[21], B[21]);
xor (result[22], A[22], B[22]);
xor (result[23], A[23], B[23]);
xor (result[24], A[24], B[24]);
xor (result[25], A[25], B[25]);
xor (result[26], A[26], B[26]);
xor (result[27], A[27], B[27]);
xor (result[28], A[28], B[28]);
xor (result[29], A[29], B[29]);
xor (result[30], A[30], B[30]);
xor (result[31], A[31], B[31]);

endmodule