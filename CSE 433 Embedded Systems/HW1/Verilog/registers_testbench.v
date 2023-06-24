module registers_testbench();
reg [2:0] N;
reg clk;
wire [2:0] R;

registers r(R, N, clk);


initial begin 
	forever begin
	#2; 
	clk = ~clk;
	end 
end

initial begin
clk = 0;
N = 3'b000;
#5;

N = 3'b001;
#5;

N = 3'b010;
#5;

N = 3'b011;
#5;

N = 3'b100;
#5;

N = 3'b101;
#5;

N = 3'b110;
#5;

N = 3'b111;
#5;

N = 3'b000;
#5;


end

endmodule