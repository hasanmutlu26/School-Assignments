module signExt6to32(result, A);
input [5:0] A;
output [31:0] result;


_32bit_2x1MUX g0(result, {26'b00_0000_0000_0000_0000_0000_0000, A}, {26'b11_1111_1111_1111_1111_1111_1111, A}, A[5]);

endmodule
