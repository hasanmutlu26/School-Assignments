module danger(dang, v, b, r, w);
input [7:0] v;
input b, r, w;
output dang;

reg greater;

always @(*) begin
    greater = (v > 8'd20);
end

and (dang, greater, ~b, ~r, w);

endmodule