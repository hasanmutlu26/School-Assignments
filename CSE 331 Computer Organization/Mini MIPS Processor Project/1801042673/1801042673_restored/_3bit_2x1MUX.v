module _3bit_2x1MUX(result, A, B, control);
input [2:0] A, B;
input control;
output [2:0] result;

wire [2:0] and1, and2;

and (and1[0], A[0], ~control);
and (and1[1], A[1], ~control);
and (and1[2], A[2], ~control);

and (and2[0], B[0], control);
and (and2[1], B[1], control);
and (and2[2], B[2], control);

or (result[0], and1[0], and2[0]);
or (result[1], and1[1], and2[1]);
or (result[2], and1[2], and2[2]);

endmodule
