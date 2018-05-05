#include "ExpressionConverter.h"
// Write all your functions here
/*-------------------------------------
       Checking if operands
--------------------------------------*/
bool isOperands(const string &symbol)
{
	bool isNumber = false;
	for (unsigned int i = 0; i < symbol.size(); i++)//go through the string
	{
		if (!isdigit(symbol[i]))//if it is a number?
			isNumber = false;//isNumber= false if it is not a number
		else
			isNumber = true;//isNumber= true if it is a number
	}
	return isNumber;//return true or false
}
/*-------------------------------------
      Checking for priority Operators
--------------------------------------*/
int Priority(const string &c)
{
	//*,/ have the same priority and higher than +,_
	if (c == "*" || c == "/")
		return 2;
	if (c == "+" || c == "-")
		return 1;
	else
		return 0;
}
/*-------------------------------------
       Checking if operators
--------------------------------------*/
bool isOperator(const string &c)
{
	if (c == "+" || c == "-" || c == "*" || c == "/")//if they are operators?
		return true;//yes, return true
	else
		return false;//no, return false
}
/*-------------------------------------
     Convert infix string to RPN
--------------------------------------*/
string Infix2RPN(string infix)
{
	//These steps following the algorithm givin in project 4 pdf file.
	//using string stream to break out words
	istringstream iss(infix);
	vector<string> tokens;//store the tokens here
	while (iss)
	{
		string temp;
		iss >> temp;
		tokens.push_back(temp);
	}
	vector<string> outputList;//output vector
	stack<string> opStack;//keeping operators

	for (unsigned int i = 0; i < tokens.size(); i++) //scan the token list
	{
		//if it is operand, add it to outputList
		if (isOperands(tokens[i]))
		{
			outputList.push_back(tokens[i]);
		}

		//if it is left parenthesis, push it on opStack
		if (tokens[i] == "(")
		{
			opStack.push(tokens[i]);
		}

		//If the token is a right parenthesis, pop the opstack until the corresponding 
		//left parenthesis is removed. Add each operator to the end of the output list.
		if (tokens[i] == ")")
		{
			while (!opStack.empty() && opStack.top() != "(")
			{
				outputList.push_back(opStack.top());
				opStack.pop();
			}
			opStack.pop();
		}

		//If the token is an operator, *, / , +, or -, push it on the opstack.
		//However, first remove any operators already on the opstack that 
		//have higher or equal precedence and add them to the output list
		if (isOperator(tokens[i]) == true)
		{
			while (!opStack.empty() && Priority(opStack.top()) >= Priority(tokens[i]))
			{
				outputList.push_back(opStack.top());
				opStack.pop();
			}
			opStack.push(tokens[i]);
		}
	}
	//pop any remaining operators from the stack and insert to outputlist
	while (!opStack.empty())
	{
		outputList.push_back(opStack.top());
		opStack.pop();
	}
	//adding outputList into a postFix string to return
	string postFix;//string holding postFix string
	for (unsigned int i = 0; i < outputList.size(); i++)
	{
		//The first one adding to string postFix,  no needed space
		if (i == 0)
			postFix += outputList[i];
		//Adding next string to postFix string with a space preceded
		else
			postFix += " " + outputList[i];
	}
	return postFix;
}
/*-------------------------------------
      Convert RPN to infix string
--------------------------------------*/
string RPN2Infix(string postFix) {
	//using string stream to break out words from a string
	//read in the postFix string, break it out, push each word on popFixList
	stack<string>operandStack;
	istringstream iss(postFix);
	vector<string> postFixList;//store the tokens here
	while (iss)
	{
		string temp;
		iss >> temp;
		postFixList.push_back(temp);
	}
	//Go throug the list checking for operands and operators
	for (int i = 0; i < postFixList.size(); i++) {
		//if it is operand push it on operandStack
		if (isOperands(postFixList[i]))
		{
			operandStack.push(postFixList[i]);
		}
		//if it is operator, adding it to its operands with opening and
		//closing parenthesis into a single string, push
		//this string to operandStack, make sure to pop out 2 operands
		if (isOperator(postFixList[i]))
		{
			string temp1 = operandStack.top();
			operandStack.pop();
			string temp2 = operandStack.top();
			operandStack.pop();
			string temp3 = "("+temp2 + postFixList[i] + temp1+")";
			operandStack.push(temp3);
		}
	}
	return operandStack.top();
}
/*-------------------------------------
      Evaluating RPN
--------------------------------------*/
int EvaluateRPN(string postFix) {
	int result;//holding result of evaluating RPN
    //read in the postFix string, break it out, push each word on popFixList
	stack<int>operandStack;
	istringstream iss(postFix);
	vector<string> postFixList;//store the tokens here
	while (iss)
	{
		string temp;
		iss >> temp;
		postFixList.push_back(temp);
	}
	//go through the list, find operands and operators then do 
	//the calculations for result
	for (int i = 0; i < postFixList.size(); i++)
	{
		if (isOperands(postFixList[i]))
		{
			operandStack.push(stoi(postFixList[i]));
		}
		if (postFixList[i] == "+")
		{
			int x = operandStack.top();
			operandStack.pop();
			int y = operandStack.top();
			operandStack.pop();
			result = x + y;
			operandStack.push(result);
		}
		if (postFixList[i] == "-")
		{
			int x = operandStack.top();
			operandStack.pop();
			int y = operandStack.top();
			operandStack.pop();
			result = y - x;
			operandStack.push(result);
		}
		if (postFixList[i] == "*")
		{
			int x = operandStack.top();
			operandStack.pop();
			int y = operandStack.top();
			operandStack.pop();
			result = x*y;
			operandStack.push(result);
		}
		if (postFixList[i] == "/")
		{
			int x = operandStack.top();
			operandStack.pop();
			int y = operandStack.top();
			operandStack.pop();
			result = y / x;
			operandStack.push(result);
		}
	}
	return result;
}
/*------------------------------------------------------------
    Evaluating Infix
---------------------------------------------------------------*/
int EvaluateInfix(string infix) {
	//Calling Infix2RPN function to convert infix to RPN
	//then calling EvaluateRPN function to evaluate it
	//return value of infix.
	return EvaluateRPN(Infix2RPN(infix));
}
/*---------------------------------------------------------------
   Convert From RPN Expression to Tree
---------------------------------------------------------------*/
ArithNode *RPN2ExpressionTree(string postFix) {
	//Read the input postFix string, break out words
	//These are following the algorithm given in project 4 pdf file.
	stringstream ss;
	ss.str(postFix);
	string s;//holding a string word
	stack<ArithNode*>stackNode;
	ArithNode* root;
	while (ss>>s)
	{
		if (!isOperator(s))
		{
			int num = stoi(s);
			ArithNode *node = new ArithNode(num);
			stackNode.push(node);
		}
		else
		{
			root = new ArithNode(s[0]);
			root->right = stackNode.top();
			stackNode.pop();
			root->left = stackNode.top();
			stackNode.pop();
			stackNode.push(root);
		}
	}
	root = stackNode.top();
	return root;
}
/*-------------------------------------
  Convert From Infix Expression to Tree
--------------------------------------*/
ArithNode *Infix2ExpressionTree(string inFix) {
	//calling Infix2RPN function to convert infix
	// string to RPN, Then calling RPN2ExpressionTree
	//to convert to Tree, return string in infix
	return RPN2ExpressionTree(Infix2RPN(inFix));
}
