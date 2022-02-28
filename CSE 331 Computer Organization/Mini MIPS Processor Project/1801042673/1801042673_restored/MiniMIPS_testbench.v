module MiniMIPS_testbench();
reg clk;
reg [15:0] instruction [29:0];
wire [31:0] PCounter;

MiniMIPS minimips (instruction[PCounter], PCounter, clk);

initial begin
minimips.PCounter = 32'd0;
clk = 0;
$readmemb("registers.txt", minimips.regist.registers);
$readmemb("memory.txt", minimips.memory.memory);
$readmemb("instruction.txt", instruction);

end

initial begin 
	forever begin
	#2; 
	clk = ~clk;
	end 
end

initial begin
#500;
$writememb("registers_output.txt", minimips.regist.registers);
$writememh("registers_output_hex.txt", minimips.regist.registers);
$writememb("memory_output.txt", minimips.memory.memory);
$writememb("instruction_output.txt", instruction);
$stop();
end


endmodule
