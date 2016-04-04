-module(realfs).
-compile(export_all).
-define(WORKER_PATH,"./").

% este modulo es el encargado de encapsular la interaccion con el sistema de archivos real subyacente
% Funciones
% openr : String -> Handle (abre un archivo en modo lectura)
% openw : String -> Handle (abre un archivo en modo escritura)
% read : Handle, Size -> Data | eof (trata de leer hasta Size bytes del archivo, si puede leer algo devuelve los caracteres leidos, si el archivo ya terminó devuelve eof)
% write : Handle, String -> ok (escribe String en el archivo)
% create : String -> ok (crea un archivo)
% delete : String -> ok (crea un archivo existente)



openr( NameFile) -> {ok, Handle} = file:open(?WORKER_PATH ++ NameFile, [read]),
                    Handle.
openw( NameFile) -> {ok, Handle} = file:open(?WORKER_PATH ++ NameFile, [append]),
                    Handle.
read ( Handle, Size) -> case file:read(Handle,Size) of
                            eof -> eof;
                            {ok,Data} -> Data;
                            _ -> error("No deberia ocurrir")
                        end.
write ( Handle, Data) -> file:write(Handle, Data),
                            ok.
close ( Handle ) -> file:close( Handle ), ok.

create( NameFile ) -> file:write_file(?WORKER_PATH ++ NameFile,""), ok.

delete( NameFile ) -> file:delete(?WORKER_PATH ++ NameFile), ok.
