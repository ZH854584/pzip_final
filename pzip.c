#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/io.h>
#include <string.h>
#include <stdbool.h>


void *compressFile(void *arg) 
{
  FILE *fp_out = fopen("testzip.z", "ab");
  char *input = (char *)arg;
  char *output = NULL;
  int cou;
  const char ter = '\0';
  char cha = '\0';
  for(int x = 0; input[x]!='\0';x++)
  {
    cou=1;
    cha = input[x];
    while(input[x+1]==cha)
    {
      x++;
      cou++;
    }
    if(cha != '\n')
    {
      fwrite(&cou,sizeof(int),1,fp_out);
      fwrite(&cha,sizeof(char),1,fp_out);
    }
  }
  fwrite(&ter, sizeof(char),1,fp_out);
  fflush(fp_out);
  fclose(fp_out);
}


char* stringfromFile(char *in)
{
  unsigned char *f;
  int size;
  struct stat s;
  //printf("%s \n", in);
  const char *  file_name = in;
  int fd = open (file_name, O_RDONLY);
  // printf ("%d \n", fd);
  /* Get the size of the file. */
  int status = fstat (fd, & s);
  size = s.st_size;
  f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
  // printf("%s", f);
  for (int i = 0; i < size; i++)
  {
     char c;
     c = f[i];
     putchar(c);
   }
    return f;

}


int main(int argc, char *argv[])
{


  //runs until the argument list is empty (out of files)
  for(int x = 2; x!=(argc);x++)
  {
    //FILE *fp = fopen (argv[x], "r");
    FILE *fp_out = fopen("testzip.z", "wb");
    //Implement a check for empty file (FILE == NULL)
    //if (fp == NULL)
    //{
      //printf("cannot open file\n");
      //exit(1);
    //}

    int tcount = *argv[1];
    //reads entire file into one array
    char* buf = stringfromFile(argv[x]);
    printf("\n");
    if(tcount == '1')
    {
      // no division necessary for one thread
      pthread_t p1;     
      int rc;
      rc =printf("Running on one thread \n");
      rc =pthread_create(&p1, NULL, compressFile, buf);
      //join waits for the threads to finish
      pthread_join(p1, NULL);
      printf("Finished running on one thread \n");
    }


    if (tcount == '2')
    {
      int length =strlen(buf);
      //printf("length = %d\n",length);
      int newlen = (length/2);
      // printf("%d\n", newlen);
      char firstHalf [newlen];
      char secondHalf [newlen];
      //printf("firshalf length: %d\n",(int)sizeof(firstHalf));
      for(int x = 0; x<newlen;x++)
      {
       // printf("%c", buf[x]);
        firstHalf[x] = buf[x];
      }
      //printf("\n%s\n\n","finished filling first half");
      //printf("first half is %s \n", firstHalf);
      for(int y = 0; y <= newlen; y++)
      {
        //printf("%c", buf[y+newlen]);
        secondHalf[y] = buf[(y+newlen)];
      }
      //printf("%s\n\n","finished filling second half");
      printf("first half: %s\n", firstHalf);
      printf("second half: %s\n", secondHalf);
      pthread_t p1, p2;
      int rc;
      printf("Running on two threads \n");
      rc = pthread_create(&p1, NULL, compressFile, firstHalf);
      printf("%d", rc);
      rc = pthread_create(&p2, NULL, compressFile, secondHalf);
      // join waits for the threads to finish
      rc = pthread_join(p1,NULL);
      rc = pthread_join(p2, NULL);
      printf("Finished running on two threads \n");
    }



    if (tcount == '4')
    {
      //divide input string into 4
      pthread_t p1, p2, p3, p4;
      int rc;
      printf("Running on four threads \n");
      rc = pthread_create(&p1, NULL, compressFile, "A");
      rc = pthread_create(&p2, NULL, compressFile, "B");
      rc = pthread_create(&p3, NULL, compressFile, "C");
      rc = pthread_create(&p4, NULL, compressFile, "D");
      // join waits for the threads to finish
      rc = pthread_join(p1, NULL);
      rc = pthread_join(p2, NULL);
      rc = pthread_join(p3, NULL);
      rc = pthread_join(p4, NULL);
      printf("Finished running on four threads \n");
    }


    if (tcount == '8')
    {
      //divide input string into 8
      pthread_t p1, p2, p3, p4, p5, p6, p7, p8;
      int rc;
      printf("Running on eight threads \n");
      rc = pthread_create(&p1, NULL, compressFile, "A");
      rc = pthread_create(&p2, NULL, compressFile, "B");
      rc = pthread_create(&p3, NULL, compressFile, "C");
      rc = pthread_create(&p4, NULL, compressFile, "D");
      rc = pthread_create(&p5, NULL, compressFile, "E");
      rc = pthread_create(&p6, NULL, compressFile, "F");
      rc = pthread_create(&p7, NULL, compressFile, "H");
      rc = pthread_create(&p8, NULL, compressFile, "I");
      // join waits for the threads to finish
      rc = pthread_join(p1, NULL);
      rc = pthread_join(p2, NULL);
      rc = pthread_join(p3, NULL);
      rc = pthread_join(p4, NULL);
      rc = pthread_join(p5, NULL);
      rc = pthread_join(p6, NULL);
      rc = pthread_join(p7, NULL);
      rc = pthread_join(p8, NULL);
      printf("Finished running on eight threads \n");
    }


//    if (tcount == *"10")
//    {
       //divide input string into 10
//      pthread_t p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
//      int rc;
//      printf("Running on ten threads \n");
//      rc = pthread_create(&p1, NULL, compressFile, "A");
//      rc = pthread_create(&p2, NULL, compressFile, "B");
//      rc = pthread_create(&p3, NULL, compressFile, "C");
//      rc = pthread_create(&p4, NULL, compressFile, "D");
//      rc = pthread_create(&p5, NULL, compressFile, "E");
//      rc = pthread_create(&p6, NULL, compressFile, "F");
//      rc = pthread_create(&p7, NULL, compressFile, "G");
//      rc = pthread_create(&p8, NULL, compressFile, "H");
//      rc = pthread_create(&p9, NULL, compressFile, "I");
//      rc = pthread_create(&p10, NULL, compressFile, "J");
      // join waits for the threads to finish
//      rc = pthread_join(p1, NULL);
//      rc = pthread_join(p2, NULL);
//      rc = pthread_join(p3, NULL);
//      rc = pthread_join(p4, NULL);
//     rc = pthread_join(p5, NULL);
//      rc = pthread_join(p6, NULL);
//      rc = pthread_join(p7, NULL);
//      rc = pthread_join(p8, NULL);
//      rc = pthread_join(p9, NULL);
//      rc = pthread_join(p10, NULL);
//      printf("Finished running on ten threads \n");
//    }


  }
  return 0;
}

