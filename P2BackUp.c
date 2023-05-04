#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

char *uint32_to_str(uint32_t i)
{
   int length = snprintf(NULL, 0, "%lu", (unsigned long)i);       // pretend to print to a string to determine length
   char* str = malloc(length + 1);                        // allocate space for the actual string
   snprintf(str, length + 1, "%lu", (unsigned long)i);            // print to string

   return str;
}

int P2(int argc, char* argv[])
{
   chdir(argv[1]);
   char buf[1024];
   char tuff[1024];
   char buffer[1024];
   int data[1024]; // inode name
   char diff[1024]; // file or dir name
   int Type;
   FILE* Inodes;
   FILE* CurDir;
   int PWD = 0;
   int iNum = 0;
   int inodeNum;
   int inodeList[1024];
   int next_inode = 0;
   int inode_num;
   char type;
   int newDirNum;
   char* newDirType = "d";

   while(!feof(stdin)){
           printf("%s ", ">");
           scanf("%s", buf);

           Inodes = fopen("inodes_list", "rb");
           CurDir = fopen(uint32_to_str(PWD), "rb");


           //code for ls command
           if(strncmp(buf , "ls", 2) == 0){
                while((inodeNum = fread(data, sizeof(int), 1, CurDir)) == 1 && (Type = fread(diff, 32, 1, CurDir)) == 1){
                        printf("%d %s\t \n", *data, diff);
                }
                fclose(CurDir);
           }

           //code for cd command
           if(strncmp(buf, "cd", 2) == 0){
                scanf("%s", tuff);
                while((inodeNum = fread(data, sizeof(int), 1, CurDir)) == 1 && (Type = fread(diff, 32, 1, CurDir)) == 1){
                        if(strcmp(diff, tuff) == 0){
                                PWD = *data;
                        }
                }
           }

           //code for mkdir command
           else if(strncmp(buf, "mkdir", 5) == 0){
                scanf("%s", tuff);
                while((inodeNum = fread(data, sizeof(int), 1, CurDir)) == 1 && (Type = fread(diff, 32, 1, CurDir)) == 1){
                        if(strcmp(diff, tuff) == 0){
                               printf("Dir Exists\n");
                               break;
                        }
                        //condition to see if dir name exists
                        else if(strcmp(diff, tuff) != 0){
                               //open the inodes list
                               Inodes;
                               //reading the inode number and the inode type
                               while((fread(&inode_num, sizeof(int), 1, Inodes) == 1) && (fread(&type, sizeof(char), 1, Inodes) == 1)){
                                       //set type to a space in this array
                                       inodeList[inode_num] = type;
                               }
                               //increment the inode number 1 to get new inode number
                               newDirNum = inode_num + 1;

                               //update inodes by writing new inode number
                               //update inodes list by writing new type ("d")
                               fwrite(newDirNum, sizeof(int), 1, Inodes);
                               fwrite(newDirType, sizeof(char), 1, Inodes);

                               //create new inode file
                                        //include initialization of directories
                                        //. (current)
                                        //.. (parent)
                               //add new directory to current directory
                                        //grab the current working directory (PWD)
                                        //write the inode number to the directory
                                        //write the directory name to the new directory
                        }
                }
           }
        /*
           //code for touch command
           else if(strncmp(buf, "touch", 5) == 0){
           }
        */
           //code for exit command
           else if(strncmp(buf, "exit", 4) == 0){
                   exit(0);
           }

           fclose(Inodes);

   }

   if(feof(stdin)){
           printf("end of file\n");
   }
   return 0;
}

int main(int argc, char *argv[]){
        P2(argc, argv);
}
