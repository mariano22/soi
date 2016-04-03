-module(mensaje).
-compile(export_all).

mOk()  -> ["OK"].
mErr() -> ["ERROR"].
addArg(C,L) -> lists:append(L,[C]).
say(M) -> lists:concat(M).

archivoNoExiste() -> addArgc("notExist",mErr()).

