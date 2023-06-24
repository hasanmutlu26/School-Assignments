module sortsearch_control(
	input CLK,
	input reset,
	input i_lt_sm1,
	input j_lt_smi1,
	input arrj_lt_arrjp1,
	input left_lt_right,
	input arrmid_gt_target,
	input arrmid_eq_target,
	input go,
	output reg inc_i,
	output reg init_j,
	output reg inc_j,
	output reg swap,
	output reg init_left,
	output reg init_right,
	output reg setmid,
	output reg lefthalf, 
	output reg righthalf,
	output reg found,
	output reg fin
);

reg [3:0] state;
reg [3:0] next_state;

localparam 	IDLE 			= 4'b0000,
				START 		= 4'b0001,
				FOR1_CONTR 	= 4'b0010,
				FOR2_CONTR 	= 4'b0011,
				SWAP 			= 4'b0100,
				INCR_J 		= 4'b0101,
				INCR_I 		= 4'b0110,
				SEARCH_INIT = 4'b0111,
				WHILE 		= 4'b1000,
				CHECKMID 	= 4'b1001,
				LEFTHALF		= 4'b1010,
				RIGHTHALF	= 4'b1011,
				FOUND			= 4'b1100,
				NOTFOUND		= 4'b1101;
				


always @(posedge CLK) 
begin
	if(reset)
		state <= IDLE;
	else
		state <= next_state;	
end



always @(*) begin
	
	case(state)
		IDLE:
			begin
			if(go)
				next_state = START;
			else
				next_state = IDLE;
			end
		
		START:
			begin
			next_state = FOR1_CONTR;
			end
		
		FOR1_CONTR:
			begin
			if(i_lt_sm1)
				next_state = FOR2_CONTR;
			else
				next_state = SEARCH_INIT;
			end
		
		FOR2_CONTR:
			begin
			if(!j_lt_smi1)
				next_state = INCR_I;
			else if(arrj_lt_arrjp1)
				next_state = SWAP;
			else
				next_state = INCR_J;
			end
			
		SWAP:
			begin
			next_state = INCR_J;
			end
			
		INCR_J:
			begin
			next_state = FOR2_CONTR;
			end
			
		INCR_I:
			begin
			next_state = FOR1_CONTR;
			end
		
		
		SEARCH_INIT:
			begin
			next_state = WHILE;
			end
			
		WHILE:
			begin
			if(arrmid_eq_target)
				next_state = FOUND;
			else if(left_lt_right)
				next_state = CHECKMID;
			else 
				next_state = NOTFOUND;
			end
			
		CHECKMID:
			begin
			if(arrmid_gt_target)
				next_state = LEFTHALF;
			else
				next_state = RIGHTHALF;
			end	
		
		LEFTHALF:
			begin
			next_state = WHILE;
			end
			
		RIGHTHALF:
			begin
			next_state = WHILE;
			end
			
		FOUND:
			begin
			next_state = FOUND;
			end
			
		NOTFOUND:
			begin
			next_state = NOTFOUND;
			end
			
				
	endcase

end


always @(*)
begin
	
	inc_i = 1'b0;
	init_j = 1'b0; 
	inc_j = 1'b0;
	swap = 1'b0;
	init_left = 1'b0;
	init_right = 1'b0;
	setmid = 1'b0;
	lefthalf = 1'b0;
	righthalf = 1'b0;
	fin = 1'b0;
	found = 1'b0;
	
	case(state)
	
		IDLE:
			begin
			end
		
		START:
			begin
			end
		
		FOR1_CONTR:
			begin
			init_j = 1'b1;
			end
		
		FOR2_CONTR:
			begin
			end
			
		SWAP:
			begin
			swap = 1'b1;
			end
			
		INCR_J:
			begin
			inc_j = 1'b1;
			end
			
		INCR_I:
			begin
			inc_i = 1'b1;
			end
		
		SEARCH_INIT:
			begin
			init_left = 1'b1;
			init_right = 1'b1;
			end
		
		WHILE:
			begin
			setmid = 1'b1;
			end
			
		CHECKMID:
			begin
			end
			
		LEFTHALF:
			begin
			lefthalf = 1'b1;
			end
		
		RIGHTHALF:
			begin
			righthalf = 1'b1;
			end
			
		FOUND:
			begin
			found = 1'b1;
			fin = 1'b1;
			end
			
		NOTFOUND:
			begin
			found = 1'b0;
			fin = 1'b1;
			end
		
			
	endcase
	
end


endmodule