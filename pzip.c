/*
	Zachary Hansen
        May 2nd, 2019
        Assignment 4
*/

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
#include <time.h>

//@input: int
//@return: void
//stops program from running for a short period of time
//I used this to make sure my threads do not finish out of order
void delay(int number_of_seconds){
int milli_seconds = 1000 * number_of_seconds;
clock_t start_time = clock();
while(clock()< start_time + milli_seconds);
}

//@input: void*
//@return: void
//performs RLE compression on a string
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

//@input: char*
//@return: char*
//reads a file into a single array using mmap
char* stringfromFile(char *in)
{
  unsigned char *f;
  int size;
  struct stat s;
  const char *  file_name = in;
  int fd = open (file_name, O_RDONLY);
  int status = fstat (fd, & s);
  if(status == -1){
    printf("couldnt find file: %s \n",file_name);
    exit(1);
  }
  size = s.st_size;
  f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
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
    FILE *fp_out = fopen("testzip.z", "wb");
    int tcount = *argv[1];
    char* buf = stringfromFile(argv[x]);
    printf("\n");



    //WORKS FOR ONE FILE ANY SIZE
    //DOES NOT WORK FOR MULTIPLE FILES 
    if(tcount == '1')
    {
      pthread_t p1;
      int rc;
      rc =printf("Running on one thread \n");
      rc =pthread_create(&p1, NULL, compressFile, buf);
      pthread_join(p1, NULL);
      printf("Finished running on one thread \n");
    }

    //WORKS FOR ONE FILE
    //DOES NOT WORK FOR MULTIPLE FILES(Overwrite error ) 
    //DOES NOT WORK FOR A SINGLE SIMPLE FILE e.g(aabbcc) 
    if (tcount == '2')
    {
      int length =strlen(buf);
      int newlen = (length/2);
      char firstHalf [newlen];
      char secondHalf [newlen];
      for(int x = 0; x<newlen;x++)
      {
        firstHalf[x] = buf[x];
      }
      for(int y = 0; y <= newlen; y++)
      {
        secondHalf[y] = buf[(y+newlen)];
      }

      printf("first half: %s\n", firstHalf);
      printf("second half: %s\n", secondHalf);
      pthread_t p1, p2;
      int rc;
      printf("Running on two threads \n");
      rc = pthread_create(&p1, NULL, compressFile, firstHalf);
      delay(1);
      rc = pthread_create(&p2, NULL, compressFile, secondHalf);
      rc = pthread_join(p1,NULL);
      rc = pthread_join(p2, NULL);
      printf("Finished running on two threads \n");
    }

    //WORKS FOR ONE FILE
    //DOES NOT WORK FOR MULTIPLE FILES (OVERWRITE ERROR)
    //DOES NOT WORK FOR SINGLE SIMPLE FILE e.g(aabbcc)
    if (tcount == '4')
    {
      //divide input string into 4
      int length = strlen(buf);
      int newlen = length/4;
      char firstquarter[newlen];
      char secondquarter[newlen];
      char thirdquarter[newlen];
      char lastquarter[newlen];
      for(int h = 0; h < newlen; h++){
        firstquarter[h]= buf[h];
      }
      printf("first quarter is: %s \n", firstquarter);
      for(int i = newlen; i < (newlen + newlen); i++){
        secondquarter[i-newlen] =  buf[i];
      }
      printf("second quarter is: %s \n", secondquarter);
      for(int j = (newlen + newlen); j < (newlen +newlen+ newlen ); j++){
        thirdquarter[j-(newlen +newlen)] = buf[j];
      }
      printf("third quarter is: %s \n",thirdquarter);
      for(int k = (newlen + newlen + newlen); k < (length); k++){
        lastquarter[k - (newlen + newlen + newlen)] = buf[k];
      }
      printf("fourth quarter is: %s \n", lastquarter);
      pthread_t p1, p2, p3, p4;
      int rc;
      printf("Running on four threads \n");
      rc = pthread_create(&p1, NULL, compressFile, firstquarter);
      delay(1);
      rc = pthread_create(&p2, NULL, compressFile, secondquarter);
      delay(1);
      rc = pthread_create(&p3, NULL, compressFile, thirdquarter);
      delay(1);
      rc = pthread_create(&p4, NULL, compressFile, lastquarter);
      rc = pthread_join(p1, NULL);
      rc = pthread_join(p2, NULL);
      rc = pthread_join(p3, NULL);
      rc = pthread_join(p4, NULL);
      printf("Finished running on four threads \n");
    }

    //WORKS FOR ONE FILE
    //DOES NOT WORK FOR MULTIPLE FILES(overwrite error)
    //DOES NOT  ENTIRELY WORK ON A SINGLE SIMPLE FILE e.g(aabbcc)
    if (tcount == '8')
    {
      int length = strlen(buf);
      int newlen = length/8;
      char arr1[newlen];
      char arr2[newlen];
      char arr3[newlen];
      char arr4[newlen];
      char arr5[newlen];
      char arr6[newlen];
      char arr7[newlen];
      char arr8[newlen];
      //filling arr1
      for(int a = 0; a < newlen; a++)
      {
        arr1[a]= buf[a];
      }
      //filling arr2
      for(int b = newlen; b < (newlen*2); b++ )
      {
        arr2[b-newlen] = buf[b];
      }
      //filling arr3
      for(int c = (newlen*2); c<(newlen*3);c++)
      {
        arr3[c - (newlen*2)] = buf[c];
      }
      //filling arr4
      for(int d = (newlen*3); d<(newlen*4); d++)
      {
        arr4[d - (newlen*3)] = buf[d];
      }
      //filling arr5
      for(int e = (newlen*4); e< (newlen*5); e++)
      {
        arr5[e - (newlen*4)] = buf[e];
      }
      //filling arr6
      for(int f = (newlen*5); f <(newlen*6);f++)
      {
        arr6[f - (newlen*5)] = buf[f];

      }
      for(int g = (newlen*6); g < (newlen*7);g++)
      {
        arr7[g - (newlen*6)] = buf[g];
      }
      for(int h = (newlen*7);h < length;h++){
        arr8[h - (newlen*7)] = buf[h];
      }
      pthread_t p1, p2, p3, p4, p5, p6, p7, p8;
      int rc;
      printf("Running on eight threads \n");
      rc = pthread_create(&p1, NULL, compressFile, arr1);
      delay(1);
      rc = pthread_create(&p2, NULL, compressFile, arr2);
      delay(1);
      rc = pthread_create(&p3, NULL, compressFile, arr3);
      delay(1);
      rc = pthread_create(&p4, NULL, compressFile, arr4);
      delay(1);
      rc = pthread_create(&p5, NULL, compressFile, arr5);
      delay(1);
      rc = pthread_create(&p6, NULL, compressFile, arr6);
      delay(1);
      rc = pthread_create(&p7, NULL, compressFile, arr7);
      delay(1);
      rc = pthread_create(&p8, NULL, compressFile, arr8);
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


    if (tcount == 10)
    {
      pthread_t p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
      int rc;
      printf("Running on ten threads \n");
      rc = pthread_create(&p1, NULL, compressFile, "A");
      rc = pthread_create(&p2, NULL, compressFile, "B");
      rc = pthread_create(&p3, NULL, compressFile, "C");
      rc = pthread_create(&p4, NULL, compressFile, "D");
      rc = pthread_create(&p5, NULL, compressFile, "E");
      rc = pthread_create(&p6, NULL, compressFile, "F");
      rc = pthread_create(&p7, NULL, compressFile, "G");
      rc = pthread_create(&p8, NULL, compressFile, "H");
      rc = pthread_create(&p9, NULL, compressFile, "I");
      rc = pthread_create(&p10, NULL, compressFile, "J");
      rc = pthread_join(p1, NULL);
      rc = pthread_join(p2, NULL);
      rc = pthread_join(p3, NULL);
      rc = pthread_join(p4, NULL);
      rc = pthread_join(p5, NULL);
      rc = pthread_join(p6, NULL);
      rc = pthread_join(p7, NULL);
      rc = pthread_join(p8, NULL);
      rc = pthread_join(p9, NULL);
      rc = pthread_join(p10, NULL);
      printf("Finished running on ten threads \n");
    }
  }
  return 0;
}

