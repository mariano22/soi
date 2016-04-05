-module(proctask).
-compile(export_all).


loop() -> 
    receive
        Task      -> proc(task:name(Task),Task)
        after 500 -> ok
    end,
    loop().
    

% OrdenName == userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, userWrite, userRead, userClose, userBye
% OrdenWorkerName == workerDelete, workerOpenRead, workerWrite, workerRead, workerOpenWrite, workerSay, workerOpenSucc, workerClose, workerCloseBye, workerCloseSucc, workerToken

%getOwner( s: String ) = noowner | WorkerId
%myFiles (file: String) = s : NoFile | Unused | Reading | Writing

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%LSD
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
proc( userLsd , Task ) ->
    mensaje = lists:foldl( fun(C,L) ->mensaje:addArg(C,L) end, mensaje:mOk(),  globalfiles:archivosActuales() ),
    comunic:responderCliente( task:cliente(Task), mensaje ),
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Create
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc( userCreate , Task ) ->
    Name = task:fileName(Task),
    C  = task:cliente(Task),
    case globalfiles:getOwner(Name) of
         noOwner -> tokenqueues:newCreate(Name,C);
         _       -> responderClienteRemoto(C, mensaje:archivoExistente())
    end,
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Open
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Write
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc( userOpenWrite, Task)->
    Name = task:fileName(Task),
    Idg  = task:idGlobal(Task),
    C    = task:cliente(Task),
    case globalfiles:getOwner(Name) of
         noOwner -> responderClienteRemoto(Idg, mensaje:archivoNoExiste());
         W       -> Orden = task:crear_workerOpenWrite(Name, ids:makeIdGlobal(ids:myId(),C)),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;

proc (workerOpenWrite, Task)->
    Name = task:fileName(Task),
    IdG  = task:idGlobal(Task),
    case localfiles:status(Name) of
         noFile  -> responderClienteRemoto(IdG, mensaje:archivoNoExiste());
         unused  -> Handle  = realfs:openw(Name),% openr dado un Name te devuelve un handle
                    LocalFd = fdmanage:registerFd(IdG,Handle),
                    localfiles:openW(Name),
                    Gfd = ids:makeGlobalFd(LocalFd, ids:myId()),
                    C   = ids:globalIdToClient(IdG),
                    W   = ids:globalIdToWorker(IdG),
                    Orden = task:crear_workerOpenSucc(Gfd, C),
                    comunic:enviarWorker(W,Orden);
         _       -> responderClienteRemoto(IdG, mensaje:archivoOcupado())
    end,
    ok;



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Read
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc( userOpenRead, Task)->
    Name = task:fileName(Task),
    Idg  = task:idGlobal(Task),
    C    = task:cliente(Task),
    case globalfiles:getOwner(Name) of
         noOwner -> responderClienteRemoto(Idg, mensaje:archivoNoExiste());
         W       -> Orden = task:crear_workerOpenRead(Name, ids:makeIdGlobal(ids:myId(),C)),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;


proc (workerOpenRead, Task)->
    Name = task:fileName(Task),
    IdG  = task:idGlobal(Task),
    case localfiles:status(Name) of
         noFile  -> responderClienteRemoto(IdG, mensaje:archivoNoExiste());
         writing -> responderClienteRemoto(IdG, mensaje:archivoOcupado());
         _       -> Handle  = realfs:openr(Name),
                    LocalFd = fdmanage:registerFd(IdG,Handle),
                    localfiles:openR(Name),
                    Gfd = ids:makeGlobalFd(LocalFd, ids:myId()),
                    C   = ids:globalIdToClient(IdG),
                    W   = ids:globalIdToWorker(IdG),
                    Orden = task:crear_workerOpenSucc(Gfd, C),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%OpenSucc
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


proc(workerOpenSucc, Task)->
    Gfd = ids:fdGlobal(Task),
    C   = ids:cliente(Task),
    openedfiles:registerOpen(Gfd,C),
    W   = ids:globalFdToWorker(Gfd),
    IdG = ids:makeIdGlobal(W,C),
    responderClienteRemoto(IdG, mensaje:archivoOcupado()),
    ok;


%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Write & Read
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Write
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc(userWrite, Task)->
    Gfd = task:fdGlobal(Task),
    C   = task:cliente(Task),
    IdG = ids:makeIdGlobal(ids:myId(),C),
    W   = ids:globalFdToWorker(Gfd),
    Orden = task:crear_workerWrite(Gfd, IdG),
    responderClienteRemoto(W,Orden),
    ok;

proc(workerWrite, Task)->
    Gfd = task:fdGlobal(Task),
    IdG = task:idGlobal(Task),
    Fd  = ids:globalFdToLocalFd(Gfd),
    case fdmanage:getOwner(Fd)==IdG of
         false -> responderClienteRemoto( IdG, mensaje:permisoDenegado());
         true  -> Handle = fdmanage:getHandle(Fd),
                  Data   = task:strTxt(Task),
                  realfs:write ( Handle, Data),
                  responderClienteRemoto( IdG, mensaje:archivoWriteSucc())
    end,
    ok;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Read
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc(userRead, Task)->
    Gfd = task:fdGlobal(Task),
    C   = task:cliente(Task),
    Sz  = task:sizeTxt(Task),
    IdG = ids:makeIdGlobal(ids:myId(),C),
    W   = ids:globalFdToWorker(Gfd),
    Orden = task:crear_workerRead(Sz, Gfd, IdG),
    responderClienteRemoto(W,Orden),
    ok;

proc(workerRead, Task)->
    Gfd = task:fdGlobal(Task),
    IdG = task:idGlobal(Task),
    Fd  = ids:globalFdToLocalFd(Gfd),
    case fdmanage:getOwner(Fd)==IdG of
         false -> responderClienteRemoto( IdG, mensaje:permisoDenegado());
         true  -> Handle = fdmanage:getHandle(Fd),
                  Sz     = task:sizeTxt(Task),
                  Txt    = realfs:read(Handle,Sz),
                  responderClienteRemoto( IdG, mensaje:archivoReadSucc(Txt))
    end,
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Close
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
         false -> responderClienteRemoto(Idg, mensaje:permisoDenegado());
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

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Delete
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



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
         noFile -> responderClienteRemoto(Idg, mensaje:archivoNoExiste());
         unused -> tokenqueues:newDelete(Name),
                   responderClienteRemoto(Idg, mensaje:archivoBorrado()),
                   localfiles:delete(Name);
         _      -> responderClienteRemoto(Idg, mensaje:archivoOcupado())
    end,
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Bye
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc ( userBye, Task )->
    C        = task:cliente(Task),
    GlobalFD = openedfile:globalFdList(C),
    Fun = fun(Gfd) -> 
             Orden = task:crear_workerCloseBye(Gfd),
             W     = ids:globalFdToWorker(Gfd),
             comunic:enviarWorker(W,Orden),
             ok
          end,
    lists:foreach(Fun,GlobalFD),
    ok;

proc( workerCloseBye, Task ) ->
    Gfd = task:fdGlobal(Task),
    Fd  = ids:globalFdToLocalFd(Gfd),
    fdManage:unregisterFd(Fd),
    F     = task:fileName(Task),
    localfiles:close(F),
    openedfiles:registerClose(Gfd),
    ok;

%---------------------------------------------------------------------------------------------------------------


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Token
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% newCreate : String, ClientId -> ok (agrega a la lista de candidatos a crearse, el cliente es para saber a quien responder)
% newDelete : String -> ok (agrega a la lista de bajas a informarse)
% getCreates : () -> [{String,WiD}] (extrae la lista de creaciones candidatas)
% getDeletes : () -> [String] (extrae la lista de bajas a informarse)

% tickTime : () -> Int (devuelve el tiempo que tarda el token en ser procesado)
% recvT : Token -> ok (notifica al modulo que se recibio el Token y lo almacena)
% getT : () -> Token (devuelve el Token almacenado)
% mustProc() : () -> true | false (indica si se debe procesar o no el Token)
      %  ListaBajas = getListaBajasToken();
     %   ListaAltas = getListaAltasToken();
    %    Filtrar los que tienen WorkerId == myid() (ambas listas)
   %     Los de la cola de delete ponerlos en ListaBajas
  %      Los de mi cola de create que estene en ListaAltras contestar que no se puede
 %       Los de mi cola de create que NO estene en ListaAltras contestar que se pudo crear (antes modificar estado mi BD local)  y Agregar a lista de altas
%
 %       Modificar estado global con las listas Alta/Baja
%        pasar el token

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
proc(workerToken, Task) ->
    Token = task:token(Task),
    task:recvT(Token),
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Say
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc(workerSay, Task)->
    C   = task:cliente(Task),
    Msj = task:mensaje(Task),
    comunic:responderCliente(C,Msj),    
    ok.

responderClienteRemoto( Idg, Msj) -> 
    W = ids:globalIdToWorker(Idg),
    C = ids:globalIdToClient(Idg),
    T = task:crear_workerSay(C, Msj ),
    comunic:enviarWorker(W, T).

%---------------------------------------------------------------------------------------------------------------

    



