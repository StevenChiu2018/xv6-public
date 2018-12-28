#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  
  int readFile;

  if(argc <= 3){
    printf(1,"Need 3 arguments\n");
    exit();
  }

  //open read file
  if((readFile = open(argv[1], O_RDONLY)) < 0){
    printf(1, "sp: cannot open %s\n", argv[1]);
    exit();
  } 
  //get file size
  int fileSize;
  struct stat st;
  if(fstat(readFile, &st) < 0){
    printf(2, "sp: cannot stat %s\n", argv[1]);
    close(readFile);
    exit();
  }
  fileSize=st.size;
  //split count
  int spCount=0;
  for(int i=0;i<strlen(argv[3]);i++)
  {
    int mul=1;
    for(int j=0;j<strlen(argv[3])-i-1;j++)
      mul*=10;
    spCount+=(argv[3][i]-'0')*mul;
  }
  //split
  int writeFile,n,splitSize;
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
    char fileName[strlen(argv[2])+countLen+1];
    strcpy(fileName,argv[2]);
    fileName[strlen(argv[2])+countLen]='\0';
    tmp=i+1;
    for(int j=countLen-1;j>=0;j--)
    {         
      fileName[strlen(argv[2])+j]=(tmp%10)+'0';
      tmp/=10;
    }
    //open write file
    if((writeFile = open(fileName, O_CREATE|O_RDWR)) < 0)
    {
      printf(1, "sp: cannot open %s\n", fileName);
      exit();
    } 
    //split size
    splitSize=fileSize/spCount;
    //last file
    if(i==spCount-1)
      splitSize=fileSize-(splitSize*(spCount-1));
    //split file
    //512btyes
    while(splitSize>=512)
    {
      char buf[512];
      if((n=read(readFile, buf, sizeof(buf)))>0)
      {
        write(writeFile,buf,n);  
        splitSize-=512;
      }
    }
    //remainder
    char buf[splitSize];
    if((n=read(readFile, buf, sizeof(buf)))>0)
      write(writeFile,buf,n);  

    close(writeFile);
  } 

  close(readFile);
  exit();
}