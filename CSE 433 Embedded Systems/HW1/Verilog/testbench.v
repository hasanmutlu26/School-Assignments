module testbench();
reg CLOCK_50;

wire [3:0] KEY;
wire RESET_N;
wire [9:0] LEDR;
wire [9:0] SW;

reg [7:0] v;
reg b, r, w;
reg min1, min2, min3;




buf(KEY[0], ~min3);
buf(KEY[1], ~min2);
buf(KEY[2], ~min1);

buf(SW[0], v[0]);
buf(SW[1], v[1]);
buf(SW[2], v[2]);
buf(SW[3], v[3]);
buf(SW[4], v[4]);
buf(SW[5], v[5]);
buf(SW[6], v[6]);
buf(SW[7], v[7]);

buf(SW[8], r);
buf(SW[9], w);
buf(KEY[3], ~b);


DE0_CV de0(CLOCK_50, KEY, RESET_N, LEDR, SW);


initial begin 
	forever begin
	#2; 
	CLOCK_50 = ~CLOCK_50;
	end 
end

initial begin
CLOCK_50 = 0;
v = 8'd21;
b = 0;
r = 0;
w = 1;
min1 = 0;
min2 = 0;
min3 = 0;
#10;
min1 = 1;
#2;
min1 = 0;

#18;
v = 8'd19;

#20;
v = 8'd21;

#30;
min3 = 1;
#3;
min3 = 0;

#17;
min2 = 1;
#3;
min2 = 0;

#17;
min2 = 1;
#3;
min2 = 0;

#27;
min3 = 1;
#3;
min3 = 0;

#20;
v = 20'd19;

#20;








end

endmodule
