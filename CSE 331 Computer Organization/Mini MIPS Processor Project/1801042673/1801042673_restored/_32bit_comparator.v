module _32bit_comparator(result, A, B);
input [31:0] A, B;
output result;
wire wXor0, wXor1, wXor2, wXor3, wXor4, wXor5, wXor6, wXor7, wXor8, wXor9, wXor10, wXor11, wXor12, wXor13, wXor14, wXor15, wXor16, wXor17, wXor18, wXor19, wXor20, wXor21, wXor22, wXor23, wXor24, wXor25, wXor26, wXor27, wXor28, wXor29, wXor30, wXor31;

xor (wXor0, A[0], B[0]);
xor (wXor1, A[1], B[1]);
xor (wXor2, A[2], B[2]);
xor (wXor3, A[3], B[3]);
xor (wXor4, A[4], B[4]);
xor (wXor5, A[5], B[5]);
xor (wXor6, A[6], B[6]);
xor (wXor7, A[7], B[7]);
xor (wXor8, A[8], B[8]);
xor (wXor9, A[9], B[9]);
xor (wXor10, A[10], B[10]);
xor (wXor11, A[11], B[11]);
xor (wXor12, A[12], B[12]);
xor (wXor13, A[13], B[13]);
xor (wXor14, A[14], B[14]);
xor (wXor15, A[15], B[15]);
xor (wXor16, A[16], B[16]);
xor (wXor17, A[17], B[17]);
xor (wXor18, A[18], B[18]);
xor (wXor19, A[19], B[19]);
xor (wXor20, A[20], B[20]);
xor (wXor21, A[21], B[21]);
xor (wXor22, A[22], B[22]);
xor (wXor23, A[23], B[23]);
xor (wXor24, A[24], B[24]);
xor (wXor25, A[25], B[25]);
xor (wXor26, A[26], B[26]);
xor (wXor27, A[27], B[27]);
xor (wXor28, A[28], B[28]);
xor (wXor29, A[29], B[29]);
xor (wXor30, A[30], B[30]);
xor (wXor31, A[31], B[31]);

nor (result, wXor0, wXor1, wXor2, wXor3, wXor4, wXor5, wXor6, wXor7, wXor8, wXor9, wXor10, wXor11, wXor12, wXor13, wXor14, wXor15, wXor16, wXor17, wXor18, wXor19, wXor20, wXor21, wXor22, wXor23, wXor24, wXor25, wXor26, wXor27, wXor28, wXor29, wXor30, wXor31);

endmodule