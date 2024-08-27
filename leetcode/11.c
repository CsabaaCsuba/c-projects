#include <stdio.h>
#include <stdlib.h>

int CalculateWaterBetweenSlopes(int* height, int Slope1Idx, int Slope2Idx){
    printf("Slope1 idx: %d\n", Slope1Idx);
    printf("Slope2 idx: %d\n", Slope2Idx);
    int WaterWidth = Slope2Idx - Slope1Idx;
    printf("Water width: %d\n", WaterWidth);
    printf("height[Slope1Idx]: %d\n", height[Slope1Idx]);
    printf("height[Slope2Idx]: %d\n", height[Slope2Idx]);
    int WaterHeight = 0;
    if (height[Slope1Idx] <= height[Slope2Idx]){
        WaterHeight = height[Slope1Idx];
    }
    else {
        WaterHeight = height[Slope2Idx];
    }
    printf("Water height: %d\n", WaterHeight);
    return WaterWidth * WaterHeight;
}


int BruteForceArea(int* height, int HeightSize){
    int AreaMax = 0;
    for (int i=0; i<HeightSize;i++){
        for (int j=0; j<HeightSize; j++){
            if (i-j * height[j] < AreaMax || i-j * height[i] < AreaMax){
                continue;
            }
            int CurrentWidth = i-j;
            int CurrentHeight;
            if (height[i] <= height[j]){
                CurrentHeight = height[i];
            }
            else {
                CurrentHeight = height[j];
            }
            if (CurrentHeight*CurrentWidth > AreaMax){
                AreaMax = CurrentHeight * CurrentWidth;
            }
        }
    }
    return AreaMax;
}

int* GetLargestIdx(int* height, int heightSize){
    int* largest = malloc(2*sizeof(int)+sizeof(int));
    //initialized the array's values for the comparison.
    largest[0] = -1;
    largest[1] = -1;
    //0 = number, 1 = index
    
    if (largest == NULL){
        printf("failed");
        return NULL;
        perror("program failed");
    }
    
    printf("last element: %d\n", height[heightSize-1]);
    for (int i=0; i<heightSize;i++){
        printf("iterated\n");
        if (height[i] > largest[0]){
            largest[0] = height[i];
            printf("New largest element: %d\n", largest[0]);
            largest[1] = i;
            printf("index of new largest element: %d\n", largest[1]);
        }
    }
    printf("Current largest: %d\n", largest[0]);
    return largest;
}

//sort the array, 
int maxArea(int* height, int heightSize) {
    // printf("HeightSize: %d\n", heightSize);


    // int Calculated = CalculateWaterBetweenSlopes(height, 1, 8);
    // int Calculated = BruteForceArea(height, heightSize);
    int* LargestArrPtr = GetLargestIdx(height, heightSize);
    if (LargestArrPtr == NULL){
        return -1;
    }
    int Calculated = LargestArrPtr[0];
    printf("Calculated water value: %d\n", Calculated);

    return Calculated;
}

int main(){
    int array[] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    printf("%d", maxArea((int*)&array, 9));
}