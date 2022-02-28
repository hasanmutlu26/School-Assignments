module _64bit_register_withLoad(value, data, load, clk);
input [63:0] data;
input load, clk;
output [63:0] value;
wire [63:0] wMux;

_64bit_2x1MUX g0(wMux, value, data, load);
_64bit_register g1 (value, wMux, clk);

endmodule