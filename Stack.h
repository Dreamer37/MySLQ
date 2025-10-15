#include <iostream>

#include <string.h>

#include "Vector.h"


template <typename T> class Stack: public Vector<T> {
public:
    void push ( T const& e ) { this->insert(this->size(), e); }
    T pop() { return this->remove(this->size() - 1); }
    T& top() { return (*this)[this->size() - 1]; }
};

void convert ( Stack<char>& S, __int64 n, int base ) {
    static char digit[]
    = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9','A', 'B', 'C', 'D', 'E', 'F' };
    while ( n > 0 ) {
        int remainder = int ( n % base );
        S.push ( digit[remainder] );
        n /= base;
    }
}

// void trim ( const char exp[], int& lo, int& hi ) {
//     while ( ( lo <= hi ) && ( exp[lo] != '(') && ( exp[lo] != ')' ) ) lo++;
//     while ( ( lo <= hi ) && ( exp[hi] != '(') && ( exp[hi] != ')' ) ) hi--;
// }

// int divide ( const char exp[], int lo, int hi ) {
//     int mi = lo; int crc = 1;
//     while ( ( 0 < crc ) && ( ++mi < hi ) ) {
//         if ( exp[mi] == ')' ) crc--; if ( exp[mi] == '(' ) crc++; 
//     }
//     return mi;
// }

// bool paren ( const char exp[], int lo, int hi ) {
//     trim ( exp, lo, hi );if ( lo > hi ) return true;
//     if ( exp[lo] != '(' ) return false;
//     if ( exp[hi] != ')' ) return false;
//     int mi = divide ( exp, lo, hi );
//     if ( mi > hi ) return false;
//     return paren ( exp, lo + 1, mi - 1 ) && paren ( exp, mi + 1, hi );
// }

bool paren ( const char exp[], int lo, int hi ) {
    Stack<char> S;
    for ( int i = lo; i < hi; i++ )
        switch ( exp[i] ) {
            case '(': case '[': case '{': S.push ( exp[i] ); break;
            case ')': if ( ( S.empty() ) || ( '(' != S.pop() ) ) return false; break;
            case ']': if ( ( S.empty() ) || ( '[' != S.pop() ) ) return false; break;
            case '}': if ( ( S.empty() ) || ( '{' != S.pop() ) ) return false; break;
            default: break;
        }
    return S.empty();
}

#define N_OPTR 9
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;
const char pri[N_OPTR][N_OPTR] = {
    /*          +    -    *    /    ^    !    (    )    \0  */
    /*  + */   '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*  - */   '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*  * */   '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  / */   '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  ^ */   '>', '>', '>', '>', '>', '<', '<', '>', '>',
    /*  ! */   '>', '>', '>', '>', '>', ' ', ' ', '>', '>',
    /*  ( */   '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /*  ) */   ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* \0 */   '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

void readNumber ( char*& p, Stack<float>& stk ) {
    stk.push ( float ( *p - '0' ) );
    while ( isdigit ( *( ++p ) ) ) 
        stk.push ( stk.pop() * 10 + float ( *p - '0' ) );
    if ( '.' != *p ) return;
    float frac = 1;
    while ( isdigit ( *( ++p ) ) ) 
        stk.push ( stk.pop() + ( frac /= 10 ) * float ( *p - '0' ) );
}

Operator optr2rank ( char op ) {
    switch ( op ) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default: exit ( 1 );
    }
}

char orderBetween ( char op1, char op2 ) {
    return pri[optr2rank ( op1 )][optr2rank ( op2 )];
}

void append ( char*& rpn, float opnd ) {
    int n = strlen ( rpn );
    char buf[32]; 
    if ( opnd != ( int ) opnd )
        sprintf ( buf, "%.2f  ", opnd );
    else
        sprintf ( buf, "%d  ", ( int ) opnd );
    rpn = ( char* ) realloc ( rpn, ( n + strlen ( buf ) + 1 ) * sizeof ( char ) );
    strcat ( rpn, buf );
}

void append ( char*& rpn, char optr ) {
    int n = strlen ( rpn );
    rpn = ( char* ) realloc ( rpn, ( n + 3 ) * sizeof ( char ) );
    sprintf ( rpn + n, "%c ", optr ); rpn[n + 2] = '\0';
}

float calcu ( char optr, float opnd ) {
    switch ( optr ) {
        case '!': {
            float result = 1;
            for ( int i = 1; i <= ( int ) opnd; i++ ) result *= i;
            return result;
        }
        default: exit ( 1 );
    }
}

float calcu ( float opnd1, char optr, float opnd2 ) {
    switch ( optr ) {
        case '+': return opnd1 + opnd2;
        case '-': return opnd1 - opnd2;
        case '*': return opnd1 * opnd2;
        case '/': 
            if ( 0 == opnd2 ) exit ( 1 );
            return opnd1 / opnd2;
        case '^': {
            float result = 1;
            for ( int i = 0; i < ( int ) opnd2; i++ ) result *= opnd1;
            return result;
        }
        default: exit ( 1 );
    }
}

float evaluate ( char* S, char*& RPN ) {
    if (!RPN) { RPN = (char*)malloc(1); RPN[0] = '\0'; }
    Stack<float> opnd; Stack<char> optr; 
    optr.push ( '\0' );
    while ( !optr.empty() ) {
        if ( isdigit ( *S ) ) {
            readNumber ( S, opnd ); append ( RPN, opnd.top() );
        } else 
            switch ( orderBetween ( optr.top(), *S ) ) {
                case '<': 
                    optr.push ( *S ); S++;
                    break;
                case '=': 
                    optr.pop(); S++; 
                    break;
                case '>': {
                    char op = optr.pop(); append ( RPN, op );
                    if ( '!' == op ) {
                        float pOpnd = opnd.pop();
                        opnd.push ( calcu ( op, pOpnd ) );
                    } else {
                        float pOpnd2 = opnd.pop(); float pOpnd1 = opnd.pop();
                        opnd.push ( calcu ( pOpnd1, op, pOpnd2 ) );
                    }
                    break;
                }
                default : exit ( 1 ); 
            }
    }    
    return opnd.pop();
}

