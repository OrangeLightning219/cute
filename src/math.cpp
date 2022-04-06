#pragma once
#include <math.h>

#ifndef UNITY_BUILD
    #include "utils/utils.h"
    #include <stdio.h>
#endif

//@PERFORMANCE: If perfomance becomes an issue try going back to references in operator overloads

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

    Vector3 &operator+=( Vector3 v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3 &operator-=( Vector3 v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3 &operator*=( Vector3 v )
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vector3 &operator/=( Vector3 v )
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    Vector3 &operator+=( float32 v )
    {
        x += v;
        y += v;
        z += v;
        return *this;
    }

    Vector3 &operator-=( float32 v )
    {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }

    Vector3 &operator*=( float32 v )
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    Vector3 &operator/=( float32 v )
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }
};

internal inline Vector3 operator+( Vector3 a, Vector3 b )
{
    return Vector3( a.x + b.x, a.y + b.y, a.z + b.z );
}

internal inline Vector3 operator-( Vector3 a, Vector3 b )
{
    return Vector3( a.x - b.x, a.y - b.y, a.z - b.z );
}

internal inline Vector3 operator*( Vector3 v, float32 s )
{
    return Vector3( v.x * s, v.y * s, v.z * s );
}

internal inline Vector3 operator/( Vector3 v, float32 s )
{
    s = 1.0f / s;
    return Vector3( v.x * s, v.y * s, v.z * s );
}

internal inline Vector3 operator-( Vector3 v )
{
    return Vector3( -v.x, -v.y, -v.z );
}

internal inline float32 Magnitude( Vector3 v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
}

internal inline Vector3 Normalize( Vector3 v )
{
    return v / Magnitude( v ); //@TODO: Optimize with reciprocal square root function (Chapter 1.2 page 7)
}

internal inline float32 Dot( Vector3 a, Vector3 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

internal inline Vector3 Cross( Vector3 a, Vector3 b )
{
    return Vector3( a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x );
}

internal inline Vector3 Project( Vector3 a, Vector3 b )
{
    return b * ( Dot( a, b ) / Dot( b, b ) );
}

internal inline Vector3 Reject( Vector3 a, Vector3 b )
{
    return a - Project( a, b );
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

    Vector4 &operator+=( Vector4 v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    Vector4 &operator-=( Vector4 v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    Vector4 &operator*=( Vector4 v )
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }

    Vector4 &operator/=( Vector4 v )
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }

    Vector4 &operator+=( float32 v )
    {
        x += v;
        y += v;
        z += v;
        w += v;
        return *this;
    }

    Vector4 &operator-=( float32 v )
    {
        x -= v;
        y -= v;
        z -= v;
        w -= v;
        return *this;
    }

    Vector4 &operator*=( float32 v )
    {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
        return *this;
    }

    Vector4 &operator/=( float32 v )
    {
        x /= v;
        y /= v;
        z /= v;
        w /= v;
        return *this;
    }
};

internal inline Vector4 operator+( Vector4 a, Vector4 b )
{
    return Vector4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w );
}

internal inline Vector4 operator-( Vector4 a, Vector4 b )
{
    return Vector4( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w );
}

internal inline Vector4 operator*( Vector4 v, float32 s )
{
    return Vector4( v.x * s, v.y * s, v.z * s, v.w * s );
}

internal inline Vector4 operator/( Vector4 v, float32 s )
{
    s = 1.0f / s;
    return Vector4( v.x * s, v.y * s, v.z * s, v.w * s );
}

internal inline Vector4 operator-( Vector4 v )
{
    return Vector4( -v.x, -v.y, -v.z, -v.w );
}

internal inline float32 Magnitude( Vector4 v )
{
    return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w );
}

internal inline Vector4 Normalize( Vector4 v )
{
    return v / Magnitude( v ); //@TODO: Optimize with reciprocal square root function (Chapter 1.2 page 7)
}

internal inline float32 Dot( Vector4 a, Vector4 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

struct Matrix3
{
    float32 data[ 3 ][ 3 ] = {};

    Matrix3( float32 n00, float32 n01, float32 n02,
             float32 n10, float32 n11, float32 n12,
             float32 n20, float32 n21, float32 n22 )
    {
        data[ 0 ][ 0 ] = n00;
        data[ 0 ][ 1 ] = n10;
        data[ 0 ][ 2 ] = n20;

        data[ 1 ][ 0 ] = n01;
        data[ 1 ][ 1 ] = n11;
        data[ 1 ][ 2 ] = n21;

        data[ 2 ][ 0 ] = n02;
        data[ 2 ][ 1 ] = n12;
        data[ 2 ][ 2 ] = n22;
    }

    Matrix3( Vector3 a, Vector3 b, Vector3 c )
    {
        data[ 0 ][ 0 ] = a.x;
        data[ 0 ][ 1 ] = a.y;
        data[ 0 ][ 2 ] = a.z;

        data[ 1 ][ 0 ] = b.x;
        data[ 1 ][ 1 ] = b.y;
        data[ 1 ][ 2 ] = b.z;

        data[ 2 ][ 0 ] = c.x;
        data[ 2 ][ 1 ] = c.y;
        data[ 2 ][ 2 ] = c.z;
    }

    float &operator()( int i, int j )
    {
        return data[ j ][ i ]; //@NOTE: [j][i] instead of [i][j] because the matrix has column-major order
    }

    Vector3 &operator[]( int i )
    {
        return *( Vector3 * ) data[ i ];
    }
};

internal inline Matrix3 operator*( Matrix3 a, Matrix3 b )
{
    return Matrix3( a( 0, 0 ) * b( 0, 0 ) + a( 0, 1 ) * b( 1, 0 ) + a( 0, 2 ) * b( 2, 0 ),
                    a( 0, 0 ) * b( 0, 1 ) + a( 0, 1 ) * b( 1, 1 ) + a( 0, 2 ) * b( 2, 1 ),
                    a( 0, 0 ) * b( 0, 2 ) + a( 0, 1 ) * b( 1, 2 ) + a( 0, 2 ) * b( 2, 2 ),

                    a( 1, 0 ) * b( 0, 0 ) + a( 1, 1 ) * b( 1, 0 ) + a( 1, 2 ) * b( 2, 0 ),
                    a( 1, 0 ) * b( 0, 1 ) + a( 1, 1 ) * b( 1, 1 ) + a( 1, 2 ) * b( 2, 1 ),
                    a( 1, 0 ) * b( 0, 2 ) + a( 1, 1 ) * b( 1, 2 ) + a( 1, 2 ) * b( 2, 2 ),

                    a( 2, 0 ) * b( 0, 0 ) + a( 2, 1 ) * b( 1, 0 ) + a( 2, 2 ) * b( 2, 0 ),
                    a( 2, 0 ) * b( 0, 1 ) + a( 2, 1 ) * b( 1, 1 ) + a( 2, 2 ) * b( 2, 1 ),
                    a( 2, 0 ) * b( 0, 2 ) + a( 2, 1 ) * b( 1, 2 ) + a( 2, 2 ) * b( 2, 2 ) );
}

internal inline Vector3 operator*( Matrix3 m, Vector3 v )
{
    return Vector3( m( 0, 0 ) * v.x + m( 0, 1 ) * v.y + m( 0, 2 ) * v.z,
                    m( 1, 0 ) * v.x + m( 1, 1 ) * v.y + m( 1, 2 ) * v.z,
                    m( 2, 0 ) * v.x + m( 2, 1 ) * v.y + m( 2, 2 ) * v.z );
}

internal float32 Determinant( Matrix3 &m )
{
    return m( 0, 0 ) * ( m( 1, 1 ) * m( 2, 2 ) - m( 1, 2 ) * m( 2, 1 ) ) +
           m( 0, 1 ) * ( m( 1, 2 ) * m( 2, 0 ) - m( 1, 0 ) * m( 2, 2 ) ) +
           m( 0, 2 ) * ( m( 1, 0 ) * m( 2, 1 ) - m( 1, 1 ) * m( 2, 0 ) );
}

internal Matrix3 Inverse( Matrix3 &m )
{
    Vector3 &a = m[ 0 ];
    Vector3 &b = m[ 1 ];
    Vector3 &c = m[ 2 ];

    Vector3 row0 = Cross( b, c );
    Vector3 row1 = Cross( c, a );
    Vector3 row2 = Cross( a, b );

    float32 inverseDeterminant = 1.0f / Dot( row2, c );

    return Matrix3( row0.x * inverseDeterminant, row0.y * inverseDeterminant, row0.z * inverseDeterminant,
                    row1.x * inverseDeterminant, row1.y * inverseDeterminant, row1.z * inverseDeterminant,
                    row2.x * inverseDeterminant, row2.y * inverseDeterminant, row2.z * inverseDeterminant );
}

struct Matrix4
{
    float32 data[ 4 ][ 4 ] = {};

    Matrix4( float32 n00, float32 n01, float32 n02, float32 n03,
             float32 n10, float32 n11, float32 n12, float32 n13,
             float32 n20, float32 n21, float32 n22, float32 n23,
             float32 n30, float32 n31, float32 n32, float32 n33 )
    {
        data[ 0 ][ 0 ] = n00;
        data[ 0 ][ 1 ] = n10;
        data[ 0 ][ 2 ] = n20;
        data[ 0 ][ 3 ] = n30;

        data[ 1 ][ 0 ] = n01;
        data[ 1 ][ 1 ] = n11;
        data[ 1 ][ 2 ] = n21;
        data[ 1 ][ 3 ] = n31;

        data[ 2 ][ 0 ] = n02;
        data[ 2 ][ 1 ] = n12;
        data[ 2 ][ 2 ] = n22;
        data[ 2 ][ 3 ] = n32;

        data[ 3 ][ 0 ] = n03;
        data[ 3 ][ 1 ] = n13;
        data[ 3 ][ 2 ] = n23;
        data[ 3 ][ 3 ] = n33;
    }

    Matrix4( Vector4 a, Vector4 b, Vector4 c, Vector4 d )
    {
        data[ 0 ][ 0 ] = a.x;
        data[ 0 ][ 1 ] = a.y;
        data[ 0 ][ 2 ] = a.z;
        data[ 0 ][ 3 ] = a.w;

        data[ 1 ][ 0 ] = b.x;
        data[ 1 ][ 1 ] = b.y;
        data[ 1 ][ 2 ] = b.z;
        data[ 1 ][ 3 ] = b.w;

        data[ 2 ][ 0 ] = c.x;
        data[ 2 ][ 1 ] = c.y;
        data[ 2 ][ 2 ] = c.z;
        data[ 2 ][ 3 ] = c.w;

        data[ 3 ][ 0 ] = d.x;
        data[ 3 ][ 1 ] = d.y;
        data[ 3 ][ 2 ] = d.z;
        data[ 3 ][ 3 ] = d.w;
    }

    float &operator()( int i, int j )
    {
        return data[ j ][ i ]; //@NOTE: [j][i] instead of [i][j] because the matrix has column-major order
    }

    Vector4 &operator[]( int i )
    {
        return *( Vector4 * ) data[ i ];
    }
};

internal inline Matrix4 operator*( Matrix4 a, Matrix4 b )
{
    return Matrix4( a( 0, 0 ) * b( 0, 0 ) + a( 0, 1 ) * b( 1, 0 ) + a( 0, 2 ) * b( 2, 0 ) + a( 0, 3 ) * b( 3, 0 ),
                    a( 0, 0 ) * b( 0, 1 ) + a( 0, 1 ) * b( 1, 1 ) + a( 0, 2 ) * b( 2, 1 ) + a( 0, 3 ) * b( 3, 1 ),
                    a( 0, 0 ) * b( 0, 2 ) + a( 0, 1 ) * b( 1, 2 ) + a( 0, 2 ) * b( 2, 2 ) + a( 0, 3 ) * b( 3, 2 ),
                    a( 0, 0 ) * b( 0, 3 ) + a( 0, 1 ) * b( 1, 3 ) + a( 0, 2 ) * b( 2, 3 ) + a( 0, 3 ) * b( 3, 3 ),

                    a( 1, 0 ) * b( 0, 0 ) + a( 1, 1 ) * b( 1, 0 ) + a( 1, 2 ) * b( 2, 0 ) + a( 1, 3 ) * b( 3, 0 ),
                    a( 1, 0 ) * b( 0, 1 ) + a( 1, 1 ) * b( 1, 1 ) + a( 1, 2 ) * b( 2, 1 ) + a( 1, 3 ) * b( 3, 1 ),
                    a( 1, 0 ) * b( 0, 2 ) + a( 1, 1 ) * b( 1, 2 ) + a( 1, 2 ) * b( 2, 2 ) + a( 1, 3 ) * b( 3, 2 ),
                    a( 1, 0 ) * b( 0, 3 ) + a( 1, 1 ) * b( 1, 3 ) + a( 1, 2 ) * b( 2, 3 ) + a( 1, 3 ) * b( 3, 3 ),

                    a( 2, 0 ) * b( 0, 0 ) + a( 2, 1 ) * b( 1, 0 ) + a( 2, 2 ) * b( 2, 0 ) + a( 2, 3 ) * b( 3, 0 ),
                    a( 2, 0 ) * b( 0, 1 ) + a( 2, 1 ) * b( 1, 1 ) + a( 2, 2 ) * b( 2, 1 ) + a( 2, 3 ) * b( 3, 1 ),
                    a( 2, 0 ) * b( 0, 2 ) + a( 2, 1 ) * b( 1, 2 ) + a( 2, 2 ) * b( 2, 2 ) + a( 2, 3 ) * b( 3, 2 ),
                    a( 2, 0 ) * b( 0, 3 ) + a( 2, 1 ) * b( 1, 3 ) + a( 2, 2 ) * b( 2, 3 ) + a( 2, 3 ) * b( 3, 3 ),

                    a( 3, 0 ) * b( 0, 0 ) + a( 3, 1 ) * b( 1, 0 ) + a( 3, 2 ) * b( 2, 0 ) + a( 3, 3 ) * b( 3, 0 ),
                    a( 3, 0 ) * b( 0, 1 ) + a( 3, 1 ) * b( 1, 1 ) + a( 3, 2 ) * b( 2, 1 ) + a( 3, 3 ) * b( 3, 1 ),
                    a( 3, 0 ) * b( 0, 2 ) + a( 3, 1 ) * b( 1, 2 ) + a( 3, 2 ) * b( 2, 2 ) + a( 3, 3 ) * b( 3, 2 ),
                    a( 3, 0 ) * b( 0, 3 ) + a( 3, 1 ) * b( 1, 3 ) + a( 3, 2 ) * b( 2, 3 ) + a( 3, 3 ) * b( 3, 3 ) );
}

internal inline Vector4 operator*( Matrix4 m, Vector4 v )
{
    return Vector4( m( 0, 0 ) * v.x + m( 0, 1 ) * v.y + m( 0, 2 ) * v.z + m( 0, 3 ) * v.w,
                    m( 1, 0 ) * v.x + m( 1, 1 ) * v.y + m( 1, 2 ) * v.z + m( 1, 3 ) * v.w,
                    m( 2, 0 ) * v.x + m( 2, 1 ) * v.y + m( 2, 2 ) * v.z + m( 2, 3 ) * v.w,
                    m( 3, 0 ) * v.x + m( 3, 1 ) * v.y + m( 3, 2 ) * v.z + m( 3, 3 ) * v.w );
}

internal Matrix4 Inverse( Matrix4 &m )
{
    Vector3 &a = ( Vector3 & ) m[ 0 ];
    Vector3 &b = ( Vector3 & ) m[ 1 ];
    Vector3 &c = ( Vector3 & ) m[ 2 ];
    Vector3 &d = ( Vector3 & ) m[ 3 ];

    float32 x = m( 3, 0 );
    float32 y = m( 3, 1 );
    float32 z = m( 3, 2 );
    float32 w = m( 3, 3 );

    Vector3 s = Cross( a, b );
    Vector3 t = Cross( c, d );
    Vector3 u = a * y - b * x;
    Vector3 v = c * w - d * z;

    float32 inverseDeterminant = 1.0f / ( Dot( s, v ) + Dot( t, u ) );
    s *= inverseDeterminant;
    t *= inverseDeterminant;
    u *= inverseDeterminant;
    v *= inverseDeterminant;

    Vector3 row0 = Cross( b, v ) + t * y;
    Vector3 row1 = Cross( v, a ) - t * x;
    Vector3 row2 = Cross( d, u ) + s * w;
    Vector3 row3 = Cross( u, c ) - s * z;

    return Matrix4( row0.x, row0.y, row0.z, -Dot( b, t ),
                    row1.x, row1.y, row1.z, Dot( a, t ),
                    row2.x, row2.y, row2.z, -Dot( d, s ),
                    row3.x, row3.y, row3.z, Dot( c, s ) );
}

internal void PrintMatrix( Matrix4 m )
{
    for ( int i = 0; i < 4; ++i )
    {
        for ( int j = 0; j < 4; ++j )
        {
            printf( "% 5.4f ", m( i, j ) );
        }
        printf( "\n" );
    }
}

internal void PrintMatrix( Matrix3 m )
{
    for ( int i = 0; i < 3; ++i )
    {
        for ( int j = 0; j < 3; ++j )
        {
            printf( "% 5.4f ", m( i, j ) );
        }
        printf( "\n" );
    }
}

struct Transform
{
    float32 data[ 4 ][ 4 ] = {};

    Transform( float32 n00, float32 n01, float32 n02, float32 n03,
               float32 n10, float32 n11, float32 n12, float32 n13,
               float32 n20, float32 n21, float32 n22, float32 n23 )
    {
        data[ 0 ][ 0 ] = n00;
        data[ 0 ][ 1 ] = n10;
        data[ 0 ][ 2 ] = n20;
        data[ 0 ][ 3 ] = 0.0f;

        data[ 1 ][ 0 ] = n01;
        data[ 1 ][ 1 ] = n11;
        data[ 1 ][ 2 ] = n21;
        data[ 1 ][ 3 ] = 0.0f;

        data[ 2 ][ 0 ] = n02;
        data[ 2 ][ 1 ] = n12;
        data[ 2 ][ 2 ] = n22;
        data[ 2 ][ 3 ] = 0.0f;

        data[ 3 ][ 0 ] = n03;
        data[ 3 ][ 1 ] = n13;
        data[ 3 ][ 2 ] = n23;
        data[ 3 ][ 3 ] = 1.0f;
    }

    Transform( Vector3 a, Vector3 b, Vector3 c, Vector3 d )
    {
        data[ 0 ][ 0 ] = a.x;
        data[ 0 ][ 1 ] = a.y;
        data[ 0 ][ 2 ] = a.z;
        data[ 0 ][ 3 ] = 0.0f;

        data[ 1 ][ 0 ] = b.x;
        data[ 1 ][ 1 ] = b.y;
        data[ 1 ][ 2 ] = b.z;
        data[ 1 ][ 3 ] = 0.0f;

        data[ 2 ][ 0 ] = c.x;
        data[ 2 ][ 1 ] = c.y;
        data[ 2 ][ 2 ] = c.z;
        data[ 2 ][ 3 ] = 0.0f;

        data[ 3 ][ 0 ] = d.x;
        data[ 3 ][ 1 ] = d.y;
        data[ 3 ][ 2 ] = d.z;
        data[ 3 ][ 3 ] = 1.0f;
    }

    Transform( Matrix3 m )
    {
        data[ 0 ][ 0 ] = m.data[ 0 ][ 0 ];
        data[ 0 ][ 1 ] = m.data[ 0 ][ 1 ];
        data[ 0 ][ 2 ] = m.data[ 0 ][ 2 ];
        data[ 0 ][ 3 ] = 0.0f;

        data[ 1 ][ 0 ] = m.data[ 1 ][ 0 ];
        data[ 1 ][ 1 ] = m.data[ 1 ][ 1 ];
        data[ 1 ][ 2 ] = m.data[ 1 ][ 2 ];
        data[ 1 ][ 3 ] = 0.0f;

        data[ 2 ][ 0 ] = m.data[ 2 ][ 0 ];
        data[ 2 ][ 1 ] = m.data[ 2 ][ 1 ];
        data[ 2 ][ 2 ] = m.data[ 2 ][ 2 ];
        data[ 2 ][ 3 ] = 0.0f;

        data[ 3 ][ 0 ] = 0.0f;
        data[ 3 ][ 1 ] = 0.0f;
        data[ 3 ][ 2 ] = 0.0f;
        data[ 3 ][ 3 ] = 1.0f;
    }
    float &operator()( int i, int j )
    {
        return data[ j ][ i ]; //@NOTE: [j][i] instead of [i][j] because the matrix has column-major order
    }

    Vector3 &operator[]( int i )
    {
        return *( Vector3 * ) data[ i ];
    }
};

internal inline Transform operator*( Transform a, Transform b )
{
    return Transform( a( 0, 0 ) * b( 0, 0 ) + a( 0, 1 ) * b( 1, 0 ) + a( 0, 2 ) * b( 2, 0 ),
                      a( 0, 0 ) * b( 0, 1 ) + a( 0, 1 ) * b( 1, 1 ) + a( 0, 2 ) * b( 2, 1 ),
                      a( 0, 0 ) * b( 0, 2 ) + a( 0, 1 ) * b( 1, 2 ) + a( 0, 2 ) * b( 2, 2 ),
                      a( 0, 0 ) * b( 0, 3 ) + a( 0, 1 ) * b( 1, 3 ) + a( 0, 2 ) * b( 2, 3 ) + a( 0, 3 ),

                      a( 1, 0 ) * b( 0, 0 ) + a( 1, 1 ) * b( 1, 0 ) + a( 1, 2 ) * b( 2, 0 ),
                      a( 1, 0 ) * b( 0, 1 ) + a( 1, 1 ) * b( 1, 1 ) + a( 1, 2 ) * b( 2, 1 ),
                      a( 1, 0 ) * b( 0, 2 ) + a( 1, 1 ) * b( 1, 2 ) + a( 1, 2 ) * b( 2, 2 ),
                      a( 1, 0 ) * b( 0, 3 ) + a( 1, 1 ) * b( 1, 3 ) + a( 1, 2 ) * b( 2, 3 ) + a( 1, 3 ),

                      a( 2, 0 ) * b( 0, 0 ) + a( 2, 1 ) * b( 1, 0 ) + a( 2, 2 ) * b( 2, 0 ),
                      a( 2, 0 ) * b( 0, 1 ) + a( 2, 1 ) * b( 1, 1 ) + a( 2, 2 ) * b( 2, 1 ),
                      a( 2, 0 ) * b( 0, 2 ) + a( 2, 1 ) * b( 1, 2 ) + a( 2, 2 ) * b( 2, 2 ),
                      a( 2, 0 ) * b( 0, 3 ) + a( 2, 1 ) * b( 1, 3 ) + a( 2, 2 ) * b( 2, 3 ) + a( 2, 3 ) );
}

//@TODO: Make a distinction between point and direction vectors?
internal inline Vector3 operator*( Transform m, Vector3 v )
{
    return Vector3( m( 0, 0 ) * v.x + m( 0, 1 ) * v.y + m( 0, 2 ) * v.z,
                    m( 1, 0 ) * v.x + m( 1, 1 ) * v.y + m( 1, 2 ) * v.z,
                    m( 2, 0 ) * v.x + m( 2, 1 ) * v.y + m( 2, 2 ) * v.z );
}

internal Transform Inverse( Transform &m )
{
    Vector3 &a = m[ 0 ];
    Vector3 &b = m[ 1 ];
    Vector3 &c = m[ 2 ];
    Vector3 &d = m[ 3 ];

    Vector3 s = Cross( a, b );
    Vector3 t = Cross( c, d );

    float32 inverseDeterminant = 1.0f / Dot( s, c );
    s *= inverseDeterminant;
    t *= inverseDeterminant;
    Vector3 v = c * inverseDeterminant;

    Vector3 row0 = Cross( b, v );
    Vector3 row1 = Cross( v, a );

    return Transform( row0.x, row0.y, row0.z, -Dot( b, t ),
                      row1.x, row1.y, row1.z, Dot( a, t ),
                      s.x, s.y, s.z, -Dot( d, s ) );
}

internal inline Vector3 GetTranslation( Transform &t )
{
    return t[ 3 ];
}

internal inline void SetTranslation( Transform &t, Vector3 p )
{
    t.data[ 3 ][ 0 ] = p.x;
    t.data[ 3 ][ 1 ] = p.y;
    t.data[ 3 ][ 2 ] = p.z;
}

internal Transform CreateRotationX( float32 angle )
{
    angle = DEGREES_TO_RADIANS( angle );
    float32 c = cosf( angle );
    float32 s = sinf( angle );

    return Transform( 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, c, -s, 0.0f,
                      0.0f, s, c, 0.0f );
}

internal Transform CreateRotationY( float32 angle )
{
    angle = DEGREES_TO_RADIANS( angle );
    float32 c = cosf( angle );
    float32 s = sinf( angle );

    return Transform( c, 0.0f, s, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      -s, 0.0f, c, 0.0f );
}

internal Transform CreateRotationZ( float32 angle )
{
    angle = DEGREES_TO_RADIANS( angle );
    float32 c = cosf( angle );
    float32 s = sinf( angle );

    return Transform( c, -s, 0.0f, 0.0f,
                      s, c, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f );
}

internal Transform CreateRotation( float32 angle, Vector3 axis )
{
    angle = DEGREES_TO_RADIANS( angle );
    float32 c = cosf( angle );
    float32 s = sinf( angle );
    float32 d = 1.0f - c;

    float32 x = axis.x * d;
    float32 y = axis.y * d;
    float32 z = axis.z * d;

    float32 axay = x * axis.y;
    float32 axaz = x * axis.z;
    float32 ayaz = y * axis.z;

    return Transform( c + x * axis.x, axay - s * axis.z, axaz + s * axis.y, 0.0f,
                      axay + s * axis.z, c + y * axis.y, ayaz - s * axis.x, 0.0f,
                      axaz - s * axis.y, ayaz + s * axis.x, c + z * axis.z, 0.0f );
}

internal Transform CreateReflection( Vector3 axis )
{
    float32 x = axis.x * -2.0f;
    float32 y = axis.y * -2.0f;
    float32 z = axis.z * -2.0f;
    float32 axay = x * axis.y;
    float32 axaz = x * axis.z;
    float32 ayaz = y * axis.z;

    return Transform( x * axis.x + 1.0f, axay, axaz, 0.0f,
                      axay, y * axis.y + 1.0f, ayaz, 0.0f,
                      axaz, ayaz, z * axis.z + 1.0f, 0.0f );
}

internal Transform CreateInvolution( Vector3 axis )
{
    float32 x = axis.x * 2.0f;
    float32 y = axis.y * 2.0f;
    float32 z = axis.z * 2.0f;
    float32 axay = x * axis.y;
    float32 axaz = x * axis.z;
    float32 ayaz = y * axis.z;

    return Transform( x * axis.x - 1.0f, axay, axaz, 0.0f,
                      axay, y * axis.y - 1.0f, ayaz, 0.0f,
                      axaz, ayaz, z * axis.z - 1.0f, 0.0f );
}

internal Transform CreateScale( float32 scaleX, float32 scaleY, float32 scaleZ )
{
    return Transform( scaleX, 0.0f, 0.0f, 0.0f,
                      0.0f, scaleY, 0.0f, 0.0f,
                      0.0f, 0.0f, scaleZ, 0.0f );
}

internal Transform CreateScale( float32 scale, Vector3 axis )
{
    scale -= 1.0f;
    float32 x = axis.x * scale;
    float32 y = axis.y * scale;
    float32 z = axis.z * scale;
    float32 axay = x * axis.y;
    float32 axaz = x * axis.z;
    float32 ayaz = y * axis.z;

    return Transform( x * axis.x + 1.0f, axay, axaz, 0.0f,
                      axay, y * axis.y + 1.0f, ayaz, 0.0f,
                      axaz, ayaz, z * axis.z + 1.0f, 0.0f );
}

internal Transform CreateSkew( float32 angle, Vector3 a, Vector3 b )
{
    angle = DEGREES_TO_RADIANS( angle );
    float32 t = tanf( angle );
    float32 x = a.x * t;
    float32 y = a.y * t;
    float32 z = a.z * t;

    return Transform( x * b.x + 1.0f, x * b.y, x * b.z, 0.0f,
                      y * b.x, y * b.y + 1.0f, y * b.z, 0.0f,
                      z * b.x, z * b.y, z * b.z + 1.0f, 0.0f );
}
