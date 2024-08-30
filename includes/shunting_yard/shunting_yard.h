#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include "../../includes/stack/MyStack.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/token/token_base.h"
#include "../../includes/token/leftparen.h"
#include "../../includes/token/rightparen.h"
#include "../../includes/token/logical.h"
#include "../../includes/token/operator.h"
#include "../../includes/token/relational.h"

#include <vector>

using namespace std;

class ShuntingYard
{
public:
    ShuntingYard(){}
    ShuntingYard(vector<string> v)
    {
        //cout << "VECTOR(SY): " << v << endl;
        _vector = v;
    }
    ShuntingYard(Queue<Token_Base*> infix)
    {
        _infix_q = infix;
    }

    void infix()
    {
        Queue<Token_Base*> infix;
        for (int i = 0; i < _vector.size(); i++)
        {
            //cout << "VECTOR[i]: " << _vector[i] << endl;
        
            if(_vector[i] == "=" || _vector[i] == ">" || _vector[i] == "<" || _vector[i] == ">=" || _vector[i] == "<=")
            {
                infix.push(new Relational(_vector[i]));
            }
            else if(_vector[i] == "and" || _vector[i] == "or")
            {
                infix.push(new Logical(_vector[i]));
            }
            else if(_vector[i] == "(")
            {
                infix.push(new LeftParen());
            }
            else if (_vector[i] == ")")
            {
                infix.push(new RightParen());
            }
            else
            {
                infix.push(new TokenStr(_vector[i]));
            }
        }
        _infix_q = infix;
        //cout << "INFIX_Q: " << _infix_q << endl;
    }
    Queue<Token_Base*> postfix(Queue<Token_Base*> infix)
    {
        //Sets new infix.
        _infix_q = infix;
        
        //cout << "INFIX: " << _infix_q << endl;

        //Creates stack and queue for ops and integers.
        Stack<Token_Base*> op_stack;
        Queue<Token_Base*> postfix;
        Token_Base* pop_val;


        //Organizes stacks and queues.
        while (!_infix_q.empty())
        {
            pop_val = _infix_q.pop();
            switch (pop_val->TypeOf())
            {
                case 1:                     // Pushes integers into postfix queue.
                    postfix.push(pop_val);
                    break;
                case 2:                     //Pushes operators/functions into op stack.
                    if (static_cast<Operator*>(pop_val)->get_prec() == -1)
                    {
                        postfix.push(pop_val);
                    }
                    else 
                    {
                        while (!op_stack.empty())
                        {
                            if (op_stack.top()->TkStr() == "(")
                            {
                                break;
                            }
                            int pop_prec = static_cast<Operator*>(pop_val)->get_prec();
                            int op_prec = static_cast<Operator*>(op_stack.top())->get_prec();
                            if (op_prec < pop_prec)
                            {
                                break;
                            }
                            postfix.push(op_stack.pop());
                        }
                        op_stack.push(pop_val);
                    }
                    break;       
                case 3:                     //Pushes left parenthesis into op_stack.
                    op_stack.push(pop_val);
                    break;
                case 4:                     //Pops stack when right parenthesis is seen.
                    while (op_stack.top()->TkStr() != "(")
                    {
                        postfix.push(op_stack.pop());
                    }
                    op_stack.pop(); //disposes of left parenthesis.
                    break;  
                case 7:
                    //postfix.push(pop_val);
                    break;
                case 8:
                    postfix.push(pop_val);
                    break;      
                default:
                    break;
            }
        }
        //Pops operators into postfix
        while (!op_stack.empty())
        {
            pop_val = op_stack.pop();
            postfix.push(pop_val);
        }

        //cout << "POSTFIX: " << postfix << endl;

        return postfix;
    }
    Queue<Token_Base*> postfix()
    {
        return postfix(_infix_q);
    }

    void infix (Queue<Token_Base*> infix)
    {
        _infix_q = infix;
    }

private:
    Queue<Token_Base*> _infix_q;
    vector<string> _vector;
};


#endif
