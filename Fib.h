// Fib.h
#ifndef FIB_H
#define FIB_H

class Fib {
private:
    int f, g; // f = Fib(k-1), g = Fib(k)
public:
    Fib(int n) { // 初始化为不小于n的最小Fibonacci项
        f = 1; g = 0;
        while (g < n) next();
    }
    int get() { return g; } // 获取当前Fibonacci项
    int next() { g += f; f = g - f; return g; } // 转至下一Fibonacci项
    int prev() { f = g - f; g -= f; return g; } // 转至上一Fibonacci项
};

#endif // FIB_H
