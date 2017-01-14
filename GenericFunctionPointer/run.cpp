#include "GenericFunctionPointer.hpp"

#include <memory>
#include <iostream>


namespace {
    // Free Function
    int Mul(int lhs, int rhs)
    {
        return lhs * rhs;
    }
} // anonymous namespace


class GenFuncWrapper {
public:
    GenFuncWrapper(std::unique_ptr<BinaryOperationType> func)
      : func_(std::move(func))
    {}

    int operator() (int lhs, int rhs)
    {
        return (*func_)(lhs, rhs);
    }

private:
    std::unique_ptr<BinaryOperationType> func_;
};


int main()
{
    GenFuncWrapper add(std::make_unique<BinaryOperationType>(Add()));
    auto result = add(10, 20);
    std::cout << "- Function Object Add: " << result << std::endl;

    GenFuncWrapper sub(std::make_unique<BinaryOperationType>(Sub()));
    result = sub(20, 10);
    std::cout << "- Function Object Sub: " << result << std::endl;

    GenFuncWrapper mul(std::make_unique<BinaryOperationType>(Mul));
    result = mul(10, 20);
    std::cout << "-   Free Function Mul: " << result << std::endl;

    GenFuncWrapper div(std::make_unique<BinaryOperationType>(
        [](int lhs, int rhs) {return lhs / rhs;}));
    result = div(20, 10);
    std::cout << "- Lamdba Function Div: " << result << std::endl;

    return 0;
}