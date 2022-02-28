module _32bit_2scomplement(result,A);
input [31:0] A;
output [31:0] result;
wire [31:0] w1;

_32bit_xor g0(w1, A, 32'b1111_1111_1111_1111_1111_1111_1111_1111);

_32bit_adder g1(result, w1, 32'b0000_0000_0000_0000_0000_0000_0000_0001, 1'b0);

endmodule