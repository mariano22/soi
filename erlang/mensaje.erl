-module(mensaje).
-compile([mOk/0, mErr/0, say/1, archivoNoExiste/0, archivoOcupado/0, archivoExistente/0, permisoDenegado/0, finDeArchivo/0, archivoCreado/0, archivoBorrado/0, archivoCerrado/0, archivoWriteSucc/0, archivoReadSucc/1, archivoAbierto/1, coneccionEstablecida/1]).

mOk()  -> ["OK"].
mErr() -> ["ERROR"].
addArg(C,L) -> lists:append(L,[C]).
say(M) -> lists:foldr( fun(X,A)->X++" "++A end, "\n", M).

archivoNoExiste()    -> addArg("notExist",mErr()).
archivoOcupado()     -> addArg("Ocuppied",mErr()).
archivoExistente()   -> addArg("Exist",mErr()).
permisoDenegado()    -> addArg("AccessDenied",mErr()).
finDeArchivo()       -> addArg("EOF",mErr()).


archivoCreado()      -> addArg("Create",mOk()).
archivoBorrado()     -> addArg("Deleted",mOk()).
archivoCerrado()     -> addArg("Close",mOk()).
archivoWriteSucc()   -> addArg("Writed",mOk()).
archivoReadSucc(Txt) -> addArg(Txt,mOk()).
archivoAbierto(Gfd)  -> addArg(integer_to_list(Gfd),mOk()).
coneccionEstablecida(IdCon) -> addArg(integer_to_list(IdCon),mOk()).
