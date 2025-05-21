
module reloj_soc (
	buttons_export,
	clk_clk,
	leds_export,
	reset_reset_n);	

	input	[31:0]	buttons_export;
	input		clk_clk;
	output	[31:0]	leds_export;
	input		reset_reset_n;
endmodule
