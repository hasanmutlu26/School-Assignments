module _32bit_4x1MUX(result, A, B, C, D, control);

input [31:0] A, B, C, D;
input [1:0] control;
output [31:0] result;

wire [31:0] m1, m2;

_32bit_2x1MUX mux1(m1, A, B, control[0]);
_32bit_2x1MUX mux2(m2, C, D, control[0]);

_32bit_2x1MUX mux3(result, m1, m2, control[1]);

endmodule