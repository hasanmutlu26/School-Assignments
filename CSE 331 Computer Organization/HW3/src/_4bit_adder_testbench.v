module _4bit_adder_testbench();
reg [3:0] A, B;
reg C0;
wire C;
wire [3:0] S;

_4bit_adder g0 (S,C,A,B,C0);

initial begin
A = 4'b0101;
B = 4'b1010;
C0 = 1'b0;
#20;

A = 4'b0000;
B = 4'b1100;
C0 = 1'b0;
#20;
end

endmodule