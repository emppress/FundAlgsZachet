#include <stdio.h>

// возвращает индекс найденного элемента
int bin_search(int const *arr, int value, int size_arr)
{
    int l, r, mid;
    l = 0, r = size_arr - 1;
    while (l <= r)
    {
        mid = l + (r - l) / 2;
        if (arr[mid] == value)
        {
            return mid;
        }
        if (arr[mid] > value)
        {
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    return -1;
}

int main()
{
    int arr[] = {1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9};

    int idx = bin_search(arr, 3, 11);
    return 0;
}