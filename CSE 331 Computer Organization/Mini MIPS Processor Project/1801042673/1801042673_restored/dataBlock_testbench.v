module dataBlock_testbench();
reg [31:0] writeData, address;
reg memWrite, clk;
wire [31:0] readData;

dataBlock d0(readData, writeData, address, memWrite, clk);


initial begin
clk = 0;
$readmemb("memory.txt", d0.memory);
end

initial begin 
	forever begin
	#2; 
	clk = ~clk;
	end 
end

initial begin
writeData = 32'd5;
memWrite = 0;
address = 32'b0000_0000_0000_0000_0000_0000_0000_0000;
#20;

address = 32'b0000_0000_0000_0000_0000_0000_0000_1000;
#20;

address = 32'b0000_0000_0000_0000_0000_0000_1000_0000;
#20;

address = 32'b0000_0000_0000_0000_0000_0000_0000_0010;
memWrite = 1;
#20;

writeData = 32'd3;
address = 32'b0000_0000_0000_0000_0000_0000_0000_0001;
memWrite = 1;
#20;

$writememb("memory_output.txt", d0.memory);
$stop();
end

endmodule

