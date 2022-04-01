#pragma once
#include <math.h>

#ifndef UNITY_BUILD
    #include "utils/utils.h"
#endif

struct Vector3
{
    union
    {
        float32 x = 0.0f;
        float32 r;
    };

    union
    {
        float32 y = 0.0f;
        float32 g;
    };

    union
    {
        float32 z = 0.0f;
        float32 b;
    };

    Vector3( float32 x, float32 y, float32 z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float32 &operator[]( int i )
    {
        return ( ( &x )[ i ] );
    }

    Vector3 &operator+=( Vector3 &v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3 &operator-=( Vector3 &v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
};

inline Vector3 operator+( Vector3 &a, Vector3 &b )
{
    return Vector3( a.x + b.x, a.y + b.y, a.z + b.z );
}

inline Vector3 operator-( Vector3 &a, Vector3 &b )
{
    return Vector3( a.x - b.x, a.y - b.y, a.z - b.z );
}

inline Vector3 operator*( Vector3 &v, float32 s )
{
    return Vector3( v.x * s, v.y * s, v.z * s );
}

inline Vector3 operator/( Vector3 &v, float32 s )
{
    s = 1.0f / s;
    return Vector3( v.x * s, v.y * s, v.z * s );
}

inline Vector3 operator-( Vector3 &v )
{
    return Vector3( -v.x, -v.y, -v.z );
}

inline float32 Magnitude( Vector3 &v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
}

inline Vector3 Normalize( Vector3 &v )
{
    return v / Magnitude( v ); //@TODO: Optimize with reciprocal square root function (Chapter 1.2 page 7)
}

struct Vector4
{
    union
    {
        float32 x = 0.0f;
        float32 r;
    };

    union
    {
        float32 y = 0.0f;
        float32 g;
    };

    union
    {
        float32 z = 0.0f;
        float32 b;
    };

    union
    {
        float32 w = 0.0f;
        float32 a;
    };

    Vector4( float32 x, float32 y, float32 z, float32 w )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    float32 &operator[]( int i )
    {
        return ( ( &x )[ i ] );
    }

    Vector4 &operator+=( Vector4 &v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    Vector4 &operator-=( Vector4 &v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }
};

inline Vector4 operator+( Vector4 &a, Vector4 &b )
{
    return Vector4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w );
}

inline Vector4 operator-( Vector4 &a, Vector4 &b )
{
    return Vector4( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w );
}

inline Vector4 operator*( Vector4 &v, float32 s )
{
    return Vector4( v.x * s, v.y * s, v.z * s, v.w * s );
}

inline Vector4 operator/( Vector4 &v, float32 s )
{
    s = 1.0f / s;
    return Vector4( v.x * s, v.y * s, v.z * s, v.w * s );
}

inline Vector4 operator-( Vector4 &v )
{
    return Vector4( -v.x, -v.y, -v.z, -v.w );
}

inline float32 Magnitude( Vector4 &v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w );
}

inline Vector4 Normalize( Vector4 &v )
{
    return v / Magnitude( v ); //@TODO: Optimize with reciprocal square root function (Chapter 1.2 page 7)
}
