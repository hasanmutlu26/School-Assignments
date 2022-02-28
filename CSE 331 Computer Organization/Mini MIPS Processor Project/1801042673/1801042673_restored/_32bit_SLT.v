module _32bit_SLT (result, A, B);
input [31:0] A, B;
output [31:0] result;

wire [31:0] w1;

_32bit_SUB c0 (w1, A, B);
_32bit_OR c1 (result, {31'b0000_0000_0000_0000_0000_0000_0000_000,w1[31]}, 32'b0000_0000_0000_0000_0000_0000_0000_0000);

endmodule