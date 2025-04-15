#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct
{
    int frames;
    int animationDelay;
    int y;
} animationProperties;

typedef struct
{
    animationProperties idle;
    animationProperties walk;
    animationProperties attack_horizontal;
    animationProperties attack_top;
    animationProperties attack_down;

    animationProperties death;
} PlayerAnimationData;

typedef struct
{
    animationProperties idle;

    animationProperties death;
} EntityAnimationData;


#endif
