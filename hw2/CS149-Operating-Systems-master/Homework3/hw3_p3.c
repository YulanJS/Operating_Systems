monitor bounded_buffer {
    int items[MAX_ITEMS]; /* MAX_ITEM is a constant defined elsewhere */
    int numItems = 0; /* # of items in the items array */
    condition full, empty;
    void produce(int v); /* deposit the value v to the items array */
    int consume(); /* remove the last item from items, and return the value */
    
    void produce(int v) {
        while (numItems == MAX ITEMS) 
            full.wait();
        items[numItems++] = v;
        empty.signal();
    }
    int consume() {
        int tempVal;
        while (numItems == 0) 
            empty.wait();    
        tempVal = items[--numItems];
        full.signal();
            return tempVal;
    }
}