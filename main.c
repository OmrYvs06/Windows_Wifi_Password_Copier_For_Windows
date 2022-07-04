#include <stdio.h>
#include <stdlib.h>

int findFunction(const char* mainStr, const char* findStr){ // it is not working every str, but it works for this code
    int i=0,j=0;
    int flag = 0;
    while(mainStr[i] != '\0'){
        if(mainStr[i++] == findStr[j++]){
            if(findStr[j] == '\0'){
                flag = 1;
                break;
            }
        }
        else
            j = 0;
    }

    if(flag){
        return i;
    }else{
        return -1;
    }
}
int findThFile(char* name){
    FILE* keyFptr;
    if((keyFptr = fopen(name, "r")) == NULL){
        fclose(keyFptr);
        return 0;
    }

    char* string = malloc(255*sizeof(char));
    int i;
    for (i = 0; name[i] != '\0'; i++)
        string[i] = name[i];
    string[i] = '\0';

    for(i = 0 ; string[i] != '\0' ; i++);
    i--;

    while(string[i] != '.')
        i--;

    while(string[i] != '\0')
        string[i] = '\0';

    char* newName = malloc(18*sizeof(char));
    i = 0;
    do{
        i++;
        snprintf(newName,255,"%s (%d).txt",string,i);
    }while((keyFptr = fopen(newName, "r")) != NULL);
    fclose(keyFptr);
    return i;
}

int main() {
    system("netsh wlan show profile > names.txt");
    FILE* ProfilesFptr = fopen("names.txt", "r");
    FILE* keyFptr;
    int a = findThFile("keys.txt");
    if(a == 0){
        keyFptr = fopen("keys.txt", "w");
    }else{
        char* name = malloc(255*sizeof(char));
        snprintf(name,18,"keys (%d).txt",a);
        keyFptr = fopen(name, "w");
        free(name);
    }


    char *Interface = malloc(255*sizeof(char));
    while(!feof(ProfilesFptr)){
        char *String = malloc(255 * sizeof(char));
        char *ProfileName = malloc(255 * sizeof(char));
        fgets(String, 255, ProfilesFptr);
        
        int x = findFunction(String, "Profiles on interface ");
        if(x != -1){
            fprintf(keyFptr, "\n%s\n", String + x);
            snprintf(Interface,255* sizeof(char), "%s", String + x);
            {
                int i;
                for(i = 0; Interface[i] != '\n' && Interface[i] != '\0' ;i++);
                Interface[i-1] = '\0';
                Interface[i] = '\0';
            }
        }
		x = findFunction(String, "All User Profile     : ");
        if (x == -1)
            continue;
        char *cmdStr = malloc(255 * sizeof(char));
        snprintf(ProfileName, 255 * sizeof(char), "%s", String + x);
        {
            int i;
            for (i = 0; ProfileName[i] != '\n' && ProfileName[i] != '\0'; i++);
            ProfileName[i] = '\0';
        }
        snprintf(cmdStr,
                 255*sizeof(char),
                 "netsh wlan show profile name = \"%s\" Interface = \"%s\" key=clear  > key.txt",
                 ProfileName, Interface);

        system(cmdStr);
        FILE* fptr3 = fopen("key.txt", "r");
        int y;
        do{
            fgets(cmdStr, 255, fptr3);
            y = findFunction(cmdStr, "Key Content            : ");
        }while(y == -1 && !feof(fptr3));
        if(y == -1){
            fclose(fptr3);
            continue;
        }
        fprintf(keyFptr, "%-20s : %s", ProfileName, cmdStr + y);
        fclose(fptr3);
        free(String);
        free(cmdStr);
        free(ProfileName);
    }

    free(Interface);
    fclose(ProfilesFptr);
    fclose(keyFptr);
    system("del names.txt key.txt");
    return 0;
}
