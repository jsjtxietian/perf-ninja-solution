#include "solution.h"

#include <random>

#ifdef SOLUTION

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    // 80% reduction of cpu time
    // InstanceArray arrayA;
    // InstanceArray arrayB;
    // InstanceArray arrayC;
    // for (std::size_t i = 0; i < N; i++) {
    //     int value = distribution(generator);
    //     if (value == 0) {
    //         arrayA.push_back(std::make_unique<ClassA>());
    //     } else if (value == 1) {
    //         arrayB.push_back(std::make_unique<ClassB>());
    //     } else {
    //         arrayC.push_back(std::make_unique<ClassC>());
    //     }
    // }

    // array.insert( array.end(), std::make_move_iterator(arrayA.begin()), std::make_move_iterator(arrayA.end()) );
    // array.insert( array.end(), std::make_move_iterator(arrayB.begin()), std::make_move_iterator(arrayB.end()) );
    // array.insert( array.end(), std::make_move_iterator(arrayC.begin()), std::make_move_iterator(arrayC.end()) );
    
    // 98% reduction of cpu time
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
#ifdef SOLUTION
        item.handle(data);
#else
        item->handle(data);
#endif
    }
}
