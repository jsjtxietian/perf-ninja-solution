#include "wordcount.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

// Assumptions
// 1. Function should read the input from the file, i.e. caching the input is
// not allowed.
// 2. The input is always encoded in UTF-8.
// 3. Break only on space, tab and newline (do not break on non-breaking space).
// 4. Sort words by frequency AND secondary sort in alphabetical order.

// Implementation rules
// 1. You can add new files but dependencies are generally not allowed unless it
// is a header-only library.
// 2. Your submission must be single-threaded, however feel free to implement
// multi-threaded version (optional).
#ifdef SOLUTION
#include "MappedFile.hpp"
#include <memory_resource>
#include <cstring>

std::vector<WordCount> wordcount(std::string filePath)
{
	std::cout << "Opt Solution.\n";

	MappedFile mappedFile = MappedFile{filePath};
	std::string_view content = mappedFile.getContents();
	size_t fileLength = content.length();

	void* buffer = malloc(fileLength*3);
    std::pmr::monotonic_buffer_resource pool(buffer, fileLength*3);
	std::pmr::unordered_map<std::pmr::string, int> m(&pool);
	m.max_load_factor(0.5);

	std::vector<WordCount> mvec;

	size_t word_start = 0;
	for (size_t i = 0; i < fileLength; ++i) {
		if (content[i] == ' ' || content[i] == '\n' || content[i] == '\t'
			|| content[i] == '\r' || content[i] == '\v' || content[i] == '\f' ) {
			if (i > word_start) {
				std::string_view word_view(content.data() + word_start, i - word_start);
				m[std::pmr::string(word_view,&pool)]++;
			}
			word_start = i + 1;
		}
	}
	if (fileLength > word_start) {
		std::string_view word_view(content.data() + word_start, fileLength - word_start);
		m[std::pmr::string(word_view,&pool)]++;
	}

	mvec.reserve(m.size());
	for (auto &p : m)
		mvec.emplace_back(WordCount{p.second, std::string(p.first.begin(),p.first.end())});

	std::sort(mvec.begin(), mvec.end(), std::greater<WordCount>());
	return mvec;
}

#else

#include "MappedFile.hpp"

std::vector<WordCount> wordcount(std::string filePath)
{
	std::cout << "Opt Solution.\n";
	std::unordered_map<std::string, int> m;
	m.max_load_factor(0.5);
	std::vector<WordCount> mvec;

	MappedFile mappedFile = MappedFile{filePath};
	std::string_view content = mappedFile.getContents();
	size_t fileLength = content.length();
	std::string s;

	size_t word_start = 0;
	for (size_t i = 0; i < fileLength; ++i) {
		if (content[i] == ' ' || content[i] == '\n' || content[i] == '\t'
			|| content[i] == '\r' || content[i] == '\v' || content[i] == '\f' ) {
			if (i > word_start) {
				std::string_view word_view(content.data() + word_start, i - word_start);
				m[std::string{word_view}]++;
			}
			word_start = i + 1;
		}
	}
	if (fileLength > word_start) {
		std::string_view word_view(content.data() + word_start, fileLength - word_start);
		m[std::string{word_view}]++;
	}

	mvec.reserve(m.size());
	for (auto &p : m)
		mvec.emplace_back(WordCount{p.second, move(p.first)});

	std::sort(mvec.begin(), mvec.end(), std::greater<WordCount>());
	return mvec;
}

// Baseline solution.
// Do not change it - you can use for quickly checking speedups
// of your solution agains the baseline, see check_speedup.py

// std::vector<WordCount> wordcount(std::string filePath)
// {
// 	std::unordered_map<std::string, int> m;
// 	m.max_load_factor(0.5);

// 	std::vector<WordCount> mvec;

// 	std::ifstream inFile{filePath};
// 	if (!inFile)
// 	{
// 		std::cerr << "Invalid input file: " << filePath << "\n";
// 		return mvec;
// 	}

// 	std::string s;
// 	while (inFile >> s)
// 		m[s]++;

// 	mvec.reserve(m.size());
// 	for (auto &p : m)
// 		mvec.emplace_back(WordCount{p.second, move(p.first)});

// 	std::sort(mvec.begin(), mvec.end(), std::greater<WordCount>());
// 	return mvec;
// }
#endif

// test
// std::cout << mvec.size() << ' ' << mvec[0].count << mvec[0].word
// 	<< mvec[100].count << mvec[100].word <<  mvec[5555].count << mvec[5555].word
// 	<< mvec[mvec.size()-1].count << mvec[mvec.size()-1].word;
// for(const auto& v: mvec)
// {
// 	std::cout << v.word << ' ' << v.count << '\n';
// }