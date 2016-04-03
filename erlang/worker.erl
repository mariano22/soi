-module(worker).
-compile(export_all).


loop() -> 
    receive
        Task      -> proc(task:name(Task),Task)
        after 500 -> ok
    end,
    loop().
    

% OrdenName == userLsd, userDelete, userCreate, opr, opw, wrt, rea, clo, bye
% OrdenWorkerName == workerDelete, wOpr, wWrite, wRead, wOpW, wOpR, wSay, opsucc


proc( userLsd , Task ) ->
    mensaje = lists:foldl( fun(C,L) ->mensaje:addArg(C,L) end, mensaje:mOk(),  globalfiles:archivosActuales() ),
    comunic:responderCliente( task:cliente(Task), mensaje ),
    ok;
 
proc( userDelete , Task ) ->
    Name = task:fileName(Task),
    C = task:cliente(Task),
    case globalfiles:getOwner(name) of
         noOwner -> comunic:responderCliente( c , mensaje:archivoNoExiste() ) ;
         W       -> Orden = task:crear_workerDelete(Name, ids:makeIdGlobal(ids:myId(),C)),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;


proc( workerDelete , Task ) ->
    Name = task:fileName(Task),
    Idg  = task:idGlobal(Task),
    case localfiles:status(Name) of
        noFile -> comunic:responderClienteRemoto(Idg, mensaje:archivoNoExiste());
        unused -> tokenQueues:newDelete(Name),
                  comunic:responderClienteRemoto(Idg, mensaje:archivoBorrado()),
                  localfiles:delete(Name);
        _      -> comunic:responderClienteRemoto(Idg, mensaje:archivoOcupado())
    end,
    ok;


proc( userCreate , Task ) ->
    Name = task:fileName(Task),
    C  = task:cliente(Task),
    case globalfiles:getOwner(Name) of
        noOwner -> tokenQueues:newCreate(Name,C);
        _      -> comunic:responderClienteRemoto(C, mensaje:archivoYaExistente())
    end,
    ok.
        







