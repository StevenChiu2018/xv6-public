#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

int
main(int argc, char *argv[])
{
  int readFile, writeFile, n;

  if(argc <= 2){
    printf(1,"Need 2 arguments\n");
    exit();
  }

  //open file
  if((readFile = open(argv[1], O_RDONLY)) < 0){
    printf(1, "cp: cannot open %s\n", argv[1]);
    exit();
  }
  if((writeFile = open(argv[2], O_CREATE|O_RDWR)) < 0)
  {
    printf(1, "cp: cannot open %s\n", argv[1]);
    exit();
  }   
  //copy file
  while((n=read(readFile, buf, sizeof(buf)))>0)
    write(writeFile,buf,n);
  
  close(readFile);
  close(writeFile);
  exit();
}