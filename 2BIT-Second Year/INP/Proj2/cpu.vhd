-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2014 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Marek Marusic, xmarus05, 
-- email : xmarus05@stud.fit.vutbr.cz

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
  type state is (sIdle, sFetch, sDecode,sPutCReadData,sGetCReq,sFetchWait, sDataIncWrite, sDataDecWrite, sPtrInc, sPtrDec, sDataInc, sDataDec, sWhileInc,sWhileInc2, sWhileInc3, sWhileInc4, sWhileInc5, sWhileDec,sWhileDec2,sWhileDec3,sWhileDec4,sWhileDec5, sPutC, sGetC, sRet, sNop);
  signal presentS, nextS : state;
  signal pc : std_logic_vector(12 downto 0);
  signal cnt : std_logic_vector(7 downto 0);
  signal ptr : std_logic_vector(12 downto 0);
  signal pcInc, pcDec, cntInc, cntDec, ptrInc, ptrDec : std_logic := '0';
  --Multiplexory
  signal mxSel1 :std_logic;
  signal mxSel2 :std_logic_vector(1 downto 0);
  type instr_type is (IptrInc, IptrDec, IdataInc, IdataDec, IwhileInc, IwhileDec, IputC, IgetC, Iret, Inop);
  signal instr : instr_type;
begin
--Program counter register
pcCnt : process ( RESET, CLK)
begin
  if (RESET = '1') then
    pc <= (others => '0');
  elsif rising_edge(CLK) then
    if pcInc = '1' then
        pc <= pc + 1;
    elsif pcDec = '1' then
        pc <= pc - '1';
      end if; 
  end if;
end process;

--Pointer counter register
ptrCnt : process ( RESET, CLK)
begin
  if RESET = '1' then
    ptr <= "1000000000000";
  elsif rising_edge(CLK) then
    if ptrInc = '1' then
      ptr <= ptr + 1;
    elsif ptrDec = '1' then
      ptr <= ptr - 1;
    end if;
  end if;
end process;

--CNT counter register( count of cycles)
CntCnt : process ( RESET, CLK)
begin
  if RESET = '1' then
    cnt <= (others => '0');
  elsif rising_edge(CLK) then
    if cntInc = '1' then
      cnt <= cnt + '1';
    elsif cntDec = '1' then
      cnt <= cnt - '1';
    end if;
  end if;
end process;

--Present state
fsmPresentS : process (RESET, CLK)
begin
  if (RESET = '1') then
    presentS <= sIdle;
  elsif rising_edge(CLK) and EN = '1' then
    presentS <= nextS;
  end if;
end process;

--Instruction decoder
instrDecode : process (DATA_RDATA)
begin
  case( DATA_RDATA ) is
    when X"3E" => instr <= IptrInc;
    when X"3C" => instr <= IptrDec;
    when X"2B" => instr <= IdataInc;
    when X"2D" => instr <= IdataDec;
    when X"5B" => instr <= IwhileInc;
    when X"5D" => instr <= IwhileDec;
    when X"2E" => instr <= IputC;
    when X"2C" => instr <= IgetC;
    when X"00" => instr <= Iret;
    when others=> instr <= Inop;
  end case;
end process;

-- Next state FSM
fsmNextS : process( OUT_BUSY, IN_VLD, EN, presentS, instr )
begin
  DATA_RDWR   <= '0';
  DATA_EN     <= '0';
  IN_REQ      <= '0';
  OUT_WE      <= '0';
  pcInc      <= '0';
  pcDec      <= '0';
  cntInc      <= '0';
  cntDec      <= '0';
  ptrInc      <= '0';
  ptrDec      <= '0';
  mxSel1 <= '0'; --TODO NOT SURE ABOUT THESE 2
  mxSel2 <= "00";

  case( presentS ) is
    --IDLE
    when sIdle =>
      nextS <= sFetch;
    --FETCH
    when sFetch =>
      mxSel1 <= '0';
      DATA_EN <= '1';
      nextS <= sDecode;
    --DECODE
    when sDecode =>
      case( instr ) is
        when IptrInc    => nextS <= sPtrInc;
        when IptrDec    => nextS <= sPtrDec;
        when IdataInc   => nextS <= sDataInc;
        when IdataDec   => nextS <= sDataDec;
        when IputC      => nextS <= sPutCReadData;
        when IgetC      => nextS <= sGetCReq;
        when IwhileInc  => nextS <= sWhileInc;
        when IwhileDec  => nextS <= sWhileDec;
        when Iret       => nextS <= sRet;
        when others     => nextS <= sNop;
      end case;
   
    --PTR INCREMENT
    when sPtrInc => 
      pcInc <= '1';
      ptrInc <= '1';
      nextS <= sFetch;
    --PTR DECREMENT
    when sPtrDec =>
      pcInc <= '1';
      ptrDec <= '1';
      nextS <= sFetch;
    --TODO domyslet tie MUX2
    --DATA INCREMENT
    when sDataInc =>
      DATA_EN <= '1';
      mxSel1 <= '1'; --SET MUX1 TO POINTER ADDRESS !!!!TODO  not sure how to use this and return it to 0 after
      mxSel2 <= "10"; --SET MUX2 TO INCREMENT DATA_RDATA
      DATA_RDWR <= '0';
      nextS <= sDataIncWrite;
    --DATA DECREMENT
    when sDataDec =>
      DATA_RDWR <= '0';
      DATA_EN <= '1';
      mxSel1 <= '1'; --SET MUX1 TO POINTER ADDRESS !!!!TODO  not sure how to use this and return it to 0 after
      mxSel2 <= "01"; --SET MUX2 TO DECREMENT DATA_RDATA
      nextS <= sDataDecWrite;
  --WRITE DATA --NOT SURE
    when sDataIncWrite =>
      DATA_EN <= '1';
      DATA_RDWR <= '1';
      mxSel1 <= '1'; --SET MUX1 TO POINTER ADDRESS !!!!TODO  not sure how to use this and return it to 0 after
      mxSel2 <= "10"; --SET MUX2 TO INCREMENT DATA_RDATA
      pcInc <= '1';
      nextS <= sFetch;
    when sDataDecWrite =>
      DATA_EN <= '1';
      DATA_RDWR <= '1';
      mxSel1 <= '1'; --SET MUX1 TO POINTER ADDRESS !!!!TODO  not sure how to use this and return it to 0 after
      mxSel2 <= "01"; --SET MUX2 TO DECREMENT DATA_RDATA
      pcInc <= '1';
    nextS <= sFetch;
--Putchar
    when sPutCReadData =>
      mxSel1 <= '1';
      DATA_EN <= '1';
      DATA_RDWR <= '0';
      nextS <= sPutC;
    when sPutC =>
      nextS <= sPutC;
      -- while(OUT_BUSY){}
      if(OUT_BUSY = '0') then
        nextS <= sFetch;
        OUT_WE <= '1';
        pcInc <= '1';
      end if;
-- GETCHAR
    when sGetCReq => 
      nextS <= sGetC;
      IN_REQ <= '1';
    when sGetC => 
      nextS    <= sGetC;
      IN_REQ        <= '1';
      if (IN_VLD = '1') then
        nextS    <= sFetch;
        mxsel1     <= '1';
        DATA_EN     <= '1';
        DATA_RDWR   <= '1';
        mxSel2 <= "00";
        pcInc  <= '1';
      end if;
--WHILE INC
    when sWhileInc => 
      mxSel1 <= '1';
      DATA_EN     <= '1';
      DATA_RDWR   <= '0';
      nextS    <= sWhileInc2;
      pcInc  <= '1';
    when sWhileInc2 =>
      mxSel1 <= '1';
      DATA_EN     <= '1';
      if (DATA_RDATA = "00000000") then
        nextS    <= sWhileInc3;
        cntInc   <= '1';
      else
        nextS    <= sFetch;
      end if;
    when sWhileInc3 =>
      DATA_EN     <= '1';
      mxSel1       <= '0';
      nextS    <= sWhileInc4;
    when sWhileInc4 =>
      nextS    <= sWhileInc5;
      if (instr = IwhileInc) then
        cntInc <= '1';
      elsif (instr = IwhileDec) then
        cntDec   <= '1';
      end if;
      pcInc  <= '1';
    when sWhileInc5 =>
      nextS    <= sWhileInc3;
    if (cnt = X"00000000") then
      nextS    <= sFetch;
    end if;
--WHILE DEC
    when sWhileDec =>
      mxSel1       <= '1';
      DATA_EN     <= '1';
      DATA_RDWR     <= '0';
      nextS    <= sWhileDec2;        
    when sWhileDec2 =>
      nextS    <= sFetch;
      if (DATA_RDATA = "00000000") then
        pcInc  <= '1';
      else
        pcDec  <= '1';
        cntInc   <= '1';
        nextS    <= sWhileDec3;
      end if;
    when sWhileDec3 =>
      mxSel1     <= '0';
      DATA_EN     <= '1';
      nextS    <= sWhileDec4;
    when sWhileDec4 =>
      nextS    <= sWhileDec5;
      if (instr = IwhileInc) then
        cntDec   <= '1';     -------------TODO WTF naopak ??
      elsif (instr = IwhileDec) then
        cntInc   <= '1';   -------------TODO WTF naopak ??
      end if;
    when sWhileDec5 =>
      nextS    <= sFetch;
      if (cnt = "00000000") then
        pcInc  <= '1';
      else
        pcDec  <= '1';
        nextS <= sWhileDec3;
      end if;
  --HALT
    when sRet =>
      nextS <= sRet;
    --NOP
    when sNop =>
      nextS <= sFetch;
      pcInc <= '1';

    when others => pcInc <= '1';
  end case;
end process fsmNextS;

MX1 : process ( mxSel1, pc, ptr)
begin
  if mxSel1 = '0' then
    DATA_ADDR <= pc;
  elsif mxSel1 = '1' then
    DATA_ADDR <= ptr;
  else 
    DATA_ADDR <= (others => '0');
  end if;
end process;

MX2 : process ( mxSel2, IN_DATA, DATA_RDATA)
begin
  if mxSel2 = "00" then
    DATA_WDATA <= IN_DATA;
  elsif mxSel2 = "01" then
    DATA_WDATA <= DATA_RDATA - '1';
  elsif mxSel2 = "10" then
    DATA_WDATA <= DATA_RDATA + '1';
  else 
    DATA_WDATA <= (others => '0');
  end if;
end process;

OUT_DATA <= DATA_RDATA;

end behavioral;
 
