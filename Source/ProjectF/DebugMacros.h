#pragma once
#include "DrawDebugHelpers.h"
#define DRAW_SPHERE(Location)if(GetWorld())DrawDebugSphere(GetWorld(),Location,20.f,24,FColor::Cyan,true)
#define DRAW_SPHERE_COLOR(Location,Color)(DrawDebugSphere(GetWorld(),Location,10.f,24,Color,false,5.f))
#define DRAW_SPHERE_SingleFrame(Location)if(GetWorld())DrawDebugSphere(GetWorld(),Location,20.f,24,FColor::Cyan,false,-1.f)
#define DRAW_LINE(StartLocation,EndLocation)if(GetWorld())DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Blue,true,-1.f,0,1.f)
#define DRAW_LINE_SingleFrame(StartLocation,EndLocation)if(GetWorld())DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Blue,false,-1.f,0,1.f)
#define DRAW_POINT(Arrow)if(GetWorld())DrawDebugPoint(GetWorld(),Arrow,25.f,FColor::Red,true)
#define DRAW_POINT_SingleFrame(Arrow)if(GetWorld())DrawDebugPoint(GetWorld(),Arrow,25.f,FColor::Red,false,1.f)
#define DRAW_VECTOR(StartLocation,EndLocation)if(GetWorld())\
{\
DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Blue,true,-1.f,0,1.f);\
DrawDebugPoint(GetWorld(),EndLocation,25.f,FColor::Red,true);\
}
#define DRAW_VECTOR_SingleFrame(StartLocation,EndLocation)if(GetWorld())\
{\
DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Blue,false,-1.f,0,1.f);\
DrawDebugPoint(GetWorld(),EndLocation,25.f,FColor::Red,false,-1.f);\
}