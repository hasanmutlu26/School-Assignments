library verilog;
use verilog.vl_types.all;
entity half_adder is
    port(
        sum             : out    vl_logic;
        carry_out       : out    vl_logic;
        a               : in     vl_logic;
        b               : in     vl_logic
    );
end half_adder;
