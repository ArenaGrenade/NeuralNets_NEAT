#ifndef lint
#define lint long long int
#endif

class InnovationCounter {
    private:
    lint present_count;
    public:
    InnovationCounter() {
        //the counter is 1-indexed.
        present_count = 1;
    }

    //this is the main function of the counter - returns the value and increments the counter.
    lint getInnovationNumber() {
        return present_count++;
    }
};