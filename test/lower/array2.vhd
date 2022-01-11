entity array2 is
end entity;

architecture test of array2 is
    type int_vec is array (natural range <>) of integer;
    type int_vec2x2 is array (natural range <>, natural range <>) of integer;
begin

    p1: process is
        variable v : int_vec(1 to 2);
        variable x : integer;
    begin
        v := (x, x);
        wait;
    end process;

    p2: process is
        variable v : int_vec(1 to 2);
        variable x : integer;
    begin
        v := (1 => x, others => 0);
        wait;
    end process;

    p3: process is
        variable v    : int_vec2x2(1 to 2, 1 to 2);
        variable x, y : integer;
    begin
        v := ((x, x), (y, y));
        wait;
    end process;

    p4: process is
        variable v    : int_vec2x2(1 to 2, 1 to 2);
        variable x, y : integer;
    begin
        v := ((x, x), others => (y, y));
        wait;
    end process;

end architecture;
