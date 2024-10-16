#include <cstdint>
#include <vector>
#include <memory>

// Assume this constant never changes
constexpr std::size_t N = 64 * 1024;

struct BaseClass {
    virtual ~BaseClass() = default;

    virtual void handle(std::size_t& data) const = 0;
};

struct ClassA : public BaseClass {
    void handle(std::size_t& data) const override {
        data += 1;
    }
};

struct ClassB : public BaseClass {
    void handle(std::size_t& data) const override {
        data += 2;
    }
};

struct ClassC : public BaseClass {
    void handle(std::size_t& data) const override {
        data += 3;
    }
};

class ClassD {
private:
    int type;

public:
    ClassD(int _type) {
        type = _type;
    }
    void handle(std::size_t& data) const{
        if(type == 0) {
            data += 1;
        } else if(type == 1) {
            data += 2;
        }
        else{
            data += 3;
        }
    }
};

#if SOLUTION
using InstanceArray = std::vector<ClassD>;
#else
using InstanceArray = std::vector<std::unique_ptr<BaseClass>>;
#endif

void generateObjects(InstanceArray& array);
void invoke(InstanceArray& array, std::size_t& data);
