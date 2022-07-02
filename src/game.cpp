#include "game.h"
#include "cute.cpp"

enum Entity_Type
{
    PLAYER,
    PLAYER_SEGMENT,
    WALL,
    FLOOR,
    FOOD
};

enum Player_Movement_Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Entity
{
    Entity_Type type;
    Transform transform;

    int playerSegmentCount;
    Entity *playerSegments;
    float32 playerMovementSpeed;
    float32 playerRotationSpeed;
    Player_Movement_Direction playerMovementDirection;
    Vector3 playerMovementDirectionVector;
};

struct Game_State
{
    bool isInitialized;

    Memory_Arena entitiesArena;
    Camera camera;
    int windowWidth;
    int windowHeight;

    bool firstMouse;
    float32 lastMouseX;
    float32 lastMouseY;

    int entityCount;
    Entity *entities;

    Matrix4 projectionMatrix;
};

internal inline Game_State *GetGameState( Game_Memory *memory )
{
    return ( Game_State * ) memory->permanentStorage;
}

Platform_Api platform;

internal void SetProjectionMatrix( Game_State *gameState )
{
    gameState->projectionMatrix = GetProjectionMatrix( 45.0f, ( float32 ) gameState->windowWidth / ( float32 ) gameState->windowHeight,
                                                       0.1f, 500.0f );
}

internal void WindowChanged( Game_State *gameState, int windowWidth, int windowHeight )
{
    gameState->windowWidth = windowWidth;
    gameState->windowHeight = windowHeight;
    SetProjectionMatrix( gameState );
}

internal void InitializeArena( Memory_Arena *arena, memory_index size, u8 *base )
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}

// internal void RotatePlayer( Entity *player, float32 angle )
// {
//     Rotate( player->transform, AXIS_Y, angle );
//     Vector3 frontVector = player->transform.position + player->playerMovementDirection;
//     RotateAroundPoint( frontVector, player->transform.position,
//                        AXIS_Y, angle );
//     player->playerMovementDirection = Normalize( frontVector - player->transform.position );
// }

internal void GameInit( Game_Memory *memory, int windowWidth, int windowHeight )
{
    Game_State *gameState = GetGameState( memory );
    gameState->firstMouse = true;
    CuteInit( memory );

    if ( !platform.RendererInit() )
    {
        printf( "Failed to init the renderer!" );
        return;
    }
    // gameState->camera = CreateCamera( Vector3( 0.0f, 0.0f, 6.0f ), defaultYaw, 0.0f );
    gameState->camera = CreateCamera( Vector3( 0.0f, 15.0f, 5.0f ), defaultYaw, -70.0f );

    WindowChanged( gameState, windowWidth, windowHeight );

    InitializeArena( &gameState->entitiesArena, memory->permanentStorageSize - sizeof( Game_State ),
                     ( u8 * ) memory->permanentStorage + sizeof( Game_State ) );

    gameState->entityCount = 0;
    gameState->entities = PushArray( &gameState->entitiesArena, 7, Entity );
    gameState->entityCount += 7;

    Entity *player = gameState->entities;
    Entity *food = &gameState->entities[ 1 ];
    Entity *floor = &gameState->entities[ 2 ];
    Entity *walls = &gameState->entities[ 3 ];

    player->type = PLAYER;
    player->transform = Transform();
    player->transform.position = Vector3( -2.0f, 0.0f, -2.0f );
    player->transform.scale = Vector3( 0.75f );
    player->playerSegments = gameState->entities + 7;
    player->playerSegmentCount = 0;
    player->playerMovementDirection = UP;
    player->playerMovementDirectionVector = Vector3( 0.0f, 0.0f, -1.0f );
    player->playerMovementSpeed = 2.0f;
    player->playerRotationSpeed = 90.0f;

    food->type = FOOD;
    Transform foodTransform = {};
    foodTransform.position = Vector3( 2.0f, 0.0f, 0.0f );
    foodTransform.scale = Vector3( 0.5f );
    food->transform = foodTransform;

    floor->type = FLOOR;
    Transform floorTransform = {};
    floorTransform.position = Vector3( 0.0f, -1.0f, 0.0f );
    floorTransform.scale = Vector3( 10.0f, 1.0f, 10.0f );
    floor->transform = floorTransform;

    for ( int i = 0; i < 4; ++i )
    {
        Entity *wall = &walls[ i ];
        wall->type = WALL;
        Transform wallTransform = {};
        wallTransform.position = Vector3( 0.0f, 0.0f, -5.0f );
        wallTransform.scale = Vector3( 10.0f, 1.0f, 0.5f );
        RotateAroundPoint( wallTransform, floorTransform.position, AXIS_Y, i * 90.0f );
        wall->transform = wallTransform;
    }

    for ( int i = 0; i < 3; ++i )
    {
        Entity *segment = PushStruct( &gameState->entitiesArena, Entity );
        // gameState->playerSegmentCount += 1;
    }

    gameState->isInitialized = true;
}

extern "C" __declspec( dllexport )
GAME_UPDATE_AND_RENDER( GameUpdateAndRender )
{
    platform = memory->platformApi;
    Game_State *gameState = GetGameState( memory );

    if ( !gameState->isInitialized ) { GameInit( memory, windowWidth, windowHeight ); }

    if ( gameState->windowWidth != windowWidth || gameState->windowHeight != windowHeight )
    {
        WindowChanged( gameState, windowWidth, windowHeight );
    }

    Entity *player = gameState->entities;

    float32 dt = input->dtForUpdate;
    for ( int controllerIndex = 0; controllerIndex < ArrayCount( input->controllers ); ++controllerIndex )
    {
        Game_Controller_Input *controller = getController( input, controllerIndex );
        if ( !controller->isConnected ) { continue; }
        if ( controller->isAnalog )
        {
        }
        else
        {
            //             Camera *camera = &gameState->camera;
            //             float32 velocity = camera->movementSpeed * dt;
            //             if ( controller->up.endedDown ) { camera->position += camera->front * velocity; }
            //             if ( controller->down.endedDown ) { camera->position -= camera->front * velocity; }
            //             if ( controller->left.endedDown ) { camera->position -= camera->right * velocity; }
            //             if ( controller->right.endedDown ) { camera->position += camera->right * velocity; }

            //             if ( controller->a.endedDown ) { camera->movementSpeed = 7.0f; }
            //             else { camera->movementSpeed = defaultSpeed; }

            if ( controller->up.endedDown && player->playerMovementDirection != DOWN )
            {
                player->playerMovementDirection = UP;
                player->playerMovementDirectionVector = Vector3( 0.0f, 0.0f, -1.0f );
            }
            if ( controller->down.endedDown && player->playerMovementDirection != UP )
            {
                player->playerMovementDirection = DOWN;
                player->playerMovementDirectionVector = Vector3( 0.0f, 0.0f, 1.0f );
            }
            if ( controller->left.endedDown && player->playerMovementDirection != RIGHT )
            {
                player->playerMovementDirection = LEFT;
                player->playerMovementDirectionVector = Vector3( -1.0f, 0.0f, 0.0f );
                // RotatePlayer( player, player->playerRotationSpeed * dt );
            }
            if ( controller->right.endedDown && player->playerMovementDirection != LEFT )
            {
                player->playerMovementDirection = RIGHT;
                player->playerMovementDirectionVector = Vector3( 1.0f, 0.0f, 0.0f );
                // RotatePlayer( player, -player->playerRotationSpeed * dt );
            }
        }
    }

    if ( gameState->firstMouse )
    {
        gameState->lastMouseX = ( float32 ) input->mouseX;
        gameState->lastMouseY = ( float32 ) input->mouseY;
        gameState->firstMouse = false;
    }

    float32 xOffset = ( float32 ) input->mouseX - gameState->lastMouseX;
    float32 yOffset = gameState->lastMouseY - ( float32 ) input->mouseY;
    gameState->lastMouseX = ( float32 ) input->mouseX;
    gameState->lastMouseY = ( float32 ) input->mouseY;
    // CameraProcessMouse( &gameState->camera, xOffset, yOffset );

    Translate( player->transform, player->playerMovementDirectionVector * ( player->playerMovementSpeed * dt ) );

    Matrix4 view = GetViewMatrix( &gameState->camera );
    Matrix4 viewProjection = gameState->projectionMatrix * view;

    platform.RendererBeginFrame( viewProjection );
    for ( int i = 0; i < gameState->entityCount; ++i )
    {
        Entity *entity = &gameState->entities[ i ];
        switch ( entity->type )
        {
            case PLAYER:
            {
                platform.RendererDrawCube( entity->transform, Vector4( 0.5f, 0.0f, 0.7f, 1.0f ) );
                Transform directionTransform = {};
                directionTransform.position = entity->transform.position + entity->playerMovementDirectionVector;
                directionTransform.scale = Vector3( 0.2f );
                platform.RendererDrawCube( directionTransform, Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) );
            }
            break;

            case FOOD:
            {
                platform.RendererDrawCube( entity->transform, Vector4( 0.7f, 0.2f, 0.2f, 1.0f ) );
            }
            break;

            case PLAYER_SEGMENT:
            {
            }
            break;

            case FLOOR:
            {
                platform.RendererDrawCube( entity->transform, Vector4( 0.2f, 0.5f, 0.2f, 1.0f ) );
            }
            break;

            case WALL:
            {
                platform.RendererDrawCube( entity->transform, Vector4( 0.2f, 0.7f, 0.2f, 1.0f ) );
            }
            break;

            default:
            {
                Assert( !"Entity without type!" );
            }
        }
    }
    platform.RendererEndFrame();
}
