-module(sockaux).
-compile(export_all).
%-export([gets/1]).

% Lee una línea (hasta '\0' o '\n') o hasta que ocurra algún error en la conexión (como que el cliente se desconecte)
gets(Sock) -> case gen_tcp:recv(Sock,1) of
					  {ok, Data} -> Byte = lists:nth(1,Data),
									if (Byte==0) or (Byte==10) -> []; 
									true -> case gets(Sock) of
												error -> [Byte];
												Cad -> [Byte | Cad]
											end
									end;
					  _ -> error
					  end.

