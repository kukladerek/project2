

#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;
struct NODE* last;
char* path;

struct NODE* nodeSearch(struct NODE* directory, char* dirName, char* parentName) {
    if(directory == NULL) { return NULL; }
    if((dirName != NULL) && (strcmp(directory->name, dirName) == 0) && (strcmp(directory->parentPtr->name, parentName) == 0)) { return directory; }
    struct NODE* found = nodeSearch(directory->siblingPtr, dirName, parentName);
    if(found != NULL) { return found; }
    return nodeSearch(directory->childPtr, dirName, parentName);
}

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW
    path = malloc(100);
    if(strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char* direct = malloc(100);
    char* base = malloc(100);

    struct NODE* returnNode = splitPath(pathName, base, direct);

    if(returnNode == NULL) {
        char* token = strtok(path, "/");
        struct NODE* checker = nodeSearch(root, token, "/");
        while(token != NULL) {
            if(checker == NULL) {
                break;
            }
            token = strtok(NULL, "/");
            checker = nodeSearch(root, token, checker->name);
        }
        printf("ERROR: directory %s does not exist\n", token);
        return;
    }
    if(nodeSearch(root, base, returnNode->name) != NULL) {
        printf("MKDIR ERROR: directory %s already exists\n", path);
        return;
    }
    struct NODE* newNode = malloc(sizeof(struct NODE));
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = returnNode;
    newNode->fileType = 'D';
    strcpy(newNode->name, base);
    if(returnNode->childPtr == NULL) { returnNode->childPtr = newNode; }
    else if(returnNode->childPtr != NULL) {
        last = returnNode->childPtr;
        while(1) {
            if(last->siblingPtr == NULL) {
                last->siblingPtr = newNode;
                break;
            }
            last = last->siblingPtr;
        }
    }
    printf("MKDIR SUCCESS: node %s successfully created\n", path);
    return;
}


//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    strcpy(path, pathName);
    if(strcmp(pathName, "/") == 0) {
        strcpy(baseName, pathName);
        strcpy(dirName, "");
        return root;
    }

    char* last = strrchr(pathName, '/');
    char* target = malloc(100);
    char* lateDir = malloc(100);

    if(last == NULL) {
        strcpy(baseName, pathName);
        strcpy(dirName, "");
        return cwd;
    }
    else {
        strcpy(baseName, last + 1);
        *last = '\0';
        strcpy(dirName, pathName);
        char* parentSlash = strrchr(pathName, '/');
        if(parentSlash == NULL) {
            strcpy(lateDir, "/");
            strcpy(target, pathName);
        }
        else {
            *parentSlash = '\0';
            char* lateName = strrchr(pathName, '/');
            strcpy(target, parentSlash + 1);
            strcpy(lateDir, pathName);
            if(lateName != NULL) {
                strcpy(lateDir, lateName + 1);
            }
        }
    }
    //
    strcpy(pathName, path);
    return nodeSearch(root, target, lateDir);
}
