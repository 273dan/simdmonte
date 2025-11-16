#include <immintrin.h>
namespace simdmonte {
namespace constants {
// Peter Acklam's inverse cdf normal approximation
// https://stackedboxes.org/2017/05/01/acklams-normal-quantile-function/

constexpr float a1 = -3.969683028665376e+01f;
constexpr float a2 = 2.209460984245205e+02f;
constexpr float a3 = -2.759285104469687e+02f;
constexpr float a4 = 1.383577518672690e+02f;
constexpr float a5 = -3.066479806614716e+01f;
constexpr float a6 = 2.506628277459239e+00f;
constexpr float b1 = -5.447609879822406e+01f;
constexpr float b2 = 1.615858368580409e+02f;
constexpr float b3 = -1.556989798598866e+02f;
constexpr float b4 = 6.680131188771972e+01f;
constexpr float b5 = -1.328068155288572e+01f;
constexpr float c1 = -7.784894002430293e-03f;
constexpr float c2 = -3.223964580411365e-01f;
constexpr float c3 = -2.400758277161838e+00f;
constexpr float c4 = -2.549732539343734e+00f;
constexpr float c5 = 4.374664141464968e+00f;
constexpr float c6 = 2.938163982698783e+00f;
constexpr float d1 = 7.784695709041462e-03f;
constexpr float d2 = 3.224671290700398e-01f;
constexpr float d3 = 2.445134137142996e+00f;
constexpr float d4 = 3.754408661907416e+00f;

constexpr float P_LOW = 0.02425f;
constexpr float P_HIGH = 1.0f - P_LOW;

constexpr float half = 0.5f;
constexpr float one = 1.0f;
constexpr float negtwo = -2.0f;
constexpr float zero = 0.0f;

static inline __m256 HALF = _mm256_set1_ps(half);
static inline __m256 ONES = _mm256_set1_ps(one);
static inline __m256 NEGTWOS = _mm256_set1_ps(negtwo);
static inline __m256 ZEROES = _mm256_set1_ps(zero);

static inline __m256 P_LOWS = _mm256_set1_ps(P_LOW);
static inline __m256 P_HIGHS = _mm256_set1_ps(P_HIGH);

static inline __m256 A1 = _mm256_set1_ps(a1);
static inline __m256 A2 = _mm256_set1_ps(a2);
static inline __m256 A3 = _mm256_set1_ps(a3);
static inline __m256 A4 = _mm256_set1_ps(a4);
static inline __m256 A5 = _mm256_set1_ps(a5);
static inline __m256 A6 = _mm256_set1_ps(a6);
static inline __m256 B1 = _mm256_set1_ps(b1);
static inline __m256 B2 = _mm256_set1_ps(b2);
static inline __m256 B3 = _mm256_set1_ps(b3);
static inline __m256 B4 = _mm256_set1_ps(b4);
static inline __m256 B5 = _mm256_set1_ps(b5);
static inline __m256 C1 = _mm256_set1_ps(c1);
static inline __m256 C2 = _mm256_set1_ps(c2);
static inline __m256 C3 = _mm256_set1_ps(c3);
static inline __m256 C4 = _mm256_set1_ps(c4);
static inline __m256 C5 = _mm256_set1_ps(c5);
static inline __m256 C6 = _mm256_set1_ps(c6);
static inline __m256 D1 = _mm256_set1_ps(d1);
static inline __m256 D2 = _mm256_set1_ps(d2);
static inline __m256 D3 = _mm256_set1_ps(d3);
static inline __m256 D4 = _mm256_set1_ps(d4);

}  // namespace constants
}  // namespace simdmonte
