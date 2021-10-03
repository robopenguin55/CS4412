#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}



//void qSort(int[] list, int left, int right)
//{
//    int lastsmall, foo;
//    if (left < right)
//    {
//        lastsmall = left;
//        for (int i = left + 1; i <= right; i++)
//        {
//            if (list[i] < list[left])
//            {
//                lastsmall++;
//                foo = list[i];
//                list[i] = list[lastsmall];
//                list[lastsmall] = foo;
//            }
//        }
//        foo = list[left];
//        list[left] = list[lastsmall];
//        list[lastsmall] = foo;
//
//        qSort(list, left, lastsmall - 1);
//        qSort(list, lastsmall + 1, right);
//    }
//}

