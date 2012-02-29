//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////


#pragma once
#ifndef SHARED_H
#define SHARED_H

//////////////////////////////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////////////////////////////
// Static const definition
#define SCDEFINE static const
// System
SCDEFINE float FRAMES_PER_SECOND = 29;
SCDEFINE int WINDOW_HEIGHT = 480;
SCDEFINE int WINDOW_WIDTH = 800;
SCDEFINE int WINDOW_POSITION_X = 0;
SCDEFINE int WINDOW_POSITION_Y = 0;

// Keys
SCDEFINE int KEY_BACKSPACE = 8;
SCDEFINE int KEY_ENTER = 13;
SCDEFINE int KEY_ESCAPE = 27;
SCDEFINE int KEY_SPACE = 32;
SCDEFINE int EXIT_VIA_MENU = 1;

// Color
#define R 0
#define G 1
#define B 2
#define A 3

enum 
{
    X, Y, Z, W
};

enum ROW
{
    READ,
    WRITE
};

#define SINGLETON_DECLARATION(x) static x* GetInstance(void);\
    static void DestroyInstance(void);
#define SINGLETON_DEFINITION(x) static x* m_instance; x* x::GetInstance(void){if(m_instance != NULL){return m_instance;}else{m_instance = new x();return m_instance;}} void x::DestroyInstance(void){if (m_instance != NULL) delete m_instance;}

//////////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////////
// bit sized integers
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
#ifdef _WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
#include <stdint.h>
typedef int64_t int64;
typedef uint64_t uint64;
#endif

// function pointers
typedef void (*vfptr)(void);
typedef uint32 (*intfptr)( void );
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
// simple vectors
typedef float vec_f;
typedef vec_f vec2_f[2];
typedef vec_f vec3_f[3];
typedef vec_f vec4_f[4];
SCDEFINE vec2_f VEC2_F_NULL = {0.0f, 0.0f};
SCDEFINE vec3_f VEC3_F_NULL = {0.0f, 0.0f, 0.0f};
SCDEFINE vec4_f VEC4_F_NULL = {0.0f, 0.0f, 0.0f, 0.0f};

typedef int32 vec_i;
typedef vec_i vec2_i[2];
typedef vec_i vec3_i[3];
typedef vec_i vec4_i[4];
SCDEFINE vec2_i VEC2_I_NULL = {0, 0};
SCDEFINE vec3_i VEC3_I_NULL = {0, 0, 0};
SCDEFINE vec4_i VEC4_I_NULL = {0, 0, 0, 0};

// colors
static vec4_f colorBlack  = {0, 0, 0, 1};
static vec4_f colorRed    = {1, 0, 0, 1};
static vec4_f colorGreen  = {0, 1, 0, 1};
static vec4_f colorBlue   = {0, 0, 1, 1};
static vec4_f colorYellow = {1, 1, 0, 1};
static vec4_f colorMagenta = {1, 0, 1, 1};
static vec4_f colorCyan   = {0, 1, 1, 1};
static vec4_f colorWhite  = {1, 1, 1, 1};
static vec4_f colorLtGrey = {0.75, 0.75, 0.75, 1};
static vec4_f colorMdGrey = {0.5, 0.5, 0.5, 1};
static vec4_f colorDkGrey = {0.25, 0.25, 0.25, 1};

static vec3_f Red = {1, 0, 0};
static vec3_f Green = {0, 1, 0};
static vec3_f Blue = {0, 0, 1};
static vec3_f Yellow = {1,1,0};
static vec3_f Cyan = {0, 1, 1};
static vec3_f Magenta = {1, 0, 1};
static float White = 1.0f;
static float Black = 0.0f;

// These grays are useful for fine-tuning lighting color values
// and for other areas where subtle variations of grays are needed.
// PERCENTAGE GRAYS:
static float Gray05 = White*0.05;
static float Gray10 = White*0.10;
static float Gray15 = White*0.15;
static float Gray20 = White*0.20;
static float Gray25 = White*0.25;
static float Gray30 = White*0.30;
static float Gray35 = White*0.35;
static float Gray40 = White*0.40;
static float Gray45 = White*0.45;
static float Gray50 = White*0.50;
static float Gray55 = White*0.55;
static float Gray60 = White*0.60;
static float Gray65 = White*0.65;
static float Gray70 = White*0.70;
static float Gray75 = White*0.75;
static float Gray80 = White*0.80;
static float Gray85 = White*0.85;
static float Gray90 = White*0.90;
static float Gray95 = White*0.95;

// OTHER GRAYS
static vec3_f DimGray = { 0.329412 , 0.329412 , 0.329412};
static vec3_f DimGrey = { 0.329412 , 0.329412 , 0.329412};
static vec3_f Gray = { 0.752941 , 0.752941 , 0.752941};
static vec3_f Grey = { 0.752941 , 0.752941 , 0.752941};
static vec3_f LightGray = { 0.658824 , 0.658824 , 0.658824};
static vec3_f LightGrey = { 0.658824 , 0.658824 , 0.658824};
static vec3_f VLightGray = { 0.80 , 0.80 , 0.80};
static vec3_f VLightGrey = { 0.80 , 0.80 , 0.80};

static vec3_f Aquamarine = { 0.439216 , 0.858824 , 0.576471};
static vec3_f Violet = { 0.62352 , 0.372549 , 0.623529};
static vec3_f Brown = { 0.647059 , 0.164706 , 0.164706};
static vec3_f Cadet = { 0.372549 , 0.623529 , 0.623529};
static vec3_f Coral = { 1.0 , 0.498039 , 0.0};
static vec3_f Cornflower = { 0.258824 , 0.258824 , 0.435294};
static vec3_f Dark = { 0.184314 , 0.309804 , 0.184314};
static vec3_f DarkOlive = { 0.309804 , 0.309804 , 0.184314};
static vec3_f DarkOrchid = { 0.6 , 0.196078 , 0.8};
static vec3_f DarkSlate = { 0.419608 , 0.137255 , 0.556863};
static vec3_f DarkSlateGray = { 0.184314 , 0.309804 , 0.309804};
static vec3_f DarkSlateGrey = { 0.184314 , 0.309804 , 0.309804};
static vec3_f DarkTurquoise = { 0.439216 , 0.576471 , 0.858824};
static vec3_f Firebrick = { 0.556863 , 0.137255 , 0.137255};
static vec3_f ForestGreen = { 0.137255 , 0.556863 , 0.137255};
static vec3_f Gold = { 0.8 , 0.498039 , 0.196078};
static vec3_f Goldenrod = { 0.858824 , 0.858824 , 0.439216};
static vec3_f GreenYellow = { 0.576471 , 0.858824 , 0.439216};
static vec3_f IndianRed = { 0.309804 , 0.184314 , 0.184314};
static vec3_f Khaki = { 0.623529 , 0.623529 , 0.372549};
static vec3_f LightBlue = { 0.74902 , 0.847059 , 0.847059};
static vec3_f LightSteelBlue = { 0.560784 , 0.560784 , 0.737255};
static vec3_f LimeGreen = { 0.196078 , 0.8 , 0.196078};
static vec3_f Maroon = { 0.556863 , 0.137255 , 0.419608};
static vec3_f MediumAquamarine = { 0.196078 , 0.8 , 0.6};
static vec3_f MediumBlue = { 0.196078 , 0.196078 , 0.8};
static vec3_f MediumForestGreen = { 0.419608 , 0.556863 , 0.137255};
static vec3_f MediumGoldenrod = { 0.917647 , 0.917647 , 0.678431};
static vec3_f MediumOrchid = { 0.576471 , 0.439216 , 0.858824};
static vec3_f MediumSeaGreen = { 0.258824 , 0.435294 , 0.258824};
static vec3_f MediumTurquoise = { 0.439216 , 0.858824 , 0.858824};
static vec3_f MediumVioletRed = { 0.858824 , 0.439216 , 0.576471};
static vec3_f Navy = { 0.137255 , 0.137255 , 0.556863};
static vec3_f Orange = { 1 , 0.5 , 0.0};
static vec3_f Orchid = { 0.858824 , 0.439216 , 0.858824};
static vec3_f Pale = { 0.560784 , 0.737255 , 0.560784};
static vec3_f Pink = { 0.737255 , 0.560784 , 0.560784};
static vec3_f Plum = { 0.917647 , 0.678431 , 0.917647};
static vec3_f Salmon = { 0.435294 , 0.258824 , 0.258824};
static vec3_f Sienna = { 0.556863 , 0.419608 , 0.137255};
static vec3_f SteelBlue = { 0.137255 , 0.419608 , 0.556863};
static vec3_f Tan = { 0.858824 , 0.576471 , 0.439216};
static vec3_f Thistle = { 0.847059 , 0.74902 , 0.847059};
static vec3_f Turquoise = { 0.678431 , 0.917647 , 0.917647};
static vec3_f VioletRed = { 0.8 , 0.196078 , 0.6};
static vec3_f Wheat = { 0.847059 , 0.847059 , 0.74902};
static vec3_f SummerSky = { 0.22 , 0.69 , 0.87};
static vec3_f Brass = { 0.71 , 0.65 , 0.26};
static vec3_f Copper = { 0.72 , 0.45 , 0.20};
static vec3_f Bronze = { 0.55 , 0.47 , 0.14};
static vec3_f Bronze2 = { 0.65 , 0.49 , 0.24};
static vec3_f Silver = { 0.90 , 0.91 , 0.98};
static vec3_f BrightGold = { 0.85 , 0.85 , 0.10};
static vec3_f OldGold = { 0.81 , 0.71 , 0.23};
static vec3_f Feldspar = { 0.82 , 0.57 , 0.46};
static vec3_f Quartz = { 0.85 , 0.85 , 0.95};
static vec3_f NeonPink = { 1.00 , 0.43 , 0.78};
static vec3_f DarkPurple = { 0.53 , 0.12 , 0.47};
static vec3_f CoolCopper = { 0.85 , 0.53 , 0.10};
static vec3_f MandarinOrange = { 0.89 , 0.47 , 0.20};
static vec3_f LightWood = { 0.91 , 0.76 , 0.65};
static vec3_f MediumWood = { 0.65 , 0.50 , 0.39};
static vec3_f DarkWood = { 0.52 , 0.37 , 0.26};
static vec3_f SpicyPink = { 1.00 , 0.11 , 0.68};
static vec3_f SemiSweetChoc = { 0.42 , 0.26 , 0.15};
static vec3_f BakersChoc = { 0.36 , 0.20 , 0.09};
static vec3_f Flesh = { 0.96 , 0.80 , 0.69};
static vec3_f NewTan = { 0.92 , 0.78 , 0.62};
static vec3_f NewMidnight = { 0.00 , 0.00 , 0.61};
static vec3_f VeryDarkBrown = { 0.35 , 0.16 , 0.14};
static vec3_f DarkBrown = { 0.36 , 0.25 , 0.20};
static vec3_f DarkTan = { 0.59 , 0.41 , 0.31};
static vec3_f GreenCopper = { 0.32 , 0.49 , 0.46};
static vec3_f DkGreenCopper = { 0.29 , 0.46 , 0.43};
static vec3_f DustyRose = { 0.52 , 0.39 , 0.39};
static vec3_f HuntersGreen = { 0.13 , 0.37 , 0.31};
static vec3_f Scarlet = { 0.55 , 0.09 , 0.09};

static vec3_f Med_Purple = { 0.73 , 0.16 , 0.96 };
static vec3_f Light_Purple = { 0.87 , 0.58 , 0.98 };
static vec3_f Very_Light_Purple = { 0.94 , 0.81 , 0.99 };

#pragma warning(pop)
static const void CloneColor(const vec4_f colorDefinition, vec4_f & color )
{
    color[R] = colorDefinition[R];
    color[G] = colorDefinition[G];
    color[B] = colorDefinition[B];
    color[A] = colorDefinition[A];
}

// Common STD containers
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// todo: swap out shitty vec2 system with oop system
//template<class T>
//struct vec2
//{
//    T x;
//    T y;
//    vec2() { x = 0; y = 0; }
//    vec2(const & vec2 vec) { x = vec.x; y = vec.y; }
//    static vec2 Up(void)      { vec2 vec; vec.x = 0; vec.y = 1; return vec; } const;
//    static vec2 Down(void)    { vec2<T> vec; vec.x = 0; vec.y =-1; return vec; } const;
//    static vec2 Left(void)    { vec2<T> vec; vec.x =-1; vec.y = 0; return vec; } const;
//    static vec2 Right(void)   { vec2<T> vec; vec.x = 1; vec.y = 0; return vec; } const;
//    vec2 operator+ (vec2 const& v1, vec2 const& v2) 
//    {
//        vec2 v;
//        return v;
//        
//    }
//};

struct Vertex
{
    float x, y, r, g, b, a;
};

struct Point
{
    float x, y;
};

#endif // SHARED_H