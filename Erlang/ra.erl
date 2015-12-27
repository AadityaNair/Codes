-module(ra).
%-export([process_spawner/1]).
-compile(export_all).

process_spawner(0) -> io:format("All processes created.~n");
process_spawner(N) ->
    erlang:spawn(?MODULE, site, [N]),
    process_spawner(N-1).

site(Proc_id) ->
    io:format("done~n").
