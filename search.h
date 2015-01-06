using namespace std;

#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))
#define MAX2(x,y) ((x) > (y) ? return (x) : return (y)
#define MAX3(x,y,z) ( x > z ? return MAX2((x), (y)) : return MAX2((z),(y))
#define N = 11
#define K = 10

struct interval_t {
	long start, end;
};

struct triplet_t {
	long index;
	struct interval_t interval;
};


long binary_search_left (string &S, long *SA, char comparing_character, long query_offset, long start_interval, long end_interval);

long binary_search_right (string &S, long *SA, char comparing_character, long query_offset, long start_interval, long end_interval);

triplet_t topdown_search (string &S, long *SA, char comparing_character, long query_offset, long start_interval, long end_interval);

interval_t search_string(string &S, long *SA, string query_string); 
/*
triplet_t traverse(p, d:[s..e], M);

void findL (p, i, q);

void collect_MEMs (p, d:[s..e], q:[l..r]);

void find_MEMs(p0); 
*/
triplet_t suffix_link (long offset, long start_interval, long end_interval);

triplet_t expand_link (long offset, long start_interval, long end_interval);







