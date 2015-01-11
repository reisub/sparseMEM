

using namespace std;

#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))
#define MAX2(x,y) ((x) > (y)) ? (x) : (y)
#define MAX3(x,y,z) ((x) > (z)) ? MAX2((x), (y)) : MAX2((z),(y))
#define N 11
// velicina ulaznog stringa
#define K 1 
// K je korak (za sparse-array)
#define L 3
// minimalni broj slova koja se trebaju poklpati ("proširena pretraga")
#define TOO_LOW -1

struct interval_t {
  long depth;
	long start, end;
};

long binary_search_left (char cmp_char, interval_t interval, string &S, long *SA);

long binary_search_right (char cmp_char, interval_t interval, string &S, long *SA);

interval_t topdown (char cmp_char, interval_t interval, string &S, long *SA);

interval_t search_string(string &S, long *SA, string &query);
 
interval_t traverse(long p, interval_t triplet, long M, string &S, long *SA, string &query);

void findL (long p, long i, long q, string &S, string &P);

void collect_MEMs (long p, interval_t SA_i, interval_t MEM, string &S, string &P, long *SA, long *LCP);

void print_MEM (long query_index, long ref_string_index, long length, string &S);

void MEM(long p0, string &S, long *ISA, long *LCP, long *SA, string &query);

interval_t suffix_link (interval_t interval, long *ISA, long *SA, long *LCP);

interval_t expand_link (interval_t interval, long *LCP);







