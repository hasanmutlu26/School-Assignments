module _64bit_shiftRight_testbench();
reg [63:0] A;
reg control;
wire [63:0] result;

_64bit_shiftRight g0 (result, A, control);

initial begin
A <= 64'b1111_0000_1111_0000_1111_0000_1111_0000_1111_0000_1111_0000_1111_0000_1111_0000_;
control = 1'b1;
#20;
end

endmodule