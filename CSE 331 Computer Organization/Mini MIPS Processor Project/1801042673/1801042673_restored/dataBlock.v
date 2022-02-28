module dataBlock(readData, writeData, address, memWrite, clk);
input [31:0] writeData, address;
input memWrite, clk;
reg [31:0] memory [255:0];
output [31:0] readData;

always @(posedge clk)
begin
if(memWrite)
	memory[address] <= writeData;
end

//256x1 MUX 
assign readData = memory[address];

endmodule
