module danger_testbench();
reg [7:0] v;
reg b, r, w;
wire dang;

danger d(dang, v, b, r, w);


initial begin
v = 8'b0;
b = 0;
r = 0;
w = 1;
#20;

v = 8'd20;
#20;

v = 8'd21;
#20;

b = 1;
#10;

b = 0;
r = 1;
#10;

end

endmodule
