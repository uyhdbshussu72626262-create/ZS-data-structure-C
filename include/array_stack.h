#pragma once
#include <stdbool.h>
#define MAX_SIZE 100

typedef struct linked_list_stack{
	int data[MAX_SIZE];
	int top;
} Stack1;

void init_stack(Stack1* s);
bool is_empty(const Stack1* s);			// 判空
bool is_full(const Stack1* s);			// 判满
bool push(Stack1* s, int value);		// 入栈
bool pop(Stack1* s, int *value);		// 出栈有具体的值
bool peek(const Stack1* s, int* value); // 求栈顶元素的值
int get_size(Stack1* s);				// 获取栈中元素的个数
void destroy_stack(Stack1* s);			// 销毁栈