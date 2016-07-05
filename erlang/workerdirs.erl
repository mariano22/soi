-module(workerdirs).
-compile(export_all).
%-export(export_all).
-include("worker_list.hrl").

externPort(WId) -> element(2,lists:keyfind(WId,1,?WORKER_LIST)).
internPort(WId) -> element(3,lists:keyfind(WId,1,?WORKER_LIST)).
ip(WId) -> element(4,lists:keyfind(WId,1,?WORKER_LIST)).
wlen() -> length(?WORKER_LIST).
