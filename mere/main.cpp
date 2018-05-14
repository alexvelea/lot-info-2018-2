#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include "mere.h"

using namespace std;

namespace helper {

class Writer {
  public:
    Writer(const char *name):
        m_stream(name) {
        memset(m_buffer, 0, sizeof(m_buffer));
        m_pos = 0;
    }

    Writer& operator<<(int64_t a) {
        if (a < 0) {
            putchar('-');
            a = -a;
        }
        int many = 0;
        do {
            digit_buffer[many++] = a % 10 + '0';
            a /= 10;
        } while (a > 0);
        for (int i = many - 1; i >= 0; --i)
            putchar(digit_buffer[i]);
        return *this;
    }

    Writer& operator<<(const char *s) {
        for (; *s; ++s)
            putchar(*s);
        return *this;
    }

    ~Writer() {
        m_stream.write(m_buffer, m_pos);
    }

  private:
    void putchar(char c) {
        m_buffer[m_pos++] = c;
        if (m_pos == kBufferSize) {
            m_stream.write(m_buffer, m_pos);
            m_pos = 0;
        }
    }
    static const int kBufferSize = 32768;
    ofstream m_stream;
    char m_buffer[kBufferSize];
    char digit_buffer[30];
    int m_pos;
};

class Reader {
  public:
    Reader(const char *name):
        m_stream(name) {
            eof = false;
            read();
        }

    Reader& operator>>(int &x) {
        x = 0;
        int multiplier = 1;
        while ((getchar() < '0' || getchar() > '9') && getchar() != '-') {
            if (getchar() == 0) {
                eof = true;
                return *this;
            }
            next();
        }

        if (getchar() == '-') {
            multiplier = -1;
            next();
        }

        while (getchar() >= '0' && getchar() <= '9') {
            x = x * 10 + getchar() - '0';
            next();
        }
        x = x * multiplier;

        return *this;
    }

    operator bool() {
        return !eof;
    }
  private:
    void read() {
        memset(m_buffer, 0, sizeof(m_buffer));
        m_stream.read(m_buffer, kBufferSize);
        m_pos = 0;
    }

    char getchar() const {
        return m_buffer[m_pos];
    }

    void next() {
        if (++m_pos == kBufferSize)
            read();
    }

    static const int kBufferSize = 32768;
    ifstream m_stream;
    char m_buffer[kBufferSize];
    int m_pos;
    bool eof;
};

void main() {
    Reader cin("mere.in");
    Writer cout("mere.out");
    int N, VA, VB; cin >> N >> VA >> VB;
    int* X = new int[N];
    int* T = new int[N];

    for (int i = 0; i < N; ++i)
        cin >> X[i] >> T[i];

    cout << count(N, VA, VB, X, T) << "\n";
}

}

int main() {
    helper::main();
}

