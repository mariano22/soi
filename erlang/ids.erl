-module(ids).
-compile(export_all).

makeIdGlobal(Worker, Client) -> error("makeIdGlobal").%TODO
myId()                       -> error("myId").%TODO
globalIdToWorker(_)          -> error("globalIdToWorker").

globalFdToWorker(_) -> error("globalFdToWorker").
globalFdToFd(_)     -> error("globalFdToFd").
globalFdToIdg(_)    -> error("globalFdToIdg").

idgToClient() -> error("idgToClient").
