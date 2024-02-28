#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "directory.h"

typedef int Myfunc(const char*,const char*,const char*);
Myfunc myfunc;
int myftw(const char *, const char*, const char* ,int, Myfunc *);
int dopath(Myfunc *, const char*, const char*,int);

char *fullpath;
size_t pathlen;

int myftw(const char *pathname, const char *pattern, const char *filetype, int symbolic_link, Myfunc *func){
    pathlen = PATH_MAX + 1;
    fullpath = malloc(pathlen);
    if (pathlen <= strlen(pathname)){
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath,pathlen)) == NULL){
            printf("realloc failed");
        }
    }
    printf("%d\n", symbolic_link);
    strcpy(fullpath,pathname);
    return dopath(func,pattern,filetype,symbolic_link);
}

int dopath(Myfunc *func, const char *pattern,const char* filetype,int symbolic_link){
    DIR *directory;
    struct dirent *entry;
    struct stat statbuf;
    int ret,n;
    if ((ret = func(fullpath,pattern,filetype)) != 0){
        return ret;
    }
    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen){
        pathlen *= 2;
        if ((fullpath = realloc(fullpath,pathlen)) == NULL){
            printf("realloc failed\n");
        }
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;
    if ((directory = opendir(fullpath)) == NULL){
        return func(fullpath,pattern,filetype);
    }
    while( (entry = readdir(directory))!= NULL){
            if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0){
                continue;
            }
            strcpy(&fullpath[n],entry->d_name);

            if (lstat(fullpath,&statbuf) == 0){
                //Check if its not a directory
                if (S_ISDIR(statbuf.st_mode) == 0){
                    if (S_ISLNK(statbuf.st_mode) && (symbolic_link == 0)){
                        printf("It's a symbolic but we're not checking");
                        continue;
                    }
                }
            }
            if ((ret = dopath(func,pattern,filetype,symbolic_link)) != 0){
                break;
            }     
    } 
    fullpath[n-1] = 0;
    if (closedir(directory) < 0){
        printf("can’t close directory %s", fullpath);
    }
    return ret;
}
int myfunc(const char* pathname, const char *pattern,const char *filetype){
    int n = strlen(pathname);
    const char *file_extension = &pathname[n-1];
    
    FILE *file = fopen(pathname,"r");
    
    if (file == NULL){
        //fprintf(stderr,"Error opening files either a directory or not a readable file\n");
        return 0;   
    }
    else{
        
        if (filetype == NULL){
            char line[512];
            int line_number = 0;    
            while (fgets(line,sizeof(line),file)){
                line_number++;
                if (strstr(line,pattern) != NULL){
                    printf("Line: %s Path: %s",line,pathname); 
                    printf("\n");         
                }           
            }
        
        }
        else if (strcmp(file_extension,filetype) == 0){
            char line[512];
            int line_number = 0;    
            while (fgets(line,sizeof(line),file)){
                line_number++;
                if (strstr(line,pattern) != NULL){
                    printf("Line:%s Path:%s\n",line,pathname); 
                            
                }           
            }
            
        }
    }
    fclose(file);
    
    return 0;
}