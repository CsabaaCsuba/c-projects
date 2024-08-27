#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_valid_raw(char* RawString, int StringLen){
    for(int i=0; i<StringLen;i++){
        if ((unsigned char)RawString[i] >> 3 == 30){
            if (i+3 > StringLen)
                    return false;
            for (int j=1; j<4; j++){
                if ((unsigned char)RawString[i+j] >> 6 != 2)
                    return false;
            }
            i+=3;
        }
        else if ((unsigned char)RawString[i] >> 4 == 14){
            if (i+2 > StringLen)
                    return false;
            for (int j=1; j<3; j++){
                if ((unsigned char)RawString[i+j] >> 6 != 2)
                    return false;
            }
            i+=2;
        }
        else if ((unsigned char)RawString[i] >> 5 == 6){
            i+=1;
            if (i > StringLen)
                return false;
            if ((unsigned char)RawString[i] >> 6 != 2)
                    return false;
        }

        else if ((unsigned char)RawString[i] >> 7 == 0){
        } 
        else return false;
    }

    return true;
}

int main(){
    char InvalidString[] = "\xf0\x28\x8c\x28";
    printf("validator returned: %d\n", is_valid_raw((char*)&InvalidString, strlen(InvalidString)));

    char ValidString[] = "𐌼";
    printf("validator returned: %d\n", is_valid_raw((char*)&ValidString, strlen(ValidString)));
    return 0;
}