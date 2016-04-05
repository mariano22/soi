-module(mensaje).
-compile(export_all).

mOk()  -> ["OK"].
mErr() -> ["ERROR"].
addArg(C,L) -> lists:append(L,[C]).
say(M) -> lists:concat(M).

archivoNoExiste()    -> addArg("notExist",mErr()).
archivoOcupado()     -> addArg("Ocuppied",mErr()).
archivaExistente()   -> addArg("Exist",mErr()).
permisoDenegado()    -> addArg("AccessDenied",mErr()).

archivoCreado()      -> addArg("Create",mOk()).
archivoBorrado()     -> addArg("Deleted",mOk()).
archivoCerrado()     -> addArg("Close",mOk()).
archivoWriteSucc()   -> addArg("Writed",mOk()).
archivoReadSucc(Txt) -> addArg(Txt,mOk()).

