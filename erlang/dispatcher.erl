%%%%%%%%%%%%%%%%%%%%%%
%  Modulo dispatcher %
%%%%%%%%%%%%%%%%%%%%%%

% Espera conexiones entrantes de los clientes.
% Cuando un cliente se conecta, crea un proceso que atienda las peticiones de dicho cliente.
% Las peticiones de dicho cliente son procesadas (parseadas y validadas) antes de ser enviadas al Worker remoto elegido. Luego se envía la respuesta del Worker al cliente.

-module(dispatcher).
-compile(export_all).
-define(DISPATCHER_LISTEN_PORT,8000).
-include("worker_list.hrl").

% Lee la lista de Workers.
% Spawnea el proceso encargado de designar a los Workers.
% Abre la conexión e inicia el loop para aceptar conexiones entrantes
init() ->
	register(pid_workers_list, spawn(?MODULE,worker_list,[?WORKER_LIST]) ),
	{ok,ListenSock} = gen_tcp:listen(?DISPATCHER_LISTEN_PORT, [list, {active,false}]),
	accept_loop(ListenSock).

% Proceso encargado de asignar worker's de manera aleatoria
worker_list(WorkersList) ->
	receive
		{one_worker_request,Pid} -> Pid ! lists:nth( random:uniform(length(WorkersList)) , WorkersList )
	end,
	worker_list(WorkersList).

% Espera una conexión entrante y crea un proceso que ejecute socket_process_start que se ocupe de dicho cliente
accept_loop(ListenSock) ->
	{ok,ClientSocket} = gen_tcp:accept(ListenSock),
	spawn(?MODULE,socket_process_start,[ClientSocket]),
	accept_loop(ListenSock).



% Le pide a pid_worker_list un Worker (IP y Puerto del Worker) para conectarse.
% Se conecta a dicho Worker y se comporta según socket_process_loop
socket_process_start(ClientSocket) ->
    pid_workers_list ! {one_worker_request,self()},
    receive {_,WorkerPort,WorkerIP} -> ok end,
    {ok, WorkerSocket} = gen_tcp:connect(WorkerIP,WorkerPort,[list, {active,false}]),
    socket_process_loop(ClientSocket,WorkerSocket).

% Recibe datos del Cliente, valida los mismos y se los envia al Worker.
% Espera recibir la respuesta del Worker, que luego reenvia al cliente.
socket_process_loop(ClientSocket,WorkerSocket) ->
	%io:format("Waitting for data"), %DEBUG
	Data = sockaux:gets(ClientSocket),
	%io:format("Data: ~p~n",[Data]), %DEBUG
	if (Data == error) -> ok;
	true ->
		Parsed_Data = parser:parse(Data),
		case protocol_validation(Parsed_Data) of
			ok	    ->  
			        %io:format("Sending ~p~n",[Parsed_Data]), %DEBUG
			        gen_tcp:send(WorkerSocket,lists:foldl(fun(X,Acc) -> Acc ++ " " ++ X end,"",Parsed_Data) ++ "\n"),
			        %io:format("Succesfully send~n"), %DEBUG
					WorkerAnswer = sockaux:gets(WorkerSocket),
					%io:format("Answer! ~p~n", [WorkerAnswer]), %DEBUG
					gen_tcp:send(ClientSocket,WorkerAnswer ++ "\0"),
					socket_process_loop(ClientSocket,WorkerSocket);
			error   ->  gen_tcp:send(ClientSocket,"Invalid command\n"),
					socket_process_loop(ClientSocket,WorkerSocket);
			exit_signal ->  gen_tcp:send(ClientSocket,"OK\0"),
							gen_tcp:send(WorkerSocket,"BYE"),
							gen_tcp:close(ClientSocket), gen_tcp:close(WorkerSocket)
		end
	end.

protocol_validation(List) -> case List of
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
