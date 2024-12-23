#include "solution.h"

#include <iostream>
#include <random>

void originalGenerateObjects(InstanceArray& array) {
#if SOLUTION
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);
        if (value == 0) {
            array.push_back(ClassD(0));
        } else if (value == 1) {
            array.push_back(ClassD(1));
        } else {
            array.push_back(ClassD(2));
        }
    }
#else
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
#endif
}

void originalInvoke(InstanceArray& array, std::size_t& data) {
    for (const auto& item: array) {
#ifdef SOLUTION
        item.handle(data);
#else
        item->handle(data);
#endif
    }
}

int main() {
    InstanceArray arr;
    generateObjects(arr);

    std::size_t data = 0;
    invoke(arr, data);

    InstanceArray arrReference;
    originalGenerateObjects(arrReference);

    std::size_t reference = 0;
    originalInvoke(arrReference, reference);

    if (data != reference) {
        std::cerr << "Result = " << data
                  << ". Expected result = " << reference << std::endl;
        return 1;
    }

    std::cout << "Validation Successful" << std::endl;
    return 0;
}
