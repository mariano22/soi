%%%%%%%%%%%%%%%%%%%%%%
%  Modulo dispatcher %
%%%%%%%%%%%%%%%%%%%%%%

% Espera conexiones entrantes de los clientes.
% Cuando un cliente se conecta, crea un proceso que atienda las peticiones de dicho cliente.
% Las peticiones de dicho cliente son procesadas (parseadas y validadas) antes de ser enviadas al Worker remoto elegido. Luego se envía la respuesta del Worker al cliente.

-module(dispatcher).
-compile(export_all).
-define(DISPATCHER_LISTEN_PORT,8080).

% Lee la lista de Workers.
% Spawnea el proceso encargado de designar a los Workers.
% Abre la conexión e inicia el loop para aceptar conexiones entrantes

init() -> setUp(?DISPATCHER_LISTEN_PORT).

setUp(ListenPort) ->
	{ok,ListenSock} = gen_tcp:listen(ListenPort, [list, {active,false}]),
	spawn(?MODULE,accept_loop,[ListenSock]),
    ok.

% Espera una conexión entrante y crea un proceso que ejecute socket_process_start que se ocupe de dicho cliente
accept_loop(ListenSock) ->
	{ok,ClientSocket} = gen_tcp:accept(ListenSock),
	spawn(?MODULE,socket_process_start,[ClientSocket]),
	accept_loop(ListenSock).

% Le pide a pid_worker_list un Worker (IP y Puerto del Worker) para conectarse.
% Se conecta a dicho Worker y se comporta según socket_process_loop
socket_process_start(ClientSocket) ->
    WId = random:uniform( workerdirs:wlen() )-1, %MODIFICAR
    io:format("Conectado a:~p ~p~n",[WId,workerdirs:wlen()]),
%    WId = 0,
    WorkerPort = workerdirs:externPort(WId),
    WorkerIP = workerdirs:ip(WId),
    {ok, WorkerSocket} = gen_tcp:connect(WorkerIP,WorkerPort,[list, {active,false}]),
    socket_process_loop(ClientSocket,WorkerSocket).

% Recibe datos del Cliente, valida los mismos y se los envia al Worker.
% Espera recibir la respuesta del Worker, que luego reenvia al cliente.
socket_process_loop(ClientSocket,WorkerSocket) ->
	%io:format("Waitting for data"), %DEBUG
	Data = sockaux:gets(ClientSocket),
	%io:format("Data: ~p~n",[Data]), %DEBUG
	if (Data == error) -> gen_tcp:close(ClientSocket), gen_tcp:close(WorkerSocket);
	true ->
		Parsed_Data = parser:parse(Data),
		case protocol_validation(Parsed_Data) of
			ok	    ->  
			        %io:format("Sending ~p~n",[Parsed_Data]), %DEBUG
			        gen_tcp:send(WorkerSocket,lists:foldl(fun(X,Acc) -> Acc ++ " " ++ X end,"",Parsed_Data) ++ "\n"),
			        %io:format("Succesfully send~n"), %DEBUG
					WorkerAnswer = sockaux:gets(WorkerSocket),
					%io:format("Answer! ~p~n", [WorkerAnswer]), %DEBUG
					gen_tcp:send(ClientSocket,WorkerAnswer ++ "\n\0"),
					socket_process_loop(ClientSocket,WorkerSocket);
			error   ->  gen_tcp:send(ClientSocket,"Invalid command\n"),
					    socket_process_loop(ClientSocket,WorkerSocket);
			exit_signal ->  gen_tcp:send(ClientSocket,"OK\0"),
							gen_tcp:send(WorkerSocket,"BYE"),
							gen_tcp:close(ClientSocket), gen_tcp:close(WorkerSocket)
		end
	end.

protocol_validation(List) -> case List of
	["CON"] -> ok;
	["LSD"] -> ok;
	["DEL", _] -> ok;
	["CRE", _] -> ok;
	["OPNR", _] -> ok;
	["OPNW", _] -> ok;
	["WRT", "FD", _, "SIZE", _, _] -> ok;
	["REA", "FD", _, "SIZE", _] -> ok;
	["CLO", "FD", _] -> ok;
	["BYE"] -> exit_signal;
	_       -> error
	end.
