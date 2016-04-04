-module(task).
-compile(export_all).
-import(parser,[parser/1]).


% Tasks
% Los modelamos como { TypeTask, [{campo,contenido}] }

fileName({_, L}) -> element(2,lists:keyfind(fileName,1,L)).
idGlobal({_, L}) -> element(2,lists:keyfind(idGlobal,1,L)).
fdGlobal({_, L}) -> element(2,lists:keyfind(fdGlobal,1,L)).
cliente({_, L}) -> element(2,lists:keyfind(cliente,1,L)).

crear_workerDelete(Name, Idg)-> { workerDelete, [{fileName,Name},{idGlobal,Idg}] }.

crear_workerOpenRead(Name, Idg)-> { workerOpenRead, [{fileName,Name},{idGlobal,Idg}] }.

crear_workerOpenWrite(Name, Idg) -> { workerOpenWrite, [{fileName,Name},{idGlobal,Idg}] }.

crear_workerClose(Gfd, Idg) -> { workerClose, [{idGlobal,Idg},{fdGlobal,Gfd}] }.

crear_workerCloseSucc(W, C) ->  { workerCloseSucc, [] }.


toData(X) -> term_to_binary(X).
fromData(X) -> binary_to_term(X).
% OrdenName == userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, wrt, rea, userClose, userBye
fromUserData(List) -> {userTask}.
%    case List of
%        ["CON"] -> { userCon, [] };
%        ["LSD"] -> { userLsd, [] };
%        ["DEL", NameFile] -> {userDelete, [{fileName,NameFile}]};
%        ["CRE", NameFile] -> {userCreate, [{fileName,NameFile}]};
%        ["OPNR", NameFile] -> {userOpenRead, [{fileName,NameFile}]};
%        ["OPNW", NameFile] -> {userOpenWrite, [{fileName,NameFile}]};
%        ["WRT", "FD", _, "SIZE", _, _] -> ok;
%        ["REA", "FD", _, "SIZE", _] -> ok;
%        ["CLO", "FD", _] -> ok;
%        ["BYE"] -> exit_signal;
%        _       -> error("Esto no debe suceder")
	%end.
