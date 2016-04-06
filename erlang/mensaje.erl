-module(mensaje).
-compile(export_all).

mOk()  -> ["OK"].
mErr() -> ["ERROR"].
addArg(C,L) -> lists:append(L,[C]).
say(M) -> lists:foldr( fun(X,A)->X++" "++A end, "\n", M).

archivoNoExiste()    -> addArg("notExist",mErr()).
archivoOcupado()     -> addArg("Ocuppied",mErr()).
archivoExistente()   -> addArg("Exist",mErr()).
permisoDenegado()    -> addArg("AccessDenied",mErr()).
finDeArchivo() -> addArg("EOF",mErr()).


archivoCreado()      -> addArg("Create",mOk()).
archivoBorrado()     -> addArg("Deleted",mOk()).
archivoCerrado()     -> addArg("Close",mOk()).
archivoWriteSucc()   -> addArg("Writed",mOk()).
archivoReadSucc(Txt) -> addArg(Txt,mOk()).
archivoAbierto(Gfd)  -> addArg(integer_to_list(Gfd),mOk()).
coneccionEstablecida(IdCon) -> addArg(integer_to_list(IdCon),mOk()).
