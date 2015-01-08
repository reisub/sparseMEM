using namespace std;

#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))
#define MAX2(x,y) ((x) > (y)) ? (x) : (y)
#define MAX3(x,y,z) ((x) > (z)) ? MAX2((x), (y)) : MAX2((z),(y))
#define N 11
#define K 2 
// K je korak (za sparse-array)
#define L 2
// minimalni broj slova koja se trebaju poklpati ("proširena pretraga")
#define TOO_LOW -1

struct interval_t {
	long start, end;
};

struct triplet_t {
	long index;
	struct interval_t interval;
};


long binary_search_left (string &S, long *SA, char cmp_char, triplet_t triplet);

long binary_search_right (string &S, long *SA, char cmp_char, triplet_t triplet);

triplet_t topdown_search (string &S, long *SA, char cmp_char, triplet_t triplet);

interval_t search_string(string &S, long *SA, string query); 
/*
triplet_t traverse(p, d:[s..e], M);

void findL (p, i, q);

void collect_MEMs (p, d:[s..e], q:[l..r]);

void find_MEMs(p0); 
*/
triplet_t suffix_link (long *ISA, long *SA, long *LCP, triplet_t triplet);

triplet_t expand_link (long *LCP, triplet_t triplet);







