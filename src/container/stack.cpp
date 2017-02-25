#include"stack.h"
#include<iostream>
#include "macrodef.h"
#include <maths.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;


namespace container {

///利用栈求输入中缀表达式的值
///编译程序经常用到这个来编译程序

//后缀表达式求值
double  compute_express(char* str)
{
    Stack<double> s(5);
    double x = 0;
    double y = 0;
    int i = 0;
    while (str[i])
    {
        if (str[i] == ' ')
        {
            i++;
            continue;
        }
        switch (str[i])
        {
        case '+':
            x = s.pop() + s.pop();
            i++;
            break;
        case '-':
            x = s.pop();
            x = s.pop() - x;
            i++;
            break;
        case '*':
            x = s.pop() * s.pop();
            i++;
            break;
        case '/':
            x = s.pop();
            if (!floatEqual(x, 0.0f))
            {
                x = s.pop()  / x;
            }
            else
            {
                cerr << "Error!" << endl;
            }
            i++;
            break;
        default:
            x = 0;
            while (str[i] >= 48 && str[i] <= 57)
            {
                x = x * 10 + str[i] - 48;
                i++;
            }
            if (str[i] == '.')
            {
                i++;
                y = 0;
                double j = 10.0;
                while (str[i] >= 48 && str[i] <= 57)
                {
                    y += (str[i] - 48) / j;
                    i++;
                    j *= 10;
                }
                x += y;
            }
        }
        s.push(x);
    }
    if (s.empty())
    {
        cerr << "Stack is empty!" << endl;
    }
    x = s.pop();
    if (s.empty())
    {
        return x;
    }
    cerr << "expression is wrong !" << endl;
    return 0;
}

int precede(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '(':
    case '@':
    default:
        return 0;
    }
}

//中缀转后缀
void change(char* s1, char* s2)
{
    Stack<char> r(5);
    r.push('@');
    int i = 0;
    int j = 0;
    char ch = s1[i];
    while (ch != '\0')
    {
        if (ch == ' ')
        {
            ch = s1[++i];
        }
        else if (ch == '(')
        {
            r.push(ch);
            ch = s1[++i];
        }
        else if (ch == ')')
        {
            while (r.peek() != '(')
            {
                s2[j++] = r.pop();
            }
            r.pop();
            ch = s1[++i];
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        {
            char w = r.peek();
            while (precede(w) >= precede(ch))
            {
                s2[j++] = w;
                r.pop();
                w = r.peek();
            }
            r.push(ch);
            ch = s1[++i];
        }
        else
        {
            if ((ch < '0' || ch > '9') && ch != '.')
            {
                cout << "Error!" << endl;
            }
            while ((ch >= '0' && ch <= '9') || ch == '.')
            {
                s2[j++] = ch;
                ch = s1[++i];
            }
            s2[j++] = ' ';
        }
    }
    ch = r.pop();
    while (ch != '@')
    {
        if (ch == '(')
        {
            cerr << "expression is wrong !" << endl;
        }
        else
        {
            s2[j++] = ch;
            ch = r.pop();
        }
    }
    s2[j++] = '\0';
}

void compute()
{
    char a[50] = {0};
    char b[50] = {0};
    cout << "输入中缀表达式" << endl;
    cin.getline(a, 50);
    change(a, b);
    cout << "中缀:" << a << endl;
    cout << "后缀:" << b << endl;
    cout << "求值:" << compute_express(b) << endl;
}

//进制转换,把n从十进制转换为r进制
void convert(int n, int r)
{	
    if (n < 0)
    {
        return;
    }
    cout << n << "=";
    int e = 0;
    Stack<int> s(10);
    while (n)
    {
        s.push(n % r);
        n = n / r;
    }
    while (!s.empty())
    {
        e = s.pop();
        cout << e;
    }
    cout << endl;
}

//括号匹配
int match()
{
    char b[80] = "{[]}#";
    int i = 0;
    int state = 1;
    char ch = (*b)++;
    Stack<int> s(100);

    while (ch != '#' && state)
    {
        switch (ch)
        {
        case '{':
        case '[':
        case '(':
            {
                s.push(ch);
            }
            break;
        case '}':
            {
                if (!s.empty() && s.peek() == '{')
                {
                    s.pop();
                }
                else
                {
                    state = 0;
                }
            }
            break;
        case ']':
            {
                if (!s.empty() && s.peek() == '[')
                {
                    s.pop();
                }
                else
                {
                    state = 0;
                }
            }
            break;
        case ')':
            {
                if (!s.empty() && s.peek() == '(')
                {
                    s.pop();
                }
                else
                {
                    state = 0;
                }
            }
            break;
        }
        //ch=(*b)++;
        ch = b[++i];
    }
    if (state && s.empty())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//背包(递归)
bool knapsack(int w[], int T, int n)
{
    if (T == 0)
    {
        return true;
    }
    else if (T < 0 || (T > 0 && n < 1)) 
    {
        return false;
    }
    if (knapsack(w, T - w[n - 1], n - 1))
    {
        cout << w[n - 1] << " ";
        return true;
    }
    else 
    {
        return knapsack(w, T, n - 1);
    }
}

//背包(非递归)
void knapsack_v2(int w[], int T, int n)
{
    Stack<WEIGHT*> s(100);
    WEIGHT *p = NULL;
    WEIGHT *e = NULL;
    WEIGHT **tmp = NULL;

    int i = 0;
    do
    {
        while (T > 0 && i < n)
        {
            if (T - w[i] >= 0)
            {
                p = new WEIGHT;
                p->i = i;
                p->wi = w[i];
                s.push(p);
                T = T - w[i];
            }
            i++;
        }
        if (T == 0) 
        {
            tmp = new WEIGHT*[s.length()];
            s.put(tmp);
            for (int k = 0; k < s.length(); k++)
            {
                cout << tmp[k]->wi << " ";
            }
            cout << endl;
            delete [] tmp;
        }
        e = s.pop();
        T += e->wi;
        i = e->i;
        delete e;
        i++;

    }while (i < n || !s.empty());
}

/////////////////////////////////////////////////////////////////////////////////////
//akm问题(递归)
unsigned int akm(unsigned int m, unsigned int n)
{
    if (m == 0)  
    {
        return n + 1; //m=0
    }
    else if (n == 0)
    {
        return akm(m - 1, 1); //m>0, n=0
    }
    else
    {
        return akm(m - 1, akm(m, n - 1)); //m>0, n>0;
    }
}

//akm问题(非递归)
unsigned int akm_v2(unsigned int m, unsigned int n)
{
    Stack<AkmNode*> s(100);
    AkmNode *node = new AkmNode;
    AkmNode *e = new AkmNode;
    node->vm = m;
    node->vn = n;
    s.push(node);

    unsigned int k1 = 0;
    unsigned int k2 = 0;
    do
    {	
        e = s.peek();
        if (e->vm == 0)
        {
            e = s.pop();
            k1 = e->vn + 1;
            if (s.length() >= 1)
            {
                e = s.pop();
                k2 = e->vm - 1;
                delete e;
                node = new AkmNode;
                node->vm = k2;
                node->vn = k1;
                s.push(node);
            }
        }
        else if (e->vm > 0 && e->vn == 0)
        {
            e = s.pop();
            node = new AkmNode;
            node->vm = e->vm - 1;
            node->vn = 1;
            delete e;
            s.push(node);			
        }
        else while (e->vm > 0 && e->vn > 0)
        {  
            node = new AkmNode;
            node->vm =  e->vm;
            node->vn = e->vn - 1;
            s.push(node);
            e = s.peek();
        }

    }while(!s.empty());

    return k1;
}

/////////////////////////////////////////////////////////////////////////////////////
//Ackerman问题(递归)
int ackerman(int n, int x, int y)
{
    if (n == 0)
    {
        return x + 1;
    }
    else if (n > 0 && 0 == y)
    {
        if (1 == n)
        {
            return x;
        }
        else if (2 == n)
        {
            return 0;
        }
        else if (3 == n)
        {
            return 1;
        }
        else if (n >= 4)
        {
            return 2;
        }
    }
    else if (n > 0 && y > 0)
    {
        return ackerman(n - 1, ackerman(n, x, y - 1), x);
    }
    return -100;
}

//Ackerman问题(非递归)
int value(int n, int x, int y)
{
    if (n == 0)
    {
        return x + 1;
    }
    else
    {
        switch (n)
        {
        case 1:
            return x;
        case 2:
            return 0;
        case 3:
            return 1;
        default:
            return 2;
        }
    }
}

//Ackerman问题(递归)
int ackerman_v2(int n, int x, int y)
{
    Stack<AckerNode*> s(100);
    AckerNode* node = new AckerNode;
    AckerNode *e = new AckerNode;
    node->nval = n;
    node->xval = x;
    node->yval = y;
    s.push(node);
    int v = -100;

    do
    {
        e = s.peek();
        while (e->nval > 0 && e->yval > 0)
        {
            node = new AckerNode;
            node->nval = e->nval;
            node->xval = e->xval;
            node->yval = e->yval - 1;
            s.push(node);
            e = s.peek();
        }

        node = new AckerNode;
        e = s.pop();
        v = value(e->nval, e->xval, e->yval);
        delete e;
        if (!s.empty())
        {
            e = s.pop();
            node->nval = e->nval - 1;
            node->yval = e->xval;
            node->xval = v;
            delete e;
            s.push(node);
        }

    }while (!s.empty());

    return v;
}

/////////////////////////////////////////////////////////////////////////////////////
//全排列问题
void permute(int a[], int s, int n)
{
    if (s == n - 1)
    {
        for (int i = 0; i < n; i++)
        {
            cout << a[i] << " ";
        }
        cout << endl;
    }
    else
    {
        for (int i = s; i < n; i++)
        {
            swap<int>(a[s], a[i]);
            permute(a, s + 1, n);
            swap<int>(a[s], a[i]);
        }
    }
}

void permute_v2(int a[],  int n)
{
    Stack<int> s(100);
    for (int i = 0; i < n; i++)
    {
        s.push(i+1);
    }
    s.put();

    bool back = false;
    while (!s.empty() && !back)   
    {
        int i = s.pop(); 
        for (; i < n; i++)
        {
            if (!s.find(i + 1))
            {
                s.push(i + 1);
                for (int j = 1; j < n + 1; j++)
                {
                    if (!s.find(j))
                    {
                        s.push(j);
                    }
                }
                s.put();
                back = false;
                break;
            }
        }
    }
}

} //namespace container
