-module(ids).
-compile(export_all).


loop( L ) ->
    io:format("myid: ~p~n",[L]), %DEBUG
    receive
        {P, myId } -> P ! {idserverResponse, L}, loop(L)
    end.

setUp(MyId) -> register( idserver, spawn(?MODULE,loop,[MyId]) ).

myId( ) -> idserver ! { self(), myId }, 
           receive {idserverResponse, X} -> X end.
nextWorkerId() ->     ( myId() +  1 ) rem workerdirs:wlen().

makeIdGlobal(Worker, Client) -> { Worker,Client }.
globalIdToWorker({W,_})     -> W.
globalIdToClient({_,C})     -> C.

globalFdToWorker(Gfd)  -> Gfd rem 10.
globalFdToLocalFd(Gfd) -> Gfd div 10.
makeGlobalFd(LocalFd, WorkerId) -> LocalFd*10+WorkerId.


