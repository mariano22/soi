%%%%%%%%%%%%%%%%%%%%%%%%
%  Modulo worker_inbox %
%%%%%%%%%%%%%%%%%%%%%%%%

-module(comunic).
-compile(export_all).
-import(parser,[parser/1]).
-import(sockaux,[sockaux_gets/1]).

test() ->
    register(mainWorker,self()),
    setUp(9001,8001),
    testloop().
testloop() -> 
    receive X -> io:format(task:toString(X)++"~n~n") end,
    testloop().

% Comienza a escuchar en el puerto que le corresponda al worker.
setUp(InternPort,ExternPort) ->
		{ok,ExternListenSock} = gen_tcp:listen(ExternPort, [list, {active,false}]),
		{ok,InternListenSock} = gen_tcp:listen(InternPort, [list, {active,false}]),
		spawn(?MODULE,externInbox,[ExternListenSock,0]),
		spawn(?MODULE,internInbox,[InternListenSock]),
		ok.

% Escucha conexiones entrantes y spawnea procesos esclavos que se encargan de él.

externInbox(ListenSock,IdCon) ->
    {ok,Socket} = gen_tcp:accept(ListenSock),
	spawn(?MODULE,externInboxSlave,[Socket,IdCon]),
	externInbox(ListenSock,IdCon+1).

internInbox(ListenSock) ->
    {ok,Socket} = gen_tcp:accept(ListenSock),
	spawn(?MODULE,internInboxSlave,[Socket]),
	internInbox(ListenSock).

% Si Type es extern, se está comunicando con el cliente, caso en el cual la comunicación se mantiene, reciviendo comandos y retransmitiendo la respuesta del worker al cliente hasta que éste último decida despedirse.
% Si Type es intern, se está comunicando con otro worker, caso en el cual se retransmite el mensaje al worker, sin esperarse respuesta alguna.
% Agrega el encabezado PS o WRK dependiendo si el mensaje viene de una fuente externa o de otro worker respectivamente.
externInboxSlave(Socket,IdCon) ->
    localconections:newC(IdCon,self()),
	Data = sockaux:gets(Socket),
    if Data==error -> gen_tcp:close(Socket), localconections:delC(IdCon);
    true -> ParsedData = parser(Data),
             Task = task:fromString(ParsedData),
             mainWorker ! Task,
             receive X -> ok end,
             gen_tcp:send(Socket,X),
             case task:name(Task) of userBye -> gen_tcp:close(Socket), localconections:delC(IdCon);
                  _ -> externInboxSlave(Socket,IdCon)
             end
    end.

internInboxSlave(Socket) ->
	Data = sockaux:gets(Socket),
    if Data==error -> gen_tcp:close(Socket);
    true -> ParsedData = parser(Data),
             Task = task:fromString(ParsedData),
             mainWorker ! Task,
             gen_tcp:close(Socket)
    end.

responderCliente(Cid, M) ->
    case localconections:find(Cid) of
        noClient -> error("Cliente no encontrado!");
        P -> P ! mensaje:say(M)
    end.

% Se encarga de mandar un mensaje a un Worker determinado
%worker_send(WorkerID, SendData, WorkerInfo) ->
	%io:format("Buscando al worker: ~p~n~p~n",[WorkerID,WorkerInfo]), DEBUG
%	WorkerIP = worker_info:worker_ip(WorkerInfo,WorkerID),
	%io:format("Worker encontrado: ~p ~p~n", [WorkerIP,WorkerID+?WORKER_START_INTERN_PORT]), DEBUG
%	{ok, Socket} = gen_tcp:connect(WorkerIP,WorkerID+?WORKER_START_INTERN_PORT,[list, {active,false}]),
	%io:format("Conexion satisfactoria, se enviara ~p~n",[SendData]), DEBUG
%	gen_tcp:send(Socket,SendData),
%	gen_tcp:close(Socket),
%	ok.

responderClienteRemoto(_, _) -> error("responderClienteRemoto").

enviarWorker( _ , _ )->error ("No definido enviarWorker~n").
