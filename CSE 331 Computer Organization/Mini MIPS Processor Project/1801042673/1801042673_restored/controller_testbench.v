module controller_testbench();
reg [3:0] OPCode;
wire RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, BranchSrc;

controller c0 (RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, BranchSrc, OPCode);

initial begin
OPCode = 4'b0000;
#20;

OPCode = 4'b0001;
#20;

OPCode = 4'b0010;
#20;

OPCode = 4'b0011;
#20;


OPCode = 4'b0100;
#20;


OPCode = 4'b0101;
#20;


OPCode = 4'b0110;
#20;


OPCode = 4'b0111;
#20;


OPCode = 4'b1000;
#20;


OPCode = 4'b1001;
#20;
$stop();
end

endmodule
