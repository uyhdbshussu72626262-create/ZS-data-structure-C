#include <stdbool.h>
#include "array_stack.h"

// 顺序栈
void init_stack(Stack1* s)
{
	s->top = -1;
}

bool is_empty(const Stack1* s)
{
	return s->top == -1;
}

bool is_full(const Stack1* s)
{
	return s->top == MAX_SIZE -1;
}

bool push(Stack1* s, int value)
{
	if (is_full(s)) return false;
	s->data[++(s->top)] = value;
	return true;
}

bool pop(Stack1* s, int* value)
{
	if (is_empty(s)) return false;
	*value = s->data[(s->top)--];
	return true;
}

bool peek(const Stack1* s, int* value)
{
	if (is_empty(s)) return false;
	*value = s->data[(s->top)];
	return true;
}

int get_size(Stack1* s)
{
	// 空栈元素数为0
	if (is_empty(s)) return 0;
	// else
	return s->top + 1;

}// 获取栈中元素的个数

void destroy_stack(Stack1* s) 
{
	s->top = -1;
}// 销毁栈