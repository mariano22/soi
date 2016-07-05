-module(fdmanage).
-compile([setUp/0, getHandle/1, getNameFile/1, getOwner/1, unregisterFd/1, registerFd/3]).

% Funciones:
% getHandle : LocalFd -> Handle (Dado un Fd devuelve el Handle del sistema de archivos real)
% getOwner  : LocalFd -> GlobalId (Dado un Fd devuelve el usuario que lo posee)
% getNameFile  : LocalFd -> String (Dado un Fd devuelve el nombre del archivo que es)
% unregisterFd : LocalFd -> ok (Borra un Fd de la tabla)
% registerFd : Globalid, Handle,NameFile -> LocalFd (Registra un nuevo Fd la tabla y devuelve le Fd asignado)

loop( L , FdC) ->
    io:format("fdmanage: ~p~n",[L]), %DEBUG
    receive
        {P, getHandle, Fd} -> case lists:keyfind(Fd,1,L) of
                                      false -> P ! noFd ;
                                      T ->  P ! {fdmanageserverResponse, element(3,T) }
                                  end,
                                  loop( L, FdC ) ;
        {P, getOwner, Fd} -> case lists:keyfind(Fd,1,L) of
                                      false -> P ! noFd ;
                                      T ->  P ! {fdmanageserverResponse, element(2,T)}
                                  end,
                                  loop( L, FdC ) ;
        {P, getNameFile, Fd} -> case lists:keyfind(Fd,1,L) of
                                      false -> P ! noFd ;
                                      T ->  P ! {fdmanageserverResponse, element(4,T)}
                                  end,
                                  loop( L, FdC ) ;
        {P, unregisterFd, Fd} -> Lp = lists:keydelete(Fd,1,L),
                                   P ! {fdmanageserverResponse, ok},
                                   loop( Lp , FdC) ;
        {P, registerFd, GId, Handle, NameFile }   -> Lp = [{FdC,GId,Handle,NameFile}|L],
                                   P ! {fdmanageserverResponse, FdC},
                                   loop( Lp, FdC+1 ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( fdmanageserver, spawn(?MODULE,loop,[[],0]) ).

getHandle( Fd ) -> fdmanageserver ! {self(), getHandle , Fd }, 
                  receive {fdmanageserverResponse,X} -> X end.
getNameFile( Fd ) -> fdmanageserver ! {self(), getNameFile , Fd }, 
                  receive {fdmanageserverResponse,X} -> X end.
getOwner( Fd ) -> fdmanageserver ! {self(), getOwner , Fd }, 
                  receive {fdmanageserverResponse,X} -> X end.
unregisterFd( Fd ) -> fdmanageserver ! {self(), unregisterFd , Fd }, 
                  receive {fdmanageserverResponse,X} -> X end.
registerFd( GId, Handle, NameFile ) -> fdmanageserver ! {self(), registerFd ,GId, Handle, NameFile }, 
                  receive {fdmanageserverResponse,X} -> X end.
