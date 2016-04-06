-module(ids).
-compile(export_all).


loop( L ) ->
    io:format("myid: ~p~n",[L]),
    receive
        {P, myId } -> P ! {idserverResponse, L}, loop(L)
    end.

setUp(MyId) -> register( idserver, spawn(?MODULE,loop,[MyId]) ).

myId( ) -> idserver ! { self(), myId }, 
           receive {idserverResponse, X} -> X end.
nextWorkerId() -> 
    A =( myId() +  1 ),
    B = workerdirs:wlen(),
    io:format("por las dudas ~p ~p~n",[A,B]),
    A rem B.
makeIdGlobal(Worker, Client) -> { Worker,Client }.
globalIdToWorker({W,_})     -> W.
globalIdToClient({_,C})     -> C.

%DEBUG
globalFdToWorker(Gfd)  -> Gfd rem 10.
globalFdToLocalFd(Gfd) -> Gfd div 10.
makeGlobalFd(LocalFd, WorkerId) -> LocalFd*10+WorkerId.


