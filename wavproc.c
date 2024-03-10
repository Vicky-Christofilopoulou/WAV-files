/* wavproc.c */
#include <stdio.h>
/*#define MODE 1,2,3,4,5*/
#define MODE 4

int converter(int byte1,int byte2,int byte3,int byte4) {/*swap little endian to big*/
  int num;
  num= byte1 + byte2 *256 + byte3 * 256 *256 + byte4 * 256 *256* 256 ; /* Multiple its byte with the correct power of 256 */
  return num;
}

int converter2(int num,int power) {/*swap big endian to little*/
  int byte;
  byte=num/power ; /* Divide num with the correct power of 256 */
  return byte;
}

int main(void)
{ int x,y,z,w,i,j,k,l,r,CountFile ,temp, Stereo ,SampleRate ,BlockAlign,BitsPerSample,power;
  long size, BytesPerSec,SizeOfData,NewSize;
  CountFile=0; /*to check if we have enough data*/
  x = getchar();
  i=1;    /* loop counter  */
  k=0; /* Counter for data in mode 4,5*/
  l=1; /* Counter for data of the left channel in mode 4,5*/
  r=0; /* Counter for data of the right channel in mode 4,5*/
  while (x != EOF){
    if(i==1){ /*Checking for riff part*/
      y=getchar();
      z=getchar();
      w=getchar();
      if ((x!='R') || (y!= 'I') || (z!= 'F') || ( w!='F')){
        fprintf(stderr,"Error! 'RIFF' not found\n");
        break;
      }
      else if (MODE>1){
        putchar(x);
        putchar(y);
        putchar(z);
        putchar(w);
      }
    }
    else if (i==2){ /*size of file*/
      y = getchar();  
      z = getchar();  
      w = getchar(); 
      size = converter(x,y,z,w);
      fprintf(stderr,"size of file: %ld\n",size);
      if ((MODE==2) || (MODE==3)){
        putchar(x);
        putchar(y);
        putchar(z);
        putchar(w);
      } 
    }
    else if (i==3){ /*Checking for WAVE part*/
      y=getchar();
      z=getchar();
      w=getchar();
      if ((x!='W') || (y!= 'A') || (z!= 'V') || ( w!='E')){
        fprintf(stderr,"Error! 'WAVE' not found\n"); 
        break;
      }
      else if ((MODE==2) || (MODE==3)){
        putchar(x);
        putchar(y);
        putchar(z);
        putchar(w);
      }
      CountFile =CountFile +4;       
    } 
    else if (i==4){  /*Checking for fmt part*/
      y=getchar();
      z=getchar();
      w=getchar();
      if ((x!='f') || (y!= 'm') || (z!= 't') || ( w!=' ')){
        fprintf(stderr,"Error! 'fmt ' not found\n");
        break;
      }
      else if ((MODE==2) || (MODE==3)){
        putchar(x);
        putchar(y);
        putchar(z);
        putchar(w);
      }
      CountFile = CountFile +4;
    }
    else if (i==5){ /*size of format chunck*/
      y = getchar();
      z = getchar(); 
      w = getchar();
      temp= converter(x,y,z,w);
      fprintf(stderr,"Size of format chunck: %d\n",temp);
      if (temp != 16){
        fprintf(stderr,"Error! size of format chunck should be 16\n");
        break;
      }
      else if ((MODE==2) || (MODE==3)){
        putchar(x);
        putchar(y);
        putchar(z);
        putchar(w);
      }
      CountFile =CountFile +4;
    }  
    else if (i==6){ /*Checking for wave format*/
      y=getchar();
      temp = converter(x,y,0,0);
      fprintf(stderr,"WAVE type format: %d\n",temp);
      if (temp != 1){
        fprintf(stderr,"Error! WAVE type format should be 1\n");
        break;
      }
      else if ((MODE==2) || (MODE==3)){
        putchar(x);
        putchar(y);
      }
      CountFile =CountFile +2;
    }
    else if (i==7){ /*Checking for Mono/Stereo*/
      y=getchar();
      Stereo = converter(x,y,0,0);
      fprintf(stderr,"mono/stereo: %d\n",Stereo);
      if ((Stereo!=1) && (Stereo!= 2)){ 
        fprintf(stderr,"Error! mono/stereo should be 1 or 2\n");
        break;
      }
      else if ((MODE==2) || (MODE==3)){
        putchar(x);
        putchar(y);
      }
      else if ((MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){ /*If there is only one channel then we putchar all the elements of the file**/
        power=1;
        for(j=1; j<=4; j++){ 
          temp = converter2(size,power);
          power= power * 256;
          putchar(temp);
        }
        putchar('W');
        putchar('A');
        putchar('V');
        putchar('E');
        putchar('f');
        putchar('m');
        putchar('t');
        putchar(0x10);
        putchar(0x00);
        putchar(0x00);
        putchar(0x00);
        putchar(0x01);
        putchar(0x00);
        putchar(x);
        putchar(y);
      }
      CountFile =CountFile +2;
    }
    else if (i==8){ /*Checking for SampleRate*/
      y= getchar();  
      z = getchar();  
      w = getchar(); 
      SampleRate = converter(x,y,z,w);
      fprintf(stderr,"sample rate: %d\n",SampleRate); 
      CountFile =CountFile +4;
      if (MODE==2){ 
        SampleRate =SampleRate/2;
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(SampleRate,power);
          power= power * 256;
          putchar(temp);
        }
      }
      else if (MODE==3){
          SampleRate =SampleRate*2;
          power=1;
          for(j=1; j<=4; j++){
            temp = converter2(SampleRate,power);
            power= power * 256;
            putchar(temp);
          }
        }
        else if ((MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){
          power=1;
          for(j=1; j<=4; j++){
            temp = converter2(SampleRate,power);
            power= power * 256;
            putchar(temp);
          }
        }
    }
    else if(i==9){ /*Checking for BytesPerSec*/
      y = getchar();  
      z = getchar();  
      w = getchar(); 
      BytesPerSec = converter(x,y,z,w);
      fprintf(stderr,"bytes/sec: %ld\n",BytesPerSec); 
      CountFile =CountFile +4;
      if (MODE==2){
        BytesPerSec=BytesPerSec/2;
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(BytesPerSec,power);
          power= power * 256;
          putchar(temp);
        }
      }
      else if (MODE==3){
        BytesPerSec=BytesPerSec*2;
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(BytesPerSec,power);
          power= power * 256;
          putchar(temp);
        }
      }
      else if ((MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(BytesPerSec,power);
          power= power * 256;
          putchar(temp);
        }
      }
    }
    else if (i==10){ /*Checking for BlockAlign*/
      y=getchar();
      BlockAlign= converter(x,y,0,0);
      fprintf(stderr,"block alignment: %d\n",BlockAlign);
      if(BytesPerSec!= SampleRate * BlockAlign){
        fprintf(stderr,"Error! bytes/second should be sample rate x block alignment\n");
        break;
      }
      else if ((MODE==2) || (MODE==3) || (MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){
        putchar(x);
        putchar(y);
      }
      CountFile =CountFile +2;
    } 
    else if(i==11){  /*Checking for BitsPerSample*/
      y=getchar();
      BitsPerSample= converter(x,y,0,0);
      fprintf(stderr,"bits/sample: %d\n",BitsPerSample);
      if ((BitsPerSample!=8) && (BitsPerSample!=16)){
        fprintf(stderr,"Error! bits/sample should be 8 or 16\n");
        break;
      }
      else if (BlockAlign!= BitsPerSample/8 * Stereo){
        fprintf(stderr,"Error! block alignment should be bits per sample / 8 * mono/stereo\n");
        break;
      }
      else if ((MODE==2) || (MODE==3) || (MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){
        putchar(x);
        putchar(y);
      }
      CountFile =CountFile +2;
    }
    else if (i==12){  /*Checking for data part*/
      y=getchar();
      z=getchar();
      w=getchar();
      if ((x!='d') || (y!= 'a') || (z!= 't') || ( w!='a')){
        fprintf(stderr,"'data' not found\n");
        break;
      }
      else if ((MODE==2) || (MODE==3) || (MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){
        putchar(x);
        putchar(y);
        putchar(z);
        putchar(w);
      }
      CountFile =CountFile +4;
    }
    else if (i==13){ /* Cheking the size of data*/
      y= getchar();  
      z = getchar();  
      w = getchar(); 
      SizeOfData= converter(x,y,z,w);
      fprintf(stderr,"size of data chunk: %ld\n",SizeOfData);
      CountFile =CountFile +4;
      if ((MODE==2) || (MODE==3) || (MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){
        putchar(x);
        putchar(y);
        putchar(z);
        putchar(w);
      }
       else if ((MODE==4 && Stereo==2) || (MODE==5 && Stereo==2)){
        NewSize= SizeOfData/2 + size-SizeOfData; /*NewSize= chunk + extra data -> Extra= Size-Chunk*/
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(NewSize,power);
          power= power * 256;
          putchar(temp);
        }
        putchar('W');
        putchar('A');
        putchar('V');
        putchar('E');
        putchar('f');
        putchar('m');
        putchar('t');
        putchar(' ');
        putchar(0x10);
        putchar(0x00);
        putchar(0x00);
        putchar(0x00);
        putchar(0x01);
        putchar(0x00);
        putchar(0x01);
        putchar(0x00);
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(SampleRate,power);
          power= power * 256;
          putchar(temp);
        }
        BytesPerSec=BytesPerSec/2;
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(BytesPerSec,power);
          power= power * 256;
          putchar(temp);
        }
        BlockAlign=BlockAlign*0.5;
        power=1;
        for(j=1; j<=2; j++){
          temp = converter2(BlockAlign,power);
          power= power * 256;
          putchar(temp);
        }
        power=1;
        for(j=1; j<=2; j++){
          temp = converter2(BitsPerSample,power);
          power= power * 256;
          putchar(temp);
        }
        putchar('d');
        putchar('a');
        putchar('t');
        putchar('a');
        power=1;
        for(j=1; j<=4; j++){
          temp = converter2(SizeOfData/2,power);
          power= power * 256;
          putchar(temp);
        }
      }
    }
    else{ /*reading data chunk*/
      if ((MODE==2) || (MODE==3) || (MODE==4 && Stereo==1) || (MODE==5 && Stereo==1)){
        putchar(x);
      }
      else if ((MODE==4 && Stereo==2) || (MODE==5 && Stereo==2)){
        k++; /* Counting the data i read*/
        if (k<=SizeOfData){ /* Data chunk*/
          if (l<=BlockAlign){ /*Data belong to the left channel*/
            if (MODE==4){
              putchar(x);
            }
            l++;
          }
          else {  /*Data belong to the right channel*/
            if (MODE==5){
              putchar(x);
            }
            r++;
          }
          if (r==BlockAlign){ /*If we have right data channel = Block Aligh then we have the number of bytes for one sample and we move on to the next one*/
            l=1;
            r=0;
          }
        }
        else { /*Extra data*/
          putchar(x);
        }
      }
      CountFile++;
    }
    x=getchar();
    i++;
  }
  if (i>13){
    if (CountFile < size ){
      fprintf(stderr,"Error! insufficient data\n");
    }
    else if ( CountFile > size){
      fprintf(stderr,"Error! bad file size\n");
    }
  }
return 0;
}