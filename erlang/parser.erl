-module(parser).
-export([parse/1]).

parse(String) -> S = lists:takewhile(fun(X) -> X/=$\n end,String),
		  string:tokens(S," ").
