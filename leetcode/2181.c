/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


typedef struct {
    int sum;
    struct ListNode* nextPtr;
} SumAndNextNodePtr;

SumAndNextNodePtr SumListNodeElementsUntilZero(struct ListNode* ptrNode){
    int NodeValue = 0;
    while (true){
        int CurrentVal = ptrNode->val;
        ptrNode = ptrNode->next;
        
        // printf("Current value: %d\n", CurrentVal);
        if (CurrentVal == 0){
            // printf("value is 0. returning from function...\n");
            SumAndNextNodePtr ReturnValues = {NodeValue, ptrNode};
            return ReturnValues;
        }
        
        NodeValue += CurrentVal;
        
    }
}

struct ListNode* mergeNodes(struct ListNode* head) {
    int MAX_LEN = 200000;
    int *integer_array = (int*)calloc(MAX_LEN, sizeof(int));
    // int *NodePartSums[MAX_LEN] = integer_array;
    struct ListNode* head2 = head->next; //skip first value
    for (int i=0; head2 != NULL;i++){
        SumAndNextNodePtr returned = SumListNodeElementsUntilZero(head2);
        integer_array[i] = returned.sum;
        head2 = returned.nextPtr;
        //because the current value is 0, it wasn't added, and the ptr can be skipped to the next one
        // head2 = head2->next;
        
    }

    head->val = integer_array[0];

    struct ListNode* head3 = head;

    for (int i=1; i<MAX_LEN; i++){
        if (integer_array[i] == 0){
            head3->next = NULL;
            break;
        }
        // printf("integer_array[%d] = %d\n", i, integer_array[i]);
        struct ListNode NewNode = {
            .val=integer_array[i],
            .next=head3->next->next,
        };
        *head3->next = NewNode;
        head3 = head3->next;
    }    
    
    return head;
}