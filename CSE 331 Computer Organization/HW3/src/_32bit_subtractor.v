module _32bit_subtractor(result, A, B);
input [31:0] A, B;
output [31:0] result;
wire [31:0] w1;

_32bit_2scomplement g0 (w1, B);
_32bit_adder  g1 (result, A, w1, 1'b0);

endmodule