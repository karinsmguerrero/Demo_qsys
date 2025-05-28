
module reloj_soc (
	clk_clk,
	leds_export,
	reset_reset_n,
	buttons_export,
	audio_0_external_interface_BCLK,
	audio_0_external_interface_DACDAT,
	audio_0_external_interface_DACLRCK);	

	input		clk_clk;
	output	[31:0]	leds_export;
	input		reset_reset_n;
	input	[31:0]	buttons_export;
	input		audio_0_external_interface_BCLK;
	output		audio_0_external_interface_DACDAT;
	input		audio_0_external_interface_DACLRCK;
endmodule
