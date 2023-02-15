#pragma once

const double pi = 3.1415926535;

struct quaternion
{
    double x;
    double y;
    double z;
    double w;
};

struct vector4
{
    float x;
    float y;
    float z;
    float w;
};

struct vector3
{
    float x;
    float y;
    float z;

    char* to_string();
};

struct vector2
{
    float x;
    float y;
};

vector3 operator-( const vector3 &lhs, const vector3 &rhs );
vector3 operator*( const vector3 &lhs, const float &rhs );
vector2 operator-( const vector2 &lhs, const vector2 &rhs );

double deg_to_rad( double angle_deg );
quaternion euler_to_quaternion(double yaw, double pitch, double roll);
void gl_perspective(float angleOfView, float imageAspectRatio, float near, float far, float* m_proj);
void gl_look_at( vector3 eye, vector3 at, vector3 up, float* m_look_at );
vector3 vec3_normalise( vector3 vector );
vector3 vec3_cross( vector3 v1, vector3 v2 );
float vec3_dot( vector3 v1, vector3 v2);
