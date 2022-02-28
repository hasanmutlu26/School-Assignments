module ALUControl(ALUOp, Opcode, Func);
input [3:0] Opcode;
input [2:0] Func;
output[2:0] ALUOp;

wire wZeroOpCode, wAnd1, wAnd2, wAnd3, wAnd4, wAnd5, wAnd6, wAnd7, wAnd8;

and (wZeroOpCode, ~Opcode[3], ~Opcode[2], ~Opcode[1], ~Opcode[0]);


and (wAnd1, wZeroOpCode, Func[2], ~Func[1]);
and (wAnd2, ~Opcode[3], Opcode[2], ~Opcode[1], ~Opcode[0]);
and (wAnd3, ~Opcode[3], Opcode[1], Opcode[0]);
or(ALUOp[2], wAnd1, wAnd2, wAnd3);


and (wAnd4, wZeroOpCode, ~Func[2], Func[1]);
and (wAnd5, ~Opcode[3], Opcode[2], Opcode[1], Opcode[0]);
or (ALUOp[1], wAnd4, wAnd5);

and (wAnd6, Func[0], wZeroOpCode);
and (wAnd7, ~Opcode[3], ~Opcode[2], Opcode[0]);
and (wAnd8, Opcode[3], ~Opcode[2], ~Opcode[1]);
or(ALUOp[0], wAnd6, wAnd7, wAnd8);

endmodule
