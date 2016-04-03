-module(comunic).
-compile(export_all).


responderCliente( C, M ) -> % TODO
    io:format(lists:concat(["Respuesta a cliente " ,C,": " ,mensaje:say(M),"~n"])).

responderClienteRemoto(Idg, M) -> error("responderClienteRemoto").

enviarWorker( W , Orden )->error ("No definido enviarWorker~n").
