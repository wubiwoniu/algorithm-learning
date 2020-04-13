#include <iostream>
using namespace std;

template <class T>
class merge
{
    private:
        T *a,*aux;
        int len;
    public:
        merge(int N);
        void sort(int low,int high);
        
};