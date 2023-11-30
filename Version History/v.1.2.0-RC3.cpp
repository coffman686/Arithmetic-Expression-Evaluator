#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
using namespace std;

vector<string> Tokenize(string &expression, unordered_map<string, int> &opMap) {
    vector<string> tokens;
    string temp = "";

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (c == ' ')
            continue;
        else if (opMap.find(string(1, c)) != opMap.end()) {
            if (temp != "") {
                tokens.push_back(temp);
                temp = "";
            }

            if ((c == '-' || c == '+') && (i == 0 || expression[i - 1] == '(' || expression[i - 1] == '^')) {
                tokens.push_back("0");  
                tokens.push_back(string(1, c));
            } else {
                tokens.push_back(string(1, c));
            }
        } else {
            temp += c;
        }
    }

    if (temp != "")
        tokens.push_back(temp);

    return tokens;
}




vector<string> InfixToPostfix(vector<string> &infix, unordered_map<string, int> &opMap) {
    vector<string> postfix;
    vector<string> stack;
    infix.push_back(")");

    for (string token : infix) {
        if (opMap.find(token) == opMap.end()) {  
            postfix.push_back(token);
        } else if (token == "(") {
            stack.push_back(token);
        } else if (token == ")") {
            while (!stack.empty() && stack.back() != "(") {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            if (!stack.empty())
                stack.pop_back();  
        } else {
            while (!stack.empty() && opMap[token] <= opMap[stack.back()]) {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(token);
        }
    }

    return postfix;
}

float CalculatePostfix(vector<string> &postfix, unordered_map<string, int> &opMap) {
    vector<float> evaluationStack;

    for (string token : postfix) {
        if (opMap.find(token) != opMap.end()) {
            if (token == "-") {  
                if (evaluationStack.size() < 2) {
                    cerr << "Error: Not enough operands for operator " << token << endl;
                    exit(1);  
                }

                float n1 = evaluationStack.back();
                evaluationStack.pop_back();
                float n2 = evaluationStack.back();
                evaluationStack.pop_back();

                float result = n2 - n1;
                evaluationStack.push_back(result);
            } else {
                if (evaluationStack.size() < 2) {
                    cerr << "Error: Not enough operands for operator " << token << endl;
                    exit(1);  //
                }

                float n1 = evaluationStack.back();
                evaluationStack.pop_back();
                float n2 = evaluationStack.back();
                evaluationStack.pop_back();

                if (token == "+") {
                    evaluationStack.push_back(n2 + n1);
                } else if (token == "*") {
                    evaluationStack.push_back(n2 * n1);
                } else if (token == "/") {
                    if (n1 != 0) {
                        evaluationStack.push_back(n2 / n1);
                    } else {
                        cerr << ("Error: Cannot divide by zero") << endl;
                    }
                } else if (token == "%") {
                    evaluationStack.push_back(fmod(n2, n1));
                } else if (token == "^") {
                    evaluationStack.push_back(pow(n2, n1));
                }
            }
        } else {
            evaluationStack.push_back(atof(token.c_str()));
        }
    }

    if (evaluationStack.size() != 1) {
        cerr << "Error: Invalid postfix expression" << endl;
        exit(1);  
    }
    return evaluationStack[0];
}


int main() {
    unordered_map<string, int> opMap;
    opMap["^"] = 4;
    opMap["*"] = 3;
    opMap["/"] = 3;
    opMap["%"] = 2;
    opMap["+"] = 1;
    opMap["-"] = 1;
    opMap["("] = -1;
    opMap[")"] = -1;

    string expression;
    cout << "Enter your expression: ";
    getline(cin, expression);

    vector<string> tokens = Tokenize(expression, opMap);     
    vector<string> postfix = InfixToPostfix(tokens, opMap);   
    float result = CalculatePostfix(postfix, opMap);
    cout << "Your result is: " << result << endl;
}

   
