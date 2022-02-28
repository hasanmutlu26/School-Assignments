module controller(RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, BranchSrc, OPCode);
input [3:0] OPCode;
output RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, BranchSrc;

wire wAnd1, wAnd2, wAnd3, wAnd4;
wire wRType, wITypeArith, wBeq, wBne, wLw, wSw;

and (wRType, ~OPCode[3], ~OPCode[2], ~OPCode[1], ~OPCode[0]);

and (wAnd1, ~OPCode[3], OPCode[2], ~OPCode[1], ~OPCode[0]);
and (wAnd2, ~OPCode[3], ~OPCode[2], OPCode[0]);
and (wAnd3, ~OPCode[3], ~OPCode[2], OPCode[1]);
and (wAnd4, OPCode[2], OPCode[1], OPCode[0]);
or (wITypeArith, wAnd1, wAnd2, wAnd3, wAnd4);

and (wBeq, ~OPCode[3], OPCode[2], ~OPCode[1], OPCode[0]);

and (wBne, ~OPCode[3], OPCode[2], OPCode[1], ~OPCode[0]);

and (wLw, OPCode[3], ~OPCode[2], ~OPCode[1], ~OPCode[0]);

and (wSw, OPCode[3], ~OPCode[2], ~OPCode[1], OPCode[0]);



not (RegDst, ~wRType);

or (ALUSrc, wITypeArith, wLw, wSw);

not (MemtoReg, ~wLw);

or (RegWrite, wRType, wITypeArith, wLw);

not (MemRead, ~wLw);

not (MemWrite, ~wSw);

or (Branch, wBeq, wBne);

not (BranchSrc, ~wBne);

endmodule
