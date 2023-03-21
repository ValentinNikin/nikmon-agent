
#include "app/Application.h"

#include <iostream>
#include <vector>
#include <deque>
#include <chrono>

struct TempStruct {
    int id;
    std::string data;
    float temp1;
    float temp2;
    float temp3;

    TempStruct(const int id_)
            : id(id_) {
        data = std::string("If both copy and move assignment operators are provided, overload resolution selects the move assignment if the argument is an rvalue (either a prvalue such as a nameless temporary or an xvalue such as the result of std::move), and selects the copy assignment if the argument is an lvalue (named object or a function/operator returning lvalue reference). If only the copy assignment is provided, all argument categories select it (as long as it takes its argument by value or as reference to const, since rvalues can bind to const references), which makes copy assignment the fallback for move assignment, when move is unavailable.\n"
                           "\n"
                           "It is unspecified whether virtual base class subobjects that are accessible through more than one path in the inheritance lattice, are assigned more than once by the implicitly-defined move assignment operator (same applies to copy assignment).\n") +
               std::to_string(id);
        temp1 = id * 100;
        temp2 = temp1 / 5.4;
        temp3 = temp1 * temp2;

//        std::cout << "Constructor " << id << std::endl;
    }

    TempStruct(const TempStruct& other) {
//        std::cout << "Copy constructor" << std::endl;
        id = other.id;
        data = other.data;
        temp1 = other.temp1;
        temp2 = other.temp2;
        temp3 = other.temp3;
    }

    TempStruct& operator=(const TempStruct& other) {
//        std::cout << "Copy assignment operator" << std::endl;
        if (this != &other) {
            id = other.id;
            data = other.data;
            temp1 = other.temp1;
            temp2 = other.temp2;
            temp3 = other.temp3;
        }
        return *this;
    }

    TempStruct(TempStruct&& other) noexcept {
        std::cout << "Move constructor" << std::endl;
        id = std::exchange(other.id, 0);
        data = std::move(other.data);
        temp1 = std::exchange(other.temp1, 0);
        temp2 = std::exchange(other.temp2, 0);
        temp3 = std::exchange(other.temp3, 0);
    }

    TempStruct& operator=(TempStruct&& other) noexcept {
        std::cout << "Move assignment operator" << std::endl;

        if (this != &other) {
            id = std::exchange(other.id, 0);
            data = std::move(other.data);
            temp1 = std::exchange(other.temp1, 0);
            temp2 = std::exchange(other.temp2, 0);
            temp3 = std::exchange(other.temp3, 0);
        }
        return *this;
    }

    ~TempStruct() {
//        std::cout << "Destructor" << std::endl;
    }
};

struct TimeMeter {
    TimeMeter() {
        start = std::chrono::high_resolution_clock::now();
    }

    ~TimeMeter() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << duration.count() << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start;
};

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::vector<std::shared_ptr<TempStruct>> arr;
    arr.reserve(1000000);

    std::cout << "-------- Initialize vector -------------" << std::endl;

    {
        TimeMeter tm1;
        for (std::size_t i = 0; i < 1000000; i++) {
            arr.emplace_back(std::make_shared<TempStruct>(i));
        }
    }

    std::cout << "----------------------" << std::endl;

    std::cout << "-------- Initialize deque -------------" << std::endl;

//    std::deque<TempStruct> deque(arr.begin(), arr.end());
    {
        TimeMeter tm2;
        std::deque<std::shared_ptr<TempStruct>> deque;
//        for (const auto& i : arr) {
//            deque.push_back(std::move(i));
//        }
        deque.insert(deque.end(), std::make_move_iterator(arr.begin()), std::make_move_iterator(arr.end()));
        arr.clear();
    }

    std::cout << "----------------------" << std::endl;

//    Application app;
//    app.run(argc, argv);
    return 0;
}