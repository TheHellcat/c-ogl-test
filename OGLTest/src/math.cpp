#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <cstdio>
#include "math.h"


vector3 operator-( const vector3 &lhs, const vector3 &rhs )
{
    vector3 r;

    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;
    r.z = lhs.z - rhs.z;

    return r;
}

vector3 operator*( const vector3 &lhs, const float &rhs )
{
    vector3 r;

    r.x = lhs.x * rhs;
    r.y = lhs.y * rhs;
    r.z = lhs.z * rhs;

    return r;
}

char* vector3::to_string()
{
    char s[] = "                                                                ";

    sprintf(s, "%f, %f, %f", x, y, z);

    return &s[0];
}

vector2 operator-( const vector2 &lhs, const vector2 &rhs )
{
    vector2 r;

    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;

    return r;
}

double deg_to_rad( double angle_deg )
{
    return angle_deg * (pi/180);
}

quaternion euler_to_quaternion(double yaw, double pitch, double roll)
{
    // for readability
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    quaternion q;

    // stuff them sines and cosines into the quat fields
    q.x = cr * cp * sy - sr * sp * cy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = sr * cp * cy - cr * sp * sy;
    q.w = cr * cp * cy + sr * sp * sy;

    return q;
}

void gl_perspective(
     float angleOfView,
     float imageAspectRatio,
     float near, float far,
     float* m_proj
     )
{
    float r, l, t, b;
    float scale = tan(angleOfView * 0.5 * pi / 180) * near;
    r = imageAspectRatio * scale, l = -r;
    t = scale, b = -t;

    // set OpenGL perspective projection matrix
    m_proj[0] = 2.0f * near / (r - l);
    m_proj[1] = 0.0f;
    m_proj[2] = 0.0f;
    m_proj[3] = 0.0f;

    m_proj[4] = 0.0f;
    m_proj[5] = 2 * near / (t - b);
    m_proj[6] = 0.0f;
    m_proj[7] = 0.0f;

    m_proj[8] = (r + l) / (r - l);
    m_proj[9] = (t + b) / (t - b);
    m_proj[10] = -(far + near) / (far - near);
    m_proj[11] = -1.0f;

    m_proj[12] = 0.0f;
    m_proj[13] = 0.0f;
    m_proj[14] = -2.0f * far * near / (far - near);
    m_proj[15] = 0.0f;
}

void gl_look_at( vector3 eye, vector3 at, vector3 up, float* m_look_at )
{
    vector3 zaxis;
    vector3 xaxis;
    vector3 yaxis;
    vector3 zero;

    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;

    zaxis = vec3_normalise( eye - at );
    xaxis = vec3_normalise( vec3_cross( up, zaxis ) );
    yaxis = vec3_cross( zaxis, xaxis );

    m_look_at[0] = xaxis.x;
    m_look_at[1] = yaxis.x;
    m_look_at[2] = zaxis.x;
    m_look_at[3] = 0.0f;

    m_look_at[4] = xaxis.y;
    m_look_at[5] = yaxis.y;
    m_look_at[6] = zaxis.y;
    m_look_at[7] = 0.0f;

    m_look_at[8] = xaxis.z;
    m_look_at[9] = yaxis.z;
    m_look_at[10] = zaxis.z;
    m_look_at[11] = 0.0f;

    m_look_at[12] = vec3_dot( xaxis, zero-eye );
    m_look_at[13] = vec3_dot( yaxis, zero-eye );
    m_look_at[14] = vec3_dot( zaxis, zero-eye );
    m_look_at[15] = 1.0f;
}

vector3 vec3_normalise( vector3 vector )
{
    float length;
    vector3 normalisedVector;

    length = sqrt( (vector.x*vector.x) + (vector.y*vector.y) + (vector.z*vector.z) );
    normalisedVector.x = vector.x / length;
    normalisedVector.y = vector.y / length;
    normalisedVector.z = vector.z / length;

    return normalisedVector;
}

vector3 vec3_cross( vector3 v1, vector3 v2 )
{
    /*
    cx = aybz − azby = 3×7 − 4×6 = −3
    cy = azbx − axbz = 4×5 − 2×7 = 6
    cz = axby − aybx = 2×6 − 3×5 = −3
     */

    vector3 crossVector;

    crossVector.x = ( v1.y * v2.z ) - ( v1.z * v2.y );
    crossVector.y = ( v1.z * v2.x ) - ( v1.x * v2.z );
    crossVector.z = ( v1.x * v2.y ) - ( v1.y * v2.x );

    return crossVector;
}

float vec3_dot( vector3 v1, vector3 v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
