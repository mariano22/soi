-module(globalfiles).
-compile(export_all).

% Funciones:
% archivosActuales () -> [String] (Lista los archivos que actualmente es la base de datos global del worker)
% getOwner : String -> noOwner | WorkerId (Dado un archivo, si existe devuelve su dueño)
% baja : String -> ok (Da de baja un archivo en la tabla)
% alta : String, WorkerId -> ok (Da de alta un archivo en la tabla, asignandole un dueño)

% DEBUG
dbg([]) -> io:format("~n~n");
dbg([X|L]) -> io:format(lists:concat(["{",element(1,X),",",element(2,X),"} "])),
              dbg(L).

loop( L ) ->
    io:format("globalfiles: ~p~n",[L]),
    receive
        {P, archivosActuales}  -> { Archivos , _ } = lists:unzip( L ),
                                  P ! Archivos ,
                                  loop( L ) ;
        {P, getOwner, NameFile} -> case lists:keyfind(NameFile,1,L) of
                                        false -> P ! noOwner ;
                                        WId   -> P ! element(2,WId)
                                   end,
                                   loop( L ) ;
        {P, baja, NameFile}     -> Lp = lists:keydelete(NameFile,1,L),
                                   P ! ok,
                                   loop( Lp ) ;
        {P, alta, NameFile, WId}   -> Lp = [{NameFile,WId}|L],
                                   P ! ok,
                                   loop( Lp ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( globalfilesserver, spawn(?MODULE,loop,[[]]) ).

archivosActuales() -> globalfilesserver ! {self(), archivosActuales }, 
                       receive X -> X end.
getOwner(NameFile)  -> globalfilesserver ! {self(), getOwner, NameFile }, 
                       receive X -> X end.
baja(NameFile) ->  globalfilesserver ! {self(), baja, NameFile }, 
                   receive X -> X end.
alta(NameFile,WId) ->  globalfilesserver ! {self(), alta, NameFile, WId }, 
                       receive X -> X end.


