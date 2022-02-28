module _32bit_2scomplement_testbench();
reg [31:0] A;
wire [31:0] result;

_32bit_2scomplement g0 (result, A);

initial begin
A = 32'd500000;
#20;

A = 32'd0;
#20;

A = 32'b1111_1111_1111_1111_1111_1111_1111_1111;
#20;

A = 32'd15;
#20;

A = 32'd1;
#20;

A = 32'd6548;
#20;
end

endmodule


