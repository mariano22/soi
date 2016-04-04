%%%%%%%%%%%%%%%%%%%%%%%%
%  Modulo worker_inbox %
%%%%%%%%%%%%%%%%%%%%%%%%

-module(comunic).
-compile(export_all).
-import(parser,[parse/1]).
-import(sockaux,[sockaux_gets/1]).
-include("worker_list.hrl").

test(ID) ->
    localconections:setUp(),
    register(mainWorker,self()),
    setUp(9000+ID,8000+ID),
    testloop().
testloop() -> 
    receive X -> io:format("~p~n~n",[X]),
                 responderCliente(task:cliente(X),mensaje:mOk())
    end,
    testloop().

% Comienza a escuchar en el puerto que le corresponda al worker.
setUp(InternPort,ExternPort) ->
		{ok,ExternListenSock} = gen_tcp:listen(ExternPort, [list, {active,false}]),
		{ok,InternListenSock} = gen_tcp:listen(InternPort, [binary,{packet,4}, {active,false}]),
		spawn(?MODULE,externInbox,[ExternListenSock,0]),
		spawn(?MODULE,internInbox,[InternListenSock]),
		ok.

% Escucha conexiones entrantes y spawnea procesos esclavos que se encargan de él.

externInbox(ListenSock,IdCon) ->
    {ok,Socket} = gen_tcp:accept(ListenSock),
	PS = spawn(?MODULE,externInboxSlave,[Socket,IdCon]),
    localconections:newC(IdCon,PS),
	externInbox(ListenSock,IdCon+1).

internInbox(ListenSock) ->
    {ok,Socket} = gen_tcp:accept(ListenSock),
	spawn(?MODULE,internInboxSlave,[Socket]),
	internInbox(ListenSock).

% Si Type es extern, se está comunicando con el cliente, caso en el cual la comunicación se mantiene, reciviendo comandos y retransmitiendo la respuesta del worker al cliente hasta que éste último decida despedirse.
% Si Type es intern, se está comunicando con otro worker, caso en el cual se retransmite el mensaje al worker, sin esperarse respuesta alguna.
% Agrega el encabezado PS o WRK dependiendo si el mensaje viene de una fuente externa o de otro worker respectivamente.
externInboxSlave(Socket,IdCon) ->
	Data = sockaux:gets(Socket),
    if Data==error -> gen_tcp:close(Socket), localconections:delC(IdCon);
    true -> ParsedData = parser:parse(Data),
             Task = task:fromUserData(ParsedData,IdCon),
             mainWorker ! Task,
             receive X -> ok end,
             gen_tcp:send(Socket,X),
             case task:name(Task) of userBye -> gen_tcp:close(Socket), localconections:delC(IdCon);
                  _ -> externInboxSlave(Socket,IdCon)
             end
    end.

internInboxSlave(Socket) ->
	{ok,Data} = gen_tcp:recv(Socket,0), % MODIFICAR!
    if Data==error -> gen_tcp:close(Socket);
    true ->  %DEBUG io:format("~p~n",[Data]),
             Task = task:fromData(Data),
             mainWorker ! Task,
             gen_tcp:close(Socket)
    end.

responderCliente(Cid, M) ->
    case localconections:find(Cid) of
        noClient -> error("Cliente no encontrado!");
        P -> %io:format("Mensaje: ~p~n",[mensaje:say(M)]), DEBUG
             P ! mensaje:say(M)
    end.

test2(P) -> enviarTask('127.0.0.1',P,{asd}).

enviarTask(WorkerIP,WorkerPort,Task) ->
    %DEBUGio:format("~p~p~n",[WorkerIP,WorkerPort]),
	{ok, Socket} = gen_tcp:connect(WorkerIP,WorkerPort,[binary,{packet,4}, {active,false}]),
    SendData = task:toData(Task),
    %DEBUG io:format("~p~n",[SendData]),
	gen_tcp:send(Socket,SendData),
	gen_tcp:close(Socket),
    ok.

% Se encarga de mandar un mensaje a un Worker determinado
enviarWorker( WId , Task ) -> 
    {_,WPort,WIP} = lists:keyfind(WId,1,?WORKER_LIST),
    enviarTask(WIP,WPort,Task).



