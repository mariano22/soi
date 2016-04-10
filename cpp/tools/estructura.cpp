\*workerDelete(Name, IdG)-----------------*\
\*workerOpenRead(Name, IdG)---------------*\
\*workerOpenWrite(Name, IdG)--------------*\

\*workerOpenSucc(Gfd, C)------------------*\
\*workerCloseBye(Gfd)---------------------*\
\*workerWrite(Gfd, IdG, Txt)--------------*\
\*workerRead(Gfd, IdG, Sz)----------------*\
\*workerClose(Gfd, IdG)-------------------*\
\*workerCloseSucc(Gfd, IdG)---------------*\

\*workerSay(C, Msj )----------------------*\
\*workerToken(Token)----------------------*\

#define NAME 4
#define IDG  4
#define CLIENTE 4
#define GFD 4
#define SZ 4
#define EXT 4

char[NAME+IDG+EXT] wDelete;
char[NAME+IDG+EXT] wOpenRead;
char[NAME+IDG+EXT] wOpenWrite;
char[GFD + C +EXT] workerOpenSucc;

char[GFD +IDG +SZ+EXT] workerRead;
char[GFD +IDG +SZ+EXT] workerWrite;

char[GFD +IDG+EXT] workerClose;
char[GFD +IDG+EXT] workerCloseSucc;
char[GFD     +EXT] workerCloseBye;
char[ C      +EXT] workerSay;
char[ aptEXT ] workerToken;


