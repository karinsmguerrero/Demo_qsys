module soc_top (input clk,
                     input rst_n,
                     input [7:0] sw,
                     output [7:0] leds);

wire [31:0] buttons_export;
wire [31:0]leds_export;

assign buttons_export[7:0] = sw;
assign leds = leds_export[7:0];
reloj_soc dut (
        .buttons_export(buttons_export), // buttons.export
        .clk_clk (clk),        //     clk.clk
        .leds_export(leds_export),    //    leds.export
        .reset_reset_n(rst_n)
        );

endmodule