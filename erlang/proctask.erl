-module(proctask).
-compile(export_all).


% OrdenName == userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, userWrite, userRead, userClose, userBye, userCon
% OrdenWorkerName == workerDelete, workerOpenRead, workerWrite, workerRead, workerOpenWrite, workerSay, workerOpenSucc, workerClose, workerCloseBye, workerCloseSucc, workerToken

%getOwner( s: String ) = noowner | WorkerId
%myFiles (file: String) = s : NoFile | Unused | Reading | Writing

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%CON
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%user%%%%%%%%%%%%%%%%%%%%%%%%%%%
proc( userCon , Task ) ->
    C = task:cliente(Task),
    comunic:responderCliente( C, mensaje:coneccionEstablecida(C) ),
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%LSD
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%user%%%%%%%%%%%%%%%%%%%%%%%%%%%
proc( userLsd , Task ) ->
    Mensaje = lists:foldl( fun(C,L) ->mensaje:addArg(C,L) end, mensaje:mOk(),  globalfiles:archivosActuales() ),
    comunic:responderCliente( task:cliente(Task), Mensaje ),
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Create
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc( userCreate , Task ) ->
    Name = task:fileName(Task),
    C    = task:cliente(Task),
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
    C    = task:cliente(Task),
    case globalfiles:getOwner(Name) of
         noOwner -> comunic:responderCliente(C, mensaje:archivoNoExiste());
         W       -> Orden = task:crear_workerOpenWrite(Name, ids:makeIdGlobal(ids:myId(),C)),
                    comunic:enviarWorker(W,Orden)
    end,
    ok;

proc (workerOpenWrite, Task)->
    Name = task:fileName(Task),
    IdG  = task:idGlobal(Task),
    case localfiles:status(Name) of
         noFile  -> responderClienteRemoto(IdG, mensaje:archivoNoExiste());
         unused  -> Handle  = realfs:openw(Name),
                    LocalFd = fdmanage:registerFd(IdG,Handle,Name),
                    localfiles:openW(Name),
                    Gfd   = ids:makeGlobalFd(LocalFd, ids:myId()),
                    C     = ids:globalIdToClient(IdG),
                    W     = ids:globalIdToWorker(IdG),
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
    C    = task:cliente(Task),
    case globalfiles:getOwner(Name) of
         noOwner -> comunic:responderCliente(C, mensaje:archivoNoExiste());
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
                    LocalFd = fdmanage:registerFd(IdG,Handle,Name),
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
    Gfd = task:fdGlobal(Task),
    C   = task:cliente(Task),
    openedfiles:registerOpen(Gfd,C),
    comunic:responderCliente(C, mensaje:archivoAbierto(Gfd)),
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
    case lists:member( Gfd, openedfiles:globalFdList(C) ) of
        false -> comunic:responderCliente(C, mensaje:permisoDenegado());
        true ->     Sz  = task:sizeTxt(Task),
                    Txt = lists:sublist(task:strTxt(Task), Sz),%trunca la cadena pasada a la longitud idicada
                    W   = ids:globalFdToWorker(Gfd),
                    Orden = task:crear_workerWrite(Txt,Gfd, IdG),
                    comunic:enviarWorker(W,Orden)
    end,
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
    case lists:member( Gfd, openedfiles:globalFdList(C) ) of
        false -> comunic:responderCliente(C, mensaje:permisoDenegado());
        true -> W   = ids:globalFdToWorker(Gfd),
                Orden = task:crear_workerRead(Sz, Gfd, IdG),
                comunic:enviarWorker(W,Orden)
    end,
    ok;

proc(workerRead, Task)->
    Gfd = task:fdGlobal(Task),
    IdG = task:idGlobal(Task),
    Fd  = ids:globalFdToLocalFd(Gfd),
    case fdmanage:getOwner(Fd)==IdG of
         false -> responderClienteRemoto( IdG, mensaje:permisoDenegado());
         true  -> Handle = fdmanage:getHandle(Fd),
                  Sz     = task:sizeTxt(Task),
                  case realfs:read(Handle,Sz) of
                        eof -> responderClienteRemoto( IdG, mensaje:finDeArchivo());
                        Txt -> responderClienteRemoto( IdG, mensaje:archivoReadSucc(Txt))
                  end
    end,
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Close
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc( userClose, Task)->
    Gfd   = task:fdGlobal(Task),
    C     = task:cliente(Task),
    case lists:member( Gfd, openedfiles:globalFdList(C) ) of
        false -> comunic:responderCliente(C, mensaje:permisoDenegado());
        true -> Orden = task:crear_workerClose(Gfd, ids:makeIdGlobal(ids:myId(),C)),
                W     = ids:globalFdToWorker(Gfd),
                comunic:enviarWorker(W,Orden)
    end,
    ok;

proc( workerClose, Task ) ->
    Gfd = task:fdGlobal(Task),
    Idg = task:idGlobal(Task),
    Fd  = ids:globalFdToLocalFd(Gfd),
    case fdmanage:getOwner(Fd)==Idg of
         false -> responderClienteRemoto(Idg, mensaje:permisoDenegado());
         true  -> F     = fdmanage:getNameFile(Fd),
                  fdmanage:unregisterFd(Fd),
                  io:format("dbg: ~p ~p~n",[Fd,F]), % DEBUG
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
    case globalfiles:getOwner(Name) of
         noOwner -> comunic:responderCliente( C , mensaje:archivoNoExiste() ) ;
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
                   localfiles:delete(Name),
                   realfs:delete(Name);
         _      -> responderClienteRemoto(Idg, mensaje:archivoOcupado())
    end,
    ok;

%---------------------------------------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Bye
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proc ( userBye, Task )->
    C        = task:cliente(Task),
    GlobalFD = openedfiles:globalFdList(C),
    comunic:responderCliente(C,mensaje:mOk()),
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
    F   = fdmanage:getNameFile(Fd),
    fdmanage:unregisterFd(Fd),
    localfiles:close(F),
    openedfiles:registerClose(Gfd),
    ok;

%---------------------------------------------------------------------------------------------------------------


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Token
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
proc(workerToken, Task) ->
    Token = task:token(Task),
    tokencontrol:recvT(Token),
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

    




