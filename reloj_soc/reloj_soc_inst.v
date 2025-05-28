	reloj_soc u0 (
		.clk_clk                            (<connected-to-clk_clk>),                            //                        clk.clk
		.leds_export                        (<connected-to-leds_export>),                        //                       leds.export
		.reset_reset_n                      (<connected-to-reset_reset_n>),                      //                      reset.reset_n
		.buttons_export                     (<connected-to-buttons_export>),                     //                    buttons.export
		.audio_0_external_interface_BCLK    (<connected-to-audio_0_external_interface_BCLK>),    // audio_0_external_interface.BCLK
		.audio_0_external_interface_DACDAT  (<connected-to-audio_0_external_interface_DACDAT>),  //                           .DACDAT
		.audio_0_external_interface_DACLRCK (<connected-to-audio_0_external_interface_DACLRCK>)  //                           .DACLRCK
	);

