module sortsearch_testbench();
reg CLK;
reg reset;
reg go;
reg [31:0] target;
reg [31:0] size;
reg [31:0] arr0;
reg [31:0] arr1;
reg [31:0] arr2;
reg [31:0] arr3;
reg [31:0] arr4;
reg [31:0] arr5;
reg [31:0] arr6;
reg [31:0] arr7;
reg [31:0] arr8;
reg [31:0] arr9;
reg [31:0] arr10;
reg [31:0] arr11;
reg [31:0] arr12;
reg [31:0] arr13;
reg [31:0] arr14;
reg [31:0] arr15;
reg [31:0] arr16;
reg [31:0] arr17;
reg [31:0] arr18;
reg [31:0] arr19;
reg [31:0] arr20;
reg [31:0] arr21;
reg [31:0] arr22;
reg [31:0] arr23;
reg [31:0] arr24;
reg [31:0] arr25;
reg [31:0] arr26;
reg [31:0] arr27;
reg [31:0] arr28;
reg [31:0] arr29;
reg [31:0] arr30;
reg [31:0] arr31;
wire found;
wire fin;


sortsearch_main s(
.CLK(CLK),
.reset(reset),
.go(go),
.target(target),
.size(size),
.arr0(arr0),
.arr1(arr1),
.arr2(arr2),
.arr3(arr3),
.arr4(arr4),
.arr5(arr5),
.arr6(arr6),
.arr7(arr7),
.arr8(arr8),
.arr9(arr9),
.arr10(arr10),
.arr11(arr11),
.arr12(arr12),
.arr13(arr13),
.arr14(arr14),
.arr15(arr15),
.arr16(arr16),
.arr17(arr17),
.arr18(arr18),
.arr19(arr19),
.arr20(arr20),
.arr21(arr21),
.arr22(arr22),
.arr23(arr23),
.arr24(arr24),
.arr25(arr25),
.arr26(arr26),
.arr27(arr27),
.arr28(arr28),
.arr29(arr29),
.arr30(arr30),
.arr31(arr31),
.found(found),
.fin(fin)
);

initial begin
CLK = 1'b0;
end

always begin 
CLK = ~CLK;
#2;
end

initial begin
arr0 = 32'd124;
arr1 = 32'd1915;
arr2 = 32'd225;
arr3 = 32'd826;
arr4 = 32'd1326;
arr5 = 32'd705;
arr6 = 32'd418;
arr7 = 32'd1638;
arr8 = 32'd454;
arr9 = 32'd596;
arr10 = 32'd1059;
arr11 = 32'd959;
arr12 = 32'd1559;
arr13 = 32'd2259;
arr14 = 32'd559;
arr15 = 32'd1259;

size = 32'd16;
target = 32'd418;
go = 1'b0;
reset = 1'b1;
#10;
go = 1'b1;
reset = 1'b0;


#2000;
reset = 1'b1;
target = 32'd200;
go = 1'b0;
#10
reset = 1'b0;
go = 1'b1;






end


endmodule