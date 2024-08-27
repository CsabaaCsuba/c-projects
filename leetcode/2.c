/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


//First I thought reversing here would be a logical step.
//It is not, since additional calculations would have to be done when summing the array values.
int* ShrinkArray(int* Array, int* ArraySize){ //this also shortens to optimal size.
    int arraySize = 0;
    for (int i=0; i<100; i++){
        if (Array[i] < 0 || Array[i] > 9){
            arraySize = i;
            break;
        }
            
    }
    
    *ArraySize = arraySize;
    // int *NewArray = malloc(arraySize+1*sizeof(int));
    int *NewArray = calloc(arraySize+1, sizeof(int));
    for (int i=0; i<arraySize; i++){
        // printf("i equals %d\n", i);
        printf("current idx: %d\n", i);
        printf("value here will be: %d\n", Array[i]);
        NewArray[i] = Array[i];
    }
    return NewArray;
}

int* GetListNodeValues(struct ListNode* NodePtr){
    int* NumberValues = malloc(100*sizeof(int)); //max amount of listnodes is 100
    for (int i=0; i<100 && NodePtr != NULL; i++){
        NumberValues[i] = NodePtr->val;
        NodePtr = NodePtr->next;
    }
    return NumberValues;
}


struct ListNode* ReverseConvertNumberToListNode(int Number){
    //807 -> [7, 0, 8]
    int i=2;
    struct ListNode ReturnNode = {
        .val = Number % 10,
        .next = NULL
    };
    struct ListNode* ReturnNodePtr = &ReturnNode;
    do { 
        int CurrentNum = (int)Number % (int)pow(10, i);
        printf("current num: %d", CurrentNum);
        printf("Size of listnode: %d", sizeof(struct ListNode));
        struct ListNode* NextNodePtr = malloc(sizeof(struct ListNode));
        NextNodePtr->val = (int)Number % (int)pow(10, i);
        NextNodePtr->next = NULL;
        ReturnNode.next=NextNodePtr;
        ReturnNode=*NextNodePtr;
        i++;
    } while ((int)Number % (int)pow(10, i) != (int)Number); //The loop won't do an other iteration if the node is done.

    return ReturnNodePtr;
}

struct ListNode* SumTwoListsAndReverseIntoListNode(int *NumList1, int * NumList2, int ArraySize1, int ArraySize2){
    int Sum1 = 0;
    int Sum2 = 0;
    for (int i=0; i<ArraySize1;i++){    
        printf("Current Array1 element: %d\n", NumList1[i]);
        Sum1 += NumList1[i]*pow(10, i);
    }

    for (int i=0; i<ArraySize2;i++){
        Sum2 += NumList2[i]*pow(10, i);
    }
    int MainSum = Sum1+Sum2;
    printf("sum 1: %d\n", Sum1);
    printf("sum 2: %d\n", Sum2);
    printf("Main sum: %d\n", MainSum);

    // struct ListNode EndNode = {1, NULL};
    struct ListNode* EndNodePtr = ReverseConvertNumberToListNode(MainSum);
    return EndNodePtr;
}


struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    int* Nums = GetListNodeValues(l1);
    int* Nums2 = GetListNodeValues(l2);
    int ArraySize1 = 0; 
    int ArraySize2 = 0;
    int* ReversedArray1 = ShrinkArray(Nums, &ArraySize1);
    // printf("Array 1 in main: \n\n");
    // for (int i=0; i<ArraySize1; i++){
    //     printf("%d\n", ReversedArray1[i]);
    // }
    int* ReversedArray2 = ShrinkArray(Nums2, &ArraySize2);

    struct ListNode* FinalNode = SumTwoListsAndReverseIntoListNode(
        ReversedArray1,
        ReversedArray2,
        ArraySize1,
        ArraySize2
    );
    
    return FinalNode;
}