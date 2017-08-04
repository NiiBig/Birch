cpp {{
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
}}
import basic;

/**
 * Operators
 * ---------
 */
/**
 * Real64 operators
 */
operator x:Real64 + y:Real64 -> Real64;
operator x:Real64 - y:Real64 -> Real64;
operator x:Real64 * y:Real64 -> Real64;
operator x:Real64 / y:Real64 -> Real64;
operator +x:Real64 -> Real64;
operator -x:Real64 -> Real64;
operator x:Real64 > y:Real64 -> Boolean;
operator x:Real64 < y:Real64 -> Boolean;
operator x:Real64 >= y:Real64 -> Boolean;
operator x:Real64 <= y:Real64 -> Boolean;
operator x:Real64 == y:Real64 -> Boolean;
operator x:Real64 != y:Real64 -> Boolean;

/**
 * Real32 operators
 */
operator x:Real32 + y:Real32 -> Real32;
operator x:Real32 - y:Real32 -> Real32;
operator x:Real32 * y:Real32 -> Real32;
operator x:Real32 / y:Real32 -> Real32;
operator +x:Real32 -> Real32;
operator -x:Real32 -> Real32;
operator x:Real32 > y:Real32 -> Boolean;
operator x:Real32 < y:Real32 -> Boolean;
operator x:Real32 >= y:Real32 -> Boolean;
operator x:Real32 <= y:Real32 -> Boolean;
operator x:Real32 == y:Real32 -> Boolean;
operator x:Real32 != y:Real32 -> Boolean;

/**
 * Integer64 operators
 */
operator x:Integer64 + y:Integer64 -> Integer64;
operator x:Integer64 - y:Integer64 -> Integer64;
operator x:Integer64 * y:Integer64 -> Integer64;
operator x:Integer64 / y:Integer64 -> Integer64;
operator +x:Integer64 -> Integer64;
operator -x:Integer64 -> Integer64;
operator x:Integer64 > y:Integer64 -> Boolean;
operator x:Integer64 < y:Integer64 -> Boolean;
operator x:Integer64 >= y:Integer64 -> Boolean;
operator x:Integer64 <= y:Integer64 -> Boolean;
operator x:Integer64 == y:Integer64 -> Boolean;
operator x:Integer64 != y:Integer64 -> Boolean;

/**
 * Integer32 operators
 */
operator x:Integer32 + y:Integer32 -> Integer32;
operator x:Integer32 - y:Integer32 -> Integer32;
operator x:Integer32 * y:Integer32 -> Integer32;
operator x:Integer32 / y:Integer32 -> Integer32;
operator +x:Integer32 -> Integer32;
operator -x:Integer32 -> Integer32;
operator x:Integer32 > y:Integer32 -> Boolean;
operator x:Integer32 < y:Integer32 -> Boolean;
operator x:Integer32 >= y:Integer32 -> Boolean;
operator x:Integer32 <= y:Integer32 -> Boolean;
operator x:Integer32 == y:Integer32 -> Boolean;
operator x:Integer32 != y:Integer32 -> Boolean;

/**
 * Boolean operators
 */
operator x:Boolean && y:Boolean -> Boolean;
operator x:Boolean || y:Boolean -> Boolean;
operator !x:Boolean -> Boolean;

/**
 * Functions
 * ---------
 */
function abs(x:Real64) -> Real64 {
  cpp {{
  return ::fabs(x_);
  }}
}

function abs(x:Real32) -> Real32 {
  cpp {{
  return ::fabsf(x_);
  }}
}

function abs(x:Integer64) -> Integer64 {
  cpp {{
  return std::abs(x_);
  }}
}

function abs(x:Integer32) -> Integer32 {
  cpp {{
  return std::abs(x_);
  }}
}

function log(x:Real64) -> Real64 {
  cpp {{
  return ::log(x_);
  }}
}

function log(x:Real32) -> Real32 {
  cpp {{
  return ::logf(x_);
  }}
}

function exp(x:Real64) -> Real64 {
  cpp {{
  return ::exp(x_);
  }}
}

function exp(x:Real32) -> Real32 {
  cpp {{
  return ::expf(x_);
  }}
}

function max(x:Real64, y:Real64) -> Real64 {
  cpp {{
  return ::fmax(x_, y_);
  }}
}

function max(x:Real32, y:Real32) -> Real32 {
  cpp {{
  return ::fmaxf(x_, y_);
  }}
}

function max(x:Integer64, y:Integer64) -> Integer64 {
  cpp {{
  return std::max(x_, y_);
  }}
}

function max(x:Integer32, y:Integer32) -> Integer32 {
  cpp {{
  return std::max(x_, y_);
  }}
}

function min(x:Real64, y:Real64) -> Real64 {
  cpp {{
  return ::fmin(x_, y_);
  }}
}

function min(x:Real32, y:Real32) -> Real32 {
  cpp {{
  return ::fminf(x_, y_);
  }}
}

function min(x:Integer64, y:Integer64) -> Integer64 {
  cpp {{
  return std::min(x_, y_);
  }}
}

function min(x:Integer32, y:Integer32) -> Integer32 {
  cpp {{
  return std::min(x_, y_);
  }}
}

function sqrt(x:Real64) -> Real64 {
  cpp {{
  return ::sqrt(x_);
  }}
}

function sqrt(x:Real32) -> Real32 {
  cpp {{
  return ::sqrtf(x_);
  }}
}

function pow(x:Real64, y:Real64) -> Real64 {
  cpp {{
  return ::pow(x_, y_);
  }}
}

function pow(x:Real32, y:Real32) -> Real32 {
  cpp {{
  return ::powf(x_, y_);
  }}
}

function mod(x:Real64, y:Real64) -> Real64 {
  cpp {{
  return ::fmod(x_, y_);
  }}
}

function mod(x:Real32, y:Real32) -> Real32 {
  cpp {{
  return ::fmodf(x_, y_);
  }}
}

function ceil(x:Real64) -> Real64 {
  cpp {{
  return ::ceil(x_);
  }}
}

function ceil(x:Real32) -> Real32 {
  cpp {{
  return ::ceilf(x_);
  }}
}

function floor(x:Real64) -> Real64 {
  cpp {{
  return ::floor(x_);
  }}
}

function floor(x:Real32) -> Real32 {
  cpp {{
  return ::floorf(x_);
  }}
}

function round(x:Real64) -> Real64 {
  cpp {{
  return ::round(x_);
  }}
}

function round(x:Real32) -> Real32 {
  cpp {{
  return ::roundf(x_);
  }}
}

function gamma(x:Real64) -> Real64 {
  cpp {{
  return ::tgamma(x_);
  }}
}

function gamma(x:Real32) -> Real32 {
  cpp {{
  return ::tgammaf(x_);
  }}
}

function lgamma(x:Real64) -> Real64 {
  cpp {{
  return ::lgamma(x_);
  }}
}

function lgamma(x:Real32) -> Real32 {
  cpp {{
  return ::lgammaf(x_);
  }}
}

function sin(x:Real64) -> Real64 {
  cpp {{
  return ::sin(x_);
  }}
}

function sin(x:Real32) -> Real32 {
  cpp {{
  return ::sinf(x_);
  }}
}

function cos(x:Real64) -> Real64 {
  cpp {{
  return ::cos(x_);
  }}
}

function cos(x:Real32) -> Real32 {
  cpp {{
  return ::cosf(x_);
  }}
}

function tan(x:Real64) -> Real64 {
  cpp {{
  return ::tan(x_);
  }}
}

function tan(x:Real32) -> Real32 {
  cpp {{
  return ::tanf(x_);
  }}
}

function asin(x:Real64) -> Real64 {
  cpp {{
  return ::asin(x_);
  }}
}

function asin(x:Real32) -> Real32 {
 cpp {{
  return  ::asinf(x_);
  }}
}

function acos(x:Real64) -> Real64 {
  cpp {{
  return ::acos(x_);
  }}
}

function acos(x:Real32) -> Real32 {
  cpp {{
  return ::acosf(x_);
  }}
}

function atan(x:Real64) -> Real64 {
  cpp {{
  return ::atan(x_);
  }}
}

function atan(x:Real32) -> Real32 {
  cpp {{
  return ::atanf(x_);
  }}
}

function atan2(x:Real64, y:Real64) -> Real64 {
  cpp {{
  return ::atan2(x_, y_);
  }}
}

function atan2(x:Real32, y:Real32) -> Real32 {
  cpp {{
  return ::atan2f(x_, y_);
  }}
}

function sinh(x:Real64) -> Real64 {
  cpp {{
  return ::sinh(x_);
  }}
}

function sinh(x:Real32) -> Real32 {
  cpp {{
  return ::sinhf(x_);
  }}
}

function cosh(x:Real64) -> Real64 {
  cpp {{
  return ::cosh(x_);
  }}
}

function cosh(x:Real32) -> Real32 {
  cpp {{
  return ::coshf(x_);
  }}
}

function tanh(x:Real64) -> Real64 {
  cpp {{
  return ::tanh(x_);
  }}
}

function tanh(x:Real32) -> Real32 {
  cpp {{
  return ::tanhf(x_);
  }}
}

function asinh(x:Real64) -> Real64 {
  cpp {{
  return ::asinh(x_);
  }}
}

function asinh(x:Real32) -> Real32 {
  cpp {{
  return ::asinhf(x_);
  }}
}

function acosh(x:Real64) -> Real64 {
  cpp {{
  return ::acosh(x_);
  }}
}

function acosh(x:Real32) -> Real32 {
  cpp {{
  return ::acoshf(x_);
  }}
}

function atanh(x:Real64) -> Real64 {
  cpp {{
  return ::atanh(x_);
  }}
}

function atanh(x:Real32) -> Real32 {
  cpp {{
  return ::atanhf(x_);
  }}
}

function erf(x:Real64) -> Real64 {
  cpp {{
  return ::erf(x_);
  }}
}

function erf(x:Real32) -> Real32 {
  cpp {{
  return ::erff(x_);
  }}
}

function erfc(x:Real64) -> Real64 {
  cpp {{
  return ::erfc(x_);
  }}
}

function erfc(x:Real32) -> Real32 {
  cpp {{
  return ::erfcf(x_);
  }}
}

function copysign(x:Real64, y:Real64) -> Real64 {
  cpp {{
  return ::copysign(x_, y_);
  }}
}

function copysign(x:Real32, y:Real32) -> Real32 {
  cpp {{
  return ::copysignf(x_, y_);
  }}
}
     
function isnan(x:Real64) -> Boolean {
  return x != x;
}

function isnan(x:Real32) -> Boolean {
  return x != x;
}

/**
 * Constants
 * ---------
 */
/**
 * $\pi$
 */
π:Real64 <- 3.1415926535897932384626433832795;

/**
 * $\infty$
 */
inf:Real64 <- 1.0/0.0;
