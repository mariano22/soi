-module(localfiles).
-compile(export_all).
%-export([setUp/0, status/1, delete/1, openR/1, openW/1, create/1, close/1]).

% Funciones
% status : String -> noFile | reading | writing | unused (Dado un archivo, si esta localmente devuelve el estado)
% delete : String -> ok (Dado un archivo lo borra de la tabla de los locales)
% create : String -> ok (Dado un archivo lo crea en la tabla de los locales, inicialmente unused)
% openR : String -> ok (Suma un lector al archivo)
% openW : String -> ok (Suma un escritor al archivo, el llamante se asegura que nunca halla mas de uno)
% close : String -> ok (Resta un usuario al archivo, detectando si esta siendo leido o escrito)


loop( L ) ->
        io:format("localfiles: ~p~n",[L]), % DEBUG
    receive
        {P, status, NameFile} -> case lists:keyfind(NameFile,1,L) of
                                      false -> P ! { localfilesserverResponse, noFile };
                                      T -> if element(2,T)>0 -> P ! { localfilesserverResponse, reading };
                                              element(3,T)>0 -> P ! { localfilesserverResponse, writing };
                                              true           -> P ! { localfilesserverResponse, unused}
                                           end
                                  end,
                                  loop( L ) ;
        {P, delete, NameFile}     -> Lp = lists:keydelete(NameFile,1,L),
                                   P ! { localfilesserverResponse, ok},
                                   loop( Lp ) ;
        {P, openR, NameFile}     -> Lb = lists:keydelete(NameFile,1,L),
                                    T = lists:keyfind(NameFile,1,L),
                                    Lp = [ {element(1,T),element(2,T)+1,element(3,T)} | Lb ],
                                    P ! { localfilesserverResponse, ok},
                                    loop( Lp ) ;
        {P, openW, NameFile}     -> Lb = lists:keydelete(NameFile,1,L),
                                    T = lists:keyfind(NameFile,1,L),
                                    Lp = [ {element(1,T),element(2,T),element(3,T)+1} | Lb ],
                                    P ! { localfilesserverResponse, ok},
                                    loop( Lp ) ;
        {P, close, NameFile }   -> Lb = lists:keydelete(NameFile,1,L),
                                   T = lists:keyfind(NameFile,1,L),
                                   Tp= if element(2,T)>0 -> {element(1,T),element(2,T)-1,element(3,T)};
                                          element(3,T)>0 -> {element(1,T),element(2,T),element(3,T)-1};
                                          true -> error("No deberia suceder") end,
                                   Lp = [ Tp | Lb ],
                                   P ! { localfilesserverResponse, ok},
                                   loop( Lp ) ;
        {P, create, NameFile }   -> Lp = [{NameFile,0,0}|L],
                                   P ! { localfilesserverResponse, ok},
                                   loop( Lp ) ;
        _ -> error("esto no deberia suceder 666") 
    end.

setUp() -> register( localfilesserver, spawn(?MODULE,loop,[[]]) ).

status( NameFile ) -> localfilesserver ! {self(), status , NameFile }, 
                  receive { localfilesserverResponse, X } -> X end.
delete( NameFile ) -> localfilesserver ! {self(), delete , NameFile }, 
                  receive { localfilesserverResponse, X } -> X end.
openR( NameFile ) -> localfilesserver ! {self(), openR , NameFile }, 
                  receive { localfilesserverResponse, X } -> X end.
openW( NameFile ) -> localfilesserver ! {self(), openW , NameFile }, 
                  receive { localfilesserverResponse, X } -> X end.
create( NameFile ) -> localfilesserver ! {self(), create , NameFile }, 
                  receive { localfilesserverResponse, X } -> X end.
close( NameFile ) -> localfilesserver ! {self(), close , NameFile }, 
                  receive { localfilesserverResponse, X } -> X end.


