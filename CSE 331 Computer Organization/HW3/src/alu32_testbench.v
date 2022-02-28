module alu32_testbench();
reg [31:0] A, B;
reg [2:0] ALUop;
wire [31:0] result;

alu32 g0 (result, A, B, ALUop);

initial begin
A = 32'd999999;
B = 32'd1111111;

ALUop = 3'b000;
#20;

ALUop = 3'b001;
#20;

ALUop = 3'b010;
#20;

ALUop = 3'b011;
#20;

ALUop = 3'b100;
#20;

ALUop = 3'b101;
#20;

ALUop = 3'b110;
#20;

ALUop = 3'b111;
#20;

end

endmodule