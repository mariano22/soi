-module(fdmanage).
-compile(export_all).

% Funciones:
% getHandle : LocalFd -> Handle (Dado un Fd devuelve el Handle del sistema de archivos real)
% getOwner  : LocalFd -> GlobalId (Dado un Fd devuelve el usuario que lo posee)
% unregisterFd : LocalFd -> ok (Borra un Fd de la tabla)
% registerFd : Globalid, Handle -> LocalFd (Registra un nuevo Fd la tabla y devuelve le Fd asignado)

% DEBUG
dbg([]) -> io:format("~n~n");
dbg([X|L]) -> io:format(lists:concat(["{",element(1,X),",",element(2,X),",",element(3,X),"} "])),
              dbg(L).

loop( L , FdC) ->
    io:format("fdmanage: ~p~n",[L]),
    receive
        {P, getHandle, Fd} -> case lists:keyfind(Fd,1,L) of
                                      false -> P ! noFd ;
                                      T ->  P ! element(3,T)
                                  end,
                                  loop( L, FdC ) ;
        {P, getOwner, Fd} -> case lists:keyfind(Fd,1,L) of
                                      false -> P ! noFd ;
                                      T ->  P ! element(2,T)
                                  end,
                                  loop( L, FdC ) ;
        {P, unregisterFd, Fd} -> Lp = lists:keydelete(Fd,1,L),
                                   P ! ok,
                                   loop( Lp , FdC) ;
        {P, registerFd, GId, Handle }   -> Lp = [{FdC,GId,Handle}|L],
                                   P ! FdC,
                                   loop( Lp, FdC+1 ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( fdmanageserver, spawn(?MODULE,loop,[[],0]) ).

getHandle( Fd ) -> fdmanageserver ! {self(), getHandle , Fd }, 
                  receive X -> X end.
getOwner( Fd ) -> fdmanageserver ! {self(), getOwner , Fd }, 
                  receive X -> X end.
unregisterFd( Fd ) -> fdmanageserver ! {self(), unregisterFd , Fd }, 
                  receive X -> X end.
registerFd( GId, Handle ) -> fdmanageserver ! {self(), registerFd ,GId, Handle }, 
                  receive X -> X end.
