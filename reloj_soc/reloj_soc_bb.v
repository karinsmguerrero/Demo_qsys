
module reloj_soc (
	clk_clk,
	reset_reset_n,
	leds_export,
	buttons_export);	

	input		clk_clk;
	input		reset_reset_n;
	output	[31:0]	leds_export;
	input	[31:0]	buttons_export;
endmodule
