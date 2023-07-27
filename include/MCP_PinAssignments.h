#pragma once

/*
 * MCP Pin > CH Assignment
 * 
 * Some Explanation:
 * in the final code, there wont be a linear successsion of pins
 * We'll change pins in favor of easyer layout.

	Changes 22-07: to prevent valves from hanging,
	we refer to a routine in valvelogic.h to verify the written
	value. 

 */


// MCPwriteOutput(bool,uint16_t);
 
#define CH01  MCPwriteOutput(0,0b1000000000000000)
#define CH02  MCPwriteOutput(0,0b0000000000000001)
#define CH03  MCPwriteOutput(0,0b0100000000000000)
#define CH04  MCPwriteOutput(0,0b0000000000000010)
#define CH05  MCPwriteOutput(0,0b0010000000000000)
#define CH06  MCPwriteOutput(0,0b0000000000000100)
#define CH07  MCPwriteOutput(0,0b0001000000000000)
#define CH08  MCPwriteOutput(0,0b0000000000001000)
#define CH09  MCPwriteOutput(0,0b0000100000000000)
#define CH10  MCPwriteOutput(0,0b0000000000010000)
#define CH11  MCPwriteOutput(0,0b0000010000000000)
#define CH12  MCPwriteOutput(0,0b0000000000100000)
#define CH13  MCPwriteOutput(0,0b0000001000000000)
#define CH14  MCPwriteOutput(0,0b0000000001000000)
#define CH15  MCPwriteOutput(0,0b0000000100000000)
#define CH16  MCPwriteOutput(0,0b0000000010000000)

#define CH17  MCPwriteOutput(1,0b1000000000000000)
#define CH18  MCPwriteOutput(1,0b0000000000000001)
#define CH19  MCPwriteOutput(1,0b0100000000000000)
#define CH20  MCPwriteOutput(1,0b0000000000000010)
#define CH21  MCPwriteOutput(1,0b0010000000000000)
#define CH22  MCPwriteOutput(1,0b0000000000000100)
#define CH23  MCPwriteOutput(1,0b0001000000000000)
#define CH24  MCPwriteOutput(1,0b0000000000001000)
#define CH25  MCPwriteOutput(1,0b0000100000000000)
#define CH26  MCPwriteOutput(1,0b0000000000010000)
#define CH27  MCPwriteOutput(1,0b0000010000000000)
#define CH28  MCPwriteOutput(1,0b0000000000100000)
#define CH29  MCPwriteOutput(1,0b0000001000000000)
#define CH30  MCPwriteOutput(1,0b0000000001000000)
#define CH31  MCPwriteOutput(1,0b0000000100000000)
#define CH32  MCPwriteOutput(1,0b0000000010000000)

#define BYPASSVALVE  MCPwriteOutput(1,0b0000000010000000)

#include "valvelogic.h"
