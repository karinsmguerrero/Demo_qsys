module soc_top (
    input        clk,
    input        rst_n,
    input  [7:0] sw,
    output [7:0] leds,

    // Audio interface (solo los necesarios para DAC)
    input        AUD_BCLK,
    input        AUD_DACLRCK,
    output       AUD_DACDAT,
    output       AUD_XCK
);

    wire [31:0] buttons_export;
    wire [31:0] leds_export;

    assign buttons_export[7:0] = sw;
    assign leds = leds_export[7:0];

    reloj_soc dut (
        .buttons_export(buttons_export),
        .clk_clk(clk),
        .reset_reset_n(rst_n),
        .leds_export(leds_export),

        // Audio connections
        .audio_0_external_interface_BCLK(AUD_BCLK),
        .audio_0_external_interface_DACDAT(AUD_DACDAT),
        .audio_0_external_interface_DACLRCK(AUD_DACLRCK),
        .audio_pll_0_audio_clk_clk(AUD_XCK)
    );

endmodule
