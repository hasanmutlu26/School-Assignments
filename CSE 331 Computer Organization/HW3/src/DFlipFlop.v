module DFlipFlop (Q, Qn, D, clk);
input D, clk;
output Q, Qn;
wire w1, w2, w3;

not n1(w1,D);
nand n2(w2, D, clk);
nand n3(w3, w1, clk);
nand n4 (Q, w2, Qn);
nand n5 (Qn, w3, Q);

endmodule