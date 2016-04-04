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

crear_workerDelete(Name, Idg)-> { workerDelete, [{fileName,Name},{idGlobal,Idg}] }.

crear_workerOpenRead(Name, Idg)-> { workerOpenRead, [{fileName,Name},{idGlobal,Idg}] }.

crear_workerOpenWrite(Name, Idg) -> { workerOpenWrite, [{fileName,Name},{idGlobal,Idg}] }.

crear_workerClose(Gfd, Idg) -> { workerClose, [{idGlobal,Idg},{fdGlobal,Gfd}] }.

crear_workerCloseSucc(W, C) ->  { workerCloseSucc, [] }.


toData(X) -> term_to_binary(X).
fromData(X) -> binary_to_term(X).
% OrdenName == userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, wrt, rea, userClose, userBye
fromUserData(List,IdCon) ->
    case List of
        ["CON"] -> { userCon, [{cliente,IdCon}] };
        ["LSD"] -> { userLsd, [{cliente,IdCon}] };
        ["DEL", NameFile] -> {userDelete, [{cliente,IdCon}, {fileName,NameFile}]};
        ["CRE", NameFile] -> {userCreate, [{cliente,IdCon}, {fileName,NameFile}]};
        ["OPNR", NameFile] -> {userOpenRead, [{cliente,IdCon}, {fileName,NameFile}]};
        ["OPNW", NameFile] -> {userOpenWrite, [{cliente,IdCon}, {fileName,NameFile}]};
        ["WRT", "FD", GFd, "SIZE", Size, Text] -> {userWrite, [{cliente,IdCon}, {fdGlobal,list_to_integer(GFd)}, {sizeTxt,list_to_integer(Size)}, {strTxt,Text} ]};
        ["REA", "FD", GFd, "SIZE", Size] -> {userWrite, [{cliente,IdCon}, {fdGlobal,list_to_integer(GFd)} , {sizeTxt,list_to_integer(Size)} ]};
        ["CLO", "FD", GFd] -> {userClose, [{cliente,IdCon}, {fdGlobal,list_to_integer(GFd)}]};
        ["BYE"] -> {userBye, [{cliente,IdCon}]};
        _       -> error("Esto no debe suceder")
	end.
