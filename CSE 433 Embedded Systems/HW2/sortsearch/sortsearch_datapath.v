module sortsearch_datapath(
	input CLK,
	input reset,
	input inc_i,
	input init_j,
	input inc_j,
	input swap,
	input init_left,
	input init_right,
	input setmid,
	input lefthalf, 
	input righthalf,
	input [31:0] size,
	input [31:0] target,
	input [1023:0] array,
	output i_lt_sm1,
	output j_lt_smi1,
	output arrj_lt_arrjp1,
	output left_lt_right,
	output arrmid_gt_target,
	output arrmid_eq_target
);




// Declare registers
reg [31:0] i;
reg [31:0] j;
reg [31:0] left;
reg [31:0] right;
reg [31:0] mid;
reg [31:0] arr [31:0];


// Declare wires.
wire [31:0] w_i;
wire [31:0] w_j;
wire [31:0] w_left;
wire [31:0] w_right;
wire [31:0] w_mid;

wire [31:0] w_arr0;
wire [31:0] w_arr1;
wire [31:0] w_arr2;
wire [31:0] w_arr3;
wire [31:0] w_arr4;
wire [31:0] w_arr5;
wire [31:0] w_arr6;
wire [31:0] w_arr7;
wire [31:0] w_arr8;
wire [31:0] w_arr9;
wire [31:0] w_arr10;
wire [31:0] w_arr11;
wire [31:0] w_arr12;
wire [31:0] w_arr13;
wire [31:0] w_arr14;
wire [31:0] w_arr15;
wire [31:0] w_arr16;
wire [31:0] w_arr17;
wire [31:0] w_arr18;
wire [31:0] w_arr19;
wire [31:0] w_arr20;
wire [31:0] w_arr21;
wire [31:0] w_arr22;
wire [31:0] w_arr23;
wire [31:0] w_arr24;
wire [31:0] w_arr25;
wire [31:0] w_arr26;
wire [31:0] w_arr27;
wire [31:0] w_arr28;
wire [31:0] w_arr29;
wire [31:0] w_arr30;
wire [31:0] w_arr31;



// Define registers

initial begin
i <= 32'd0;
end

always @(posedge CLK) begin
	if(reset) 
		begin
		i <= 32'd0;
		j <= 32'd0;
		left <= 32'd0;
		right <= 32'd0;
		mid <= 32'd0;
		
		
		arr[0] <= array[31:0];
		arr[1] <= array[63:32];
		arr[2] <= array[95:64];
		arr[3] <= array[127:96];
		arr[4] <= array[159:128];
		arr[5] <= array[191:160];
		arr[6] <= array[223:192];
		arr[7] <= array[255:224];
		arr[8] <= array[287:256];
		arr[9] <= array[319:288];
		arr[10] <= array[351:320];
		arr[11] <= array[383:352];
		arr[12] <= array[415:384];
		arr[13] <= array[447:416];
		arr[14] <= array[479:448];
		arr[15] <= array[511:480];
		arr[16] <= array[543:512];
		arr[17] <= array[575:544];
		arr[18] <= array[607:576];
		arr[19] <= array[639:608];
		arr[20] <= array[671:640];
		arr[21] <= array[703:672];
		arr[22] <= array[735:704];
		arr[23] <= array[767:736];
		arr[24] <= array[799:768];
		arr[25] <= array[831:800];
		arr[26] <= array[863:832];
		arr[27] <= array[895:864];
		arr[28] <= array[927:896];
		arr[29] <= array[959:928];
		arr[30] <= array[991:960];
		arr[31] <= array[1023:992];
			
		
		end
	else
		begin
			i <= w_i;
			j <= w_j;
			left <= w_left;
			right <= w_right;
			mid <= w_mid;
			
			arr[0] <= w_arr0;
			arr[1] <= w_arr1;
			arr[2] <= w_arr2;
			arr[3] <= w_arr3;
			arr[4] <= w_arr4;
			arr[5] <= w_arr5;
			arr[6] <= w_arr6;
			arr[7] <= w_arr7;
			arr[8] <= w_arr8;
			arr[9] <= w_arr9;
			arr[10] <= w_arr10;
			arr[11] <= w_arr11;
			arr[12] <= w_arr12;
			arr[13] <= w_arr13;
			arr[14] <= w_arr14;
			arr[15] <= w_arr15;
			arr[16] <= w_arr16;
			arr[17] <= w_arr17;
			arr[18] <= w_arr18;
			arr[19] <= w_arr19;
			arr[20] <= w_arr20;
			arr[21] <= w_arr21;
			arr[22] <= w_arr22;
			arr[23] <= w_arr23;
			arr[24] <= w_arr24;
			arr[25] <= w_arr25;
			arr[26] <= w_arr26;
			arr[27] <= w_arr27;
			arr[28] <= w_arr28;
			arr[29] <= w_arr29;
			arr[30] <= w_arr30;
			arr[31] <= w_arr31;
		end
end


assign w_i = (inc_i) ? i + 32'd1 : i;
assign w_j = (init_j) ? 0 : (inc_j) ? j + 1 : j;
assign w_left = (init_left) ? 0 : (righthalf) ? mid + 1 : left;
assign w_right = (init_right) ? size - 1 : (lefthalf) ? mid - 1 : right;
assign w_mid = (setmid) ? left + (right - left) / 2 : mid;

assign i_lt_sm1 = (i < size - 1);
assign j_lt_smi1 = (j < size - 1 - i);
assign arrj_lt_arrjp1 = (arr[j] > arr[j+1]);
assign left_lt_right = (left <= right) && !right[31];
assign arrmid_gt_target = (arr[mid] > target);
assign arrmid_eq_target = (arr[mid] == target);

assign w_arr0 = (swap && j == 0) ? arr[1] : arr[0];
assign w_arr1 = (swap && j == 0) ? arr[0] : (swap && j == 1) ? arr[2] : arr[1];
assign w_arr2 = (swap && j == 1) ? arr[1] : (swap && j == 2) ? arr[3] : arr[2];
assign w_arr3 = (swap && j == 2) ? arr[2] : (swap && j == 3) ? arr[4] : arr[3];
assign w_arr4 = (swap && j == 3) ? arr[3] : (swap && j == 4) ? arr[5] : arr[4];
assign w_arr5 = (swap && j == 4) ? arr[4] : (swap && j == 5) ? arr[6] : arr[5];
assign w_arr6 = (swap && j == 5) ? arr[5] : (swap && j == 6) ? arr[7] : arr[6];
assign w_arr7 = (swap && j == 6) ? arr[6] : (swap && j == 7) ? arr[8] : arr[7];
assign w_arr8 = (swap && j == 7) ? arr[7] : (swap && j == 8) ? arr[9] : arr[8];
assign w_arr9 = (swap && j == 8) ? arr[8] : (swap && j == 9) ? arr[10] : arr[9];
assign w_arr10 = (swap && j == 9) ? arr[9] : (swap && j == 10) ? arr[11] : arr[10];
assign w_arr11 = (swap && j == 10) ? arr[10] : (swap && j == 11) ? arr[12] : arr[11];
assign w_arr12 = (swap && j == 11) ? arr[11] : (swap && j == 12) ? arr[13] : arr[12];
assign w_arr13 = (swap && j == 12) ? arr[12] : (swap && j == 13) ? arr[14] : arr[13];
assign w_arr14 = (swap && j == 13) ? arr[13] : (swap && j == 14) ? arr[15] : arr[14];
assign w_arr15 = (swap && j == 14) ? arr[14] : (swap && j == 15) ? arr[16] : arr[15];
assign w_arr16 = (swap && j == 15) ? arr[15] : (swap && j == 16) ? arr[17] : arr[16];
assign w_arr17 = (swap && j == 16) ? arr[16] : (swap && j == 17) ? arr[18] : arr[17];
assign w_arr18 = (swap && j == 17) ? arr[17] : (swap && j == 18) ? arr[19] : arr[18];
assign w_arr19 = (swap && j == 18) ? arr[18] : (swap && j == 19) ? arr[20] : arr[19];
assign w_arr20 = (swap && j == 19) ? arr[19] : (swap && j == 20) ? arr[21] : arr[20];
assign w_arr21 = (swap && j == 20) ? arr[20] : (swap && j == 21) ? arr[22] : arr[21];
assign w_arr22 = (swap && j == 21) ? arr[21] : (swap && j == 22) ? arr[23] : arr[22];
assign w_arr23 = (swap && j == 22) ? arr[22] : (swap && j == 23) ? arr[24] : arr[23];
assign w_arr24 = (swap && j == 23) ? arr[23] : (swap && j == 24) ? arr[25] : arr[24];
assign w_arr25 = (swap && j == 24) ? arr[24] : (swap && j == 25) ? arr[26] : arr[25];
assign w_arr26 = (swap && j == 25) ? arr[25] : (swap && j == 26) ? arr[27] : arr[26];
assign w_arr27 = (swap && j == 26) ? arr[26] : (swap && j == 27) ? arr[28] : arr[27];
assign w_arr28 = (swap && j == 27) ? arr[27] : (swap && j == 28) ? arr[29] : arr[28];
assign w_arr29 = (swap && j == 28) ? arr[28] : (swap && j == 29) ? arr[30] : arr[29];
assign w_arr30 = (swap && j == 29) ? arr[29] : (swap && j == 30) ? arr[31] : arr[30];
assign w_arr31 = (swap && j == 30) ? arr[30] : arr[31];

/*
always @(posedge CLK) begin
	if(swap) begin
		arr[j] = arr[j+1];
		arr[j+1] = arr[j];
	end

end
*/

endmodule
