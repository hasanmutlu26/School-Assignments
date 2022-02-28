module _32bit4x1MUX_testbench();
reg [31:0] A, B, C, D;
reg [1:0] control;
wire [31:0] result;

_32bit_4x1MUX x (result, A,B,C,D,control);

initial begin
A = 32'd100;
B = 32'd200;
C = 32'd300;
D = 32'd400;

control = 2'b00;
#20;
control = 2'b01;
#20;
control = 2'b10;
#20;
control = 2'b11;
#20;
end

endmodule