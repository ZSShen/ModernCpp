#pragma once


template <typename T, T Begin = T::Begin, T End = T::End>
class Enum {
public:
    Enum() {}
    ~Enum() {}

    Enum(const Enum&) = delete;
    Enum(Enum&&) = delete;
    Enum& operator=(const Enum&) = delete;
    Enum& operator=(Enum&&) = delete;

    class Iterator {
    public:
        Iterator(int value)
          : value_(value)
        {}

        T operator*() const
        {
            return static_cast<T>(value_);
        }

        void operator++()
        {
            ++value_;
        }

        bool operator!=(const Iterator& rhs)
        {
            return value_ != rhs.value_;
        }

    private:
        int value_;
    };

    Iterator begin()
    {
        return Iterator(static_cast<int>(Begin));
    }

    Iterator end()
    {
        return Iterator(static_cast<int>(End) + 1);
    }
};
