-module(mensaje).
-compile(export_all).

mOk()  -> ["OK"].
mErr() -> ["ERROR"].
addArg(C,L) -> lists:append(L,[C]).
say(M) -> lists:concat(M).

archivoNoExiste()    -> addArg("notExist",mErr()).
archivoOcupado()     -> addArg("Ocuppied",mErr()).
archivoYaExistente() -> addArg("Exist",mErr()).


archivoBorrado()     -> addArg("Deleted",mOk()).
archivoCreado()      -> addArg("Create",mOk()).
