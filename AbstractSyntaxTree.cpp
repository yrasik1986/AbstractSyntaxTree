#include <iostream>
#include <memory>
#include <stack>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

// Внимание!
// Для простоты разбора будем использовать только числа из одной цифры,
// а также не будет скобок, пробелов и ненужных символов.
// При этом, будем считать, что выражение всегда корректно.

struct Node {
    virtual int Evaluate() const = 0;
};

struct Value : public Node {
    Value(char digit) : _value(digit - '0') {}

    int Evaluate() const override { return _value; }

private:
    const uint8_t _value;
};

struct Variable : public Node {
    Variable(const int& x) : _x(x) {}// передаем зеначение по константной ссылке, чтобы не миеть возможность изменить значение

    int Evaluate() const override { return _x; }

private:
    const int& _x; // класс хранит ссылку на  переменную
};

struct Op : public Node {
    Op(char value) // в конструктор передается знак операции которым инициализируется поле _op и устанавливается приоритет операции
        : precedence([value] {
        if (value == '*') { // операция более приоритеная перед + -
            return 2;
        }
        else {
            return 1; // приоритет +-
        }
    }()),
        _op(value) {}

    const uint8_t precedence;

    int Evaluate() const override { // в завистмости от знака операции на выход выдаем значение операции
        if (_op == '*') {
            return _left->Evaluate() * _right->Evaluate();
        }
        else if (_op == '+') {
            return _left->Evaluate() + _right->Evaluate();
        }
        else if (_op == '-') {
            return _left->Evaluate() - _right->Evaluate();
        }

        return 0;
    }

    void SetLeft(shared_ptr<Node> node) { _left = node; }
    void SetRight(shared_ptr<Node> node) { _right = node; }

private:
    const char _op;
    shared_ptr<const Node> _left, _right;
};

template <class Iterator>
shared_ptr<Node> Parse(Iterator token, Iterator end, const int& x) {
    // Empty expression
    if (token == end) {
        return make_shared<Value>('0');
    }

    stack<shared_ptr<Node>> values;
    stack<shared_ptr<Op>> ops;

    auto PopOps = [&](int precedence) {
        while (!ops.empty() && ops.top()->precedence >= precedence) {
            auto value1 = values.top();
            values.pop();
            auto value2 = values.top();
            values.pop();
            auto op = ops.top();
            ops.pop();

            op->SetRight(value1);
            op->SetLeft(value2);

            values.push(op);
        }
    };

    while (token != end) {
        const auto& value = *token;
        if (value >= '0' && value <= '9') {
            values.push(make_shared<Value>(value));
        }
        else if (value == 'x') {
            values.push(make_shared<Variable>(x));
        }
        else if (value == '*') {
            PopOps(2);
            ops.push(make_shared<Op>(value));
        }
        else if (value == '+' || value == '-') {
            PopOps(1);
            ops.push(make_shared<Op>(value));
        }

        ++token;
    }

    while (!ops.empty()) {
        PopOps(0);
    }

    return values.top();
}

int main() {
    string tokens;
    cout << "Enter expression: ";
    getline(cin, tokens);

    int x = 0;
    auto node = Parse(tokens.begin(), tokens.end(), x);

    cout << "Enter x: ";
    while (cin >> x) {
        cout << "Expression value: " << node->Evaluate() << endl;
        cout << "Enter x: ";
    }

    return 0;
}