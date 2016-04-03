-module(task).
-compile(export_all).


% Tasks
% userTasks [userTask, clientid, ...]
% workerTasks [workerTask, ...]

cliente( [userTask | [C|_] ] ) -> C;
cliente( _ ) -> error("acceso a cliente a algo inadecuado~n").


