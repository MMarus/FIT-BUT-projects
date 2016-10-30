-- fsm.vhd: Finite State Machine
-- Author(s): Marek Marusic, FIT-VUT, 6.5.2014
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST_ENT,TEST1, TESTAB, TESTA3, TESTA4, TESTA5, TESTA6, TESTA7, TESTA8, TESTA9, TESTB3, TESTB4, TESTB5, TESTB6, TESTB7, TESTB8, TESTB9, TESTB10, FAIL, PRINT_MESSAGE_OK, PRINT_MESSAGE_ERR, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(9) = '1') then
         next_state <= TESTAB; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTAB =>
      next_state <= TESTAB;
      if (KEY(4) = '1') then
         next_state <= TESTA3; 
      elsif (KEY(5) = '1') then
         next_state <= TESTB3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTA3 =>
      next_state <= TESTA3;
      if (KEY(7) = '1') then
         next_state <= TESTA4; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTB3 =>
      next_state <= TESTB3;
      if (KEY(1) = '1') then
         next_state <= TESTB4; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTA4 =>
      next_state <= TESTA4;
      if (KEY(5) = '1') then
         next_state <= TESTA5; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTB4 =>
      next_state <= TESTB4;
      if (KEY(6) = '1') then
         next_state <= TESTB5; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTA5 =>
      next_state <= TESTA5;
      if (KEY(7) = '1') then
         next_state <= TESTA6; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTB5 =>
      next_state <= TESTB5;
      if (KEY(1) = '1') then
         next_state <= TESTB6; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTA6 =>
      next_state <= TESTA6;
      if (KEY(0) = '1') then
         next_state <= TESTA7; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTB6 =>
      next_state <= TESTB6;
      if (KEY(1) = '1') then
         next_state <= TESTB7; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTA7 =>
      next_state <= TESTA7;
      if (KEY(8) = '1') then
         next_state <= TESTA8; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTB7 =>
      next_state <= TESTB7;
      if (KEY(3) = '1') then
         next_state <= TESTB8; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTA8 =>
      next_state <= TESTA8;
      if (KEY(1) = '1') then
         next_state <= TESTA9; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTB8 =>
      next_state <= TESTB8;
      if (KEY(3) = '1') then
         next_state <= TESTB9; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - KONEC A
   when TESTA9 =>
     next_state <= TESTA9;
      if (KEY(3) = '1') then
         next_state <= TEST_ENT; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TESTB9 =>
      next_state <= TESTB9;
      if (KEY(0) = '1') then
         next_state <= TESTB10; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -KONEC B
   when TESTB10 =>
      next_state <= TESTB10;
      if (KEY(0) = '1') then
         next_state <= TEST_ENT; 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -KONEC B
   when TEST_ENT =>
      next_state <= TEST_ENT;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_OK;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
         next_state <= FAIL;
      end if;
   --  - - - - - - - - - - - - - - - - - - - - - -
   when FAIL =>
      next_state <= FAIL;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_ERR =>
      next_state <= PRINT_MESSAGE_ERR;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      next_state <= PRINT_MESSAGE_OK;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_ERR =>
      FSM_MX_MEM     <= '0';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      FSM_MX_MEM     <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;

