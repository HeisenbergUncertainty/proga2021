#include <iostream>
#include <algorithm>
using namespace std;

template <typename T>
class Grid {
private:
    struct Point {
        bool is_subgrid;
        union {
            Grid<T>* subgrid_ptr;
            T number;
        };
        Point() : is_subgrid(false), number(0) { }

        ~Point() { if (is_subgrid) delete subgrid_ptr; } // деструктор
        Point(Point const& src) = delete; // конструктор копирования

        Point& operator=(Point const& src) { // оператор копирующего присваивания
            if (not is_subgrid) {
                this->number = src.number;
            }
            else {
                if (subgrid_ptr != nullptr)
                    delete subgrid_ptr;
                subgrid_ptr = new Grid<T>(src.subgrid_ptr->x_size, src.subgrid_ptr->y_size);
                *subgrid_ptr = *src.subgrid_ptr;
            }
            return *this;
        }

        Point(Point const&& src) = delete; // конструктор перемещения
        Point& operator=(Point&& src) = delete; // оператор перемещающего присваивания
    };
    Point* memory;
    size_t x_size, y_size;

public:
    Grid() : memory(nullptr), x_size(0), y_size(0) { }
    Grid(size_t x_size, size_t y_size) : x_size(x_size), y_size(y_size) {
        memory = new Point[x_size * y_size];
    }
    Grid(size_t x_size, size_t y_size, T num) : Grid(x_size, y_size) {
        for (size_t i = 0; i < x_size * y_size; ++i) {
            memory[i].number = num;
        }
    }

    ~Grid() { delete[] memory; } // деструктор

    Grid(Grid<T> const& src) : Grid(src.x_size, src.y_size) { // конструктор копирования
        for (int i = 0; i < x_size * y_size; ++i) {
            memory[i] = src.memory[i];
        }
    }

    Grid<T>& operator=(Grid<T> const& src) { // оператор копирующего присваивания
        Grid<T> tmp(src);
        swap(memory, tmp.memory);
        swap(x_size, tmp.x_size);
        swap(y_size, tmp.y_size);
        return *this;
    }

    Grid(Grid<T>&& src) : Grid() { // конструктор перемещения
        swap(memory, src.memory);
        swap(x_size, src.x_size);
        swap(y_size, src.y_size);
    }

    Grid<T>& operator=(Grid<T>&& src) { // оператор перемещающего присваивания
        Grid<T> tmp(std::move(src));
        swap(memory, tmp.memory);
        swap(x_size, tmp.x_size);
        swap(y_size, tmp.y_size);
        return *this;
    }

    T average() {
        T sum = 0;
        for (size_t i = 0; i < x_size * y_size; ++i) {
            if (memory[i].is_subgrid)
                sum += memory[i].subgrid_ptr->average();
            else
                sum += memory[i].number;
        }
        return sum / (x_size * y_size);
    }

    template <typename Tfriend>
    friend std::ostream& operator<<(std::ostream&, Grid<Tfriend> const& b);
    template <typename Tfriend>
    friend std::istream& operator>>(std::istream&, Grid<Tfriend>& b);

    T operator()(size_t x_idx, size_t y_idx) const {
        if (memory[x_idx * y_size + y_idx]) {
            return this->average();
        }
    }
    T& operator()(size_t x_idx, size_t y_idx) { }

    size_t get_xsize() const {
        return x_size;
    };
    size_t get_ysize() const {
        return y_size;
    };

    Grid<T>& operator=(T value) {
        for (size_t i = 0; i < x_size * y_size; ++i) {
            if (memory[i].is_subgrid) {
                delete memory[i].subgrid_ptr;
                memory[i].is_subgrid = false;
            }
            memory[i].number = value;
        }
    };

    Grid<T>& make_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size) {
        if (memory[x_idx * y_size + y_idx].is_subgrid) {
            T tmp = memory[x_idx * y_size + y_idx].subgrid_ptr->average();
            delete memory[x_idx * y_size + y_idx].subgrid_ptr;
            memory[x_idx * y_size + y_idx].subgrid_ptr = new Grid<T>(x_sub_size, y_sub_size, tmp);
        }
        else {
            T tmp = memory[x_idx * y_size + y_idx].number;
            memory[x_idx * y_size + y_idx].is_subgrid = true;
            memory[x_idx * y_size + y_idx].subgrid_ptr = new Grid<T>(x_sub_size, y_sub_size, tmp);
        }
        return *this;
    };

    Grid<T>& collapse_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size) {
        if (memory[x_idx * y_size + y_idx].is_subgrid) {
            T tmp = memory[x_idx * y_size + y_idx].subgrid_ptr->average();
            delete memory[x_idx * y_size + y_idx].subgrid_ptr;
            memory[x_idx * y_size + y_idx].is_subgrid = false;
            memory[x_idx * y_size + y_idx].number = tmp;
            return *this;
        }
    };

    Grid<T>& get_subgrid(size_t x_idx, size_t y_idx) {
        if (memory[x_idx * y_size + y_idx].is_subgrid)
            return *this;
    };
    /*Grid<T> const& get_subgrid(size_t x_idx, size_t y_idx) { // не может быть перегружен
        if (memory[x_idx * y_size + y_idx].is_subgrid)
            return *this;
    };*/

    bool is_subgrid(size_t x_idx, size_t y_idx) const {
        return memory[x_idx * y_size + y_idx].is_subgrid;
    };
};

template <typename Tfriend>
std::istream& operator>>(std::istream& is, Grid<Tfriend>& b) {
    for (size_t i = 0; i < b.x_size; ++i)
        for (size_t j = 0; j < b.y_size; ++j) {
            if (not b.memory[i * b.y_size + j].is_subgrid)
                is >> b.memory[i * b.y_size + j].number;
        }
    return is;
}

template <typename Tfriend>
std::ostream& operator<<(std::ostream& os, Grid<Tfriend> const& b) {
    for (size_t i = 0; i < b.x_size; ++i) {
        for (size_t j = 0; j < b.y_size; ++j) {
            if (b.memory[i * b.y_size + j].is_subgrid)
                os << "Subgrid" << ' ';
            else
                os << b.memory[i * b.y_size + j].number << ' ';
        }
        os << endl;
    }
    return os;
}


int main() {
    Grid<int> gr(2, 3, 3);
    gr.make_subgrid(0, 0, 2, 2);
    //gr.collapse_subgrid(0,0);
    //gr(0, 0);
    std::cout << gr.get_subgrid(0, 0);
    //int(gr(0,0));
    //gr(0, 0) = 4;
    //std::cout << gr(0, 0);
    return 0;
}