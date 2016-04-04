-module(worker).
-compile(export_all).


loop() -> 
    receive
        Task      -> proc(task:name(Task),Task)
        after 500 -> ok
    end,
    loop().
    

% OrdenName == userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, userWrite, userRead, userClose, userBye
% OrdenWorkerName == workerDelete, workerOpenRead, wWrite, wRead, workerOpenWrite, workerSay, opsucc, workerClose, workerCloseSucc

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
    Gfd   = task:fdGlobal(Task),
    C     = task:cliente(Task),
    Orden = task:crear_workerClose(Gfd, ids:makeIdGlobal(ids:myId(),C)),
    W     = ids:globalFdToWorker(Gfd),
    comunic:enviarWorker(W,Orden),
    ok;


proc( workerClose, Task ) ->
    Gfd = task:fdGlobal(Task),
    Idg = task:idGlobal(Task),
    Fd  = ids:globalFdToLocalFd(Gfd),
    case fdmanage:getOwner(Fd)==Idg of
         false -> comunic:responderClienteRemoto(Idg, mensaje:permisoDenegado());
         true  -> fdManage:unregisterFd(Fd),
                  F     = task:fileName(Task),
                  localfiles:close(F),
                  Orden = task:crear_workerCloseSucc(Gfd, Idg),
                  W     = ids:globalIdToWorker(Idg),
                  comunic:enviarWorker(W,Orden)
    end,
    ok;

proc( workerCloseSucc, Task )->
    Idg   = task:idGlobal(Task),
    C     = ids:globalIdToClient(Idg),
    Gfd   = task:fdGlobal(Task),
    openedfiles:registerClose(Gfd),
    comunic:responderCliente(C,mensaje:archivoCerrado()),
    ok;


proc ( userBye, Task )->
    C        = task:cliente(Task),
    Idg      = ids:makeIdGlobal(ids:myId(),C),
    GlobalFD = openedfile:globalFdList(C),
    Fun = fun(Gfd) -> 
             Orden = task:crear_workerClose(Gfd, Idg),
             W     = ids:globalFdToWorker(Gfd),
             comunic:enviarWorker(W,Orden),
             ok
          end,
    lists:foreach(Fun,GlobalFD),
    ok;


proc (workerOpenRead, Task)->
    Name = task:fileName(Task),
    IdG  = task:idGlobal(Task),
    case localfiles:status(Name) of
         noFile  -> comunic:responderClienteRemoto(IdG, mensaje:archivoNoExiste());
         writing -> comunic:responderClienteRemoto(IdG, mensaje:archivoOcupado());
         _       -> Handle  = realfs:openr(Name),% openr dado un Name te devuelve un handle
                    LocalFd = fdmanage:registerFd(IdG,Handle),
                    localfiles:openR(Name),
                    Gfd = ids:makeGlobalFd(LocalFd, ids:myId()),
                    C   = ids:globalIdToClient(IdG),
                    W   = ids:globalIdToWorker(IdG),
                    Orden = task:crear_workerOpenSucc(Gfd, C),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;

proc (workerOpenWrite, Task)->
    Name = task:fileName(Task),
    IdG  = task:idGlobal(Task),
    case localfiles:status(Name) of
         noFile  -> comunic:responderClienteRemoto(IdG, mensaje:archivoNoExiste());
         unused  -> Handle  = realfs:openr(Name),% openr dado un Name te devuelve un handle
                    LocalFd = fdmanage:registerFd(IdG,Handle),
                    localfiles:openW(Name),
                    Gfd = ids:makeGlobalFd(LocalFd, ids:myId()),
                    C   = ids:globalIdToClient(IdG),
                    W   = ids:globalIdToWorker(IdG),
                    Orden = task:crear_workerOpenSucc(Gfd, C),
                    comunic:enviarWorker(W,Orden);
         _       -> comunic:responderClienteRemoto(IdG, mensaje:archivoOcupado())
    end,
    ok;

proc(workerOpenSucc, Task)->
    Gfd = ids:fdGlobal(Task),
    C   = ids:cliente(Task),
    openedfiles:registerOpen(Gfd,C),
    W   = ids:globalFdToWorker(Gfd),
    IdG = ids:makeIdGlobal(W,C),
    comunic:responderClienteRemoto(IdG, mensaje:archivoOcupado()),
    ok.

%actuarRead(o : Orden, gfd : GlobalFd, sz : Int) {
%	W=globalFdToWorker gfd
%	idg = makeIdGlobal( myId() , cliente(o) )
%	orden=crearwRead(sz, gfd, idg)
%	enviarAWorker(W,orden)
%}

proc(userRead, Task)->
    Gfd = task:fdGlobal(Task),
    C   = task:cliente(Task),
    IdG = makeIdGlobal(myId(),C)
    W   = ids:globalFdToWorker(Gfd),
    Orden = task:crear_workerWorkerRead(Sz, Gfd, IdG),
    comunic:responderClienteRemoto(W,Orden)



