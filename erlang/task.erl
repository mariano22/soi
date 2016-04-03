-module(task).
-compile(export_all).


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
