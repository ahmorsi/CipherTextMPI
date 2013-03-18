#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include<cstring>
#include <string>
using namespace std;

int myrank, size;
map <string , string > Hashing_Dictionary;
fstream cipherfile ("cypher.txt" , fstream::in | fstream::out);
#define MASTER 0
#define Last size-1
	
void read_keyfile(string path)
{
	stringstream ss;
	char c;
	char* str = new char[1000000];
	string input, key , value;
	ifstream keyfile(path);
	try{
		while(keyfile.eof())
		{
			keyfile.getline(str , 100);
			input=str;
			ss<<input;
			ss>>key>>value;
			Hashing_Dictionary[key] = value;
		}
	}
	catch(char * str)
	{
		printf("There is an Error : %s \n",str);
		delete(str);
	}
	delete(str);
}
int seek_thenextword()
{
	char c ='\0' ;
	int value = -1;
	do
	{
		c = cipherfile.get();
		value++;
	}while( c!=' ' );
	return value;
}
int get_LengthOfFile(fstream& filestream)
{
	filestream.seekg (0, filestream.end);
    return filestream.tellg();
}
int get_mystart()
{
	int index , value;
	if( myrank == MASTER)
	{
		return -1;
	}
	else
	{
		int length = get_LengthOfFile(cipherfile);
		index = (length/size);
		index *= myrank;
		cipherfile.seekg(index , cipherfile.beg);
		char ctest = cipherfile.get();
		cipherfile.seekg(index , cipherfile.beg);
		if(ctest!=' '){
			value = seek_thenextword();
		}
		return index+value;
	}
}
int get_myend()
{
	int index , value;
	int length = get_LengthOfFile(cipherfile);
	if( myrank == Last)
	{
		return length+1;
	}
	else
	{
		
		index = (length/size);
		index *= (myrank+1);
		cipherfile.seekg(index , cipherfile.beg);
		char ctest = cipherfile.get();
		cipherfile.seekg(index , cipherfile.beg);
		if(ctest!=' '){
			value = seek_thenextword();
		}
		return index+value;
	}
}
string read_nextWord(int& start)
{
	char* word = new char[100];
	cipherfile.seekg(start,cipherfile.beg);
	cipherfile.getline(word , 100,' ');
	start = cipherfile.tellg();
	string parsed_Word=word;
	delete(word);
	return parsed_Word;
}
void main (int argc, char *argv[]) {
	MPI_Init(&argc, &argv); /* Initialize MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* Get my rank */
	MPI_Comm_size(MPI_COMM_WORLD, &size); /* Get number of processors */

	int mystart = get_mystart() +1;
	int myend = get_myend()-1;
	string parsed_Word;
	printf("My Start is %d and My End is %d\n",mystart,myend);
	cout<<"myrank : "<<myrank;
	cout<<" size : "<<size<<endl;
	//for(int cur_index=mystart;cur_index <= myend && cur_index >=0 ;)
	//{
		//parsed_Word=read_nextWord(cur_index);
	parsed_Word=read_nextWord(mystart);
	  //  printf("%s\n",parsed_Word);
	cout<<parsed_Word;
		cin>>parsed_Word;
	//}
	MPI_Finalize(); /* Terminate MPI */
}