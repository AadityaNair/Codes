-module(rpn).
-compile(export_all).

%L = string:tokens("10 4 3 + 2 * -", " ").

convert(N) ->
    try erlang:list_to_integer(N) of
        _ -> list_to_integer(N)
    catch
        error:badarg -> N
    end.

rpn(L) when is_list(L) ->
    New = lists:map(fun convert/1, string:tokens(L, " ")),
    [Res]=lists:foldl(fun rpn/2, [], New),
    Res.
    

rpn("+", [N1,N2|Stack]) -> [N1+N2|Stack];
rpn(X, Stack) -> [convert(X)|Stack].

