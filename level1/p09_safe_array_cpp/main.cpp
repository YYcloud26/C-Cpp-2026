#include <stdexcept>
#include <iostream>

template <typename T>

class SafeArray {
private:
    T* data;
    int size;

    void check(int i) const {
        if (i < 0 || i >= size)
            throw std::out_of_range("SafeArray: index out of range");
    }

public:
    explicit SafeArray(int n) : size(n), data(new T[n]()) {}

    SafeArray(const SafeArray& other)
        : size(other.size), data(new T[other.size]()) {
        for (int i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    SafeArray& operator=(const SafeArray& other) {
        if (this != &other) {
            T* newData = new T[other.size]();
            for (int i = 0; i < other.size; ++i)
                newData[i] = other.data[i];
            delete[] data;
            data = newData;
            size = other.size;
        }
        return *this;
    }

    ~SafeArray() {
        delete[] data;
    }

    int getSize() const {
        return size;
    }

    T get(int i) const {
        check(i);
        return data[i];
    }

    void set(int i, const T& value) {
        check(i);
        data[i] = value;
    }

    T& operator[](int i) {
        check(i);
        return data[i];
    }

    const T& operator[](int i) const {
        check(i);
        return data[i];
    }
};

int main() {
    SafeArray<int> a(10);

    a[0] = 100;
    a[1] = 200;

    int x = a[0];
    std::cout << "a[0] = " << x << ", a[1] = " << a[1] << '\n';

    try {
        a[10] = 999;
    } catch (const std::out_of_range& e) {
        std::cout << "越界写被捕获: " << e.what() << '\n';
    }

    try {
        int y = a[-1];
        (void)y;
    } catch (const std::out_of_range& e) {
        std::cout << "越界读被捕获: " << e.what() << '\n';
    }

    const SafeArray<int> ca(5);
    int v = ca[2];
    std::cout << "ca[2] = " << v << '\n';

    a.set(2, 300);
    std::cout << "a.get(2) = " << a.get(2) << ", a[2] = " << a[2] << '\n';

    return 0;

    SafeArray<double> b(3);
    b[0] = 3.14;
    b.set(1, 2.71);
    double d = b[1];

    SafeArray<std::string> s(2);
    s[0] = "hello";
    s.set(1, "world");
    std::string w = s[1];
}