module _64bit_register_testbench();
reg [63:0] data;
reg clk;
wire [63:0] value;

_64bit_register g0 (value, data, clk);

initial begin
clk = 0;
end

initial begin 
	forever begin
	#2; 
	clk = ~clk;
	end 
end

initial begin
data = 64'd9999999;
#20;

data = 64'd111111111;
#20;

data = 64'd454654654656;
#20;

data = 64'd0;
#20;
end

endmodule