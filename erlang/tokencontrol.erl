-module(tokencontrol).
-compile(export_all).
%-export([setU/0, tickTime/0, recvT/1, getT/, mustProc/0, makeToken/2, getListaAltas/2, getListaBajas/2]).

% Este es el modulo que encapsula el manejo del token
% Funciones
% tickTime : () -> Int (devuelve el tiempo que tarda el token en ser procesado)
% recvT : Token -> ok (notifica al modulo que se recibio el Token y lo almacena)
% getT : () -> Token (devuelve el Token almacenado)
% mustProc() : () -> true | false (indica si se debe procesar o no el Token)
% makeToken : [ {NameFile, WorkerId} ],  [ {NameFile,WorkerId} ] -> Token
% getListaAltas : Token -> [ {NameFile,WorkerId} ]
% getListaBajas : Token -> [ {NameFile,WorkerId} ]

loop( T, F ) ->

%    io:format("tokencontrol: ~p ~p~n",[T,F]), %DEBUG
    receive
        { P, getT }  ->  P ! {tokencontrolserverResponse, T },
                           loop( nothing , false ) ;
        {_, recvT, TT}   ->  loop( TT, true ) ;
        {P, mustProc}   -> P ! {tokencontrolserverResponse, F},
                                loop(T,F) ;
        _ -> error("esto no deberia suceder 666")
    end.
setUp() -> register( tokencontrolserver, spawn(?MODULE,loop,[nothing,false]) ).
tickTime() -> 10.
recvT_aux(TT) -> 
            receive after tickTime() -> ok end,
            tokencontrolserver ! {self(), recvT, TT }, 
            ok.
recvT(TT) -> spawn(?MODULE,recvT_aux,[TT]), ok.
getT() -> tokencontrolserver ! {self(), getT }, 
                       receive {tokencontrolserverResponse, X} -> X end.
mustProc()-> tokencontrolserver ! {self(), mustProc }, 
                       receive {tokencontrolserverResponse, X} -> X end.

makeToken(LA,LB) -> {LA,LB}.
getListaAltas({LA,_}) -> LA.
getListaBajas({_,LB}) -> LB.
