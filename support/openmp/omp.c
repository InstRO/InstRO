
typedef struct
{
  unsigned char _x[4]
    __attribute__((__aligned__(4)));
} omp_lock_t;

typedef struct
{
  unsigned char _x[8 + sizeof (void *)]
    __attribute__((__aligned__(sizeof (void *))));
} omp_nest_lock_t;


#ifdef __cplusplus
extern "C" {
# define __GOMP_NOTHROW throw ()
#else
# define __GOMP_NOTHROW __attribute__((__nothrow__))
#endif

void omp_set_num_threads (int) __GOMP_NOTHROW{};
int omp_get_num_threads (void) __GOMP_NOTHROW {return 1;};
int omp_get_max_threads (void) __GOMP_NOTHROW {return 1;};
int omp_get_thread_num (void) __GOMP_NOTHROW {return 0;};
int omp_get_num_procs (void) __GOMP_NOTHROW {return 1;};

int omp_in_parallel (void) __GOMP_NOTHROW {return 0;};

void omp_set_dynamic (int) __GOMP_NOTHROW {};
int omp_get_dynamic (void) __GOMP_NOTHROW {return 1;};

void omp_set_nested (int) __GOMP_NOTHROW {};
int omp_get_nested (void) __GOMP_NOTHROW {return 0;};

void omp_init_lock (omp_lock_t *) __GOMP_NOTHROW {};
void omp_destroy_lock (omp_lock_t *) __GOMP_NOTHROW{};
void omp_set_lock (omp_lock_t *) __GOMP_NOTHROW{};
void omp_unset_lock (omp_lock_t *) __GOMP_NOTHROW{};
int omp_test_lock (omp_lock_t *) __GOMP_NOTHROW{return 0;};

void omp_init_nest_lock (omp_nest_lock_t *) __GOMP_NOTHROW {};
void omp_destroy_nest_lock (omp_nest_lock_t *) __GOMP_NOTHROW{};
void omp_set_nest_lock (omp_nest_lock_t *) __GOMP_NOTHROW{};
void omp_unset_nest_lock (omp_nest_lock_t *) __GOMP_NOTHROW{};
int omp_test_nest_lock (omp_nest_lock_t *) __GOMP_NOTHROW{return 0;};

double omp_get_wtime (void) __GOMP_NOTHROW{return 0.0;};
double omp_get_wtick (void) __GOMP_NOTHROW{return 0.0;};

#ifdef __cplusplus
}
#endif

