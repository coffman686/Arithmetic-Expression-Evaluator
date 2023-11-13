#include <iostream>
#include <sstream>

using namespace std;


float sum(float num1, float num2) {
    return num1 + num2;
}

float difference(float num1, float num2) {
    return num1 - num2;
}

float product(float num1, float num2) {
    return num1 * num2;
}

float quotient(float num1, float num2) {
    return num1 / num2;
}


int main() {
    // Get user input for the expression
    cout << "Enter your expression (addition/subtraction/multiplication/division supported): ";
    string expression;
    getline(cin, expression);

    // Extract the numbers and operator from the expression
    char op;
    double a, b;
    istringstream iss(expression);
    iss >> a >> op >> b;

    // Calculate and display the result based on the operator
    double result;
    if (op == '+') {
        result = sum(a, b);
        cout << "Answer: " << result << endl;
    } else if (op == '-') {
        result = difference(a, b);
        cout << "Answer: " << result << endl;
    } else if (op == '*') {
        result = product(a, b);
        cout << "Answer: " << result << endl;
    } else if (op == '/') {
        result = quotient(a, b);
        cout << "Answer: " << result << endl;
    } else {
        cout << "Invalid operator. Please use '+' or '-'." << endl;
    }

    return 0;
}