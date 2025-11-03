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

struct Node{
    char name[64];
    struct Node* next;
} typedef Node; 
struct Room
{
    char name[64];
    Node*neighbours;
    struct Room* next;
    
} typedef Room;
void usage(char*prog_name){
    fprintf(stderr,"USAGE: mapfile; %s\n", prog_name);
    exit(EXIT_FAILURE);
}
Room* traverse_head = NULL;
int MAX_DEPTH = 0;
Room* GetRoom(Room**head, const char* RoomName){
    if(*head==NULL){
        Room* newRoom = malloc(sizeof(Room));
        strcpy(newRoom->name, RoomName);
        newRoom->neighbours = NULL;
        newRoom->next =NULL;
        *head = newRoom;
        return newRoom;
    }
    else{
        
        Room*cur = *head;
        Room* temp = NULL;
        while(cur!=NULL){
            if(strcmp(cur->name, RoomName)==0){
                return cur;
            }
            temp = cur;
            cur=cur->next;
        }
        Room* newRoom = malloc(sizeof(Room));
        strcpy(newRoom->name, RoomName);
        newRoom->neighbours = NULL;
        newRoom->next =NULL;
        temp->next = newRoom;
        return newRoom;
    }
}
void addHall(Room * WRoom, const char* HallName){
    Node* new_hall = malloc(sizeof(Node));
    if(!new_hall){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(new_hall->name,HallName);
    new_hall->next = NULL;
    //printf("entering the rabitHall\n");
    if(WRoom->neighbours==NULL){
        WRoom->neighbours = new_hall;
        return;
    }
    else{
        Node* temp_hall= WRoom->neighbours;
        Node* temp;
        while (temp_hall)
        {
            temp = temp_hall;
            temp_hall = temp_hall->next;
        }
        temp->next =new_hall; 
        
    }
}

void DecodeMap(Room * head){
    for(Room*r = head; r;r=r->next){
        printf("%s ",r->name);
        for(Node* n = r->neighbours; n; n = n->next){
            printf("->%s",n->name);
        }
        printf("\n");
    }
};
void readMap(char*map_path, Room**head){
    FILE* fptr = fopen(map_path, "r");
    if(fptr == NULL){
        perror("fopen");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    char*line = NULL;
    size_t size = 0;



    while(getline(&line,&size,fptr)!=-1){
        if(line[0]=='#'){
            continue;
        }
        else{
            char * RoomName = strtok(line," ");
            char * HallName = strtok(NULL,"\t\n");
            if(RoomName && HallName){
                Room* room_to_work_with = GetRoom(head, RoomName);
                addHall(room_to_work_with, HallName);
                //printf("FOUND! [Room]%s: [Hall]%s\n",RoomName,HallName);
            }
        }
    }
    free(line);
    fclose(fptr);

}
void FreeMap(Room* head){
    Room* curr = head;
    while(curr){
        Room*next = curr->next;
        Node* current = curr->neighbours;
            while (current) {
                Node* next = current->next;
                free(current);
                current = next;
            }
        free(curr);
        curr = next;
    }
}
int traverse_dungeon(const char *fpath, const struct stat *s, int typeflag, struct FTW *ftwbuf){
   if (typeflag == FTW_D) {
        // current directory name
        const char *current = fpath + ftwbuf->base;

        // skip depth control or symlinks as needed
        if (ftwbuf->level > MAX_DEPTH) return 1;

        // parent directory (if not root)
        char parent_path[256];
        strcpy(parent_path, fpath);
        char *slash = strrchr(parent_path, '/');
        if (slash && ftwbuf->level > 0) {
            *slash = '\0';
            const char *parent = strrchr(parent_path, '/');
            if (!parent) parent = parent_path;
            else parent++;
            Room *parentRoom = GetRoom(&traverse_head, parent);
            addHall(parentRoom, current);
        } else {
            // root room
            GetRoom(&traverse_head, current);
        }
    }
    return 0;
}
int main(int argc, char**argv){
    if(argc!=2){
        usage(argv[0]);
    }
    MAX_DEPTH = atoi(argv[1]);
    if(MAX_DEPTH <= 0){
        usage(argv[0]);
    }
    if(nftw("./archive_test/real_archive", traverse_dungeon,20,FTW_PHYS)==-1){
        printf("traverse is over\n");
    };

    //Room * graph = NULL;
    //readMap(argv[1], &graph);
    //DecodeMap(graph);
    DecodeMap(traverse_head);
    FreeMap(traverse_head);
    //FreeMap(graph);
    return EXIT_SUCCESS;
}