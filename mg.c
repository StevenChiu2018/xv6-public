#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

int
main(int argc, char *argv[])
{
  
  int writeFile;

  if(argc <= 3){
    printf(1,"Need 3 arguments\n");
    exit();
  }

  //open write file
  if((writeFile = open(argv[2], O_CREATE|O_RDWR)) < 0){
    printf(1, "mg: cannot open %s\n", argv[1]);
    exit();
  } 
  //split count
  int spCount=0;
  for(int i=0;i<strlen(argv[3]);i++)
  {
    int mul=1;
    for(int j=0;j<strlen(argv[3])-i-1;j++)
      mul*=10;
    spCount+=(argv[3][i]-'0')*mul;
  }
  //merge
  int readFile,n;
  for(int i=0;i<spCount;i++)
  {
    //countLength
    int countLen=0;
    int tmp=i+1;
    while(tmp>0)
    {
      countLen++;
      tmp/=10;
    }
    //set filename
    char fileName[strlen(argv[1])+countLen+1];
    strcpy(fileName,argv[1]);
    fileName[strlen(argv[1])+countLen]='\0';
    tmp=i+1;
    for(int j=countLen-1;j>=0;j--)
    {         
      fileName[strlen(argv[1])+j]=(tmp%10)+'0';
      tmp/=10;
    }
    //open write file
    if((readFile = open(fileName, O_RDONLY)) < 0)
    {
      printf(1, "mg: cannot open %s\n", fileName);
      exit();
    } 
    //merge file
    while((n=read(readFile, buf, sizeof(buf)))>0)
    {
      write(writeFile,buf,n);  
    }
    close(readFile);
  } 

  close(writeFile);
  exit();
}