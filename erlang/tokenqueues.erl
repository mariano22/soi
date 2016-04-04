-module(tokenqueues).
-compile(export_all).

% Funciones:
% newCreate : String, ClientId -> ok (agrega a la lista de candidatos a crearse, el cliente es para saber a quien responder)
% newDelete : String -> ok (agrega a la lista de bajas a informarse)
% getCreates : () -> [{String,WiD}] (extrae la lista de creaciones candidatas)
% getDeletes : () -> [String] (extrae la lista de bajas a informarse)



loop( LC, LD ) ->
    receive
        {P, getCreates}  ->  P ! LC,
                                  loop( [], LD ) ;
        {P, getDeletes}  ->  P ! LD,
                                  loop( LC, [] ) ;
        {P, newCreate, NameFile, CId}   -> LCp = [{NameFile,CId}|LC],
                                   P ! ok,
                                   loop( LCp, LD ) ;
        {P, newDelete, NameFile}   -> LDp = [NameFile|LD],
                                   P ! ok,
                                   loop( LC, LDp ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( tokenqueuesserver, spawn(?MODULE,loop,[[],[]]) ).

newCreate(NameFile,CId) -> tokenqueuesserver ! {self(), newCreate, NameFile, CId }, 
                       receive X -> X end.
newDelete(NameFile) -> tokenqueuesserver ! {self(), newDelete, NameFile }, 
                       receive X -> X end.
getCreates() -> tokenqueuesserver ! {self(), getCreates }, 
                       receive X -> X end.
getDeletes() -> tokenqueuesserver ! {self(), getDeletes }, 
                       receive X -> X end.



