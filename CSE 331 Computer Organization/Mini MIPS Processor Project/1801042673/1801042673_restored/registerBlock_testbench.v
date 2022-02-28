module registerBlock_testbench();
reg [2:0] readReg1, readReg2, writeReg;
reg [31:0] writeData;
reg clk, regWrite;
wire [31:0] readData1, readData2;

registerBlock r0 (readData1, readData2, readReg1, readReg2, writeReg, writeData, regWrite, clk);

initial begin
clk = 0;
$readmemb("registers.txt", r0.registers);
end

initial begin 
	forever begin
	#2; 
	clk = ~clk;
	end 
end

initial begin
readReg1 = 3'b000;
readReg2 = 3'b001;

regWrite = 1;
writeReg = 3'b000;
writeData = 32'd1;
#10;

writeReg = 3'b001;
writeData = 32'd2;
#10;

writeReg = 3'b010;
writeData = 32'd3;
#10;

writeReg = 3'b011;
writeData = 32'd4;
#10;

writeReg = 3'b100;
writeData = 32'd5;
#10;

writeReg = 3'b101;
writeData = 32'd6;
#10;

writeReg = 3'b110;
writeData = 32'd7;
#10;

writeReg = 3'b111;
writeData = 32'd8;
#10;

regWrite = 0;
readReg1 = 3'b010;
readReg2 = 3'b011;
#20;

readReg1 = 3'b100;
readReg2 = 3'b101;
#20;

readReg1 = 3'b110;
readReg2 = 3'b111;
#20;

$writememb("registers_output.txt", r0.registers);
$stop();
end


endmodule
