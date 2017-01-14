#pragma once

#include <functional>
#include <memory>
#include <iostream>


using BinaryOperationType = decltype(std::function<int(int, int)>());


// Functor
class BinaryOperation {
public:
    virtual int operator() (int, int) = 0;
    virtual ~BinaryOperation() = default;
};


class Add : public BinaryOperation {
public:
    int operator() (int lhs, int rhs)
    {
        setLhs(lhs);
        setRhs(rhs);
        return doAdd();
    }

private:
    void setLhs(int lhs)
    {
        lhs_ = lhs;
    }

    void setRhs(int rhs)
    {
        rhs_ = rhs;
    }

    int doAdd()
    {
        return lhs_ + rhs_;
    }

private:
    int lhs_;
    int rhs_;
};


class Sub: public BinaryOperation {
public:
    int operator() (int lhs, int rhs)
    {
        setLhs(lhs);
        setRhs(rhs);
        return doSub();
    }

    Sub()
      : lhs_(nullptr),
        rhs_(nullptr)
    {}

    Sub(const Sub& copy)
    {
        lhs_.reset(copy.rhs_.get());
        rhs_.reset(copy.rhs_.get());
    }

    Sub(Sub&& move)
    {
        lhs_.reset(move.rhs_.get());
        rhs_.reset(move.rhs_.get());
    }

private:
    struct Integer {
        int value;
        Integer(int value)
          : value(value)
        {}

        Integer operator+ (const Integer& rhs)
        {
            return Integer(value + rhs.value);
        }

        operator int()
        {
            return value;
        }
    };

private:
    void setLhs(int lhs)
    {
        lhs_ = std::make_unique<Integer>(lhs);
    }

    void setRhs(int rhs)
    {
        rhs_ = std::make_unique<Integer>(rhs);
    }

    int doSub()
    {
        return *lhs_ - *rhs_;
    }

private:
    std::unique_ptr<Integer> lhs_;
    std::unique_ptr<Integer> rhs_;
};
