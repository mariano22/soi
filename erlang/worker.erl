-module(worker).
-compile(export_all).

init(MyId) ->
    register(mainWorker,self()),
    localconections:setUp(),
    localfiles:setUp(),
    comunic:setUp(workerdirs:internPort(MyId),workerdirs:externPort(MyId)),
    fdmanage:setUp(),
    globalfiles:setUp(),
    ids:setUp(MyId),
    openedfiles:setUp(),
    tokencontrol:setUp(),
    tokenqueues:setUp(),
    if MyId==0 -> tokencontrol:recvT(tokencontrol:makeToken([],[])); true -> ok end,
    loop().

loop() -> 
    receive
        Task             -> io:format("Task: ~p~n",[Task]),
                            proctask:proc(task:name(Task),Task)
        after tokencontrol:tickTime() -> ok
    end,
    case tokencontrol:mustProc() of true -> procToken(); _ -> ok end,
    loop().


procToken() ->
    io:format("Empezando Token~n"),
    Token = tokencontrol:getT(),
    ListaAltasOld = tokencontrol:getListaAltas(Token),
    ListaBajasOld = tokencontrol:getListaBajas(Token),
%   Filtrar los que tienen WorkerId == myid() (ambas listas)
    ListaAltas = lists:dropwhile( fun({_,WId}) -> WId==ids:myId() end, ListaAltasOld ),
    ListaBajas = lists:dropwhile( fun({_,WId}) -> WId==ids:myId() end, ListaBajasOld ),
%   Los de la cola de delete ponerlos en ListaBajas
    MyDeletes  = lists:map( fun(X) -> {X,ids:myId()} end, tokenqueues:getDeletes() ),
    ListaBajasNew = ListaBajas ++ MyDeletes,
    MyCreateRequests = tokenqueues:getCreates(),
%   Los de mi cola de create que esten en ListaAltras contestar que no se puede
    MyCreateRequestsOk = lists:dropwhile( fun({NameFile,ClientId}) ->
                                              case lists:keymember(NameFile,1,ListaAltas) of
                                                    true -> comunic:responderCliente(ClientId,mensaje:archivoExistente()),
                                                            true;
                                                    false -> comunic:responderCliente(ClientId,mensaje:mOk()),
                                                             localfiles:create(NameFile),
                                                             realfs:create(NameFile),
                                                             false
                                              end
                                          end,
                         MyCreateRequests ),
%   Los de mi cola de create que NO estene en ListaAltras contestar que se pudo crear (antes modificar estado mi BD local)  y Agregar a lista de altas
    ListaAltasNew = lists:map( fun({NameFile,_}) -> {NameFile, ids:myId() } end, MyCreateRequestsOk ) ++ ListaAltas,
%   Modificar estado global con las listas Alta/Baja
    lists:foreach( fun({NameFile,WorkerId}) -> globalfiles:alta(NameFile,WorkerId) end, ListaAltasNew ),
    lists:foreach( fun({NameFile,_}) -> globalfiles:baja(NameFile) end, ListaBajasNew ),
%        pasar el token    
    NewToken = tokencontrol:makeToken(ListaAltasNew,ListaBajasNew),
    io:format("NewToken: ~p ~p ~p~n",[ListaAltasNew,ListaBajasNew,NewToken]),
    Orden    = task:crear_workerToken(NewToken),
    io:format("NextId ~p~n",[ids:nextWorkerId()]),
    comunic:enviarWorker(ids:nextWorkerId(), Orden),
    ok.
    













