module _32bit_subtractor_testbench();
reg [31:0] A, B;
wire [31:0] result;

_32bit_subtractor g0 (result, A, B);

initial begin
A = 32'd50;
B = 32'd25;
#20;

A = 32'd0;
B = 32'd100;
#20;

A = 32'd500;
B = 32'd1000;
#20;

A = 32'd800;
B = 32'd800;
#20;
end

endmodule