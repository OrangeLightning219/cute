#pragma once
#ifndef UNITY_BUILD
    #include "math.cpp"
    #include "glad.c"
#endif
global_variable float32 defaultYaw = -90.0f;
global_variable float32 defaultPitch = 0.0f;
global_variable float32 defaultSpeed = 2.5f;
global_variable float32 defaultZoom = 45.0f;
global_variable float32 defaultSensitivity = 0.1f;

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct Camera
{
    Vector3 position;
    Vector3 front;
    Vector3 up;
    Vector3 right;
    Vector3 worldUp;

    float32 yaw;
    float32 pitch;

    float32 movementSpeed;
    float32 mouseSensitivity;
    float32 zoom;
};

internal void UpdateCameraVectors( Camera *camera )
{
    Vector3 front;
    front.x = cosf( DEGREES_TO_RADIANS( camera->yaw ) ) * cosf( DEGREES_TO_RADIANS( camera->pitch ) );
    front.y = sinf( DEGREES_TO_RADIANS( camera->pitch ) );
    front.z = sinf( DEGREES_TO_RADIANS( camera->yaw ) ) * cosf( DEGREES_TO_RADIANS( camera->pitch ) );
    camera->front = Normalize( front );

    camera->right = Normalize( Cross( camera->front, camera->worldUp ) );
    camera->up = Normalize( Cross( camera->right, camera->front ) );
}

internal Camera CreateCamera( Vector3 position, float32 yaw = defaultYaw, float32 pitch = defaultPitch,
                              Vector3 worldUp = Vector3( 0.0f, 1.0f, 0.0f ) )
{
    Camera result = {};
    result.position = position;
    result.front = Vector3( 0.0f, 0.0f, -1.0f );
    result.worldUp = worldUp;
    result.yaw = yaw;
    result.pitch = pitch;
    result.movementSpeed = defaultSpeed;
    result.mouseSensitivity = defaultSensitivity;
    result.zoom = defaultZoom;
    UpdateCameraVectors( &result );
    return result;
}

// Matrix4 GetProjectionMatrix( float32 fov, float32 ratio, float32 nearPlane, float32 farPlane )
// {
//     float32 g = 1.0f / tanf( DEGREES_TO_RADIANS( fov ) * 0.5f );
//     float32 k = farPlane / ( farPlane - nearPlane );

//     return Matrix4( g / ratio, 0.0f, 0.0f, 0.0f,
//                     0.0f, g, 0.0f, 0.0f,
//                     0.0f, 0.0f, k, -nearPlane * k,
//                     0.0f, 0.0f, 1.0f, 0.0f );
// }

Matrix4 GetProjectionMatrix( float32 fov, float32 ratio, float32 zNear, float32 zFar )
{
    float32 halfTan = tanf( DEGREES_TO_RADIANS( fov ) * 0.5f );

    return Matrix4( 1.0f / ( ratio * halfTan ), 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f / halfTan, 0.0f, 0.0f,
                    0.0f, 0.0f, -( zFar + zNear ) / ( zFar - zNear ), -( 2 * zFar * zNear ) / ( zFar - zNear ),
                    0.0f, 0.0f, -1.0f, 0.0f );
}
Matrix4 GetViewMatrix( Camera *camera )
{
    Vector3 &eye = camera->position;
    Vector3 center = camera->position + camera->front;
    Vector3 &up = camera->up;

    Vector3 f = Normalize( center - eye );
    Vector3 s = Normalize( Cross( f, up ) );
    Vector3 u = Cross( s, f );

    return Matrix4( s.x, s.y, s.z, -Dot( s, eye ),
                    u.x, u.y, u.z, -Dot( u, eye ),
                    -f.x, -f.y, -f.z, Dot( f, eye ),
                    0.0f, 0.0f, 0.0f, 1.0f );
}

internal void CameraProcessMouse( Camera *camera, float32 xOffset, float32 yOffset, bool constrainPitch = true )
{
    xOffset *= camera->mouseSensitivity;
    yOffset *= camera->mouseSensitivity;

    camera->yaw += xOffset;
    camera->pitch += yOffset;

    if ( constrainPitch )
    {
        if ( camera->pitch > 89.0f ) { camera->pitch = 89.0f; }
        if ( camera->pitch < -89.0f ) { camera->pitch = -89.0f; }
    }

    UpdateCameraVectors( camera );
}
