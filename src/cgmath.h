#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
// C standard
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// STL
#include <algorithm>
#include <array>
#include <map>
#include <set>
#include <string>
#include <vector>
// C++11
#if (_MSC_VER>=1600/*VS2010*/) || (__cplusplus>199711L)
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif
// windows/GCC
#if !defined(__GNUC__)&&(defined(_WIN32)||defined(_WIN64))
#include <windows.h>
#include <wchar.h>
#elif defined(__GNUC__)&&!defined(__forceinline)
#define __forceinline inline __attribute__((__always_inline__))
#endif
// common macros
#ifndef PI
#define PI 3.141592653589793f
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef clamp
#define clamp(value,vmin,vmax) (max(vmin,min(vmax,value)))
#endif
#ifndef isnan
#define	isnan(x) (x!=x)
#endif

//*******************************************************************
// user types
typedef unsigned int	uint;
typedef unsigned short	ushort;
typedef unsigned char	uchar;

// template constants
template <class T> struct precision { static const T value(){ return std::numeric_limits<T>::epsilon() * 10; } };	// need to be 10x for robust practical test

//*******************************************************************
template <class T> struct tvec2
{
	union{ struct { T r, g; }; struct { T x, y; }; };

	// constructor/set
	tvec2(){ x = y = 0; }
	tvec2(T a){ x = y = a; }						inline void set(T a){ x = y = a; }
	tvec2(T a, T b){ x = a; y = b; }				inline void set(T a, T b){ x = a; y = b; }
	tvec2(const tvec2& v){ x = v.x; y = v.y; }		inline void set(const tvec2& v){ x = v.x; y = v.y; }
	tvec2(const T* a){ x = a[0]; y = a[1]; }		inline void set(const T* a){ x = a[0]; y = a[1]; }

	// assignment / compound assignment operators
	inline tvec2& operator=(T a){ set(a); return *this; }
	inline tvec2& operator+=(const tvec2& v){ x += v.x; y += v.y; return *this; }
	inline tvec2& operator-=(const tvec2& v){ x -= v.x; y -= v.y; return *this; }
	inline tvec2& operator*=(const tvec2& v){ x *= v.x; y *= v.y; return *this; }
	inline tvec2& operator/=(const tvec2& v){ x /= v.x; y /= v.y; return *this; }
	inline tvec2& operator+=(T a){ x += a; y += a; return *this; }
	inline tvec2& operator-=(T a){ x -= a; y -= a; return *this; }
	inline tvec2& operator*=(T a){ x *= a; y *= a; return *this; }
	inline tvec2& operator/=(T a){ x /= a; y /= a; return *this; }

	// comparison operators
	inline bool operator==(const tvec2& v) const { return std::abs(x - v.x) <= precision<T>::value() && std::abs(y - v.y) <= precision<T>::value(); }
	inline bool operator!=(const tvec2& v) const { return !operator==(v); }

	// casting operators
	inline operator T*(){ return &x; }
	inline operator const T*() const { return &x; }

	// array access operators
	inline T& operator[](int i){ return (&r)[i]; }
	inline const T& operator[](int i) const { return (&r)[i]; }
	inline T& at(int i){ return (&r)[i]; }
	inline const T& at(int i) const { return (&r)[i]; }

	// unary operators
	inline tvec2 operator+() const { return tvec2(r, g); }
	inline tvec2 operator-() const { return tvec2(-r, -g); }

	// binary operators
	inline tvec2 operator+(const tvec2& v) const { return tvec2(r + v.r, g + v.g); }
	inline tvec2 operator-(const tvec2& v) const { return tvec2(r - v.r, g - v.g); }
	inline tvec2 operator*(const tvec2& v) const { return tvec2(r*v.r, g*v.g); }
	inline tvec2 operator/(const tvec2& v) const { return tvec2(r / v.r, g / v.g); }
	inline tvec2 operator+(T a) const { return tvec2(r + a, g + a); }
	inline tvec2 operator-(T a) const { return tvec2(r - a, g - a); }
	inline tvec2 operator*(T a) const { return tvec2(r*a, g*a); }
	inline tvec2 operator/(T a) const { return tvec2(r / a, g / a); }

	// length, normalize, dot product
	inline T length() const { return (T)(sqrt(r*r + g*g)); }
	inline T dot(const tvec2& v) const { return (T)(r*v.r + g*v.g); }
	inline tvec2 normalize() const { return tvec2(r, g) / length(); }
	inline T length2() const { return (T)(r*r + g*g); }
};

//*******************************************************************
template <class T> struct tvec3
{
	union { struct { T r, g, b; }; struct { T x, y, z; }; };

	// constructor/set
	tvec3(){ x = y = z = 0; }
	tvec3(T a){ x = y = z = a; }								inline void set(T a){ x = y = z = a; }
	tvec3(T a, T b, T c){ x = a; y = b; z = c; }					inline void set(T a, T b, T c){ x = a; y = b; z = c; }
	tvec3(const tvec3& v){ x = v.x; y = v.y; z = v.z; }			inline void set(const tvec3& v){ x = v.x; y = v.y; z = v.z; }
	tvec3(const T* a){ x = a[0]; y = a[1]; z = a[2]; }			inline void set(const T* a){ x = a[0]; y = a[1]; z = a[2]; }
	tvec3(const tvec2<T>& v, T c){ x = v.x; y = v.y; z = c; }		inline void set(const tvec2<T>& v, T c){ x = v.x; y = v.y; z = c; }
	tvec3(T a, const tvec2<T>& v){ x = a; y = v.x; z = v.y; }		inline void set(T a, const tvec2<T>& v){ x = a; y = v.x; z = v.y; }

	// assignment / compound assignment operators
	inline tvec3& operator=(T a){ set(a); return *this; }
	inline tvec3& operator=(const T* a){ set(a); return *this; }
	inline tvec3& operator+=(const tvec3& v){ x += v.x; y += v.y; z += v.z; return *this; }
	inline tvec3& operator-=(const tvec3& v){ x -= v.x; y -= v.y; z -= v.z; return *this; }
	inline tvec3& operator*=(const tvec3& v){ x *= v.x; y *= v.y; z *= v.z; return *this; }
	inline tvec3& operator/=(const tvec3& v){ x /= v.x; y /= v.y; z /= v.z; return *this; }
	inline tvec3& operator+=(T a){ x += a; y += a; z += a; return *this; }
	inline tvec3& operator-=(T a){ x -= a; y -= a; z -= a; return *this; }
	inline tvec3& operator*=(T a){ x *= a; y *= a; z *= a; return *this; }
	inline tvec3& operator/=(T a){ x /= a; y /= a; z /= a; return *this; }

	// comparison operators
	inline bool operator==(const tvec3& v) const { return std::abs(x - v.x) <= precision<T>::value() && std::abs(y - v.y) <= precision<T>::value() && std::abs(z - v.z) <= precision<T>::value(); }
	inline bool operator!=(const tvec3& v) const { return !operator==(v); }

	// casting operators
	inline operator T*(){ return &x; }
	inline operator const T*(){ return &x; }

	// array access operators
	inline T& operator[](int i){ return (&r)[i]; }
	inline const T& operator[](int i) const { return (&r)[i]; }
	inline T& at(int i){ return (&r)[i]; }
	inline const T& at(int i) const { return (&r)[i]; }

	// unary operators
	inline tvec3 operator+() const { return tvec3(r, g, b); }
	inline tvec3 operator-() const { return tvec3(-r, -g, -b); }

	// binary operators
	inline tvec3 operator+(const tvec3& v) const { return tvec3(r + v.r, g + v.g, b + v.b); }
	inline tvec3 operator-(const tvec3& v) const { return tvec3(r - v.r, g - v.g, b - v.b); }
	inline tvec3 operator*(const tvec3& v) const { return tvec3(r*v.r, g*v.g, b*v.b); }
	inline tvec3 operator/(const tvec3& v) const { return tvec3(r / v.r, g / v.g, b / v.b); }
	inline tvec3 operator+(T a) const { return tvec3(r + a, g + a, b + a); }
	inline tvec3 operator-(T a) const { return tvec3(r - a, g - a, b - a); }
	inline tvec3 operator*(T a) const { return tvec3(r*a, g*a, b*a); }
	inline tvec3 operator/(T a) const { return tvec3(r / a, g / a, b / a); }

	// length, normalize, dot product
	inline T length() const { return (T)(sqrt(r*r + g*g + b*b)); }
	inline tvec3 normalize() const { return tvec3(r, g, b) / length(); }
	inline T dot(const tvec3& v) const { return (T)(r*v.r + g*v.g + b*v.b); }
	inline T length2() const { return (T)(r*r + g*g + b*b); }

	// tvec3 only: cross product
	inline tvec3 operator^(const tvec3& v) const { return tvec3(g*v.b - b*v.g, b*v.r - r*v.b, r*v.g - g*v.r); }
	inline tvec3 cross(const tvec3& v) const { return tvec3(g*v.b - b*v.g, b*v.r - r*v.b, r*v.g - g*v.r); }
};

//*******************************************************************
template <class T> struct tvec4
{
	union { struct { T r, g, b, a; }; struct { T x, y, z, w; }; };

	// constructor/set
	tvec4(){ x = y = z = w = 0; }
	tvec4(T a){ x = y = z = w = a; }										inline void set(T a){ x = y = z = w = a; }
	tvec4(T a, T b, T c, T d){ x = a; y = b; z = c; w = d; }					inline void set(T a, T b, T c, T d){ x = a; y = b; z = c; w = d; }
	tvec4(const tvec4& v){ x = v.x; y = v.y; z = v.z; w = v.w; }				inline void set(const tvec4& v){ x = v.x; y = v.y; z = v.z; w = v.w; }
	tvec4(const T* a){ x = a[0]; y = a[1]; z = a[2]; w = a[3]; }				inline void set(const T* a){ x = a[0]; y = a[1]; z = a[2]; w = a[3]; }
	tvec4(const tvec2<T>& v, T c, T d){ x = v.x; y = v.y; z = c; w = d; }	inline void set(const tvec2<T>& v, T c, T d){ x = v.x; y = v.y; z = c; w = d; }
	tvec4(T a, T b, const tvec2<T>& v){ x = a; y = b; z = v.x; w = v.y; }	inline void set(T a, T b, const tvec2<T>& v){ x = a; y = b; z = v.x; w = v.y; }
	tvec4(const tvec3<T>& v, T d){ x = v.x; y = v.y; z = v.z; w = d; }		inline void set(const tvec3<T>& v, T d){ x = v.x; y = v.y; z = v.z; w = d; }
	tvec4(T a, const tvec3<T>& v){ x = a; y = v.x; z = v.y; w = v.z; }		inline void set(T a, const tvec3<T>& v){ x = a; y = v.x; z = v.y; w = v.z; }
	tvec4(const tvec2<T>& v1, const tvec2<T>& v2){ x = v1.x; y = v1.y; z = v2.x; w = v2.y; }
	inline void set(const tvec2<T>& v1, const tvec2<T>& v2){ x = v1.x; y = v1.y; z = v2.x; w = v2.y; }

	// assignment / compound assignment operators
	inline tvec4& operator=(T a){ set(a); return *this; }
	inline tvec4& operator=(const T* a){ set(a); return *this; }
	inline tvec4& operator+=(const tvec4& v){ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	inline tvec4& operator-=(const tvec4& v){ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	inline tvec4& operator*=(const tvec4& v){ x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	inline tvec4& operator/=(const tvec4& v){ x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	inline tvec4& operator+=(T a){ x += a; y += a; z += a; w += a; return *this; }
	inline tvec4& operator-=(T a){ x -= a; y -= a; z -= a; w -= a; return *this; }
	inline tvec4& operator*=(T a){ x *= a; y *= a; z *= a; w *= a; return *this; }
	inline tvec4& operator/=(T a){ x /= a; y /= a; z /= a; w /= a; return *this; }

	// comparison operators
	inline bool operator==(const tvec4& v) const { return std::abs(x - v.x) <= precision<T>::value() && std::abs(y - v.y) <= precision<T>::value() && std::abs(z - v.z) <= precision<T>::value() && std::abs(w - v.w) <= precision<T>::value(); }
	inline bool operator!=(const tvec4& v) const { return !operator==(v); }

	// casting operators
	inline operator T*(){ return &x; }
	inline operator const T*(){ return &x; }

	// array access operators
	inline T& operator[](int i){ return (&r)[i]; }
	inline const T& operator[](int i) const { return (&r)[i]; }
	inline T& at(int i){ return (&r)[i]; }
	inline const T& at(int i) const { return (&r)[i]; }

	// unary operators
	inline tvec4 operator+() const { return tvec4(r, g, b, a); }
	inline tvec4 operator-() const { return tvec4(-r, -g, -b, -a); }

	// binary operators
	inline tvec4 operator+(const tvec4& v) const { return tvec4(r + v.r, g + v.g, b + v.b, a + v.a); }
	inline tvec4 operator-(const tvec4& v) const { return tvec4(r - v.r, g - v.g, b - v.b, a - v.a); }
	inline tvec4 operator*(const tvec4& v) const { return tvec4(r*v.r, g*v.g, b*v.b, a*v.a); }
	inline tvec4 operator/(const tvec4& v) const { return tvec4(r / v.r, g / v.g, b / v.b, a / v.a); }
	inline tvec4 operator+(T a) const { return tvec4(r + a, g + a, b + a, this->a + a); }
	inline tvec4 operator-(T a) const { return tvec4(r - a, g - a, b - a, this->a - a); }
	inline tvec4 operator*(T a) const { return tvec4(r*a, g*a, b*a, this->a*a); }
	inline tvec4 operator/(T a) const { return tvec4(r / a, g / a, b / a, this->a / a); }

	// length, normalize, dot product
	inline T length() const { return (T)(sqrt(r*r + g*g + b*b + a*a)); }
	inline tvec4 normalize() const { return tvec4(r, g, b, a) / length(); }
	inline T dot(const tvec4& v) const { return (T)(r*v.r + g*v.g + b*v.b + a*v.a); }
	inline T length2() const { return (T)(r*r + g*g + b*b + a*a); }
};

//*******************************************************************
// type definitions
using vec2 = tvec2<float>;		using vec3 = tvec3<float>;		using vec4 = tvec4<float>;
using ivec2 = tvec2<int>;		using ivec3 = tvec3<int>;		using ivec4 = tvec4<int>;
using uvec2 = tvec2<uint>;		using uvec3 = tvec3<uint>;		using uvec4 = tvec4<uint>;
using dvec2 = tvec2<double>;	using dvec3 = tvec3<double>;	using dvec4 = tvec4<double>;

//*******************************************************************
// matrix 3x3: uses a standard row-major notation
struct mat3
{
	union { float a[9]; struct { float _11, _12, _13, _21, _22, _23, _31, _32, _33; }; };

	inline mat3(){ _12 = _13 = _21 = _23 = _31 = _32 = 0.0f; _11 = _22 = _33 = 1.0f; }
	inline mat3(float* f){ memcpy(a, f, sizeof(float) * 9); }
	inline mat3(float f11, float f12, float f13, float f21, float f22, float f23, float f31, float f32, float f33){ _11 = f11; _12 = f12; _13 = f13; _21 = f21; _22 = f22; _23 = f23; _31 = f31; _32 = f32; _33 = f33; }

	// comparison operators
	inline bool operator==(const mat3& m) const { for (int k = 0; k<std::extent<decltype(a)>::value; k++) if (std::abs(a[k] - m[k])>precision<float>::value()) return false; return true; }
	inline bool operator!=(const mat3& m) const { return !operator==(m); }

	// casting operators
	inline operator float*(){ return a; }
	inline operator const float*() const { return a; }

	// array access operators
	inline float& operator[](unsigned i){ return a[i]; }
	inline float& operator[](int i){ return a[i]; }
	inline const float& operator[](unsigned i) const { return a[i]; }
	inline const float& operator[](int i) const { return a[i]; }

	// row vectors
	inline vec3& rvec3(int row){ return reinterpret_cast<vec3&>(a[row * 3]); }
	inline const vec3& rvec3(int row) const { return reinterpret_cast<const vec3&>(a[row * 3]); }

	// identity and transpose
	inline static mat3 identity(){ return mat3(); }
	inline void setIdentity(){ _12 = _13 = _21 = _23 = _31 = _32 = 0.0f; _11 = _22 = _33 = 1.0f; }
	inline mat3 transpose() const { return mat3(_11, _21, _31, _12, _22, _32, _13, _23, _33); }

	// addition/subtraction operators
	inline mat3 operator+(const mat3& m) const { mat3 r; for (int k = 0; k < std::extent<decltype(a)>::value; k++) r[k] = a[k] + m[k]; return r; }
	inline mat3 operator-(const mat3& m) const { mat3 r; for (int k = 0; k < std::extent<decltype(a)>::value; k++) r[k] = a[k] - m[k]; return r; }
	inline mat3& operator+=(const mat3& m){ return *this = operator+(m); }
	inline mat3& operator-=(const mat3& m){ return *this = operator-(m); }

	// multiplication operators
	inline mat3 operator*(float f) const { mat3 r; for (int k = 0; k < std::extent<decltype(a)>::value; k++) r[k] = a[k] * f; return r; }
	inline vec3 operator*(const vec3& v) const { return vec3(rvec3(0).dot(v), rvec3(1).dot(v), rvec3(2).dot(v)); }
	inline mat3 operator*(const mat3& m) const { mat3 t = m.transpose(), r; for (uint k = 0; k<3; k++) r.rvec3(k) = t.operator*(rvec3(k)); return r; } // a bit tricky implementation
	inline mat3& operator*=(const mat3& m){ return *this = operator*(m); }

	// determinant
	inline float determinant() const { return _11*(_22*_33 - _23*_32) + _12*(_23*_31 - _21*_33) + _13*(_21*_32 - _22*_31); }

	// inverse
	inline mat3 inverse() const
	{
		float det = determinant(), s = 1.0f / det; if (det == 0) printf("mat3::inverse() might be singular.\n");
		return mat3((_22*_33 - _32*_23)*s, (_13*_32 - _12*_33)*s, (_12*_23 - _13*_22)*s, (_23*_31 - _21*_33)*s, (_11*_33 - _13*_31)*s, (_21*_13 - _11*_23)*s, (_21*_32 - _31*_22)*s, (_31*_12 - _11*_32)*s, (_11*_22 - _21*_12)*s);
	}
};

//*******************************************************************
// matrix 4x4: uses a standard row-major notation
struct mat4
{
	union { float a[16]; struct { float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; }; };

	mat4(){ _12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f; _11 = _22 = _33 = _44 = 1.0f; }
	mat4(float* f){ memcpy(a, f, sizeof(float) * 16); }
	mat4(float f11, float f12, float f13, float f14, float f21, float f22, float f23, float f24, float f31, float f32, float f33, float f34, float f41, float f42, float f43, float f44){ _11 = f11; _12 = f12; _13 = f13; _14 = f14; _21 = f21; _22 = f22; _23 = f23; _24 = f24; _31 = f31; _32 = f32; _33 = f33; _34 = f34; _41 = f41; _42 = f42; _43 = f43; _44 = f44; }

	// comparison operators
	inline bool operator==(const mat4& m) const { for (int k = 0; k<std::extent<decltype(a)>::value; k++) if (std::abs(a[k] - m[k])>precision<float>::value()) return false; return true; }
	inline bool operator!=(const mat4& m) const { return !operator==(m); }

	// casting operators
	inline operator float*(){ return a; }
	inline operator const float*() const { return a; }
	inline operator mat3() const { return mat3(_11, _12, _13, _21, _22, _23, _31, _32, _33); }

	// array access operators
	inline float& operator[](unsigned i){ return a[i]; }
	inline float& operator[](int i){ return a[i]; }
	inline const float& operator[](unsigned i) const { return a[i]; }
	inline const float& operator[](int i) const { return a[i]; }

	// row vectors
	inline vec4& rvec4(int row){ return reinterpret_cast<vec4&>(a[row * 4]); }
	inline vec3& rvec3(int row){ return reinterpret_cast<vec3&>(a[row * 4]); }
	inline const vec4& rvec4(int row) const { return reinterpret_cast<const vec4&>(a[row * 4]); }
	inline const vec3& rvec3(int row) const { return reinterpret_cast<const vec3&>(a[row * 4]); }

	// identity and transpose
	static mat4 identity(){ return mat4(); }
	inline void setIdentity(){ _12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f; _11 = _22 = _33 = _44 = 1.0f; }
	inline mat4 transpose() const { return mat4(_11, _21, _31, _41, _12, _22, _32, _42, _13, _23, _33, _43, _14, _24, _34, _44); }

	// addition/subtraction operators
	inline mat4 operator+(const mat4& m) const { mat4 r; for (int k = 0; k < std::extent<decltype(a)>::value; k++) r[k] = a[k] + m[k]; return r; }
	inline mat4 operator-(const mat4& m) const { mat4 r; for (int k = 0; k < std::extent<decltype(a)>::value; k++) r[k] = a[k] - m[k]; return r; }
	inline mat4& operator+=(const mat4& m){ return *this = operator+(m); }
	inline mat4& operator-=(const mat4& m){ return *this = operator-(m); }

	// multiplication operators
	inline mat4 operator*(float f) const { mat4 r; for (int k = 0; k < std::extent<decltype(a)>::value; k++) r[k] = a[k] * f; return r; }
	inline vec4 operator*(const vec4& v) const { return vec4(rvec4(0).dot(v), rvec4(1).dot(v), rvec4(2).dot(v), rvec4(3).dot(v)); }
	inline mat4 operator*(const mat4& m) const { mat4 t = m.transpose(), r; for (uint k = 0; k<4; k++) r.rvec4(k) = t.operator*(rvec4(k)); return r; } // a bit tricky implementation
	inline mat4& operator*=(const mat4& m){ return *this = operator*(m); }

	// determinant and inverse: see below for implementations
	inline float determinant() const;
	inline mat4 inverse() const;

	// static row-major transformations
	static mat4 translate(const vec3& v){ return mat4().setTranslate(v); }
	static mat4 translate(float x, float y, float z){ return mat4().setTranslate(x, y, z); }
	static mat4 scale(const vec3& v){ return mat4().setScale(v); }
	static mat4 scale(float x, float y, float z){ return mat4().setScale(x, y, z); }
	static mat4 rotateX(float theta){ return mat4().setRotateX(theta); }
	static mat4 rotateY(float theta){ return mat4().setRotateY(theta); }
	static mat4 rotateZ(float theta){ return mat4().setRotateZ(theta); }
	static mat4 rotate(const vec3& axis, float angle){ return mat4().setRotate(axis, angle); }
	static mat4 lookAt(const vec3& eye, const vec3& at, const vec3& up){ return mat4().setLookAt(eye, at, up); }
	static mat4 perspective(float fovy, float aspectRatio, float dNear, float dFar){ return mat4().setPerspective(fovy, aspectRatio, dNear, dFar); }

	// row-major transformations
	inline mat4& setTranslate(const vec3& v){ setIdentity(); _14 = v.x; _24 = v.y; _34 = v.z; return *this; }
	inline mat4& setTranslate(float x, float y, float z){ setIdentity(); _14 = x; _24 = y; _34 = z; return *this; }
	inline mat4& setScale(const vec3& v){ setIdentity(); _11 = v.x; _22 = v.y; _33 = v.z; return *this; }
	inline mat4& setScale(float x, float y, float z){ setIdentity(); _11 = x; _22 = y; _33 = z; return *this; }
	inline mat4& setRotateX(float theta){ return setRotate(vec3(1, 0, 0), theta); }
	inline mat4& setRotateY(float theta){ return setRotate(vec3(0, 1, 0), theta); }
	inline mat4& setRotateZ(float theta){ return setRotate(vec3(0, 0, 1), theta); }

	inline mat4& setRotate(const vec3& axis, float angle)
	{
		float c = cos(angle), s = sin(angle), x = axis.x, y = axis.y, z = axis.z;
		a[0] = x*x*(1 - c) + c;		a[1] = x*y*(1 - c) - z*s;		a[2] = x*z*(1 - c) + y*s;	a[3] = 0.0f;
		a[4] = x*y*(1 - c) + z*s;	a[5] = y*y*(1 - c) + c;			a[6] = y*z*(1 - c) - x*s;	a[7] = 0.0f;
		a[8] = x*z*(1 - c) - y*s;	a[9] = y*z*(1 - c) + x*s;		a[10] = z*z*(1 - c) + c;		a[11] = 0.0f;
		a[12] = 0;				a[13] = 0;					a[14] = 0;				a[15] = 1.0f;
		return *this;
	}

	mat4& setLookAt(const vec3& eye, const vec3& at, const vec3& up)
	{
		setIdentity();

		// define camera frame
		vec3 n = (eye - at).normalize();
		vec3 u = (up ^ n).normalize();
		vec3 v = (n ^ u).normalize();

		// calculate lookAt matrix
		_11 = u.x;  _12 = u.y;  _13 = u.z;  _14 = -u.dot(eye);
		_21 = v.x;  _22 = v.y;  _23 = v.z;  _24 = -v.dot(eye);
		_31 = n.x;  _32 = n.y;  _33 = n.z;  _34 = -n.dot(eye);

		return *this;
	};

	mat4& setPerspective(float fovy, float aspectRatio, float dNear, float dFar)
	{
		setIdentity();
		_22 = 1 / tan(fovy / 2.0f);
		_11 = _22 / aspectRatio;
		_33 = (dNear + dFar) / (dNear - dFar);
		_34 = (2 * dNear * dFar) / (dNear - dFar);
		_43 = -1;
		_44 = 0;

		return *this;
	}
};

inline float mat4::determinant() const
{
	return
		_41 * _32 * _23 * _14 - _31 * _42 * _23 * _14 - _41 * _22 * _33 * _14 + _21 * _42 * _33 * _14 +
		_31 * _22 * _43 * _14 - _21 * _32 * _43 * _14 - _41 * _32 * _13 * _24 + _31 * _42 * _13 * _24 +
		_41 * _12 * _33 * _24 - _11 * _42 * _33 * _24 - _31 * _12 * _43 * _24 + _11 * _32 * _43 * _24 +
		_41 * _22 * _13 * _34 - _21 * _42 * _13 * _34 - _41 * _12 * _23 * _34 + _11 * _42 * _23 * _34 +
		_21 * _12 * _43 * _34 - _11 * _22 * _43 * _34 - _31 * _22 * _13 * _44 + _21 * _32 * _13 * _44 +
		_31 * _12 * _23 * _44 - _11 * _32 * _23 * _44 - _21 * _12 * _33 * _44 + _11 * _22 * _33 * _44;
}

inline mat4 mat4::inverse() const
{
	float det = determinant(), s = 1.0f / det; if (det == 0) printf("mat4::inverse() might be singular.\n");
	return mat4((_32*_43*_24 - _42*_33*_24 + _42*_23*_34 - _22*_43*_34 - _32*_23*_44 + _22*_33*_44)*s,
		(_42*_33*_14 - _32*_43*_14 - _42*_13*_34 + _12*_43*_34 + _32*_13*_44 - _12*_33*_44)*s,
		(_22*_43*_14 - _42*_23*_14 + _42*_13*_24 - _12*_43*_24 - _22*_13*_44 + _12*_23*_44)*s,
		(_32*_23*_14 - _22*_33*_14 - _32*_13*_24 + _12*_33*_24 + _22*_13*_34 - _12*_23*_34)*s,

		(_41*_33*_24 - _31*_43*_24 - _41*_23*_34 + _21*_43*_34 + _31*_23*_44 - _21*_33*_44)*s,
		(_31*_43*_14 - _41*_33*_14 + _41*_13*_34 - _11*_43*_34 - _31*_13*_44 + _11*_33*_44)*s,
		(_41*_23*_14 - _21*_43*_14 - _41*_13*_24 + _11*_43*_24 + _21*_13*_44 - _11*_23*_44)*s,
		(_21*_33*_14 - _31*_23*_14 + _31*_13*_24 - _11*_33*_24 - _21*_13*_34 + _11*_23*_34)*s,

		(_31*_42*_24 - _41*_32*_24 + _41*_22*_34 - _21*_42*_34 - _31*_22*_44 + _21*_32*_44)*s,
		(_41*_32*_14 - _31*_42*_14 - _41*_12*_34 + _11*_42*_34 + _31*_12*_44 - _11*_32*_44)*s,
		(_21*_42*_14 - _41*_22*_14 + _41*_12*_24 - _11*_42*_24 - _21*_12*_44 + _11*_22*_44)*s,
		(_31*_22*_14 - _21*_32*_14 - _31*_12*_24 + _11*_32*_24 + _21*_12*_34 - _11*_22*_34)*s,

		(_41*_32*_23 - _31*_42*_23 - _41*_22*_33 + _21*_42*_33 + _31*_22*_43 - _21*_32*_43)*s,
		(_31*_42*_13 - _41*_32*_13 + _41*_12*_33 - _11*_42*_33 - _31*_12*_43 + _11*_32*_43)*s,
		(_41*_22*_13 - _21*_42*_13 - _41*_12*_23 + _11*_42*_23 + _21*_12*_43 - _11*_22*_43)*s,
		(_21*_32*_13 - _31*_22*_13 + _31*_12*_23 - _11*_32*_23 - _21*_12*_33 + _11*_22*_33)*s);
}

//*******************************************************************
// scalar-vector operators
inline vec2 operator+(float f, vec2& v){ return v + f; }
inline vec3 operator+(float f, vec3& v){ return v + f; }
inline vec4 operator+(float f, vec4& v){ return v + f; }
inline vec2 operator-(float f, vec2& v){ return -v + f; }
inline vec3 operator-(float f, vec3& v){ return -v + f; }
inline vec4 operator-(float f, vec4& v){ return -v + f; }
inline vec2 operator*(float f, vec2& v){ return v*f; }
inline vec3 operator*(float f, vec3& v){ return v*f; }
inline vec4 operator*(float f, vec4& v){ return v*f; }

//*******************************************************************
// vertor-matrix multiplications
inline vec3 mul(vec3& v, mat3& m){ return m.transpose()*v; }
inline vec4 mul(vec4& v, mat4& m){ return m.transpose()*v; }
inline vec3 mul(mat3& m, vec3& v){ return m*v; }
inline vec4 mul(mat4& m, vec4& v){ return m*v; }
inline vec3 operator*(vec3& v, mat3& m){ return m.transpose()*v; }
inline vec4 operator*(vec4& v, mat4& m){ return m.transpose()*v; }
inline float dot(const vec2& v1, const vec2& v2){ return v1.dot(v2); }
inline float dot(const vec3& v1, const vec3& v2){ return v1.dot(v2); }
inline float dot(const vec4& v1, const vec4& v2){ return v1.dot(v2); }
inline vec3 cross(const vec3& v1, const vec3& v2){ return v1.cross(v2); }

//*******************************************************************
// utility math functions
inline float deg2rad(float f){ return float(f*PI / float(180.0)); }
inline float rad2deg(float f){ return float(f*float(180.0) / PI); }
inline int get_mip_levels(int width, int height = 0){ uint l = 0, s = uint(max(width, height)); while (s){ s = s >> 1; l++; } return l; }
inline uint get_mip_levels(uint width, uint height = 0){ uint l = 0, s = uint(max(width, height)); while (s){ s = s >> 1; l++; } return l; }

//*******************************************************************
// GLSL-like math functions
inline float frac(float f){ return float(f - floor(f)); }
inline vec2 frac(const vec2& v){ return vec2(frac(v.x), frac(v.y)); }
inline vec3 frac(const vec3& v){ return vec3(frac(v.x), frac(v.y), frac(v.z)); }
inline vec4 frac(const vec4& v){ return vec4(frac(v.x), frac(v.y), frac(v.z), frac(v.w)); }
inline vec2 abs(const vec2& v){ return vec2(fabs(v.x), fabs(v.y)); }
inline vec3 abs(const vec3& v){ return vec3(fabs(v.x), fabs(v.y), fabs(v.z)); }
inline vec4 abs(const vec4& v){ return vec4(fabs(v.x), fabs(v.y), fabs(v.z), fabs(v.w)); }
inline vec2 normalize(const vec2& v){ return v.normalize(); }
inline vec3 normalize(const vec3& v){ return v.normalize(); }
inline vec4 normalize(const vec4& v){ return v.normalize(); }
inline float length(const vec2& v){ return v.length(); }
inline float length(const vec3& v){ return v.length(); }
inline float length(const vec4& v){ return v.length(); }
inline float length2(const vec2& v){ return v.length2(); }
inline float length2(const vec3& v){ return v.length2(); }
inline float length2(const vec4& v){ return v.length2(); }
inline int sign(float f){ return f>0.0f ? 1 : f<0.0f ? -1 : 0; }
inline ivec2 sign(const vec2& v){ return ivec2(sign(v.x), sign(v.y)); }
inline ivec3 sign(const vec3& v){ return ivec3(sign(v.x), sign(v.y), sign(v.z)); }
inline ivec4 sign(const vec4& v){ return ivec4(sign(v.x), sign(v.y), sign(v.z), sign(v.w)); }
inline float smoothstep(float t){ t = clamp(t, 0.0f, 1.0f); return t*t*(3 - 2 * t); }							// C1-continuity
inline double smoothstep(double t){ t = clamp(t, 0.0, 1.0); return t*t*(3 - 2 * t); }
inline float smootherstep(float t){ t = clamp(t, 0.0f, 1.0f); return t*t*t*(6.0f*t*t - 15.0f*t + 10.0f); }		// C2-continuity (by Ken Perlin)
inline double smootherstep(double t){ t = clamp(t, 0.0, 1.0); return t*t*t*(6.0*t*t - 15.0*t + 10.0); }
inline vec2 lerp(const vec2& y1, const vec2& y2, const vec2& t){ return y1*(-t + 1.0f) + y2*t; }
inline vec3 lerp(const vec3& y1, const vec3& y2, const vec3& t){ return y1*(-t + 1.0f) + y2*t; }
inline vec4 lerp(const vec4& y1, const vec4& y2, const vec4& t){ return y1*(-t + 1.0f) + y2*t; }
inline vec2 mix(vec2 v1, vec2 v2, vec2 t){ return lerp(v1, v2, t); }
inline vec3 mix(vec3 v1, vec3 v2, vec3 t){ return lerp(v1, v2, t); }
inline vec4 mix(vec4 v1, vec4 v2, vec4 t){ return lerp(v1, v2, t); }
inline float saturate(float value){ return min(max(value, 0.0f), 1.0f); }
inline vec2 saturate(const vec2& value){ return vec2(saturate(value.x), saturate(value.y)); }
inline vec3 saturate(const vec3& value){ return vec3(saturate(value.x), saturate(value.y), saturate(value.z)); }
inline vec4 saturate(const  vec4& value){ return vec4(saturate(value.x), saturate(value.y), saturate(value.z), saturate(value.w)); }