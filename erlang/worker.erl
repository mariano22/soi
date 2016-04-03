-module(worker).
-compile(export_all).


loop() -> 
    receive
        Task      -> proc(task:name(Task),Task)
        after 500 -> ok
    end,
    loop().
    

% OrdenName == userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, wrt, rea, userClose, userBye
% OrdenWorkerName == workerDelete, workerOpenRead, wWrite, wRead, wOpW, workerOpenRead, wSay, opsucc, workerClose, workerCloseSucc

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
    Idg = ids:globalFdToIdg(Gfd),
    Fd  = ids:globalFdToFd(Gfd),
    case fdmanage:getOwner(Fd)==Idg of
         false -> comunic:responderClienteRemoto(Idg, mensaje:permisoDenegado());
         true  -> fdManage:unregisterFd(Fd),
                  F     = task:fileName(Task),
                  localfiles:close(F),
                  %C     = ids:globalIdToClient(Idg),
                  Orden = task:crear_workerCloseSucc(Gfd, Idg),%crearWClo( gfd, makeIdGlobal( myId() , cliente(o) ) )
                  W     = ids:globalIdToWorker(Idg),
                  comunic:enviarWorker(W,Orden)
    end,
    ok;

proc( workerCloseSucc, Task )->
    Idg   = task:globalFdToIdg(Task),
    C     = globalIdToClient(Idg,)
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
    ok.


%actuarwOpr(o : Orden, name : String, idg : IdGlobal){
%        case myFiles(name)
%             NoFile  -> ResponderWorker(idg, archivoNoExiste() )
%             Writing -> ResponderWorker(idg, archivoOcupado() )
%             _       -> Fd = OpenRead(name,idg)
%                        Orden = Crearopsucc( ClienteId(idg),  GLOBALFD )
%                        EnviaralWorker( WorkerId(idg), Orden )
%}

proc (workerOpenRead, Task)->
    Name = task:fileName(Task),
    IdG  = task:idGlobal(Task),
    case localfiles:workerOpenRead(Name) of
         NoFile  -> comunic:responderClienteRemoto(Idg, mensaje:archivoNoExiste());
         Writing -> comunic:responderClienteRemoto(Idg, mensaje:archivoOcupado());
         _       -> Gfd = task:fdGlobal(Task),
                    C   = globalIdToClient(IdG),
                    W   = globalIdToWorker(IdG),
                    openerfiles:registerOpen(Gfd, C)
                    Orden = task:crear_workerOpenSucc(Gfd, C),
                    comunic:enviarWorker(W,Orden)




