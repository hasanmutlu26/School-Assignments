module _64bit_register_withload(value, upperData, lowerData, load, clk);
input [31:0] upperData, lowerData;
input load;
input clk;
output [63:0] value;
wire [31:0] w1;

