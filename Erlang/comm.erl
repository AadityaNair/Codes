-module(comm).
-compile(export_all).

recieve_test() ->
    receive
        {From, arg1} ->
            From ! arg1;
            %io:format("got arg1");
        {From, talk} ->
            From ! arg2;
            %io:format("got talk");

        _ -> io:format("i don't even")
    end,
    recieve_test().
