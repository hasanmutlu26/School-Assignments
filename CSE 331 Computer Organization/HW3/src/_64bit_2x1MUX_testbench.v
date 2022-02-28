module _64bit_2x1MUX_testbench();
reg [63:0] A, B;
reg control;
wire [63:0] result;

_64bit_2x1MUX g0(result, A, B, control);

initial begin
A = 64'd99999999;
B = 64'd888888888;
control = 1'b0;
#20;

control = 1'b1;
#20;

control = 1'b0;
#20;

control = 1'b1;
#20;
end

endmodule