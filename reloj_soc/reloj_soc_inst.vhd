	component reloj_soc is
		port (
			buttons_export : in  std_logic_vector(31 downto 0) := (others => 'X'); -- export
			clk_clk        : in  std_logic                     := 'X';             -- clk
			leds_export    : out std_logic_vector(31 downto 0);                    -- export
			reset_reset_n  : in  std_logic                     := 'X';             -- reset_n
			audio_BCLK     : in  std_logic                     := 'X';             -- BCLK
			audio_DACDAT   : out std_logic;                                        -- DACDAT
			audio_DACLRCK  : in  std_logic                     := 'X'              -- DACLRCK
		);
	end component reloj_soc;

	u0 : component reloj_soc
		port map (
			buttons_export => CONNECTED_TO_buttons_export, -- buttons.export
			clk_clk        => CONNECTED_TO_clk_clk,        --     clk.clk
			leds_export    => CONNECTED_TO_leds_export,    --    leds.export
			reset_reset_n  => CONNECTED_TO_reset_reset_n,  --   reset.reset_n
			audio_BCLK     => CONNECTED_TO_audio_BCLK,     --   audio.BCLK
			audio_DACDAT   => CONNECTED_TO_audio_DACDAT,   --        .DACDAT
			audio_DACLRCK  => CONNECTED_TO_audio_DACLRCK   --        .DACLRCK
		);

