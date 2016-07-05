-module(globalfiles).
-compile(export_all).

% Funciones:
% archivosActuales () -> [String] (Lista los archivos que actualmente es la base de datos global del worker)
% getOwner : String -> noOwner | WorkerId (Dado un archivo, si existe devuelve su dueño)
% baja : String -> ok (Da de baja un archivo en la tabla)
% alta : String, WorkerId -> ok (Da de alta un archivo en la tabla, asignandole un dueño)


loop( L ) ->
    io:format("globalfiles: ~p~n",[L]), %DEBUG
    receive
        {P, archivosActuales}  -> { Archivos , _ } = lists:unzip( L ),
                                  P ! { globalfilesserverResponse, Archivos },
                                  loop( L ) ;
        {P, getOwner, NameFile} ->
                                   case lists:keyfind(NameFile,1,L) of
                                        false -> P ! { globalfilesserverResponse, noOwner} ;
                                        WId   -> P ! { globalfilesserverResponse, element(2,WId)}
                                   end,
                                   loop( L ) ;
        {P, baja, NameFile}     -> Lp = lists:keydelete(NameFile,1,L),
                                   P ! { globalfilesserverResponse, ok},
                                   loop( Lp ) ;
        {P, alta, NameFile, WId}   -> Lp = [{NameFile,WId}|L],
                                   P ! { globalfilesserverResponse, ok},
                                   loop( Lp ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( globalfilesserver, spawn(?MODULE,loop,[[]]) ).

archivosActuales() -> globalfilesserver ! {self(), archivosActuales }, 
                       receive { globalfilesserverResponse, X} -> X end.
getOwner(NameFile)  -> globalfilesserver ! {self(), getOwner, NameFile }, 
                       receive { globalfilesserverResponse, X} -> X end.
baja(NameFile) ->  globalfilesserver ! {self(), baja, NameFile }, 
                       receive { globalfilesserverResponse, X} -> X end.
alta(NameFile,WId) ->  globalfilesserver ! {self(), alta, NameFile, WId }, 
                       receive { globalfilesserverResponse, X} -> X end.


