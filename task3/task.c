#define _XOPEN_SOURCE 700

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

struct Package{
    char*name;
    char*ver;
} typedef package;


int create_enviroment(char* env_name){
    char fullname[strlen(env_name)+3];
    snprintf(fullname,sizeof(fullname), "./%s",env_name);
    if(mkdir(fullname, 0777)==-1)
        ERR("mkdir");
    if(chdir(fullname)==-1){
        ERR("chdir");
    }
    int fd = open("requirements",O_CREAT, 0777);
    close(fd);
    return 1;

}
package getPack(char* pack_ver){
    package ret_pack;
    ret_pack.name =strtok(pack_ver, "==");
    ret_pack.ver = strtok(NULL,"==");
    printf("%s:%s", ret_pack.name, ret_pack.ver);
    return ret_pack;
}

int download_package(char*pack_ver, char*dir){
    if(chdir(dir)==-1){
        ERR("chdir");
    }
    package pack = getPack(pack_ver);
    FILE*fptr = fopen("requirements", "r+");
    char * line = NULL;
    size_t size = 0;
    size_t read;
    while((read = getline(&line, &size, fptr))!=-1){
        char *pkg = strtok(line,"\t\n");
        if(pkg && strcmp(pkg, pack.name)==0){
            perror("Package already downloaded\n");
            free(line);
            fclose(fptr);
            return 1;
        }
    }
    if(line)free(line);
    fprintf(fptr, "%s\t%s\n", pack.name,pack.ver);
    fclose(fptr);
    if(chdir("..")==-1)
        ERR("chdir");
    return 0 ;
}
int remove_package(char*pack_name, char*dir){
    if(chdir(dir)==-1){
        ERR("dir");
    }

    FILE*fptr = fopen("requirements", "r+");
    char * line = NULL;
    size_t size = 0;
    struct stat s;
    if(stat("requirements",&s)==-1){
        ERR("stat");
    }
    size_t read;
    size_t file_size = s.st_size-strlen(pack_name);

    char* temp_file_content = calloc(sizeof(char),file_size+1);
    while((read = getline(&line, &size, fptr))!=-1){
        char *pkg = strtok(line,"\t\n");
        char *ver = strtok(NULL,"\t\n");
        if(pkg && strcmp(pkg, pack_name)==0){
            printf("Package found\n");
            continue;
        }
        strncat(temp_file_content, pkg, strlen(pkg));
        strncat(temp_file_content, "\t", 2);
        strncat(temp_file_content, ver, strlen(ver));
    }
    printf("%s\n", temp_file_content);
    fclose(fptr);
    FILE*fptr2 = fopen("requirements", "w");
    fprintf(fptr2,"%s",temp_file_content);
    fclose(fptr2);
    free(temp_file_content);
    
return 0 ;
}   



int main(int argc, char**argv){
    int c;
    int version_switch =0;
    char*name =NULL;
    char* package =NULL;
    while((c=getopt(argc,argv, "c:v:i:r:"))!=-1){
        switch (c)
        {
        case 'c':
            name = optarg;
            create_enviroment(name);
            break;
        case 'v':
            name = optarg;
            version_switch =1;
            break;
        case 'i':
            if(version_switch!=1){
                perror("NO DIR");
                exit(EXIT_FAILURE);

            }
            package = optarg;
            download_package(package, name);
            break;
        case 'r':
            if(version_switch!=1){
                perror("NO DIR");
                exit(EXIT_FAILURE);
            }
            package = optarg;
            remove_package(package,name);
            break;
            
        default:
            perror("USAGE:p:v:i:r ");
            exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}