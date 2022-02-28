module ALU(_32bit_result, equal, A, B, ALUOp);
input [31:0] A, B;
input [2:0] ALUOp;
output [31:0]_32bit_result;
output equal;
wire [31:0] wAnd, wAdd, wSub, wXor, wNor, wOr;
wire wSlt;

_32bit_AND g0(wAnd, A, B);

_32bit_ADD g1(wAdd, A, B, 1'b0);

_32bit_SUB g2(wSub, A, B);

_32bit_XOR g3(wXor, A, B);

_32bit_NOR g4(wNor, A, B);

_32bit_OR g5(wOr, A, B);

_32bit_SLT g6(wSlt, A, B);

_32bit_comparator g7(equal, A, B);

_32bit_8x1MUX g8(_32bit_result, wAnd, wAdd, wSub, wXor, wNor, wOr, {31'd0,wSlt}, ,ALUOp);

endmodule

