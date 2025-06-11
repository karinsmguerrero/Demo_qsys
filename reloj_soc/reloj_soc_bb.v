
module reloj_soc (
	audio_0_external_interface_BCLK,
	audio_0_external_interface_DACDAT,
	audio_0_external_interface_DACLRCK,
	audio_and_video_config_0_external_interface_SDAT,
	audio_and_video_config_0_external_interface_SCLK,
	audio_pll_0_audio_clk_clk,
	buttons_export,
	clk_clk,
	leds_export,
	memory_mem_a,
	memory_mem_ba,
	memory_mem_ck,
	memory_mem_ck_n,
	memory_mem_cke,
	memory_mem_cs_n,
	memory_mem_ras_n,
	memory_mem_cas_n,
	memory_mem_we_n,
	memory_mem_reset_n,
	memory_mem_dq,
	memory_mem_dqs,
	memory_mem_dqs_n,
	memory_mem_odt,
	memory_mem_dm,
	memory_oct_rzqin,
	reset_reset_n);	

	input		audio_0_external_interface_BCLK;
	output		audio_0_external_interface_DACDAT;
	input		audio_0_external_interface_DACLRCK;
	inout		audio_and_video_config_0_external_interface_SDAT;
	output		audio_and_video_config_0_external_interface_SCLK;
	output		audio_pll_0_audio_clk_clk;
	input	[31:0]	buttons_export;
	input		clk_clk;
	output	[31:0]	leds_export;
	output	[12:0]	memory_mem_a;
	output	[2:0]	memory_mem_ba;
	output		memory_mem_ck;
	output		memory_mem_ck_n;
	output		memory_mem_cke;
	output		memory_mem_cs_n;
	output		memory_mem_ras_n;
	output		memory_mem_cas_n;
	output		memory_mem_we_n;
	output		memory_mem_reset_n;
	inout	[7:0]	memory_mem_dq;
	inout		memory_mem_dqs;
	inout		memory_mem_dqs_n;
	output		memory_mem_odt;
	output		memory_mem_dm;
	input		memory_oct_rzqin;
	input		reset_reset_n;
endmodule
