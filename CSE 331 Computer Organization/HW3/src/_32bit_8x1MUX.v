module _32bit_8x1MUX (result, A,B,C,D,E,F,G,H, control);
input [31:0] A,B,C,D,E,F,G,H;
input [2:0] control;
output [31:0] result;
wire [31:0] w1, w2;

_32bit_4x1MUX g0 (w1, A,B,C,D,control[1:0]);
_32bit_4x1MUX g1 (w2, E,F,G,H,control[1:0]);

_32bit_2x1MUX g2 (result, w1, w2, control[2]);

endmodule