#include <algorithm>
#include <cassert>
#include <vector>

void sort_merge(const int* source, const int start, const int end, int* dest) {
	int l = start;
	int m = (end + start) / 2;
	int r = m;
	for (int j = start; j < end; ++j) {
		if (l < m && (r >= end || source[l] <= source[r])) {
			dest[j] = source[l];
			l++;
		} else {
			dest[j] = source[r];
			r++;
		}
	}
}

void sort_split(int* source, const int start, const int end, int* dest) {
	if (end - start < 2) {
		return;
	}
	const int mid = (end + start) / 2;
	sort_split(source, start, mid, dest);
	sort_split(source, mid, end, dest);
	sort_merge(source, start, end, dest);
	std::copy_n(dest + start, end - start, source + start);
}

std::vector<int> sort(std::vector<int>& arrayToSort) {
	const int head = 0;
	std::vector<int> result;
	result.reserve(arrayToSort.size());
	sort_split(arrayToSort.data(), head, arrayToSort.size(), result.data());
	return result;
}

int main(int argc, char** argv) {
	std::vector<int> arr;
	for (int i = 100; i > 0; --i) {
		arr.push_back(i);
	}
	std::iota(arr.begin(), arr.end(), 0);
	std::shuffle(arr.begin(), arr.end(), std::mt19937_64{std::random_device{}()});

	std::vector<int> sortedArr = sort(arr);

	for (int i = 1; i < 100; ++i) {
		assert(sortedArr[i - 1] < sortedArr[i]);
	}

	return 0;
}
