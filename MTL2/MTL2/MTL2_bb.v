
module MTL2 (
	clk_clk,
	mtl2_CLK,
	mtl2_HS,
	mtl2_VS,
	mtl2_DATA_EN,
	mtl2_R,
	mtl2_G,
	mtl2_B,
	reset_reset_n);	

	input		clk_clk;
	output		mtl2_CLK;
	output		mtl2_HS;
	output		mtl2_VS;
	output		mtl2_DATA_EN;
	output	[7:0]	mtl2_R;
	output	[7:0]	mtl2_G;
	output	[7:0]	mtl2_B;
	input		reset_reset_n;
endmodule
