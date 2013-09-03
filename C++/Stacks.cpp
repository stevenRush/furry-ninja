#include <stdio.h>
#include <string.h>

#define N					150000
#define BLOCK_SIZE			50
#define STACKS_NUM			1000
#define SERVICE_BLOCKS_NUM	(N / BLOCK_SIZE + STACKS_NUM) / BLOCK_SIZE
#define ARR_SIZE			N + SERVICE_BLOCKS_NUM * BLOCK_SIZE
#define BLOCKS_NUM			N / BLOCK_SIZE + SERVICE_BLOCKS_NUM
#define STACK_POINTER_START_POS 1

int arr[ARR_SIZE];
int stack_blocks[STACKS_NUM];	// number of block storing this stack
int blocks_free[BLOCKS_NUM];	// 0 - free, 1 - busy

typedef size_t size;

size GetFreeBlock()
{
	size i=0;
	while(blocks_free[i]) 
		++i;
	blocks_free[i] = 1;
	return i;
}

void FindBlock(size& block, int& pointer)
{
	while(pointer > BLOCK_SIZE)
	{
		block = arr[(block+1) * BLOCK_SIZE - 1];
		pointer -= BLOCK_SIZE;
	}
}

void Push(size index, int value)
{
	if (stack_blocks[index] == -1)
	{
		size i = GetFreeBlock();
		stack_blocks[index] = i;
		arr[i * BLOCK_SIZE] = STACK_POINTER_START_POS + 1;
		arr[i * BLOCK_SIZE + STACK_POINTER_START_POS] = value;
	}
	else
	{
		size block_num = stack_blocks[index];
		int pointer = arr[block_num * BLOCK_SIZE];
		FindBlock(block_num, pointer);
		if (pointer == BLOCK_SIZE)
		{
			pointer = 0;
			block_num = arr[block_num * BLOCK_SIZE + BLOCK_SIZE - 1];
		}
			
		if (pointer == BLOCK_SIZE - 1)
		{
			size i = GetFreeBlock();
			arr[(block_num+1) * BLOCK_SIZE - 1] = i;
			arr[i * BLOCK_SIZE] = value;
			arr[stack_blocks[index] * BLOCK_SIZE] += 2;
		}
		else
		{
			arr[block_num * BLOCK_SIZE + pointer] = value;
			++arr[stack_blocks[index] * BLOCK_SIZE];
		}
	}
}

int Pop(size index)
{
	size block_num = stack_blocks[index];
	int pointer = arr[block_num * BLOCK_SIZE];
	FindBlock(block_num, pointer);
	--arr[stack_blocks[index] * BLOCK_SIZE];
	if (pointer == BLOCK_SIZE)
	{
		--arr[stack_blocks[index] * BLOCK_SIZE];
		blocks_free[arr[(block_num+1) * BLOCK_SIZE - 1]] = 0;
		return arr[(block_num+1) * BLOCK_SIZE - 2];
	}
	else
		return arr[block_num * BLOCK_SIZE + pointer - 1];
}

int main()
{
	freopen("C:\\temp\\input.txt", "r", stdin);
	//freopen("C:\\temp\\out.txt", "w", stdout);
	memset(&arr, 0, ARR_SIZE * sizeof(int));
	memset(&stack_blocks, -1, STACKS_NUM * sizeof(int));
	memset(&blocks_free, 0, BLOCKS_NUM * sizeof(int));
	int n, num, k;
	char s[5];
	scanf("%d", &n);
	for(size_t i=0; i < n; ++i)
	{
		scanf("%s", &s);
		scanf("%d", &num);
		--num;
		if (s[1] == 'U')
		{
			scanf("%d", &k);
			Push(num, k);
		}
		else
		{
			k = Pop(num);
			printf("%d\n", k);
		}
	}
	//fclose(stdout);
	return 0;
}