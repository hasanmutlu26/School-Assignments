module _32bit_SUB(result, A, B);
input [31:0] A, B;
output [31:0] result;
wire [31:0] w1;

_32bit_XOR g0(w1, B, 32'b1111_1111_1111_1111_1111_1111_1111_1111);
_32bit_ADD g1 (result, A, w1, 1'b1);

endmodule