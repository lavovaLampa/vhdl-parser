library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;


entity fjel is
    port (
        clk:in std_logic;
        reset:in std_logic
    );
end entity fjel;

architecture rtl of fjel is
    procedure "+"(omegalul : string) is
    begin

    end procedure "+";
begin
    omegafasf: process(clk)
    begin
        if rising_edge(clk) then

        end if;
    end process omegafasf;

    tempProc : process(clk)
        variable \te\\mp\ : std_logic;-- Toto je elektro ty obmedzeny kokotko
        variable \\\\ : natural := 10; -- Toto je iny komentar ty obmedzeny kokotko
        -- najlepis hip-hop festival tu robi jopojopojopjofjaspfoasdjfosdf 10.30 "toto je este tale komen"
    begin
        if rising_edge(clk) then
            \te\\mp\ := X"0_A_A";
            \te\\mp\ := 0;
            \te\\mp\ := 0_1.3_4E+1_2;
            \te\\mp\ := 1_6#0_1.1_0#E10;
            \te\\mp\ := 16#F.FF#E+2;
            \te\\mp\ := 2#1.1111_1111_111#E11;
            \te\\mp\ := 16#E#E1;
            \te\\mp\ := 2#1110_0000#;
            \te\\mp\ := 2#1111_1111#;
            \te\\mp\ := 16#FF#;
            \te\\mp\ := 016#0FF#;
            omegalul := "Toto je elektro "" ty obmedzeny kokotko\n """"";
            meglule := """";
            megalul := "";
            siginel := '1';
            siginelli := (others => '1');
            supasoldas := 'U';
            super := 'Z';
        end if;
    end process;

    constant c1: STRING := B"1111_1111_1111";
    constant c2: BIT_VECTOR := X"FFF";
    type MVL is ('X', '0', '1', 'Z');
    type MVL_VECTOR is array (NATURAL range <>) of MVL;
    constant c3: MVL_VECTOR := O"777";
    assert
    c1'LENGTH=12and
    c2'LENGTH=12and
    c3="111111111";
end architecture;