/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 2;
    int* SolutionArr = malloc(*returnSize*sizeof(int));
    int DesiredNum;
    for (int i=0; i<numsSize; i++){
        SolutionArr[0] = i;
        for (int j=0; j<numsSize; j++){
            //the same index cannot be used twice.
            if (nums[j] == target - nums[i] && j != i){
 
                SolutionArr[1] = j;
                return SolutionArr;
            }

        }

    }
    *returnSize = 0;
    return NULL;
}