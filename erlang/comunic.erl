%%%%%%%%%%%%%%%%%%%%%%%%
%  Modulo comunic      %
%%%%%%%%%%%%%%%%%%%%%%%%

-module(comunic).
-compile(export_all).
%-export([setUp/2, responderCliente/2, enviarWorker/2]).
-import(parser,[parse/1]).
-import(sockaux,[sockaux_gets/1]).

% Comienza a escuchar en el puerto que le corresponda al worker.
setUp(InternPort,ExternPort) ->
        io:format("Puertos ~p  ~p~n~n",[InternPort,ExternPort]),
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

externInboxSlave(Socket,IdCon) ->
	Data = sockaux:gets(Socket),
    if Data==error ->
                Task = {userBye, [{cliente,IdCon}]},
                mainWorker ! Task,
                receive _ -> ok end,
                gen_tcp:close(Socket), localconections:delC(IdCon);
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
	{ok,Data} = gen_tcp:recv(Socket,0),
    if Data==error -> gen_tcp:close(Socket);
    true ->
             Task = task:fromData(Data),
             mainWorker ! Task,
             gen_tcp:close(Socket)
    end.

responderCliente(Cid, M) ->
    case localconections:find(Cid) of
        noClient -> error("Cliente no encontrado!");
        P -> % io:format("Mensaje: ~p~n",[mensaje:say(M)]), %DEBUG
             P ! mensaje:say(M)
    end.


enviarTask(WorkerIP,WorkerPort,Task) ->
    % io:format("Enviando a Worker: ~p ~p ~p~n",[WorkerIP,WorkerPort,Task]), %DEBUG
	{ok, Socket} = gen_tcp:connect(WorkerIP,WorkerPort,[binary,{packet,4}, {active,false}]),
    SendData = task:toData(Task),
    % io:format("~p~n",[SendData]), %DEBUG
	gen_tcp:send(Socket,SendData),
	gen_tcp:close(Socket),
    ok.

% Se encarga de mandar un mensaje a un Worker determinado
enviarWorker( WId , Task ) -> 
    WIP = workerdirs:ip(WId),
    WPort = workerdirs:internPort(WId),
    enviarTask(WIP,WPort,Task).



