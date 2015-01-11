

using namespace std;

#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))
#define MAX2(x,y) ((x) > (y)) ? (x) : (y)
#define MAX3(x,y,z) ((x) > (z)) ? MAX2((x), (y)) : MAX2((z),(y))


struct interval_t {
  int depth;
	int start, end;
};

int binary_search_left (char cmp_char, interval_t interval, string &S, int *SA);

int binary_search_right (char cmp_char, interval_t interval, string &S, int *SA);

interval_t topdown (char cmp_char, interval_t interval, string &S, int *SA);

interval_t search_string(string &S, int *SA, string &query);
 
interval_t traverse(int p, interval_t triplet, int M, string &S, int *SA, string &query);

void findL (int p, int i, int q, string &S, string &P, int K, int L);

void collect_MEMs (int p, interval_t SA_i, interval_t MEM, string &S, string &P, int *SA, int *LCP, int K, int N, int L);

void print_MEM (int query_index, int ref_string_index, int length, string &S);

void MEM(int p0, string &S, int *ISA, int *LCP, int *SA, string &query, int K, int N, int L);

interval_t suffix_link (interval_t interval, int *ISA, int *SA, int *LCP, int K, int N);

interval_t expand_link (interval_t interval, int *LCP, int K, int N);







