/*
 从键盘上任意输入一个算术表达式，编写程序，计算表达式的值。
例如：键盘输入12+25*4,，自动计算出结果为112。
提示：利用堆栈来完成该问题的求解，设计两个栈，一个处理运算数，另一个处理运算符。
这是计算整数版本
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 100

// 定义栈结构
typedef struct
{
  int top;
  int data[MAX_SIZE];
} Stack;

// 初始化栈
void initStack(Stack *stack)
{
  stack->top = -1;
}

// 判断栈是否为空
int isEmpty(Stack *stack)
{
  return stack->top == -1;
}

// 判断栈是否已满
int isFull(Stack *stack)
{
  return stack->top == MAX_SIZE - 1;
}

// 入栈
void push(Stack *stack, int value)
{
  if (isFull(stack))
  {
    printf("Stack is full. Cannot push element.\n");
    return;
  }

  stack->data[++stack->top] = value;
}

// 出栈
int pop(Stack *stack)
{
  if (isEmpty(stack))
  {
    printf("Stack is empty. Cannot pop element.\n");
    return -1;
  }

  return stack->data[stack->top--];
}

// 获取栈顶元素
int peek(Stack *stack)
{
  if (isEmpty(stack))
  {
    printf("Stack is empty.\n");
    return -1;
  }

  return stack->data[stack->top];
}

int hasHigherPrecedence(char op1, char op2)
{
  int precedence1, precedence2;

  switch (op1)
  {
  case '+':
  case '-':
    precedence1 = 1;
    break;
  case '*':
  case '/':
    precedence1 = 2;
    break;
  default:
    precedence1 = 0;
    break;
  }

  switch (op2)
  {
  case '+':
  case '-':
    precedence2 = 1;
    break;
  case '*':
  case '/':
    precedence2 = 2;
    break;
  default:
    precedence2 = 0;
    break;
  }

  return precedence1 >= precedence2;
}

// 执行运算
int performOperation(int operand1, int operand2, char op)
{
  switch (op)
  {
  case '+':
    return operand1 + operand2;
  case '-':
    return operand1 - operand2;
  case '*':
    return operand1 * operand2;
  case '/':
    return operand1 / operand2;
  default:
    printf("Invalid operator.\n");
    return 0;
  }
}

// 计算表达式的值
int evaluateExpression(char *expression)
{
  Stack operandStack;
  Stack operatorStack;
  initStack(&operandStack);
  initStack(&operatorStack);

  int i = 0;
  while (expression[i] != '\0')
  {
    if (expression[i] >= '0' && expression[i] <= '9')
    {
      int operand = 0;
      while (expression[i] >= '0' && expression[i] <= '9')
      {
        operand = operand * 10 + (expression[i] - '0');
        i++;
      }
      push(&operandStack, operand);
    }
    else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
    {
      while (!isEmpty(&operatorStack) && peek(&operatorStack) != '(' && hasHigherPrecedence(peek(&operatorStack), expression[i]))
      {
        int operand2 = pop(&operandStack);
        int operand1 = pop(&operandStack);
        char op = pop(&operatorStack);
        int result = performOperation(operand1, operand2, op);
        push(&operandStack, result);
      }
      push(&operatorStack, expression[i]);
      i++;
    }
    else if (expression[i] == '(')
    {
      push(&operatorStack, expression[i]);
      i++;
    }
    else if (expression[i] == ')')
    {
      while (!isEmpty(&operatorStack) && peek(&operatorStack) != '(')
      {
        int operand2 = pop(&operandStack);
        int operand1 = pop(&operandStack);
        char op = pop(&operatorStack);
        int result = performOperation(operand1, operand2, op);
        push(&operandStack, result);
      }
      if (!isEmpty(&operatorStack) && peek(&operatorStack) == '(')
      {
        pop(&operatorStack);
      }
      else
      {
        printf("Invalid expression. Mismatched parentheses.\n");
        return 0;
      }
      i++;
    }
    else
    {
      printf("Invalid character in the expression.\n");
      return 0;
    }
  }

  while (!isEmpty(&operatorStack))
  {
    int operand2 = pop(&operandStack);
    int operand1 = pop(&operandStack);
    char op = pop(&operatorStack);
    int result = performOperation(operand1, operand2, op);
    push(&operandStack, result);
  }

  if (isEmpty(&operandStack))
  {
    printf("Invalid expression.\n");
    return 0;
  }

  return pop(&operandStack);
}

int main()
{
  char expression[MAX_SIZE];
  printf("Enter an arithmetic expression: ");
  fgets(expression, sizeof(expression), stdin);
  expression[strcspn(expression, "\n")] = '\0'; // 移除换行符

  int result = evaluateExpression(expression);
  printf("Result: %d\n", result);

  return 0;
}