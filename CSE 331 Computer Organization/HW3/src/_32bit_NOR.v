module _32bit_NOR (result, A, B);
input [31:0] A, B;
output [31:0] result;

nor (result[0], A[0], B[0]);
nor (result[1], A[1], B[1]);
nor (result[2], A[2], B[2]);
nor (result[3], A[3], B[3]);
nor (result[4], A[4], B[4]);
nor (result[5], A[5], B[5]);
nor (result[6], A[6], B[6]);
nor (result[7], A[7], B[7]);
nor (result[8], A[8], B[8]);
nor (result[9], A[9], B[9]);
nor (result[10], A[10], B[10]);
nor (result[11], A[11], B[11]);
nor (result[12], A[12], B[12]);
nor (result[13], A[13], B[13]);
nor (result[14], A[14], B[14]);
nor (result[15], A[15], B[15]);
nor (result[16], A[16], B[16]);
nor (result[17], A[17], B[17]);
nor (result[18], A[18], B[18]);
nor (result[19], A[19], B[19]);
nor (result[20], A[20], B[20]);
nor (result[21], A[21], B[21]);
nor (result[22], A[22], B[22]);
nor (result[23], A[23], B[23]);
nor (result[24], A[24], B[24]);
nor (result[25], A[25], B[25]);
nor (result[26], A[26], B[26]);
nor (result[27], A[27], B[27]);
nor (result[28], A[28], B[28]);
nor (result[29], A[29], B[29]);
nor (result[30], A[30], B[30]);
nor (result[31], A[31], B[31]);

endmodule