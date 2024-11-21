// g++ -o llaas llaas.cpp -std=c++17

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <signal.h>
#include <unistd.h>

#define DEFAULT_INITIALIZER -1337
#define MAX_SIZE 0x1337

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    signal(SIGALRM, exit);
    alarm(0x20);
}

void error(std::string msg) {
    std::cerr << 
        "ERROR: " << msg
    << std::endl;
}

template <typename _T = __int64_t*>
class LinkedList {
public:
    void print(bool with_index = true) {
        for(size_t i = 0; i < this->size(); ++i) {
            auto itm = this->list[i];
            if(*itm == DEFAULT_INITIALIZER) continue;
            if(with_index) {
                std::cout << i << ") ";
            }
            std::cout << this->list[i] << std::endl;
        }
    }

    void print_at_idx(__int64_t idx) {
        if(idx >= MAX_SIZE) {
            error("Out of bound read!");
            return;
        }
        std::cout << *(this->list[idx]) << std::endl;
    }

    void insert(_T data) {
        _T *v = new _T(data);
        list.push_back(*v);
    }

    void insert_at_index(_T data, __int64_t idx) {
        list[idx] = (__int64_t*)*data;
    }

    void remove() { list.pop_back(); }

    void resize(__int64_t size) { list.resize(size); }

    __int64_t size() const { return list.size(); }

    LinkedList(__int64_t max_size = 0x10) : max_size(max_size) {
        list.resize(max_size);
        list.assign(max_size, new __int64_t(DEFAULT_INITIALIZER));
    }

    ~LinkedList() { list.resize(0); }

private:
    std::vector<_T> list;
    __int64_t max_size;
};

template <typename _T = __int64_t*>
class Menu {
    typedef void (*hwnd_func)(LinkedList<_T>*);
public:
    static void insert(LinkedList<_T> *lst) {
        _T data;
        std::cout << "Enter the data you want to insert: ";
        std::cin >> *data;
        lst->insert(data);
    }
    static void insert_at_index(LinkedList<_T> *lst) {
        _T data;
        __int64_t idx;
        std::cout << "Enter the data you want to insert: ";
        std::cin >> *data;
        std::cout << "Enter the index you want to insert at: ";
        std::cin >> idx;
        lst->insert_at_index(data, idx);
    }
    static void print_list(LinkedList<_T> *lst) { lst->print(); }
    static void print_idx(LinkedList<_T> *lst) {
        __int64_t idx;
        std::cout << "Enter the index you want to print: ";
        std::cin >> idx;
        lst->print_at_idx(idx);
    }
    static void remove(LinkedList<_T> *lst) {lst->remove(); }
    static void resize(LinkedList<_T> *lst) {
        __int64_t size;
        std::cout << "Please enter the size you want to resize to: ";
        std::cin >> size;
        lst->resize(size);
    }

    static void _exit(LinkedList<_T> *lst) {
        std::cout << "Thank you for using our program!" << std::endl;
        exit(0);
    }

    Menu() {
        vtable.emplace(1, &insert);
        vtable.emplace(2, &insert_at_index);
        vtable.emplace(3, &print_list);
        vtable.emplace(4, &print_idx);
        vtable.emplace(5, &remove);
        vtable.emplace(6, &resize);
        vtable.emplace(0, &_exit);
    }

    void print() {
        std::cout << "1. Insert an item." << std::endl;
        std::cout << "2. Insert an item at an index." << std::endl;
        std::cout << "3. Print list." << std::endl;
        std::cout << "4. Print index." << std::endl;
        std::cout << "5. Remove an item." << std::endl;
        std::cout << "6. Resize buffer" << std::endl;
        std::cout << "0. Exit." << std::endl;
    }

    __int64_t get_choice(bool user_input=true) {
        if(user_input) {
            std::cout << ">> ";
            std::cin >> this->choice;
        }
        return this->choice;
    }

    std::unordered_map<__int64_t, hwnd_func> vtable;
private:
    __int64_t choice;
};

int main(int argc, char* argv[]) {

    LinkedList<__int64_t*>* list = new LinkedList<int64_t*>;
    Menu<__int64_t*> menu;
    __int64_t choice;
    do {
        menu.print();
        choice = menu.get_choice();
        if(choice <= -1 || choice >= 7) {
            error("Invalid choice!\n");
            continue;
        }
        menu.vtable[choice](list);
    } while(choice != 0);

    return 0;
}