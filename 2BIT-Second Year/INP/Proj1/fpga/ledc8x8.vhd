--Autor: Marek Marusic, xmarus05
--email: xmarus05@stud.fit.vutbr.cz

library IEEE;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;


entity ledc8x8 is
  port (
	SMCLK	: in std_logic;
	RESET	: in std_logic;
	ROW		: out std_logic_vector(0 to 7);
	LED		: out std_logic_vector(0 to 7)
  );
end entity ; -- ledc8x8

architecture main of ledc8x8 is
	signal CE			: std_logic := '0';
	signal CE_counter	: std_logic_vector(7 downto 0) := "00000000";
	signal ROW_in		: std_logic_vector(7 downto 0) := "10000000";
begin

CE_gen : process( SMCLK, RESET ) -- SMCLK/256 (vygenerovat CE)
begin
	if RESET = '1' then
		CE_counter <= "00000000";
	elsif SMCLK'event and SMCLK = '1' then
		CE_counter <= CE_counter +1;
	end if ;	
end process ; -- CE_gen

CE <= '1' when CE_counter = X"FF" else '0';

ROW_posuv : process( RESET, SMCLK, CE) --posuv riadkov (posuvny register)
begin
	if RESET = '1' then
		ROW_in	<= "10000000";
	elsif SMCLK'event and SMCLK = '1' and CE = '1' then
		ROW_in <= ROW_in(0) & ROW_in(7 downto 1);
	end if ;
end process ; -- ROW_posuv

LED_gen : process(SMCLK, ROW_in)	--rozsvietit ledky v danom riadku
begin
	case( ROW_in ) is	
		when "10000000" => LED <= "01110111";
		when "01000000" => LED <= "00100111";
		when "00100000" => LED <= "01010111";
		when "00010000" => LED <= "01110111";
		when "00001000" => LED <= "11101110";
		when "00000100" => LED <= "11100100";
		when "00000010" => LED <= "11101010";
		when "00000001" => LED <= "11101110";
	
		when others => LED <= "11111111";
	end case ;	
end process ; -- LED_gen

ROW <= ROW_in;

end architecture ; -- main