module registers(R, N, clk);
input [2:0] N;
input clk;
output reg [2:0] R;


initial begin
R <= 3'b000;
end

 always @(posedge clk) begin
	 R <= N;
end

endmodule