module registerBlock(readData1, readData2, readReg1, readReg2, writeDst, writeData, regWrite, clk);
input [2:0] readReg1, readReg2, writeDst;
input [31:0] writeData;
input clk, regWrite;
output [31:0] readData1, readData2;
reg [31:0] registers [7:0];

always @ (posedge clk)
begin
if(regWrite && writeDst != 0)
	registers[writeDst] <= writeData;
end

_32bit_8x1MUX m0 (readData1, registers[0], registers[1], registers[2], registers[3], registers[4], registers[5], registers[6], registers[7], readReg1);

_32bit_8x1MUX m1 (readData2, registers[0], registers[1], registers[2], registers[3], registers[4], registers[5], registers[6], registers[7], readReg2);

endmodule
