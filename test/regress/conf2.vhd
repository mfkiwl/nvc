entity comparison is
    port (
        a : in integer;
        b : in integer;
        q : out boolean );
end comparison;

architecture rtl of comparison is
begin
    q <= a = b;
end architecture;

-------------------------------------------------------------------------------

entity comparison_tb is
end entity;

architecture sim of comparison_tb is
    signal a : integer;
    signal b : integer;
    signal q : boolean;

    component comparison
        port (
            a : in integer;
            b : in integer;
            q : out boolean );
    end component;

    procedure result(a, b : in integer; q : in boolean) is
    begin
        report integer'image(a) & " <=> " & integer'image(b)
            & " = " & boolean'image(q);
    end procedure;

begin

    DUT: component comparison
        port map ( a, b,  q );

    SEQUENCER_PROC: process
    begin
        a <= 4;
        b <= 2;
        wait for 1 ns;
        result(a, b, q);

        b <= 7;
        wait for 1 ns;
        result(a, b, q);

        wait;
    end process;

end architecture;

-------------------------------------------------------------------------------

entity greater_than is
    port (
        a : in integer;
        b : in integer;
        q : out boolean );
end greater_than;

architecture rtl of greater_than is
begin
    q <= a > b;
end architecture;

-------------------------------------------------------------------------------

configuration eq of comparison_tb is
    for sim
    end for;
end configuration;

-------------------------------------------------------------------------------

configuration gt of comparison_tb is
    for sim
        for DUT : comparison
            use entity work.greater_than(rtl);
        end for;
    end for;
end configuration;

-------------------------------------------------------------------------------

configuration lt of comparison_tb is
    for sim
        for DUT : comparison
            use entity work.greater_than(rtl)
                port map ( b, a, q );
        end for;
    end for;
end configuration;

-------------------------------------------------------------------------------

entity conf2 is
end entity;

architecture test of conf2 is
begin

    uut1: configuration work.lt;

    uut2: configuration work.gt;

end architecture;
