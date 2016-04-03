-module(task).
-compile(export_all).


% Tasks
% userTasks [userTask, clientid, ...]
% workerTasks [workerTask, ...]

fileName(_)->error("fileName").
idGlobal(_)->error("idGLobal").
nameGlobal(_)->error("nameGlobal").

cliente( [userTask | [C|_] ] ) -> C;
cliente( _ ) -> error("acceso a cliente a algo inadecuado~n").

crear_workerDelete(Name, Idg)->error("crear worker Delete").

crear_workerOpenRead(Name, Idg)->error("crear worker Open Read").

crear_workerOpenWrite(Name, Idg)->error("crear worker Open Write").

crear_workerClose(Gfd, Idg)->error("crear worker Close").

crear_workerCloseSucc(W, C)->error("crear worker CloseSucc").
