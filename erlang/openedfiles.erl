-module(openedfiles).
-compile(setUp/0, registerOpen/2, registerClose/1, globalFdList/1]).

% La funcion de este modulo es llevar un seguimiento de los GlobalFd tomados por clientes conectados a este worker
% Funciones
% registerOpen : GlobalFd, ClientId -> ok (Registra que el GlobalFd fue tomado por el ClientId)
% registerClose : GlobalFd -> ok (Elimina a GlobalFd de la tabla, para cuando el cliente cierra el archivo)
% globalFdList : ClientId -> [GlobalFd] (Lista los GlobalFd que actualmente tiene en posecion el cliente, muy util a la hora de hacer un bye)


loop( L ) ->
    io:format("openedfiles: ~p~n",[L]), % DEBUG
    receive
        {P, globalFdList , ClientId }    -> Rp = lists:filter(fun(X) -> element(2,X)==ClientId end,L),
                                            R = lists:map(fun(X) -> element(1,X) end,Rp),
                                            P ! {openedfilesserverResponse, R} ,
                                            loop( L ) ;                                    
        {P,  registerClose , GlobalFd }  -> Lp = lists:keydelete(GlobalFd,1,L),
                                            P ! {openedfilesserverResponse, ok},
                                            loop( Lp ) ;
        {P, registerOpen, GlobalFd, ClientId }   -> Lp = [{GlobalFd,ClientId}|L],
                                   P ! {openedfilesserverResponse, ok},
                                   loop( Lp ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( openedfilesserver, spawn(?MODULE,loop,[[]]) ).

registerOpen( GlobalFd, ClientId ) -> openedfilesserver ! {self(), registerOpen , GlobalFd, ClientId }, 
                  receive {openedfilesserverResponse, X} -> X end.
registerClose( GlobalFd ) -> openedfilesserver ! {self(), registerClose , GlobalFd }, 
                  receive {openedfilesserverResponse, X} -> X end.
globalFdList( ClientId ) -> openedfilesserver ! {self(), globalFdList , ClientId }, 
                  receive {openedfilesserverResponse, X} -> X end.




