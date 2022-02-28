module _32bit_xor_testbench();
reg [31:0] A, B;
wire [31:0] result;

_32bit_xor g0(result, A, B);

initial begin
A = 32'd5000000;
B = 32'd5000000;
#20;

A = 32'd5000001;
B = 32'd3000000;
#20;

A = 32'd5000000;
B = 32'd0;
#20;

A = 32'b1111_1111_1111_1111_1111_1111_1111_1111;
B = 32'd5000000;
#20;
end

endmodule


