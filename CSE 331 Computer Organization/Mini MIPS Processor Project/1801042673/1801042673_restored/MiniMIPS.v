module MiniMIPS(instruction, PCounter, clk);
input clk;
input [15:0] instruction;
output reg [31:0] PCounter;

wire RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, BranchSrc;

wire [31:0] readRegData1, readRegData2, writeDataMUX, signExtImm, ALUSrc2MUX, readMemData, wAdder1, wAdder2, wPC, ALUResult;
wire [15:0] instruction;
wire [2:0] ALUOp, writeDstMUX;
wire equal, wBranch, BranchSrcMUX;

always @ (posedge clk)
begin
	PCounter <= wPC;
end

//instructionBlock i0 (instruction, PCounter, 16'd0, 1'b0,clk);

controller control(RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, BranchSrc, instruction[15:12]);

registerBlock regist(readRegData1, readRegData2, instruction[11:9], instruction[8:6], writeDstMUX, writeDataMUX, RegWrite, clk);

_3bit_2x1MUX writeDestMux(writeDstMUX, instruction[8:6], instruction[5:3], RegDst);

signExt6to32 signEx(signExtImm, instruction[5:0]);

ALUControl aluctr(ALUOp, instruction[15:12], instruction[2:0]);

ALU alu(ALUResult, equal, readRegData1, ALUSrc2MUX, ALUOp);

_32bit_2x1MUX alusrcmux(ALUSrc2MUX, readRegData2, signExtImm, ALUSrc);

dataBlock memory(readMemData, readRegData2, ALUResult, MemWrite, clk);

_32bit_2x1MUX writedata (writeDataMUX, ALUResult, readMemData, MemtoReg);

_1bit_2x1MUX branchsrc (BranchSrcMUX, equal, ~equal, BranchSrc);

and branchyes(wBranch, Branch, BranchSrcMUX);

_32bit_2x1MUX pcmux (wPC, wAdder1, wAdder2, wBranch);

_32bit_ADD addermux1 (wAdder1, PCounter, 32'd0, 1'b1);

_32bit_ADD addermux2 (wAdder2, wAdder1, signExtImm, 1'b0);

endmodule

