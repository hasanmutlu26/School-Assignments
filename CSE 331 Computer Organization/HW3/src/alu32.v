module alu32 (result, A, B, ALUop);
input [31:0] A, B;
input [2:0] ALUop;
output [31:0] result;
wire [31:0] wAdd, wXor, wSub, wMult, wSlt, wNor, wAnd, wOr;

_32bit_adder g0 (wAdd, A, B, 1'b0);

_32bit_xor g1 (wXor, A, B);

_32bit_subtractor g2 (wSub, A, B);

/*mult*/

_32bit_SLT g4 (wSlt, A, B);

_32bit_NOR g5 (wNor, A, B);

_32bit_AND g6 (wAnd, A, B);

_32bit_OR g7 (wOr, A, B);

_32bit_8x1MUX g8(result, wAdd, wXor, wSub, wMult, wSlt, wNor, wAnd, wOr, ALUop);



endmodule