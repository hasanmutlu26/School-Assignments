module _32bit_comparator_testbench();
reg [31:0] A, B;
wire result;

_32bit_comparator g0(result, A, B);

initial begin
A = 32'd5621;
B = 32'd5621;
#20;

A = 32'd9684;
B = 32'd9683;
#20;

A = 32'd3333;
B = 32'd4444;
#20;

A = 32'd0;
B = 32'd0;
#20;

A = 32'd15;
B = 32'd0;
#20;

end

endmodule
