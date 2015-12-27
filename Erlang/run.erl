-module(run).
-compile(export_all).

test_func() ->
    [1 || X <- lists:seq(1,10)],
    io:format("thisis it.").
