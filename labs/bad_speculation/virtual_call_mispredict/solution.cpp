#include "solution.h"

#include <random>

#ifdef SOLUTION

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    int count0 = 0,count1 = 0,count2 = 0;

    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);
        if (value == 0) {
            count0++;
        } else if (value == 1) {
            count1++;
        } else {
            count2++;
        }
    }

    while (count0 != 0)
    {
        array.push_back(std::make_unique<ClassA>());
        count0--;
    }
    while (count1 != 0)
    {
        array.push_back(std::make_unique<ClassB>());
        count1--;
    }
     while (count2 != 0)
    {
        array.push_back(std::make_unique<ClassC>());
        count2--;
    }
    
}

#else

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);
        if (value == 0) {
            array.push_back(std::make_unique<ClassA>());
        } else if (value == 1) {
            array.push_back(std::make_unique<ClassB>());
        } else {
            array.push_back(std::make_unique<ClassC>());
        }
    }
}
#endif

// Invoke the `handle` method on all instances in `output`
void invoke(InstanceArray& array, std::size_t& data) {
    for (const auto& item: array) {
        item->handle(data);
    }
}
