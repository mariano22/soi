-module(localconections).
-compile(export_all).

% Funciones:
% newC : Clientid , Pid -> ok (registra que el cliente esta atendida su comunicacion por medio del esclavo cuyo pid es Pid)
% find  : Clientid      -> ok  (devuelve el pid del proceso que atiende la comunicacion de un cliente)
% delC :  Clientid      -> ok  (borra un cliente, porque se desconecto)

% DEBUG

loop( L ) ->
    io:format("~p~n~n",[L]),
    receive
        {P,  newC, Cid,Pid }  ->   Lp = [{Cid,Pid}|L],
                                   P ! ok,
                                   loop( Lp ) ;
        {P, find, Cid } -> case lists:keyfind(Cid,1,L) of
                                false -> P ! noClient ;
                                Cr   -> P ! element(2,Cr)
                           end,
                           loop( L ) ;
        {P, delC, Cid }     -> Lp = lists:keydelete(Cid,1,L),
                                   P ! ok,
                                   loop( Lp ) ;
        _ -> error("esto no deberia suceder 66+6") 
    end.

setUp() -> register( localconectionsserver, spawn(?MODULE,loop,[[]]) ).

newC(Cid,Pid) -> localconectionsserver ! {self(), newC, Cid,Pid }, 
                       receive X -> X end.
find(Cid) -> localconectionsserver ! {self(), find, Cid }, 
                       receive X -> X end.
delC(Cid) -> localconectionsserver ! {self(), delC, Cid }, 
                       receive X -> X end.

