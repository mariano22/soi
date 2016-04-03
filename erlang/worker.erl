-module(worker).
-compile(export_all).


loop() -> 
    receive
        Task      -> proc(task:name(Task),Task)
        after 500 -> ok
    end,
    loop().
    

% OrdenName == userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, wrt, rea, clo, bye
% OrdenWorkerName == workerDelete, workerOpenRead, wWrite, wRead, wOpW, wOpR, wSay, opsucc, workerClose

%getOwner( s: String ) = noowner | WorkerId
%myFiles (file: String) = s : NoFile | Unused | Reading | Writing

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
         _       -> comunic:responderClienteRemoto(C, mensaje:archivoYaExistente())
    end,
    ok;


proc( userOpenRead, Task)->
    Name = task:fileName(Task),
    Idg  = task:idGlobal(Task),
    C    = task:cliente(Task),
    case globalfiles:getOwner(Name) of
         noOwner -> comunic:responderClienteRemoto(Idg, mensaje:archivoNoExiste());
         W       -> Orden = task:crear_workerOpenRead(Name, ids:makeIdGlobal(ids:myId(),C)),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;


proc( userOpenWrite, Task)->
    Name = task:fileName(Task),
    Idg  = task:idGlobal(Task),
    C    = task:cliente(Task),
    case globalfiles:getOwner(Name) of
         noOwner -> comunic:responderClienteRemoto(Idg, mensaje:archivoNoExiste());
         W       -> Orden = task:crear_workerOpenWrite(Name, ids:makeIdGlobal(ids:myId(),C)),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;


proc( userClose, Task)->
    Gfd   = task:nameGlobal(Task),
    C     = task:cliente(Task),
    Orden = task:crear_workerClose(Gfd, ids:makeIdGlobal(ids:myId(),C)),
    W     = ids:globalFdToWorker(Gfd),
    comunic:enviarWorker(W,Orden),
    ok.

%actuarwClo(o : Orden, gfd : GlobalFd, idg : IdGlobal){
%	fd = GlobalFdToFd(gfd)
%        case check FdToOwner fd == idg \
%             false -> ResponderWorker(idg, errorPermisoDenegado())
%             true  -> borrar de las tablas FdToHandle y FdToOwner las entradas con fd
%                      closestatus(namefromfd(Fd))
%                      Orden = crearCloSucc(myid(),cliente(idg))
%                      EnviarAlWorker(worker(idg), Orden)
%}


proc( workerClose , Task ) ->
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





