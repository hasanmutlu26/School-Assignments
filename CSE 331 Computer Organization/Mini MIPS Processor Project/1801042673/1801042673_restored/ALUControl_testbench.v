module ALUControl_testbench();
reg [3:0] Opcode;
reg [2:0] Func;
wire [2:0] ALUOp;

ALUControl g0(ALUOp, Opcode, Func);

initial begin
Opcode = 4'b0000;
Func = 3'b000;
#20;

Func = 3'b001;
#20;

Func = 3'b010;
#20;

Func = 3'b011;
#20;

Func = 3'b100;
#20;

Func = 3'b101;
#20;

Opcode = 4'b0001;
#20;

Opcode = 4'b0010;
#20;


Opcode = 4'b0011;
#20;


Opcode = 4'b0100;
#20;


Opcode = 4'b0101;
#20;


Opcode = 4'b0110;
#20;


Opcode = 4'b0111;
#20;


Opcode = 4'b1000;
#20;


Opcode = 4'b1001;
#20;

end

endmodule
