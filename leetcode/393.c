bool is_valid_raw(int* RawString, int StringLen){
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

bool validUtf8(int* data, int dataSize) {
    bool is_valid = is_valid_raw(data, dataSize);
    return is_valid;
}