/*
Name: Hady Hassan El Fadaly - ID: 20236113
Name: Mohamed Amr Ahmed - ID: 20236087
Name: Omar Waleed ElSobky - ID:20237008
*/

#include <bits/stdc++.h>

using namespace std;

class Heap
{

private:

    vector <int> arr;
    int size;

public:

    Heap(vector <int> arr)
    {

        this->arr = arr;
        size = this->arr.size();

        //build heap from given
        buildHeap();

    }

    void heapify(int i, int n)
    {

        //getting right child index and left child index
        int l = 2*i+1, r = 2*i+2, maximum = i;

        //making sure its a max heap
        if(l < n && arr[l] > arr[maximum]) maximum = l;
        if(r < n && arr[r] > arr[maximum]) maximum = r;
        if(maximum != i)
        {

            swap(arr[i], arr[maximum]);
            heapify(maximum, n); //heapify the subtree

        }

    }
    void buildHeap()
    {

        //heapify each level
        for(int i = (size/2)-1; i >= 0; i--) heapify(i, size);

    }
    void insert(int val)
    {

        //insert the new value at the end of the heap
        arr.push_back(val);
        size++;

        //rebuild the heap
        buildHeap();

    }
    int getMax()
    {

        if(size == 0)
        {

            cout<<"Heap is empty\n";


            return -1; //indication that heap is empty

        }

        return arr[0]; //first element as it is a max heap

    }
    int getMin()
    {

        if(size == 0)
        {

            cout<<"Heap is empty\n";


            return -1; //indication that heap is empty

        }

        int minimum = arr[size/2]; //first leaf node

        //iterate over all leaf nodes to find the minimum
        for(int i = size/2; i < size; i++) if(arr[i] < minimum) minimum = arr[i];

        return minimum;

    }
    void heapSort()
    {

        //building the heap before sorting
        buildHeap();

        for(int i = size-1; i > 0; i--)
        {

            swap(arr[0], arr[i]);
            heapify(0, i);

        }

    }

};
class PriorityQueue : Heap
{

private:

    //map to store priority of each element
    unordered_multimap<int, int> priorityMap;

public:

    PriorityQueue(vector <int> arr) : Heap(arr)
    {

        //map the priorities of the arr to be the val itself as a default value
        for(int val : arr) priorityMap.insert({val, val});

    }
    PriorityQueue() : Heap(vector <int> {}) //another constructor for empty priority queue
    {}

    //insert function to insert elements according to priority
    void insert(int priority, int val)
    {

        //inserting the value into the heap
        Heap::insert(priority);

        //mapping the value to its priority
        priorityMap.insert({priority, val});

    }
    int getHighestPriorityElement()
    {

        //get the highest priority from the heap
        int maxPriority = getMax();

        //return the element corresponding to that priority
        return priorityMap.find(maxPriority)->second;

    }

};

int main()
{

    PriorityQueue pq;

    pq.insert(5, 100);
    pq.insert(1, 200);
    pq.insert(3, 300);

    cout<<"Element with highest priority: "<<pq.getHighestPriorityElement()<<endl;

    Heap heap({4, 10, 3, 5, 1});

    cout<<"Maximum element in the heap: "<<heap.getMax()<<endl;
    cout<<"Minimum element in the heap: "<<heap.getMin()<<endl;

    heap.heapSort();

    cout<<"First element in the heap after sorting: "<<heap.getMax()<<endl;

    return 0;

}
