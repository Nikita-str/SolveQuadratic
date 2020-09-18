#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <string.h>

/*
struct ChildString
	{
	char* parent_next_char;
	unsigned int length;
	};
*/

/*
#define SizeCheck(A, B, C) \
if(A == B) \
	{
	C = (typeof(C))realloc(C, B * 2);
	CheckMem(C);
	}
*/

struct WeirdString
	{
	size_t all_str_len;//len(that str) + len(all children str and their children str and ...) 

	size_t s_length;//len of s_str (i.e. that str without children)
	size_t s_length_max;//when we need alloc
	char* s_ptr;
	
	struct WeirdString** child_strs;
	//size_t child_now; // now always = lengths_now - 1; I can del it, but code will grow and it is not primary target
	//size_t child_max; // now always = lengths_max - 1; I can del it, ...

	size_t* lengths;//array of segments length
	size_t lengths_now;//amount of segments
	size_t lengths_max;//when we need in alloc
	};

typedef struct WeirdString WeirdString;

void CheckMem(void* ptr)
	{
	if(!ptr)
		{
		printf("allocated memory error\n");
		exit(1);
		}
	}

static WeirdString* privateCreateWeirdString(size_t Chars, size_t Childs, size_t Lengths)
	{
	assert(Chars);
	assert(Childs);
	assert(Lengths);
	WeirdString* ws = (WeirdString*)calloc(1, sizeof(*ws));
	CheckMem(ws);

	ws->all_str_len = 0;

	ws->s_ptr = (char*)calloc(Chars, sizeof(*ws->s_ptr));
	CheckMem(ws->s_ptr);
	ws->s_length_max = Chars;
	ws->s_length = 0;

	ws->child_strs = (WeirdString**)calloc(Childs, sizeof(*ws->child_strs));
	CheckMem(ws->child_strs);

	ws->lengths = (size_t*)calloc(Lengths, sizeof(*ws->lengths));
	CheckMem(ws->lengths);
	ws->lengths_max = Lengths;
	ws->lengths_now = 1;
	}

WeirdString* CreateWeirdString()
	{
	enum { CHARS = 16, CHILDS = 2, LENGHTS = CHILDS + 1 };
	//enum { CHARS = 2, CHILDS = 1, LENGHTS = CHILDS + 1}; //TODO:FOR TEST
	WeirdString* ws = (WeirdString*)calloc(1, sizeof(*ws));
	return privateCreateWeirdString(CHARS, CHILDS, LENGHTS);
	}

void FreeWeirdString(WeirdString* ws)
	{
	if(!ws)return;
	for(size_t i = 0; i < ws->lengths_now - 1; i++)
		{
		FreeWeirdString(ws->child_strs[i]);
		}
	free(ws->child_strs);
	free(ws->lengths);
	free(ws);
	}


char WeirdString_GetChar(WeirdString* ws, size_t position)
	{
	assert(ws);
	if(position > ws->all_str_len)return 0;
	char ret = 0;
	size_t len_now = 0;
	size_t child_len_now = 0;
	for(size_t i = 0; i < ws->lengths_now; i++)
		{
		len_now += ws->lengths[i];
		if(len_now >= position)
			{
			assert(position - child_len_now < ws->s_length);
			return ws->s_ptr[position - child_len_now];
			}

		assert(i != ws->lengths_now - 1);///< in the last spin of the for, we can not turn up here
		if(len_now + ws->child_strs[i]->all_str_len >= position)
			{
			return WeirdString_GetChar(ws->child_strs[i], position - len_now);
			}
		len_now += ws->child_strs[i]->all_str_len;
		child_len_now += ws->child_strs[i]->all_str_len;
		}

	assert(!sizeof("we can't being here"));
	return 0;
	}

void WeirdString_AddCharEnd(WeirdString* ws, char c)
	{
	assert(ws);
	if(ws->s_length == ws->s_length_max)
		{
		ws->s_length_max *= 2;
		ws->s_ptr = (char*)realloc(ws->s_ptr, (ws->s_length_max) * sizeof(*ws->s_ptr));
		CheckMem(ws->s_ptr);
		}
	ws->lengths[ws->lengths_now - 1]++;
	ws->s_ptr[ws->s_length++] = c;
	ws->all_str_len++;
	}

void WeirdString_AddChar(WeirdString* ws, size_t position, char c)
	{
	assert(ws);
	assert(ws->lengths_now <= ws->lengths_max);
	assert(ws->s_length <= ws->s_length_max);
	assert(position <= ws->all_str_len);
	if(position == ws->all_str_len)
		{
		WeirdString_AddCharEnd(ws, c);
		return;
		}
	size_t len_now = 0;
	size_t child_len_now = 0;
	for(size_t i = 0; i < ws->lengths_now; i++)
		{
		size_t len_back = len_now;
		len_now += ws->lengths[i];
		if(len_now >= position)
			{
			assert(position - child_len_now < ws->s_length);
			WeirdString* new_ws = CreateWeirdString();
			if(ws->lengths_now == ws->lengths_max)
				{
				ws->lengths_max = (ws->lengths_max - 1) * 2 + 1;
				ws->child_strs = (WeirdString**)realloc(ws->child_strs, (ws->lengths_max - 1) * sizeof(*ws->child_strs));
				CheckMem(ws->child_strs);
				ws->lengths = (size_t*)realloc(ws->lengths, ws->lengths_max * sizeof(*ws->lengths));
				CheckMem(ws->lengths);
				}
			ws->lengths_now++;
			ws->child_strs[ws->lengths_now - 2] = new_ws;

			for(size_t j = ws->lengths_now - 1; j > i; j--)//swap segments length
				{
				size_t temp = ws->lengths[j];
				ws->lengths[j] = ws->lengths[j - 1];
				ws->lengths[j - 1] = temp;
				}
			for(size_t j = ws->lengths_now - 2; j > i; j--)//swap children
				{
				WeirdString* temp = ws->child_strs[j];
				ws->child_strs[j] = ws->child_strs[j - 1];
				ws->child_strs[j - 1] = temp;
				}
			ws->lengths[i + 1] = len_now - position;
			ws->lengths[i] = position - len_back;
			WeirdString_AddCharEnd(new_ws, c);
			ws->all_str_len++;
			return;
			}

		assert(i != ws->lengths_now - 1);///< in the last spin of the for, we can not turn up here
		if(len_now + ws->child_strs[i]->all_str_len >= position)
			{
			WeirdString_AddChar(ws->child_strs[i], position - len_now, c);
			ws->all_str_len++;
			return;
			}
		len_now += ws->child_strs[i]->all_str_len;
		child_len_now += ws->child_strs[i]->all_str_len;
		}

	assert(!sizeof("we can't being here"));
	return;
	}


void WeirdString_DelChar(WeirdString* ws, size_t position)
	{
	assert(ws);
	assert(ws->lengths_now <= ws->lengths_max);
	assert(ws->s_length <= ws->s_length_max);
	assert(position < ws->all_str_len);
	size_t len_in_cur_s = 0;
	size_t len_now = 0;
	size_t child_len_now = 0;
	for(size_t i = 0; i < ws->lengths_now; i++)
		{
		size_t len_back = len_now;
		len_now += ws->lengths[i];
		if(len_now > position)
			{
			assert(position - child_len_now < ws->s_length);
			WeirdString* new_ws = privateCreateWeirdString(1, 1, 1);
			if(ws->lengths_now == ws->lengths_max)
				{
				ws->lengths_max = (ws->lengths_max - 1) * 2 + 1;
				ws->child_strs = (WeirdString**)realloc(ws->child_strs, (ws->lengths_max - 1) * sizeof(*ws->child_strs));
				CheckMem(ws->child_strs);
				ws->lengths = (size_t*)realloc(ws->lengths, ws->lengths_max * sizeof(*ws->lengths));
				CheckMem(ws->lengths);
				}
			ws->lengths_now++;
			ws->child_strs[ws->lengths_now - 2] = new_ws;

			for(size_t j = ws->lengths_now - 1; j > i; j--)//swap segments length
				{
				size_t temp = ws->lengths[j];
				ws->lengths[j] = ws->lengths[j - 1];
				ws->lengths[j - 1] = temp;
				}
			for(size_t j = ws->lengths_now - 2; j > i; j--)//swap children
				{
				WeirdString* temp = ws->child_strs[j];
				ws->child_strs[j] = ws->child_strs[j - 1];
				ws->child_strs[j - 1] = temp;
				}
			ws->lengths[i + 1] = len_now - position - 1;
			ws->lengths[i] = position - len_back;
			for(size_t j = len_in_cur_s + ws->lengths[i]; j < ws->s_length; j++)
				{
				ws->s_ptr[j] = ws->s_ptr[j+1];
				}
			ws->all_str_len--;
			ws->s_length--;
			return;
			}
		len_in_cur_s += ws->lengths[i];

		assert(i != ws->lengths_now - 1);///< in the last spin of the for, we can not turn up here
		if(len_now + ws->child_strs[i]->all_str_len > position)
			{
			WeirdString_DelChar(ws->child_strs[i], position - len_now);
			ws->all_str_len--;
			return;
			}
		len_now += ws->child_strs[i]->all_str_len;
		child_len_now += ws->child_strs[i]->all_str_len;
		}

	assert(!sizeof("we can't being here"));
	return;
	}

static void Help_WeirdString_GNS(char* str, WeirdString* ws)
	{
	size_t len_s = 0;
	size_t len_now = 0;
	size_t child_len_now = 0;
	for(size_t i = 0; i < ws->lengths_now; i++)
		{
		assert(0 <= ws->lengths[i]);
		memcpy(str + len_now, ws->s_ptr + len_s, ws->lengths[i]);
		/*
		//TODO: del after debug
		size_t to = ws->lengths[i];
		for(size_t j = 0; j < to; j++)
			str[len_now + j] = ws->s_ptr[len_s + j];
		*/
		len_s += ws->lengths[i];
		len_now += ws->lengths[i];

		if(i != ws->lengths_now - 1)
			{
			Help_WeirdString_GNS(str + len_now, ws->child_strs[i]);
			child_len_now += ws->child_strs[i]->all_str_len;
			len_now += ws->child_strs[i]->all_str_len;
			}
		}
	}

char* WeirdString_GetNormalString(WeirdString* ws, size_t* out_str_len)
	{
	assert(ws);
	assert(out_str_len);
	*out_str_len = ws->all_str_len + 1;
	char* str = (char*)calloc(*out_str_len, sizeof(*str));
	CheckMem(str);
	Help_WeirdString_GNS(str, ws);
	str[*out_str_len - 1] = '\0';
	return str;
	}


void WS_TEST_00()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'A');
	WeirdString_AddChar(ws, 1, 'B');
	WeirdString_AddChar(ws, 1, 'C');
	WeirdString_AddChar(ws, 2, 'D');
	WeirdString_AddChar(ws, 1, 'E');
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: AECDB
	FreeWeirdString(ws);
	}

void WS_TEST_01()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'A');
	WeirdString_AddChar(ws, 1, 'B');
	WeirdString_AddChar(ws, 2, 'C');
	WeirdString_AddChar(ws, 3, 'D');
	WeirdString_AddChar(ws, 4, 'E');
	WeirdString_AddChar(ws, 5, 'F');
	WeirdString_AddChar(ws, 6, 'G');
	WeirdString_AddChar(ws, 7, 'H');
	WeirdString_AddChar(ws, 8, 'N');
	WeirdString_AddChar(ws, 9, 'I');
	WeirdString_AddChar(ws, 10, 'T');
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: ABCDEFGHNIT
	FreeWeirdString(ws);
	}

void WS_TEST_02()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'A');
	WeirdString_AddChar(ws, 0, 'B');
	WeirdString_AddChar(ws, 0, 'C');
	WeirdString_AddChar(ws, 0, 'D');
	WeirdString_AddChar(ws, 0, 'E');
	WeirdString_AddChar(ws, 0, 'F');
	WeirdString_AddChar(ws, 0, 'G');
	WeirdString_AddChar(ws, 0, 'H');
	WeirdString_AddChar(ws, 0, 'N');
	WeirdString_AddChar(ws, 0, 'I');
	WeirdString_AddChar(ws, 0, 'T');
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: NIMHGFEDCBA
	FreeWeirdString(ws);
	}

void WS_TEST_03()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'C');
	WeirdString_AddChar(ws, 1, 'H');
	WeirdString_AddChar(ws, 2, 'O');
	WeirdString_AddChar(ws, 3, '-');
	WeirdString_AddChar(ws, 4, 'T');
	WeirdString_AddChar(ws, 5, 'O');
	WeirdString_AddChar(ws, 6, ' ');
	WeirdString_AddChar(ws, 7, 'U');
	WeirdString_AddChar(ws, 8, 'S');
	WeirdString_AddChar(ws, 9, 'T');
	WeirdString_AddChar(ws, 10, 'A');
	WeirdString_AddChar(ws, 11, 'L');
	WeirdString_DelChar(ws, 11);
	WeirdString_DelChar(ws, 10);
	WeirdString_DelChar(ws, 9);
	WeirdString_DelChar(ws, 8);
	WeirdString_DelChar(ws, 7);
	WeirdString_DelChar(ws, 6);
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: "CHO-TO"
	FreeWeirdString(ws);
	}

void WS_TEST_04()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'C');
	WeirdString_AddChar(ws, 1, 'H');
	WeirdString_AddChar(ws, 2, 'O');
	WeirdString_AddChar(ws, 3, '-');
	WeirdString_AddChar(ws, 4, 'T');
	WeirdString_AddChar(ws, 5, 'O');
	WeirdString_AddChar(ws, 6, ' ');
	WeirdString_AddChar(ws, 7, 'U');
	WeirdString_AddChar(ws, 8, 'S');
	WeirdString_AddChar(ws, 9, 'T');
	WeirdString_AddChar(ws, 10, 'A');
	WeirdString_AddChar(ws, 11, 'L');
	WeirdString_DelChar(ws, 7);
	WeirdString_DelChar(ws, 7);
	WeirdString_DelChar(ws, 7);
	WeirdString_DelChar(ws, 7);
	WeirdString_AddChar(ws, 8, 'O');
	WeirdString_AddChar(ws, 8, 'A');
	WeirdString_AddChar(ws, 9, 'N');
	WeirdString_AddChar(ws, 9, 'D');
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: "CHO-TO LADNO"
	FreeWeirdString(ws);
	}

void WS_TEST_05()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'C');
	WeirdString_AddChar(ws, 1, 'H');
	WeirdString_AddChar(ws, 2, 'O');
	WeirdString_AddChar(ws, 3, '-');
	WeirdString_AddChar(ws, 4, 'T');
	WeirdString_AddChar(ws, 5, 'O');
	WeirdString_AddChar(ws, 6, ' ');
	WeirdString_AddChar(ws, 7, 'U');
	WeirdString_AddChar(ws, 8, 'S');
	WeirdString_AddChar(ws, 9, 'T');
	WeirdString_AddChar(ws, 10, 'A');
	WeirdString_AddChar(ws, 11, 'L');
	WeirdString_DelChar(ws, 0);
	WeirdString_DelChar(ws, 0);
	WeirdString_DelChar(ws, 0);
	WeirdString_DelChar(ws, 0);
	WeirdString_DelChar(ws, 0);
	WeirdString_DelChar(ws, 0);
	WeirdString_AddChar(ws, 0, 'L');
	WeirdString_AddChar(ws, 1, 'N');
	WeirdString_AddChar(ws, 2, 'O');
	WeirdString_AddChar(ws, 1, 'D');
	WeirdString_AddChar(ws, 1, 'A');
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: "LADNO USTAL"
	FreeWeirdString(ws);
	}

void WS_TEST_06()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'A');
	WeirdString_AddChar(ws, 1, 'B');
	WeirdString_AddChar(ws, 2, 'C');
	WeirdString_AddChar(ws, 3, 'D');
	WeirdString_AddChar(ws, 4, 'E');
	WeirdString_DelChar(ws, 2);
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: ABDE
	FreeWeirdString(ws);
	}

void WS_TEST_07()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'A');
	WeirdString_AddChar(ws, 1, 'B');
	WeirdString_AddChar(ws, 1, 'C');
	WeirdString_AddChar(ws, 2, 'D');
	WeirdString_AddChar(ws, 2, 'E');
	WeirdString_DelChar(ws, 3);
	WeirdString_DelChar(ws, 2);
	WeirdString_AddChar(ws, 2, 'N');
	WeirdString_DelChar(ws, 2);
	WeirdString_DelChar(ws, 1);
	WeirdString_DelChar(ws, 0);
	WeirdString_AddChar(ws, 0, 'F');
	WeirdString_AddChar(ws, 1, 'T');
	WeirdString_AddChar(ws, 1, 'U');
	WeirdString_AddChar(ws, 2, 'L');
	WeirdString_AddChar(ws, 3, ' ');
	WeirdString_AddChar(ws, 3, 'L');
	WeirdString_AddChar(ws, 6, 'H');
	WeirdString_AddChar(ws, 8, 'P');
	WeirdString_DelChar(ws, 7);
	WeirdString_DelChar(ws, 5);
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: "FULL HP"
	FreeWeirdString(ws);
	}

void WS_TEST_08()
	{
	WeirdString* ws = CreateWeirdString();
	WeirdString_AddChar(ws, 0, 'E');
	WeirdString_AddChar(ws, 1, 'C');
	WeirdString_AddChar(ws, 2, '4');
	WeirdString_AddChar(ws, 3, '0');
	WeirdString_AddChar(ws, 4, ' ');
	WeirdString_AddChar(ws, 5, ' ');
	WeirdString_AddChar(ws, 6, 'B');
	WeirdString_AddChar(ws, 7, 'U');
	WeirdString_AddChar(ws, 8, 'K');
	WeirdString_AddChar(ws, 9, 'V');
	WeirdString_AddChar(ws, 10, 'I');

	WeirdString_AddChar(ws, 5, 'H');
	WeirdString_AddChar(ws, 6, 'A');
	WeirdString_AddChar(ws, 7, 'C');
	WeirdString_AddChar(ws, 8, 'K');
	WeirdString_AddChar(ws, 9, 'E');
	WeirdString_AddChar(ws, 10, 'R');

	WeirdString_AddChar(ws, 13, 'B');
	WeirdString_AddChar(ws, 13, 'U');

	WeirdString_DelChar(ws, 1);//C
	WeirdString_DelChar(ws, 6);//C
	WeirdString_DelChar(ws, 15);//V
	size_t sz = 0;
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: "E40 HAKER BUBUKI"
	WeirdString_DelChar(ws, 14);//K
	WeirdString_DelChar(ws, 1);//4
	WeirdString_DelChar(ws, 5);//K
	WeirdString_AddChar(ws, 11, 'L');
	WeirdString_DelChar(ws, 12);//U
	WeirdString_DelChar(ws, 1);//0
	WeirdString_AddChar(ws, 12, 'K');
	printf("%s\n", WeirdString_GetNormalString(ws, &sz)); //expect: "E HAER BUBLIK"
	FreeWeirdString(ws);
	}