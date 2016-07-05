-module(tokenqueues).
-compile(setUp/0, newCreate/2, newDelete/1, getCreates/0, getDeletes/0, isInCreate/1]).

% Funciones:
% newCreate  : String, ClientId -> ok (agrega a la lista de candidatos a crearse, el cliente es para saber a quien responder)
% newDelete  : String -> ok (agrega a la lista de bajas a informarse)
% getCreates : () -> [{String,WiD}] (extrae la lista de creaciones candidatas)
% getDeletes : () -> [String] (extrae la lista de bajas a informarse)
%isInCreate  : string -> bool (true si el archivo existe false cc)


loop( LC, LD ) ->
    io:format("tokenqueues: ~p ~p~n",[LC,LD]), %DEBUG
    receive
        {P, getCreates}  ->  P ! {tokenqueuesserver, LC},
                                  loop( [], LD ) ;
        {P, getDeletes}  ->  P ! {tokenqueuesserver, LD},
                                  loop( LC, [] ) ;
        {P, isInCreate, NameFile}  ->  case lists:keyfind(NameFile,1,LC) of
                                       false -> P ! {tokenqueuesserver, false};
                                       _ -> P ! {tokenqueuesserver, true}
                                    end,
                                  loop( LC, LD ) ;

        {P, newCreate, NameFile, CId}   -> LCp = [{NameFile,CId}|LC],
                                   P ! {tokenqueuesserver, ok},
                                   loop( LCp, LD ) ;
        {P, newDelete, NameFile}   -> LDp = [NameFile|LD],
                                   P ! {tokenqueuesserver, ok},
                                   loop( LC, LDp ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( tokenqueuesserver, spawn(?MODULE,loop,[[],[]]) ).

newCreate(NameFile,CId) -> tokenqueuesserver ! {self(), newCreate, NameFile, CId }, 
                       receive {tokenqueuesserver, X} -> X end.
newDelete(NameFile) -> tokenqueuesserver ! {self(), newDelete, NameFile }, 
                       receive {tokenqueuesserver, X} -> X end.
getCreates() -> tokenqueuesserver ! {self(), getCreates }, 
                       receive {tokenqueuesserver, X} -> X end.
getDeletes() -> tokenqueuesserver ! {self(), getDeletes }, 
                       receive {tokenqueuesserver, X} -> X end.
isInCreate(NameFile) -> tokenqueuesserver ! {self(), isInCreate, NameFile }, 
                       receive {tokenqueuesserver, X} -> X end.


