module signExt6to32_testbench();
reg [5:0] A;
wire [31:0] result;

signExt6to32 g0(result, A);

initial begin

A = 26'b01_1001;
#20;

A = 26'b11_1110;
#20;

A = 26'b00_0000;
#20;

A = 26'b10_0001;
#20;

A = 26'b00_0000;
#20;

A = 26'b10_0010;
#20;

end

endmodule

