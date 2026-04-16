/*
Name: Hady Hassan El Fadaly - ID: 20236113
Name: Mohamed Amr Ahmed - ID: 20236087
Name: Omar Waleed ElSobky - ID:20237008
*/

#include <bits/stdc++.h>

using namespace std;

int SequentialSearch(vector <int> arr,  int target)
{

    int size = arr.size();

    //if target found return index
    for(int i = 0; i < size; i++) if(arr[i] == target) return i;

    //if not found return -1 as indication that its not found
    return -1;

}
int RecursiveSequentialSearch(vector <int> arr,  int target)
{

    int size = arr.size();

    //base case return -1 if size is 0 as indication that target not found
    if(size == 0) return -1;
    if(arr[size-1] == target) return size-1; //if target found return index

    //recursive call on the rest of the array
    int res = RecursiveSequentialSearch(vector<int>(arr.begin(), arr.end()-1), target);

    return res;

}
int BinarySearch(vector <int> arr,  int target)
{

    int left = 0, right = arr.size() - 1;

    //if left index exceeds right index then target not found
    while(left <= right)
    {

        int mid = left + (right - left) / 2;

        //check if target is present at mid
        if(arr[mid] == target) return mid;

        //if target greater than mid ignore left half and start searching in right half
        if(arr[mid] < target) left = mid + 1;
        else right = mid - 1; //if target smaller than mid ignore left half and start searching in right half

    }

    //indication that target was not found in the array
    return -1;

}
int RecursiveBinarySearch(vector <int> arr,  int target, int left, int right)
{

    //base case if left index exceeds right index then target not found so return -1 as indication that target not found
    if(left > right) return -1;

    int mid = left + (right - left) / 2;

    //check if target is present at mid
    if(arr[mid] == target) return mid;

    //if target greater than mid ignore left half and start searching in right half
    if(arr[mid] < target) return RecursiveBinarySearch(arr, target, mid + 1, right);
    else return RecursiveBinarySearch(arr, target, left, mid - 1); //if target smaller than mid ignore left half and
    //start searching in right half

}

int main()
{

    cout<<"Enter number of elements in the array: ";

    int n, target;

    cin>>n;

    vector <int> arr(n);

    cout<<"Enter the elements of the array: ";

    for(int i = 0; i < n; i++) cin>>arr[i];

    cout<<"Enter the target element to search: ";

    cin>>target;

    int result = RecursiveBinarySearch(arr, target, 0, arr.size()-1);

    if(result != -1) cout<<"Element found at index: "<<result<<endl;
    else cout<<"Element not found in the array."<<endl;

    return 0;

}

