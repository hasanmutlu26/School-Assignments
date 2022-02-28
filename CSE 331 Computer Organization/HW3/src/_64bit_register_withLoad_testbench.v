module _64bit_register_withLoad_testbench();
reg [31:0] upperData, lowerData;
reg load, clk;
wire [63:0] value;

_64bit_register_withLoad g0(value, {upperData, lowerData}, load, clk);

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
load = 1'b0;
upperData = 32'd9999999;
lowerData = 32'd45451;
#20;

load = 1'b1;
#20;

upperData = 32'd111111111;
lowerData = 32'd111;

#20;

load = 1'b0;
upperData = 32'd654654656;
lowerData = 32'd15;

#20;

upperData = 32'd0;
lowerData = 32'd1;

#20;

load = 1'b1;
#20;
end

endmodule