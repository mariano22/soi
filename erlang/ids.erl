-module(ids).
-compile(export_all).


loop( L ) ->
    receive
        {P, myId } -> P ! L, loop(L)
    end.

setUp(MyId) -> register( idserver, spawn(?MODULE,loop,[MyId]) ).

myId( ) -> idserver ! { self(), myId }, 
           receive X -> X end.
makeIdGlobal(Worker, Client) -> { Worker,Client }.
globalIdToWorker({W,_})     -> W.
globalIdToClient({_,C})     -> C.

%DEBUG
globalFdToWorker(Gfd)  -> Gfd rem 10.
globalFdToLocalFd(Gfd) -> Gfd div 10.
makeGlobalFd(LocalFd, WorkerId) -> LocalFd*10+WorkerId.

