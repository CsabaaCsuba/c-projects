#ifndef matrix
    #define struct matrix{
    double *data;
    int cols;
    int rows;
    };
#endif

void ParseFileIntoMatrix(char* filename, struct matrix* MatrixPtr, int NeuronLength, int maxlines){
    FILE *fp;
    fp = fopen((const char * restrict) filename, "r");

    if (fp == NULL){
        printf("fp is null.");
        return;
    }
    char buffer[8192];
    fgets(buffer, 8192, fp); //skips the first line;
    unsigned char num;
    int LineCount = 0; //line count shouldn't be able to be higher than the neuron length;
    int NumbersPerLine = 0;
    bool NewLine = false;
    char ch = fgetc(fp);
    unsigned long int i = 0;
    do {
        if (NewLine){
            if (LineCount > maxlines){
                printf("all the data have been read\n");
                break;
            }
            unsigned char trash;
            NewLine = false;
            fscanf(fp, "%hhu", &trash);
            // printf("First num in line doesn't count\n");
            continue;
        }
        if (fscanf(fp, "%hhu", &num) == 1){ //the return value should be one, for one found value.
            // printf("number: %hhu - count: %d - i: %ld\n", num, NumbersPerLine, i);
            MatrixPtr->data[LineCount*NeuronLength+NumbersPerLine] = (double)num/255.0;
            NumbersPerLine++;
            i++;
        }
        if (fgetc(fp) == '\n'){
                NewLine = true;
                LineCount += 1;
                NumbersPerLine = 0;
        }
        
        
        
    } while (ch != EOF);

    printf("done. numbers scanned: %ld", i);
    fclose(fp);
}