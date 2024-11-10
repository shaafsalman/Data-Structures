#include "../Task 2/heap.cpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <cstdlib>
using namespace std;

int test_insert(shared_ptr<MinHeap> heap, vector<int> ref_vec)
{
	cout << "Testing Insert: ";

	int counter = 0;
	int len = ref_vec.size();
	shared_ptr<int> head = heap->getHeap();

	for (int i = 0; i < len; i++)
	{
		if (head.get()[i] == ref_vec[i])
		{
			counter++;
		}
	}

	int score = (counter / ref_vec.size()) * 5;
	cout << score << "/5" << endl;
	return score;
}

int test_delete(shared_ptr<MinHeap> heap, vector<int> ref_vec)
{
	cout << "Testing Delete: ";

	int counter = 0;
	int len = ref_vec.size();

  cout << "Initial heap state: ";
    shared_ptr<int> head = heap->getHeap();
    for (int i = 0; i < len; i++) {
        cout << head.get()[i] << " ";
    }
    cout << endl;

	shared_ptr<int> head = heap->getHeap();

	for (int i = 0; i < len; i++)
	{
		if (head.get()[i] == ref_vec[i])
		{
			counter++;
		}
		 else
        {
            std::cout << "Mismatch: Expected " << ref_vec[i] << ", got " << head.get()[i] << std::endl;
        }
	}

	int score = (counter / ref_vec.size()) * 5;
	cout << score << "/5" << endl;
	return score;
}

int test_extractMin(shared_ptr<MinHeap> heap, vector<int> ref_vec)
{
	cout << "Testing ExtractMin: ";

	int counter = 0;
	sort(ref_vec.begin(), ref_vec.end());
	int len = ref_vec.size();

	for (int i = 0; i < len; i++)
	{
		if (heap->extractMin() == ref_vec[i])
		{
			counter++;
		}
		 else
        {
            std::cout << "Mismatch: Expected " << ref_vec[i] << ", got " << heap->extractMin() << std::endl;
        }
	}

	int score = (counter / ref_vec.size()) * 2;
	cout << score << "/2" << endl;
	return score;
}

int test_decreaseKey(shared_ptr<MinHeap> heap, vector<int> ref_vec)
{
	cout << "Testing DecreaseKey: ";

	int counter = 0;
	int len = ref_vec.size();
	shared_ptr<int> head = heap->getHeap();

	for (int i = 0; i < len; i++)
	{
		if (head.get()[i] == ref_vec[i])
		{
			counter++;
		}
	}

	int score = (counter / ref_vec.size()) * 3;
	cout << score << "/3" << endl;
	return score;
}

int main()
{
	shared_ptr<MinHeap> heap(new MinHeap(100));
	vector<int> ref_vec;

	int heap_values[40] = {34, 7, 53, 12, 68, 90, 24, 71, 13, 3, 1, 61, 18, 30, 50, 11, 52, 54, 56, 55, 80, 97, 91, 23, 29, 33, 73, 77, 89, 16, 20, 35, 43, 47, 57, 66, 88, 2, 9, 64};
	int vector_values[40] = {1, 2, 16, 3, 7, 23, 18, 13, 9, 55, 12, 24, 33, 53, 20, 35, 47, 54, 11, 64, 80, 97, 91, 90, 29, 61, 73, 77, 89, 50, 30, 71, 43, 52, 57, 66, 88, 56, 34, 68};

	for (int i = 0; i < 40; i++)
	{
		heap->insertKey(heap_values[i]);
		ref_vec.push_back(vector_values[i]);
	}

	cout << "Starting Tests\n"
		 << endl;
	int score = 0;
	/* Testing Insertion*/
	score += test_insert(heap, ref_vec);

	int del_values[9] = {5, 11, 17, 29, 33, 21, 34, 5, 9};
	for (int i = 0; i < 9; i++)
	{
		heap->deleteKey(del_values[i]);
	}

	int remaining_values[32] = {1, 2, 16, 3, 7, 33, 18, 13, 9, 43, 12, 34, 61, 53, 20, 35, 47, 56, 11, 64, 80, 57, 91, 90, 68, 66, 73, 77, 89, 88, 30, 71};
	ref_vec.clear();
	for (int i = 0; i < 32; i++)
	{
		ref_vec.push_back(remaining_values[i]);
	}

	/* Testing Delete*/
	score += test_delete(heap, ref_vec);

	/* Testing ExtractMin*/
	score += test_extractMin(heap, ref_vec);

	heap.reset(new MinHeap(12));
	int final_values[12] = {121, 51, 111, 31, 101, 21, 91, 41, 81, 11, 71, 61};
	int decreased_values[12] = {2, 19, 11, 20, 41, 31, 91, 121, 21, 101, 71, 111};
	ref_vec.clear();
	for (int i = 0; i < 12; i++)
	{
		heap->insertKey(final_values[i]);
		ref_vec.push_back(decreased_values[i]);
	}

	heap->decreaseKey(3, 20);
	heap->decreaseKey(8, 19);
	heap->decreaseKey(5, 2);

	score += test_decreaseKey(heap, ref_vec);

	cout << "\nTotal Score: " << score << "/15" << endl;

	return 0;
}
