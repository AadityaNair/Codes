-module(useless).
%-export([add/2, hello/0, test/2]).
-compile(export_all).

-record(robot, {
          name,
          type=asdf,
          hobbies,
          details=[]
         }).

first_robot() ->
    #robot{name=mechatron}.
bot_factory(Name) ->
    #robot{name=Name}.

add(A,B) ->
    A+B.
hello() ->
    io:format("Hello~n").

test(5,A) ->
    io:format("five ~s ~n", [A]);
test(6,A) ->
    io:format("six ~s ~n", [A]);
test(_,A) 
  when A >= 10, A =< 20 ->
    io:format("anything ~p ~n", [A]).

heh_fine() ->
    if 1=:=1 ->
           works
    end,
    if 2 =:= 1; 1 =:= 1 ->
           works
    end,
    if 2 =:= 1, 1 =:= 1 ->
           fails;
       true -> doesnot
    end.

test2(A, B) ->
    case A of 
        5 -> five;
        6 -> six;
        _ -> idk
    end.

% recursion
factorial(N) when N =:= 0 -> 1;
factorial(N) when N > 0 -> N*factorial(N-1).

tail_fac(N) -> tail_fac(N, 1).
tail_fac(0, Acc) -> Acc;
tail_fac(N, Acc) -> tail_fac(N-1, Acc*N).

tail_len(A) -> tail_len(A, 0).
tail_len([], Acc) -> Acc;
tail_len([_|T], Acc) -> tail_len(T, Acc+1).


% higher order functions

one() -> 1.
two() -> 2.
addhh(X, Y) -> X() + Y().

incr(X) -> X+1.

map(_, []) -> [];
map(F, [H|L]) -> [F(H)|map(F,L)].
