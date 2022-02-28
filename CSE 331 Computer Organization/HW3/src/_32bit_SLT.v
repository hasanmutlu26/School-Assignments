module _32bit_SLT (result, A, B);
input [31:0] A, B;
output [31:0] result;
reg [31:0] temp;
wire [31:0] w1;

not (result[1], 1'b1);
not (result[2], 1'b1);
not (result[3], 1'b1);
not (result[4], 1'b1);
not (result[5], 1'b1);
not (result[6], 1'b1);
not (result[7], 1'b1);
not (result[8], 1'b1);
not (result[9], 1'b1);
not (result[10], 1'b1);
not (result[11], 1'b1);
not (result[12], 1'b1);
not (result[13], 1'b1);
not (result[14], 1'b1);
not (result[15], 1'b1);
not (result[16], 1'b1);
not (result[17], 1'b1);
not (result[18], 1'b1);
not (result[19], 1'b1);
not (result[20], 1'b1);
not (result[21], 1'b1);
not (result[22], 1'b1);
not (result[23], 1'b1);
not (result[24], 1'b1);
not (result[25], 1'b1);
not (result[26], 1'b1);
not (result[27], 1'b1);
not (result[28], 1'b1);
not (result[29], 1'b1);
not (result[30], 1'b1);
not (result[31], 1'b1);

_32bit_subtractor g0 (w1, A, B);
or (result[0], w1[31], 1'b0);

endmodule