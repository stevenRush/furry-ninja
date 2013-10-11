#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

#define REP(i, n) for (int i = 0; i < (int)(n); ++i)

const size_t max_len = 2 * 100001 + 1;
size_t length;

const int MAXN = 2 * max_len;
char * S;
int N, gap;
int sa[MAXN], pos[MAXN], tmp[MAXN], lcp[MAXN];

bool sufCmp(int i, int j)
{
	if (pos[i] != pos[j])
		return pos[i] < pos[j];
	i += gap;
	j += gap;
	return (i < N && j < N) ? pos[i] < pos[j] : i > j;
}

void buildSA()
{
	N = strlen(S);
	REP(i, N) sa[i] = i, pos[i] = S[i];
	for (gap = 1;; gap *= 2)
	{
		sort(sa, sa + N, sufCmp);
		REP(i, N - 1) tmp[i + 1] = tmp[i] + sufCmp(sa[i], sa[i + 1]);
		REP(i, N) pos[sa[i]] = tmp[i];
		if (tmp[N - 1] == N - 1) break;
	}
}

void buildLCP()
{
	for (int i = 0, k = 0; i < N; ++i) if (pos[i] != N - 1)
	{
		for (int j = sa[pos[i] + 1]; S[i + k] == S[j + k];)
		++k;
		lcp[pos[i]] = k;
		if (k)--k;
	}
}

void input()
{
	scanf("%d", &length);
	S = (char*)malloc(max_len * 2 * sizeof(char));
	scanf("%s", S);
	S[length] = '#';
	scanf("%s", S + length + 1);
}

void solve () {
	int len=2*length,lcs=-1,i;
	for (i=1;i<len;++i)
	{
		if ((lcs==-1 || lcp[i]>lcp[lcs+1]) && ((sa[i]<length && sa[i+1]>=length) || (sa[i]>=length && sa[i+1]<length)))
			lcs=i-1;
	}
	REP(i,lcp[lcs+1])
		putchar(S[sa[lcs+1]+i]);
	putchar('\n');
}

int main()
{
	//freopen("C:\\temp\\input.txt", "r", stdin);
	input();
	buildSA();
	buildLCP();
	solve();
	return 0;
}
