#include <cstdio>

const int BLOCK_SIZE = 50;
const int N = 140000;
const int BLOCKS_NUM = N / BLOCK_SIZE;
const int STACKS_NUM = 1000;

struct Block
{
	Block* Previous;
	int arr[BLOCK_SIZE];
};

struct StackInfo
{
	Block* LastBlock;
	size_t Pointer;
};

union Mem
{
	Block block;
	Mem *next;
};

Mem data[BLOCKS_NUM];
Mem *Avail;

StackInfo stacks[STACKS_NUM];

void Initialize()
{
	for (size_t i = 0; i + 1 < BLOCKS_NUM; ++i)
		data[i].next = &data[i + 1];
	Avail = &data[0];
	data[BLOCKS_NUM - 1].next = NULL;

	for (size_t i = 0; i < STACKS_NUM; ++i)
	{
		stacks[i].LastBlock = NULL;
		stacks[i].Pointer = -1;
	}
}

Block* New()
{
	Block *result = &Avail->block;
	Avail = Avail->next;
	return result;
}

void Delete(Block *ptr)
{
	if (!ptr)
		return;
	Mem *ptr2 = reinterpret_cast<Mem*>(ptr);
	ptr2->next = Avail;
	Avail = ptr2;
}

void Push(size_t index, int value)
{
	StackInfo &info = stacks[index];
	++info.Pointer;
	size_t pointer = info.Pointer % BLOCK_SIZE;
	if (pointer == 0)
	{
		Block *NewBlock = New();
		NewBlock->Previous = info.LastBlock;
		info.LastBlock = NewBlock;
	}

	info.LastBlock->arr[pointer] = value;
}

int Pop(size_t index)
{
	StackInfo &info = stacks[index];
	size_t pointer = info.Pointer % BLOCK_SIZE;
	int result = info.LastBlock->arr[pointer];
	if (pointer == 0)
	{
		Block *ptr = info.LastBlock;
		info.LastBlock = ptr->Previous;
		Delete(ptr);
	}
	--info.Pointer;
	return result;
}

int main()
{
	//freopen("C:\\temp\\input.txt", "r", stdin);
	Initialize();
	int n, num, k;
	char s[5];
	scanf("%d", &n);
	for (size_t i = 0; i < n; ++i)
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
	return 0;
}