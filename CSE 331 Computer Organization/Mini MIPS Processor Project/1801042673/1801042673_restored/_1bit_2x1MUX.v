module _1bit_2x1MUX(result, A, B, control);
input A, B, control;
output result;

wire and1, and2;

and (and1, A, ~control);

and (and2, B, control);

or (result, and1, and2);

endmodule