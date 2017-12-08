#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <assert.h>
#include <iostream>

#include "gsl/gsl_matrix.h"

void gemma_gsl_error_handler (const char * reason,
                              const char * file,
                              int line, int gsl_errno);

void debug_set_debug_mode(bool setting);
void debug_set_no_check_mode(bool setting);
void debug_set_strict_mode(bool setting);
void debug_set_quiet_mode(bool setting);
void debug_set_issue(uint issue);
void debug_set_legacy_mode(bool setting);

bool is_debug_mode();
bool is_no_check_mode();
bool is_check_mode();
bool is_strict_mode();
bool is_quiet_mode();
bool is_issue(uint issue);
bool is_legacy_mode();

#define check_int_mult_overflow(m,n) \
  { auto x = m * n;                                      \
    enforce_msg(x / m == n, "multiply integer overflow"); }

gsl_matrix *gsl_matrix_safe_alloc(size_t rows,size_t cols);
int gsl_matrix_safe_memcpy (gsl_matrix *dest, const gsl_matrix *src);
void gsl_matrix_safe_free (gsl_matrix *v);
void do_gsl_matrix_safe_free (gsl_matrix *m, const char *__pretty_function, const char *__file, int __line);

gsl_vector *gsl_vector_safe_alloc(size_t n);
int gsl_vector_safe_memcpy (gsl_vector *dest, const gsl_vector *src);
void gsl_vector_safe_free (gsl_vector *v);
void do_gsl_vector_safe_free (gsl_vector *v, const char *__pretty_function, const char *__file, int __line);

char *do_strtok_safe(char *tokenize, const char *delimiters, const char *__pretty_function, const char *__file, int __line);
#define strtok_safe(string,delimiters) do_strtok_safe(string,delimiters,__SHOW_FUNC,__FILE__,__LINE__)

// Validation routines
void do_validate_K(const gsl_matrix *K, const char*__pretty_func, const char *__file, int __line);

#define ROUND(f) round(f * 10000.)/10000
#define validate_K(K) do_validate_K(K,__SHOW_FUNC,__FILE__,__LINE__)

#define warning_at_msg(__file,__line,msg) cerr << "**** WARNING: " << msg << " in " << __file << " at line " << __line << endl;

inline void warnfail_at_msg(bool strict, const char *__function, const char *__file, int __line, const char *msg) {
  if (strict)
    std::cerr << "**** STRICT FAIL: ";
  else
    std::cerr << "**** WARNING: ";
  std::cerr << msg << " in " << __file << " at line " << __line << " in " << __function << std::endl;
  if (strict)
    exit(1);
}

inline void fail_at_msg(const char *__file, int __line, std::string msg) {
  std::cerr << msg << " in " << __file << " at line " << __line << std::endl;
  exit(1);
}

# ifndef __ASSERT_VOID_CAST
# define __ASSERT_VOID_CAST (void)
# endif

inline void fail_msg(const char *msg) {
  std::cerr << "**** FAILED: " << msg << std::endl;
  exit(5);
}

inline void fail_msg(std::string msg) {
  std::cerr << "**** FAILED: " << msg << std::endl;
  exit(5);
}

#if defined NDEBUG
  #define __SHOW_FUNC __func__

  #define warning_msg(msg) cerr << "**** WARNING: " << msg << endl;
  #define debug_msg(msg)
  #define assert_issue(is_issue, expr)

#else // DEBUG

  #define __SHOW_FUNC __func__

  #define warning_msg(msg) cerr << "**** WARNING: " << msg << " in " << __FILE__ << " at line " << __LINE__ << " in " << __func__ << endl;
  #define debug_msg(msg) (is_debug_mode() && cerr << "**** DEBUG: " << msg << " in " << __FILE__ << " at line " << __LINE__ << " in " << __func__ << endl);
  #define assert_issue(is_issue, expr) \
    ((is_issue) ? enforce_msg(expr,"FAIL: ISSUE assert") : __ASSERT_VOID_CAST(0))

#endif // NDEBUG

// enforce works like assert but also when NDEBUG is set (i.e., it
// always works). enforce_msg prints message instead of expr

/* This prints an "Assertion failed" message and aborts.  */
inline void __enforce_fail(const char *__assertion, const char *__file,
                    unsigned int __line,
                    const char *__function)
{
  std::cout << "ERROR: Enforce failed for " << __assertion << " in " << __file << " at line " << __line << " in " << __function << std::endl;
  exit(1);
}

#define enforce(expr)                                                          \
  ((expr)                                                                      \
       ? __ASSERT_VOID_CAST(0)                                                 \
       : __enforce_fail(__STRING(expr), __FILE__, __LINE__, __SHOW_FUNC))

#define enforce_msg(expr, msg)                                                 \
  ((expr) ? __ASSERT_VOID_CAST(0)                                              \
          : __enforce_fail(msg, __FILE__, __LINE__, __SHOW_FUNC))

#define enforce_str(expr, msg)                                                 \
  ((expr)                                                                      \
       ? __ASSERT_VOID_CAST(0)                                                 \
       : __enforce_fail((msg).c_str(), __FILE__, __LINE__, __SHOW_FUNC))

// Helpers to create a unique varname per MACRO
#define COMBINE1(X, Y) X##Y
#define COMBINE(X, Y) COMBINE1(X, Y)

#define enforce_gsl(expr)                                                      \
  auto COMBINE(res, __LINE__) = (expr);                                        \
  (COMBINE(res, __LINE__) == 0                                                 \
       ? __ASSERT_VOID_CAST(0)                                                 \
       : __enforce_fail(gsl_strerror(COMBINE(res, __LINE__)), __FILE__,         \
                        __LINE__, __SHOW_FUNC))

#define enforce_fexists(fn, msg)                                               \
  if (!fn.empty())                                                             \
    enforce_msg(stat(fn.c_str(), &fileInfo) == 0,                              \
                ((std::string(__STRING(fn)) + " " + fn + ": " + msg).c_str()));

#define gsl_matrix_safe_free(m) \
  do_gsl_matrix_safe_free(m,__SHOW_FUNC,__FILE__,__LINE__);
#define gsl_vector_safe_free(v) \
  do_gsl_vector_safe_free(v,__SHOW_FUNC,__FILE__,__LINE__);

#endif
