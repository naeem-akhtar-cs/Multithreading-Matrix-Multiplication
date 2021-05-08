#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;


int main(){

ifstream fin;

int** mat1;
int rows1;
int cols1;

int** mat2;
int rows2;
int cols2;

int** result;

fin.open("Mat1.txt");

if(fin.is_open()){
rows1=0;
cols1=0;

string str;
int size=500;
char* buffer=new char[size];
int bufIndex=0;

int check=0;

fin>>str;

while(!fin.eof()){

rows1++;

int z=0;
if(z<size){
while(str[z]!='\0'){
buffer[bufIndex]=str[z];
z++;
bufIndex++;
}
buffer[bufIndex]=':';
bufIndex++;
}
else{  //Incase data exceedes from the current buffer size, it will inccrese the size of buffer by 500
char* buffer1=new char[size];
for(int x=0;x<size;x++){
buffer1[x]=buffer[x];
}

delete[] buffer;

char* buffer=new char[size+500]; //New buffer of increased size of 500

for(int x=0;x<size;x++){
buffer[x]=buffer1[x];
}

delete [] buffer1;

}
fin>>str;
}
buffer[bufIndex]='\0';

fin.close();

int o=0;
while(buffer[o]!=':'){
if(buffer[o]==','){
cols1++;
}
o++;
}

cols1++;

mat1=new int*[rows1];

for(int i=0;i<rows1;i++){
mat1[i]=new int[cols1];
}

char* temp=new char[30];

//Matrix indices
int i1=0;
int i2=0;

int i3=0;

for(int j=0;buffer[j]!='\0';j++){

temp[i3]=buffer[j];
i3++;

if(buffer[j]==',' || buffer[j]==':'){
temp[i3-1]='\0';
mat1[i1][i2]=atoi(temp);

if(buffer[j]==','){
i2++;
} else if(buffer[j]==':'){

i2=0;
i1++;
}
i3=0;
}
}

/*
for(int i=0;i<rows1;i++){
for(int j=0;j<cols1;j++){
cout<<mat1[i][j]<<" ";
}
cout<<endl;
}
*/

delete[] buffer;
delete[] temp;

} else {
cout<<"File Mat1.txt can not be Opened"<<endl;
return 0;
}


//********************************Reading of File 1 Ended Here***************************//

fin.open("Mat2.txt");

if(fin.is_open()){
rows2=0;
cols2=0;

string str;
int size=500;
char* buffer=new char[size];
int bufIndex=0;

int check=0;

fin>>str;

while(!fin.eof()){

rows2++;

int z=0;
if(z<size){
while(str[z]!='\0'){
buffer[bufIndex]=str[z];
z++;
bufIndex++;
}
buffer[bufIndex]=':';
bufIndex++;
}
else{  //Incase data exceedes from the current buffer size, it will inccrese the size of buffer by 500
char* buffer1=new char[size];
for(int x=0;x<size;x++){
buffer1[x]=buffer[x];
}

delete[] buffer;

char* buffer=new char[size+500]; //New buffer of increased size of 500

for(int x=0;x<size;x++){
buffer[x]=buffer1[x];
}

delete [] buffer1;

}
fin>>str;
}
buffer[bufIndex]='\0';

fin.close();

int o=0;
while(buffer[o]!=':'){
if(buffer[o]==','){
cols2++;
}
o++;
}

cols2++;

mat2=new int*[rows2];

for(int i=0;i<rows2;i++){
mat2[i]=new int[cols2];
}

char* temp=new char[30];

//Matrix indices
int i1=0;
int i2=0;

int i3=0;
//cout<<buffer;
for(int j=0;buffer[j]!='\0';j++){

temp[i3]=buffer[j];
i3++;

if(buffer[j]==',' || buffer[j]==':'){
temp[i3-1]='\0';
mat2[i1][i2]=atoi(temp);

if(buffer[j]==','){
i2++;
} else if(buffer[j]==':'){

i2=0;
i1++;
}
i3=0;
}
}

/*
for(int i=0;i<rows2;i++){
for(int j=0;j<cols2;j++){
cout<<mat2[i][j]<<" ";
}
cout<<endl;
}

*/

delete[] buffer;
delete[] temp;

} else {
cout<<"File Mat2.txt can not be Opened"<<endl;
return 0;
}


//*******Both the matrices from the files are read and loaded in the memory now************//


/*
cout<<"\nMatrix-1: "<<rows1<<"*"<<cols1<<endl;
cout<<"Matrix-2: "<<rows2<<"*"<<cols2<<endl;
*/

if(cols1==rows2){ //If matrix multiplications is possible


    char buf;    
    int childCount=(rows2*cols2)*rows1;
    
    pid_t* cpid=new pid_t[childCount];
    
    int pipeCount=childCount*2;
    
    int** pipefd=new int*[pipeCount];
    
    for(int j=0;j<pipeCount;j++){
    pipefd[j]=new int[2];
    
    if (pipe(pipefd[j]) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    }
    
    int** resultfd=new int*[childCount];
    
    for(int j=0;j<childCount;j++){
    resultfd[j]=new int[2];
    
    if (pipe(resultfd[j]) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    }
    
    //Creating Pipes, 2 for each children
    
    int r1=0;
    int c1=0;
    
    int r2=0;
    int c2=0;
    
    int pipeIndex=0;
    
    for(int i=0;i<rows1;i++){
    for(int k=0;k<cols2;k++){
    for(int j=0;j<cols1;j++){
    write(pipefd[pipeIndex][1],&mat1[i][j],sizeof(int));
    pipeIndex=pipeIndex+2;
    }
    }
    }
    
    pipeIndex=1;
    
    for(int k=0;k<rows1;k++){
    for(int i=0;i<cols2;i++){
    for(int j=0;j<rows2;j++){
    write(pipefd[pipeIndex][1],&mat2[j][i],sizeof(int));
    pipeIndex=pipeIndex+2;
    }
    }
    }
    
    int* tempResult=new int[childCount];
    
    
    for(int i=0;i<childCount;i++){
    cpid[i] = fork();
    
    if (cpid[i] == -1) {
        cout<<"Child #: "<<cpid[i]<<" creation failed"<<endl;
        exit(EXIT_FAILURE);
    }
    
    if (cpid[i] == 0) {    /* Child reads from pipe */
    int a;
    int b;
    
    read(pipefd[i*2][0],&a,sizeof(int));
    read(pipefd[(i*2)+1][0],&b,sizeof(int));
    
    int c=a*b;
    
    write(resultfd[i][1],&c,sizeof(int));

   // cout<<"(C) Array Process # "<<cpid[i]<<" Process ID: "<<getpid()<<endl;
    
    _exit(EXIT_SUCCESS);
    } else {            /* Parent writes numbers to be multiplied in pipe */
        
     int r;
     read(resultfd[i][0],&r,sizeof(int));
         
    //cout<<"Array Process # "<<cpid[i]<<" Process ID: "<<getpid()<<endl;
     tempResult[i]=r;
     //cout<<"Result: "<<tempResult[i]<<endl;
     //cout<<"Child No: "<<i<<" in Parent Code"<<endl;
     wait(NULL);
    }
}

    delete[] cpid;
    
    for(int j=0;j<pipeCount;j++){
    delete pipefd[j];
}

delete[] pipefd;

    for(int j=0;j<childCount;j++){
    delete resultfd[j];
}

delete[] resultfd;


ofstream fout;
    
fout.open("Resultant.txt");
int result;
int no_of_cols=0;
for(int i=0;i<childCount;i=i+cols1){
result=0;

for(int j=0;j<cols1;j++){
result=result+tempResult[i+j];
}
no_of_cols++;

fout<<result;
if(no_of_cols!=cols2){
fout<<",";
}

if(no_of_cols==cols2){
no_of_cols=0;
fout<<"\n";
}
}

delete[] tempResult;

fout.close();
cout<<"\n\nGreat!! Matrices have been multiplied and the result has been saved in Resultant.txt in the same Directory\n\n";

} else {

cout<<"Matrix Multiplication is not Possible. Current Dimensions of of matrices are "<<rows1<<"*"<<cols1<<" and "<<rows2<<"*"<<cols2<<endl;
}

return 0;
}
