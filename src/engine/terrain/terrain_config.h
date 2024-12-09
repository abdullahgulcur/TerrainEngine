#pragma once

// DYNAMIC (input as enum)
//#define TERRAIN_SIZE 4096 // max 8192
//#define CLIPMAP_LEVEL 5 // max can be 8
//#define BLOCK_SIZE 16 // min 8

// STATIC
#define BLOCK_COUNT_PER_LEVEL 27
#define BLOCK_COUNT_INNER 9

//#define BLOCK_COUNT_LEVEL_0 36
//#define BLOCK_COUNT (BLOCK_COUNT_PER_LEVEL * CLIPMAP_LEVEL)

// bunlar kaldirilacak
//#define PHYSICAL_TEXTURE_BLOCK_COUNT_PER_ROW 16
//#define PHYSICAL_TEXTURE_SIZE_HEIGHTMAP (16 * PHYSICAL_TEXTURE_BLOCK_COUNT_PER_ROW)

// IMPORTANT NOTES
// (1) Page table texture can hold 16x16=256 different sub textures.
// (2) Total rows or columns of physical pages texture should not exceed 16. Because of the texture channel limitations. (Block position in world and physical pages texture)
// (3) CLIPMAP_LEVEL * BLOCK_COUNT_PER_LEVEL + 9 = this results consequences is important for (2)
// (4) I used uvec2 for block indices, but this is not mandatory.

// Page Table Considerations (Alternative to above)
// CLIPMAP_LEVEL * BLOCK_COUNT_PER_LEVEL + 9 = a. It is about physical page position. Physical page texture can hold x sub textures. x has to be greater than a. (Edit: I calculated, do not worry, u cannot pass that :)
// TERRAIN_SIZE / BLOCK_SIZE is about block position. Because block positions are also written to page table. This is also important for data types that are used in code.
// For now, TERRAIN_SIZE / BLOCK_SIZE should not exceed 16.