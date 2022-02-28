module DFlipFlop_testbench();
reg D, clk;
wire Q, Qn;

DFlipFlop g0 (Q, Qn, D, clk);

initial begin
clk = 0;
end

initial begin 
	forever begin
	#5; 
	clk = ~clk;
	end 
end

initial begin
D = 1'b1;
#100;

D = 1'b0;
#100;

D = 1'b1;
#2;

D = 1'b1;
#3;
end


endmodule