-module(task).
-compile(export_all).
-import(parser,[parser/1]).

% Tasks
% Los modelamos como { TypeTask, [{campo,contenido}] }

name({TT, _}) -> TT.
fileName({_, L}) -> element(2,lists:keyfind(fileName,1,L)).
idGlobal({_, L}) -> element(2,lists:keyfind(idGlobal,1,L)).
fdGlobal({_, L}) -> element(2,lists:keyfind(fdGlobal,1,L)).
cliente({_, L})  -> element(2,lists:keyfind(cliente,1,L)).
sizeTxt({_, L})  -> element(2,lists:keyfind(sizeTxt,1,L)).
strTxt({_, L})   -> element(2,lists:keyfind(strTxt,1,L)).
mensaje({_, L})  -> element(2,lists:keyfind(mensaje,1,L)).
token({_, L})    -> element(2,lists:keyfind(token,1,L)).

crear_workerDelete(Name, IdG)      -> { workerDelete, [{fileName,Name},{idGlobal,IdG}] }.

crear_workerOpenRead(Name, IdG)    -> { workerOpenRead, [{fileName,Name},{idGlobal,IdG}] }.

crear_workerOpenWrite(Name, IdG)   -> { workerOpenWrite, [{fileName,Name},{idGlobal,IdG}] }.

crear_workerOpenSucc(Gfd, C)       -> { workerOpenSucc, [{fdGlobal,Gfd},{cliente,C}]}.

crear_workerWrite(Txt,Gfd, IdG)    -> { workerWrite, [{strTxt,Txt},{idGlobal,IdG},{fdGlobal,Gfd}] }.

crear_workerRead(Sz, Gfd, IdG)     -> { workerRead, [{sizeTxt,Sz},{idGlobal,IdG},{fdGlobal,Gfd}] }.

crear_workerClose(Gfd, IdG)        -> { workerClose, [{idGlobal,IdG},{fdGlobal,Gfd}] }.

crear_workerCloseSucc(Gfd, IdG)    -> { workerCloseSucc, [{idGlobal,IdG},{fdGlobal,Gfd}] }.

crear_workerCloseBye(Gfd)          -> { workerCloseBye, [{fdGlobal,Gfd}] }.

crear_workerSay(C, Msj )           -> { workerSay, [{cliente,C},{mensaje,Msj}] }.

crear_workerToken(Token)           -> { workerToken, [{token,Token}] }.

toData(X) -> term_to_binary(X).
fromData(X) -> binary_to_term(X).
fromUserData(List,IdCon) ->
    io:format("~p~p~n",[List,IdCon]),
    case List of
        ["CON"] -> { userCon, [{cliente,IdCon}] };
        ["LSD"] -> { userLsd, [{cliente,IdCon}] };
        ["DEL", NameFile] -> {userDelete, [{cliente,IdCon}, {fileName,NameFile}]};
        ["CRE", NameFile] -> {userCreate, [{cliente,IdCon}, {fileName,NameFile}]};
        ["OPNR", NameFile] -> {userOpenRead, [{cliente,IdCon}, {fileName,NameFile}]};
        ["OPNW", NameFile] -> {userOpenWrite, [{cliente,IdCon}, {fileName,NameFile}]};
        ["WRT", "FD", GFd, "SIZE", Size, Text] -> {userWrite, [{cliente,IdCon}, {fdGlobal,list_to_integer(GFd)}, {sizeTxt,list_to_integer(Size)}, {strTxt,Text} ]};
        ["REA", "FD", GFd, "SIZE", Size] -> {userRead, [{cliente,IdCon}, {fdGlobal,list_to_integer(GFd)} , {sizeTxt,list_to_integer(Size)} ]};
        ["CLO", "FD", GFd] -> {userClose, [{cliente,IdCon}, {fdGlobal,list_to_integer(GFd)}]};
        ["BYE"] -> {userBye, [{cliente,IdCon}]};
        _       -> error("Esto no debe suceder")
	end.
