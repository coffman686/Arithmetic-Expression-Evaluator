#include <iostream>
#include <cstdlib>
#include <vector> //Could use an actual stack, but vector made more sense to us
#include <string>
#include <unordered_map> //Allows operator precedence
#include <cmath> //Saves time to avoid exponential funcs
#include <iomanip> //Using this to format test case prints
using namespace std; //Time saver for writing C++ 

bool AreParenthesesBalanced(const string &expression) {
    //Function that checks we have correct parentheses amount
    int openParenCount = 0;
    
    for (char c : expression) {
        if (c == '(') {
            openParenCount++;
        } else if (c == ')') {
            if (openParenCount == 0) {
                return false;  // More closing parentheses than opening ones
            }
            openParenCount--;
        }
    }

    return openParenCount == 0;  // True if parentheses are balanced
}

vector<string> Tokenize(string &expression, unordered_map<string, int> &opMap) {
    //Function to convert our expression to an unordered map of tokens with precedence
    vector<string> tokens;
    string temp = "";

    if (!AreParenthesesBalanced(expression)) {
        throw runtime_error("Unmatched parenthesis in expression");
        //If parentheses aren't balanced it's over
    }
    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (c == ' ') //Keep going where the token is nothing
            continue;
        else if (opMap.find(string(1, c)) != opMap.end()) { //Find the token in our operator map and push it back
            if (temp != "") {
                tokens.push_back(temp);
                temp = "";
            }

            if ((c == '-' || c == '+') && (i == 0 || expression[i - 1] == '(' || expression[i - 1] == '^')) {
                //Above statement helps with unary negation handling
                tokens.push_back("0");  
                tokens.push_back(string(1, c));
            } else {
                tokens.push_back(string(1, c));
            }
        } else if (isalpha(c) || !isalnum(c)) { //If what is found is a number or a symbol end the program
            throw runtime_error("Unrecognized arithmetic symbol '" + string(1, c) + "'");
        } else {
            temp += c;
        }
    }

    if (temp != "") //Push back whatever isn't the end or "" here
        tokens.push_back(temp);

    return tokens; //Return our new tokens
}

vector<string> InfixToPostfix(vector<string> &infix, unordered_map<string, int> &opMap) {
    /*Infix to postfix is a method of stack alteration that helps keep track of operators and their precedence 
    as such, we decided it was a useful method to know about and have included information in the FAQ. We chose
    this and string vectors as an alternative to the native C++ stack header, but it works well. Here we essentially
    determine precedence and move operators as need be */
    vector<string> postfix; //Initialize our "stacks"
    vector<string> stack;
    infix.push_back(")");

    for (string token : infix) { //Iterate through our tokens
        if (opMap.find(token) == opMap.end()) {  //If we find the token, end
            postfix.push_back(token);
        } else if (token == "(") { //Check for parentheses
            stack.push_back(token);
        } else if (token == ")") {
            while (!stack.empty() && stack.back() != "(") {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            if (!stack.empty()) //If the stack isn't empty pop back
                stack.pop_back();  
        } else {
            while (!stack.empty() && opMap[token] <= opMap[stack.back()]) { //Loop while stack isn't empty
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(token);
        }
    }

    return postfix; //Return our postfix expression
}

float CalculatePostfix(vector<string> &postfix, unordered_map<string, int> &opMap) { 
    /*Function that actually calculates our expression, and takes the postfix as an input
    as well as our 'stack' and our precedence maps that tell the calculator how to run
    calculations. */
    vector<float> evaluationStack; //Initialize our stack to be evaluated

    for (string token : postfix) { //Iterate through tokens
        if (opMap.find(token) != opMap.end()) {
            if (token == "-") {  
                if (evaluationStack.size() < 2) { //Ensure there is an expression to evaluate
                    throw runtime_error("Not enough operands for operator " + token); //End program if there aren't enough ops
                    
                }

                float n1 = evaluationStack.back(); //Initialize a float of our value to evaluate, same for next val
                evaluationStack.pop_back();
                float n2 = evaluationStack.back();
                evaluationStack.pop_back();

                float result = n2 - n1; //Same as above
                evaluationStack.push_back(result);
            } else {
                if (evaluationStack.size() < 2) { //Check size again
                     throw runtime_error("Not enough operands for operator " + token);
                      
                }

                float n1 = evaluationStack.back(); //Repeat what was done as above
                evaluationStack.pop_back();
                float n2 = evaluationStack.back();
                evaluationStack.pop_back();

                if (token == "+") { //Check operator type and push back result
                    evaluationStack.push_back(n2 + n1);
                } else if (token == "*") {
                    evaluationStack.push_back(n2 * n1);
                } else if (token == "/") {
                    if (n1 != 0) {
                        evaluationStack.push_back(n2 / n1);
                    } else {
                        cerr << ("Error: Cannot divide by zero") << endl; //Error that will spit if our n1 is zero
                    }
                } else if (token == "%") {
                    evaluationStack.push_back(fmod(n2, n1));
                } else if (token == "^") {
                    evaluationStack.push_back(pow(n2, n1));
                }
            }
        } else {
            evaluationStack.push_back(atof(token.c_str())); //If nothing else, push back
        }
    }

    if (evaluationStack.size() != 1) {
        throw runtime_error("Invalid expression syntax");
          
    }
    return evaluationStack[0];
}

void test(string expression, float expected, int casenum) {
    /* Function we made to try and test every expected case that should return a value
    and the function is definitely super wonky, but this isn't the project so don't judge it*/
    unordered_map<string, int> opMap; //Operator precedence mapping
    opMap["^"] = 4;
    opMap["*"] = 3;
    opMap["/"] = 3;
    opMap["%"] = 2;
    opMap["+"] = 1;
    opMap["-"] = 1;
    opMap["("] = -1;
    opMap[")"] = -1;

    try { //Using a try so we can catch potential errors
        vector<string> tokens = Tokenize(expression, opMap); //Initialize our stacks
        vector<string> postfix = InfixToPostfix(tokens, opMap);
        float actual = CalculatePostfix(postfix, opMap); //Calculate our expression

        // Set column width to 10
        cout << left << setw(5) << "CASE " << setw(5) << casenum << ": " << setw(20) << expression << " | ";

        if (expected != actual) { //Error throwing/handling works here
            cout << "FAILURE. Expected Value: " << setw(10) << expected << " Actual Value: " << actual << endl;
        } else {
            cout << "PASSED. Expected Value: " << setw(10) << expected << " Actual Value: " << actual << endl;
        }
    } catch (const exception &e) {
        cerr << left << setw(5) << "CASE " << setw(5) << casenum << ": " << setw(20) << expression << " | ";
        cerr << "FAILED. Unexpected Error: " << e.what() << endl;
    } catch (...) {
        cerr << left << setw(5) << "CASE " << setw(5) << casenum << ": " << setw(20) << expression << " | ";
        cerr << "FAILED. Unexpected Error." << endl;
    }
}

int main() {
    /*Main function that has operator precedence, takes user input, and makes calls
    to calculate the input. The simplest function.*/
    unordered_map<string, int> opMap; //opMap determines our operator precedence, utilize pemdas
    opMap["^"] = 4; 
    opMap["*"] = 3;
    opMap["/"] = 3;
    opMap["%"] = 2; //Wasn't really sure where to slap modulo, so it is higher precedence than +/- and less than *//
    opMap["+"] = 1;
    opMap["-"] = 1;
    opMap["("] = -1; //Parenthesis are -1 because they are handled elsewise
    opMap[")"] = -1;

    string expression;
    cout << "Enter your expression: ";
    getline(cin, expression); //Get user's expression
    vector<string> tokens = Tokenize(expression, opMap); //Create our expression token    
    vector<string> postfix = InfixToPostfix(tokens, opMap); //Find how we will calculator
    float result = CalculatePostfix(postfix, opMap); //Use above info to calculate answer
    cout << "Your result is: " << result << endl;
    
/* Test cases, if you want to use them just comment out the above block in main
    test("4 + 5", 9, 1);
    test("10 - 5", 5, 2);
    test("2 * 3", 6, 3);
    test("25 / 5", 5, 4);
    test("20 % 2", 0, 5);
    test("2 ^ 3", 8, 6);
    test("-(-(3)) + 1", 4, 7);
    test("5 + 3 - 4", 4, 8);
    test("6 * 7 / 25", 1.68, 9);
    test("8 * 9 + 4 - 3 / 10", 75.7, 10);
    test("(2 + 1) + (1)", 4, 11);
    test("(((2 + 4))) + ((2))", 8, 12);
    test("* 5 + 1", nanf(""), 13);
    test("((2 + 1)", nanf(""), 14);
    test("(2 @ 3)", -6.8, 15);
    test("3 + 4", 7, 16);
    test("8 - (5 - 2)", 5, 17);
    test("10 * 2 / 5", 4, 18);
    test("2 ^ 3", 8, 19);
    test("4 * (3 + 2) % 7 - 1", 5, 20);
    test("(((2 + 3))) + (((1 + 2)))", 8, 21);
    test("((5 * 2) - ((3 / 1) + ((4 % 3))))", 6, 22);
    test("(((2 ^ (1 + 1)) + ((3 - 1) ^ 2)) / ((4 / 2) % 3))", 4, 23);
    test("(((((5 - 3))) * (((2 + 1))) + ((2 * 3))))", 12, 24);
    test("((9 + 6)) / ((3 * 1) / (((2 + 2))) - 1)", -60, 25);
    test("+(-2) * (-3) - ((-4) / (+5))", 6.8, 26);
    test("-(+1) + (+2)", 1, 27);
    test("-(-(-3)) + (-4) + (+5)", -2, 28);
    test("+2 ^ (-3)", 0.125, 29);
    test("-(+2) * (+3) - (-4) / (-5)", -6.8, 30);
    test("2 * (4 + 3 - 1", nanf(""), 31);
    test("* 5 + 2", nanf(""), 32);
    test("4/0", nanf(""), 33);
    test("5 (2 + 3)", nanf(""), 34);
    test("7 & 3", nanf(""), 35);
    test("(((3 + 4) - 2) + (1)", nanf(""), 36);
    test("((5 + 2) / (3 * 0))", nanf(""), 37);
    test("((2 -) 1 + 3)", nanf(""), 38);
    test("((4 * 2) + ( - ))", nanf(""), 39);
    test("((7 * 3) @ 2)", nanf(""), 40);
*/

}