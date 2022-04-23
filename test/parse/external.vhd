entity external is
end entity;

architecture test of external is
begin

    main: process is
        variable i : integer;
    begin
        i := <<signal foo.bar : integer>>;  -- OK
        i := << constant x.y.z : integer >>;  -- OK
        i := <<variable aye.bee : integer>>;  -- OK
        i := << constant .x.y.z : integer>>;  -- OK
        i := << constant ^.^.foo : integer >>;  -- OK
        i := << constant @work.pack.foo : integer >>;  -- OK
    end process;

end architecture;