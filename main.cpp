#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

class Array {
public:
    using size_t = int;
    using value_t = float;
    using Iterator = value_t*;

public:

    size_t size() const { return m_size; }

    static size_t maxsize() { return MAX_SIZE; }

    std::string toString() {
        std::stringstream ss;
        for (auto& item : *this) {
            ss << item << " ";
        }
        return ss.str();
    }

    double min() {
        double min = (*this)[m_minIndex];
        for (auto& item : *this) {
            if (item <= min)
                min = item;
        }
        return min;
    }

    double max() {
        double max = (*this)[m_minIndex];
        for (auto& item : *this) {
            if (item >= max)
                max = item;
        }
        return max;
    }

    double sum() {
        double sum = 0;
        for (auto& item : *this) {
            sum += item;
        }
        return sum;
    }

    double avg() {
        return sum() / size();
    }


    friend std::istream& operator>>(std::istream& instream, Array& array) {
        for (auto& element : array) {
            instream >> element;
        }
        return instream;
    }

    friend std::ostream& operator<<(std::ostream& outstream, const Array& array) {
        for (auto& element : array) {
            outstream << element << " ";
        }
        return outstream;
    }


    Array& operator=(const Array& array) {
        if (this != &array) {
            if (size() != array.size()) {
                throw std::exception("arrays have different sizes");
            }
            for (size_t n = 0; n < size(); ++n) {
                m_data[n] = array.m_data[n];
            }
            m_minIndex = array.m_minIndex;
            m_maxIndex = array.m_maxIndex;
        }
        return *this;
    }

    value_t& operator[](size_t index) {
        if (index < m_minIndex || index > m_maxIndex) {
            throw std::out_of_range("array out of range");
        }
        else {
            return m_data[index - m_minIndex];
        }
    }

    friend Array operator+(Array array, double constant) {
        Array newArray = Array(array);
        for (int i = 0; i < array.size(); ++i) {
            newArray.m_data[i] = array.m_data[i] + constant;
        }
        return newArray;
    }

    friend Array operator-(Array array, double constant) {
        Array newArray = Array(array);
        for (int i = 0; i < array.size(); ++i) {
            newArray.m_data[i] = array.m_data[i] - constant;
        }
        return newArray;
    }

    friend Array operator*(Array array, double constant) {
        Array newArray = Array(array);
        for (int i = 0; i < array.size(); ++i) {
            double value = array.m_data[i] * constant;
            newArray.m_data[i] = std::abs(value) < array.eps ? 0 : value;
        }
        return newArray;
    }

    friend Array operator/(Array array, double constant) {
        Array newArray = Array(array);
        for (int i = 0; i < array.size(); ++i) {
            newArray.m_data[i] = array.m_data[i] / constant;
        }
        return newArray;
    }

    friend Array operator+(Array array1, Array array2) {
        Array newArray = Array(array1);
        for (int i = 0; i < array1.size(); ++i) {
            newArray.m_data[i] = array1.m_data[i] + array2.m_data[i];
        }
        return newArray;
    }

    friend Array operator-(Array array1, Array array2) {
        Array newArray = Array(array1);
        for (int i = 0; i < array1.size(); ++i) {
            newArray.m_data[i] = array1.m_data[i] - array2.m_data[i];
        }
        return newArray;
    }

    friend Array operator*(Array array1, Array array2) {
        Array newArray = Array(array1);
        for (int i = 0; i < array1.size(); ++i) {
            newArray.m_data[i] = array1.m_data[i] * array2.m_data[i];
        }
        return newArray;
    }

    friend Array operator/(Array array1, Array array2) {
        Array newArray = Array(array1);
        for (int i = 0; i < array1.size(); ++i) {
            if (array2.m_data[i] == 0) {
                throw zero_divizion("divizion by zero");
            }
            else {
                newArray.m_data[i] = (int)array1.m_data[i] / (int)array2.m_data[i];
            }
        }
        return newArray;
    }

    value_t& operator[](size_t index) const {
        if (index < m_minIndex || index > m_maxIndex) {
            throw std::out_of_range("array out of range");
        }
        else {
            return m_data[index - m_minIndex];
        }
    }

    Iterator       begin() { return m_data; }
    Iterator       end() { return m_data + m_size; }
    const Iterator begin()  const { return m_data; }
    const Iterator end()    const { return m_data + m_size; }
    const Iterator cbegin() const { return m_data; }
    const Iterator cend()   const { return m_data + m_size; }

    class zero_divizion : public std::logic_error {
    public:
        zero_divizion(const char* msg) : logic_error(msg) { }
    };

    class invalid_iterators : public std::invalid_argument {
    public:
        invalid_iterators(const char* msg) : invalid_argument(msg) { }
    };

private:
    const static size_t MAX_SIZE = 10000;
    const size_t m_size;
    const double eps = 0.00001;
    value_t* m_data;

    int m_minIndex;
    int m_maxIndex;


protected:

    Array(const size_t size) : m_size(size), m_minIndex(0), m_maxIndex(size - 1) {
        if (size > maxsize()) {
            throw std::range_error("invalid array size");
        }
        m_data = new value_t[size];
    }

    Array(const Iterator begin, const Iterator end) : Array(end - begin) {
        if (begin >= end) {
            throw invalid_iterators("invalid iterators");
        }
        size_t index = 0;
        for (Iterator it = begin; it != end; ++it) {
            m_data[index++] = *it;
        }
    }

public:
    Array(const int minIndex, const int maxIndex) : Array(maxIndex - minIndex + 1)
    {
        if (minIndex > maxIndex)
            throw std::invalid_argument("invalid index order");

        m_minIndex = minIndex;
        m_maxIndex = maxIndex;
    }

    Array(const Array& array) : Array(array.cbegin(), array.cend()) { }

    ~Array() { delete[] m_data; }
};

int main() {
    int minIndex, maxIndex, operation, index;
    double constant;
    std::cin >> minIndex >> maxIndex;

    Array array1 = Array(minIndex, maxIndex);
    Array array2 = Array(minIndex, maxIndex);

    std::cin >> array1 >> array2 >> constant;

    std::cout << "Array1= " << array1 << std::endl;
    std::cout << "Array1+const= " << (array1 + constant) << std::endl;
    std::cout << "Array1*const= " << (array1 * constant) << std::endl;
    std::cout << "Array2= " << array2 << std::endl;
    std::cout << "Array2+const= " << (array2 + constant) << std::endl;
    std::cout << "Array2*const= " << (array2 * constant) << std::endl;
    std::cout << "summa= " << array1 + array2 << std::endl;
    std::cout << "raznost= " << array1 - array2 << std::endl;
    std::cout << "proizv = " << array1 * array2 << std::endl;
    try {
        std::cout << "delenie= " << array1 / array2 << std::endl;
    }
    catch (Array::zero_divizion err) {
        std::cout << "delenie na 0" << std::endl;
    }

    while ((std::cin >> operation) && (operation != 0)) {
        std::cin >> index;
        switch (operation) {
        case 1:
            try {
                std::cout << "Array1[" << index << "]= " << array1[index] << std::endl;
            }
            catch (std::exception err) {
                std::cout << "Array1 wrong index " << index << std::endl;
            }
            break;
        case 2:
            try {
                std::cout << "Array2[" << index << "]= " << array2[index] << std::endl;
            }
            catch (std::exception err) {
                std::cout << "Array2 wrong index " << index << std::endl;
            }
            break;
        default: break;
        }
    }
}