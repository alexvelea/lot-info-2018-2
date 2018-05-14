#include <iostream>
#include <map>
#include <vector>

#define Debug(...) Base::__debugE(#__VA_ARGS__, __VA_ARGS__)

namespace Base {

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& rhs);

template <typename T, typename T2>
std::ostream& operator<<(std::ostream& out, const std::map<T, T2>& rhs) {
    out << "[";
    for (auto& itr : rhs) {
        out << "{" << itr.first << ", " << itr.second << "}";
        if (itr != *rhs.rbegin()) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& rhs) {
    out << "[";
    for (int i = 0; i < (int)rhs.size(); i += 1) {
        out << rhs[i] << (i + 1 == (int)rhs.size() ? "" : ",\t");
    }
    out << "]";
    return out;
}

template <typename T, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T, T2>& rhs) {
    out << "{" << rhs.first << ",\t" << rhs.second << "}";
    return out;
}

void Print(const char*& txt, const char* _txt) {
    std::string name = "";
    while (*txt != '\0' and *txt != ',') {
        name += *txt;
        txt++;
    }

    while (*txt != '\0' and (*txt == ',' or *txt == ' ')) {
        txt++;
    }

    if (name == "\"\\n\"") {
        std::cerr << "\n";
        return ;
    }

    std::string txt_ = _txt;
    if (name.substr(1, txt_.size()) == txt_) {
        std::cerr << txt_ << '\t';
    } else {
        std::cerr << name << " = " << txt_ << '\t';
    }
}

template <typename T>
void Print(const char*& txt, T t) {
    std::string name = "";
    while (*txt != '\0' and *txt != ',') {
        name += *txt;
        txt++;
    }

    while (*txt != '\0' and (*txt == ',' or *txt == ' ')) {
        txt++;
    }

    std::cerr << name << " = " << t << '\t';
}

template <typename T>
void _debug(const char*& txt, T t) {
    Print(txt, t);
}

template <typename T, typename... Args>
void _debug(const char*& txt, T t, Args... args) {
    Print(txt, t);
    _debug(txt, args...);
}

template <typename... Args>
void __debugE(const char* txt, Args... args) {
    _debug(txt, args...);
    std::cerr << '\n';
}

}
