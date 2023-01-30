#pragma once

#include <windows.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#ifdef _ENGINE
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define DISABLE_COPY(Class) Class(const Class &) = delete; Class &operator=(const Class &) = delete;
#define DISABLE_MOVE(Class) Class(Class &&) = delete; Class &operator=(Class &&) = delete;

#define MOVE(a) a; a = {}; 

#define RELEASE(res) if(res){res->Release(); res = NULL;}


#define BACKBUFFER_COUNT 3
