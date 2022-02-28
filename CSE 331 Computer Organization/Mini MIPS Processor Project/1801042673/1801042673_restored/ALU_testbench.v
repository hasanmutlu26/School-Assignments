module ALU_testbench();
reg [31:0] A, B;
reg [2:0] ALUOp;
wire [31:0] result;
wire equal;

ALU g0(result, equal, A, B, ALUOp);

initial begin

A = 32'd9999;
B = 32'd8111;

ALUOp = 3'b000;
#20;

ALUOp = 3'b001;
#20;

ALUOp = 3'b010;
#20;

ALUOp = 3'b011;
#20;

ALUOp = 3'b100;
#20;

ALUOp = 3'b101;
#20;

ALUOp = 3'b110;
#20;

A = 32'd8111;
B = 32'd8111;
#20;

A = 32'd8111;
B = 32'd8311;
#20;

end

endmodule
