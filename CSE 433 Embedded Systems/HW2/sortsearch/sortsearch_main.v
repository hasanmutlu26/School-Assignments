module sortsearch_main(
input CLK,
input reset,
input go,
input [31:0] target,
input [31:0] size,
input [31:0] arr0,
input [31:0] arr1,
input [31:0] arr2,
input [31:0] arr3,
input [31:0] arr4,
input [31:0] arr5,
input [31:0] arr6,
input [31:0] arr7,
input [31:0] arr8,
input [31:0] arr9,
input [31:0] arr10,
input [31:0] arr11,
input [31:0] arr12,
input [31:0] arr13,
input [31:0] arr14,
input [31:0] arr15,
input [31:0] arr16,
input [31:0] arr17,
input [31:0] arr18,
input [31:0] arr19,
input [31:0] arr20,
input [31:0] arr21,
input [31:0] arr22,
input [31:0] arr23,
input [31:0] arr24,
input [31:0] arr25,
input [31:0] arr26,
input [31:0] arr27,
input [31:0] arr28,
input [31:0] arr29,
input [31:0] arr30,
input [31:0] arr31,
output fin,
output found
);

wire i_lt_sm1;
wire j_lt_smi1;
wire arrj_lt_arrjp1;
wire left_lt_right;
wire arrmid_gt_target;
wire  arrmid_eq_target;
wire inc_i;
wire init_j;
wire inc_j;
wire swap;
wire init_left;
wire init_right;
wire setmid;
wire lefthalf;
wire righthalf;

sortsearch_control cont(
	.CLK(CLK),
	.reset(reset),
	.i_lt_sm1(i_lt_sm1),
	.j_lt_smi1(j_lt_smi1),
	.arrj_lt_arrjp1(arrj_lt_arrjp1),
	.left_lt_right(left_lt_right),
	.arrmid_gt_target(arrmid_gt_target),
	.arrmid_eq_target(arrmid_eq_target),
	.go(go),
	.inc_i(inc_i),
	.init_j(init_j),
	.inc_j(inc_j),
	.swap(swap),
	.init_left(init_left),
	.init_right(init_right),
	.setmid(setmid),
	.lefthalf(lefthalf), 
	.righthalf(righthalf),
	.found(found),
	.fin(fin)
);



sortsearch_datapath  dat(
	.CLK(CLK),
	.reset(reset),
	.inc_i(inc_i),
	.init_j(init_j),
	.inc_j(inc_j),
	.swap(swap),
	.init_left(init_left),
	.init_right(init_right),
	.setmid(setmid),
	.lefthalf(lefthalf), 
	.righthalf(righthalf),
	.size(size),
	.target(target),
	.array({arr31, arr30, arr29, arr28, arr27, arr26, arr25, arr24, arr23, arr22, arr21, arr20, arr19, arr18, arr17, arr16, arr15, arr14, arr13, arr12, arr11, arr10, arr9, arr8, arr7, arr6, arr5, arr4, arr3, arr2, arr1, arr0}),
	.i_lt_sm1(i_lt_sm1),
	.j_lt_smi1(j_lt_smi1),
	.arrj_lt_arrjp1(arrj_lt_arrjp1),
	.left_lt_right(left_lt_right),
	.arrmid_gt_target(arrmid_gt_target),
	.arrmid_eq_target(arrmid_eq_target)
);






endmodule
