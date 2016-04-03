-module(ids).
-compile(export_all).

myId()                       -> error("myId").%TODO
makeIdGlobal(Worker, Client) -> error("makeIdGlobal").%TODO
globalIdToWorker(_)          -> error("globalIdToWorker").
globalIdToClient(_)          -> error("gIdToClient").

globalFdToWorker(_) -> error("globalFdToWorker").
globalFdToFd(_)     -> error("globalFdToFd").
globalFdToIdg(_)    -> error("globalFdToIdg").


