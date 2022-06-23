#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

int findFunction(const char* mainStr, const char* findStr){ // its not working every str but it work for this code
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

int main() {
    //system("netsh wlan show profile name = \"Kaya_Home\" key=clear > x.txt");
    system("netsh wlan show profile > names.txt");
    FILE* fptr = fopen("names.txt","r");
    FILE* fptr2 = fopen("keys.txt", "w");
    while(!feof(fptr)){
        char *str1 = malloc(255*sizeof(char));
        char *str2 = malloc(255*sizeof(char));
        char *str3 = malloc(255*sizeof(char));
        fgets(str1, 255, fptr);
        int x = findFunction(str1, "All User Profile     : ");
        if (x == -1)
            continue;
        snprintf(str3,255* sizeof(char), "%s", str1 + x);
        int i;
        for(i = 0; str3[i] != '\n' && str3[i] != '\0' ;i++);
        str3[i] = '\0';
        snprintf(str2,255*sizeof(char),"netsh wlan show profile name = \"%s\" key=clear > key.txt",str3);
        system(str2);
        FILE* fptr3 = fopen("key.txt", "r");
        int y;
        do{
            fgets(str2, 255, fptr3);
            y = findFunction(str2,"Key Content            : ");
        }while(y == -1 && !feof(fptr3));
        if(y == -1){
            fclose(fptr3);
            continue;
        }
        fprintf(fptr2, "%-20s : %s\n", str3, str2 + y);
        fclose(fptr3);
        free(str1);
        free(str2);
        free(str3);
    }


    fclose(fptr);
    fclose(fptr2);
    system("del names.txt key.txt");
    return 0;
}
