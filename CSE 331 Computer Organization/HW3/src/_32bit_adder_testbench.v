module _32bit_adder_testbench();
reg [31:0] A, B;
reg C0;
wire [31:0] S;
wire C;

_32bit_adder g0 (S,C,A,B,C0);

initial begin
A = 32'd250;
B = 32'd500;
C0 = 0;
#20;

A = 32'd250;
B = 32'd500;
C0 = 1;
#20;

A = 32'd50000;
B = 32'd22222;
C0 = 0;
#20;
end

endmodule
