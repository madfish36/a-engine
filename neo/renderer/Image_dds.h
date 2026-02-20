/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.	If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.	If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/


#ifndef IMAGE_DDS_H
#define IMAGE_DDS_H
/*
=========================================================

DDS.h

=========================================================
*/

#pragma pack(push, 1)

constexpr unsigned long DDS_MAGIC = 0x20534444; // "DDS "

typedef struct {
    unsigned long size;
    unsigned long flags;
    unsigned long fourCC;
    unsigned long RGBBitCount;
    unsigned long RBitMask;
    unsigned long GBitMask;
    unsigned long BBitMask;
    unsigned long ABitMask;
} DDS_PIXELFORMAT;

//DDS_PIXELFORMAT: FLAGS
#define DDS_FOURCC			0x00000004 // DDPF_FOURCC
#define DDS_RGB				0x00000040 // DDPF_RGB
#define DDS_RGBA			0x00000041 // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE		0x00020000 // DDPF_LUMINANCE
#define DDS_LUMINANCEA		0x00020001 // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
#define DDS_ALPHAPIXELS		0x00000001 // DDPF_ALPHAPIXELS
#define DDS_ALPHA			0x00000002 // DDPF_ALPHA
#define DDS_PAL8			0x00000020 // DDPF_PALETTEINDEXED8
#define DDS_PAL8A			0x00000021 // DDPF_PALETTEINDEXED8 | DDPF_ALPHAPIXELS
#define DDS_BUMPLUMINANCE	0x00040000 // DDPF_BUMPLUMINANCE
#define DDS_BUMPDUDV		0x00080000 // DDPF_BUMPDUDV
#define DDS_BUMPDUDVA		0x00080001 // DDPF_BUMPDUDV | DDPF_ALPHAPIXELS

#ifndef DDS_MAKEFOURCC
#define DDS_MAKEFOURCC(ch0, ch1, ch2, ch3)				\
		((unsigned long)(unsigned char)(ch0)			\
		| ((unsigned long)(unsigned char)(ch1) << 8)	\
		| ((unsigned long)(unsigned char)(ch2) << 16)	\
		| ((unsigned long)(unsigned char)(ch3) << 24 ))
#endif /* DDS_MAKEFOURCC */

const DDS_PIXELFORMAT DDSPF_DXT1 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('D', 'X', 'T', '1'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_DXT2 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('D', 'X', 'T', '2'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_DXT3 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('D', 'X', 'T', '3'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_DXT4 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('D', 'X', 'T', '4'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_DXT5 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('D', 'X', 'T', '5'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_BC4_UNORM = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('B', 'C', '4', 'U'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_BC4_SNORM = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('B', 'C', '4', 'S'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_BC5_UNORM = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('B', 'C', '5', 'U'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_BC5_SNORM = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('B', 'C', '5', 'S'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_R8G8_B8G8 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('R', 'G', 'B', 'G'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_G8R8_G8B8 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('G', 'R', 'G', 'B'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_YUY2 = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('Y', 'U', 'Y', '2'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_UYVY = {
    sizeof(DDS_PIXELFORMAT), DDS_FOURCC, DDS_MAKEFOURCC('U', 'Y', 'V', 'Y'), 0, 0, 0, 0, 0
};
const DDS_PIXELFORMAT DDSPF_A8R8G8B8 = {
    sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
};
const DDS_PIXELFORMAT DDSPF_X8R8G8B8 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0};
const DDS_PIXELFORMAT DDSPF_A8B8G8R8 = {
    sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
};
const DDS_PIXELFORMAT DDSPF_X8B8G8R8 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0};
const DDS_PIXELFORMAT DDSPF_G16R16 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 112, 32, 0x0000ffff, 0xffff0000, 0, 0};
const DDS_PIXELFORMAT DDSPF_R5G6B5 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0xf800, 0x07e0, 0x001f, 0};
const DDS_PIXELFORMAT DDSPF_A1R5G5B5 = {sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x7c00, 0x03e0, 0x001f, 0x8000};
const DDS_PIXELFORMAT DDSPF_X1R5G5B5 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0x7c00, 0x03e0, 0x001f, 0};
const DDS_PIXELFORMAT DDSPF_A4R4G4B4 = {sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x0f00, 0x00f0, 0x000f, 0xf000};
const DDS_PIXELFORMAT DDSPF_X4R4G4B4 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0x0f00, 0x00f0, 0x000f, 0};
const DDS_PIXELFORMAT DDSPF_R8G8B8 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 24, 0xff0000, 0x00ff00, 0x0000ff, 0};
const DDS_PIXELFORMAT DDSPF_A8R3G3B2 = {sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00e0, 0x001c, 0x0003, 0xff00};
const DDS_PIXELFORMAT DDSPF_R3G3B2 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 8, 0xe0, 0x1c, 0x03, 0};
const DDS_PIXELFORMAT DDSPF_A4L4 = {sizeof(DDS_PIXELFORMAT), DDS_LUMINANCEA, 0, 8, 0x0f, 0, 0, 0xf0};
const DDS_PIXELFORMAT DDSPF_L8 = {sizeof(DDS_PIXELFORMAT), DDS_LUMINANCE, 0, 8, 0xff, 0, 0, 0};
const DDS_PIXELFORMAT DDSPF_L16 = {sizeof(DDS_PIXELFORMAT), DDS_LUMINANCE, 0, 16, 0xffff, 0, 0, 0};
const DDS_PIXELFORMAT DDSPF_A8L8 = {sizeof(DDS_PIXELFORMAT), DDS_LUMINANCEA, 0, 16, 0x00ff, 0, 0, 0xff00};
const DDS_PIXELFORMAT DDSPF_A8L8_ALT = {sizeof(DDS_PIXELFORMAT), DDS_LUMINANCEA, 0, 8, 0x00ff, 0, 0, 0xff00};
const DDS_PIXELFORMAT DDSPF_L8_NVTT1 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 8, 0xff, 0, 0, 0};
const DDS_PIXELFORMAT DDSPF_L16_NVTT1 = {sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0xffff, 0, 0, 0};
const DDS_PIXELFORMAT DDSPF_A8L8_NVTT1 = {sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00ff, 0, 0, 0xff00};
const DDS_PIXELFORMAT DDSPF_A8 = {sizeof(DDS_PIXELFORMAT), DDS_ALPHA, 0, 8, 0, 0, 0, 0xff};
const DDS_PIXELFORMAT DDSPF_V8U8 = {sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 16, 0x00ff, 0xff00, 0, 0};
const DDS_PIXELFORMAT DDSPF_Q8W8V8U8 = {
    sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
};
const DDS_PIXELFORMAT DDSPF_V16U16 = {sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 32, 0x0000ffff, 0xffff0000, 0, 0};

// D3DFMT_A2R10G10B10/D3DFMT_A2B10G10R10 should be written using DX10 extension to avoid D3DX 10:10:10:2 reversal issue
const DDS_PIXELFORMAT DDSPF_A2R10G10B10 = {
    sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000
};
const DDS_PIXELFORMAT DDSPF_A2B10G10R10 = {
    sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000
};

// The following legacy Direct3D 9 formats use 'mixed' signed & unsigned channels so requires special handling
const DDS_PIXELFORMAT DDSPF_A2W10V10U10 = {
    sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDVA, 0, 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000
};
const DDS_PIXELFORMAT DDSPF_L6V5U5 = {sizeof(DDS_PIXELFORMAT), DDS_BUMPLUMINANCE, 0, 16, 0x001f, 0x03e0, 0xfc00, 0};
const DDS_PIXELFORMAT DDSPF_X8L8V8U8 = {
    sizeof(DDS_PIXELFORMAT), DDS_BUMPLUMINANCE, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0
};


#define DDS_HEADER_FLAGS_TEXTURE				0x00001007	// DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP					0x00020000	// DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME					0x00800000	// DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH					0x00000008	// DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE				0x00080000	// DDSD_LINEARSIZE
#define DDS_HEIGHT								0x00000002	// DDSD_HEIGHT
#define DDSD_WIDTH								0x00000004	// DDSD_WIDTH
#define DDSCAPS_TEXTURE							0x00001000	// DDSCAPS_TEXTURE
#define DDSCAPS_CUBEMAP							0x00000008	// DDSCAPS_COMPLEX
#define DDSCAPS_MIPMAP							0x00400000	// DDSCAPS_MIPMAP
#define DDSCAPS_TEXTURE_MIPMAP					0x00400008	// DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDSCAPS2_CUBEMAP_POSITIVEX				0x00000600	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDSCAPS2_CUBEMAP_NEGATIVEX				0x00000a00	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDSCAPS2_CUBEMAP_POSITIVEY				0x00001200	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDSCAPS2_CUBEMAP_NEGATIVEY				0x00002200	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDSCAPS2_CUBEMAP_POSITIVEZ				0x00004200	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDSCAPS2_CUBEMAP_NEGATIVEZ				0x00008200	// DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ
#define DDSCAPS2_CUBEMAP						0x00000200	// DDSCAPS2_CUBEMAP
#define DDSCAPS2_VOLUME							0x00200000	// DDSCAPS2_VOLUME

#define DDSCAPS2_CUBEMAP_ALLFACES				(DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX |\
												DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY |\
												DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ )

typedef struct {
    unsigned long size;
    unsigned long flags;
    unsigned long height;
    unsigned long width;
    unsigned long pitchOrLinearSize;
    unsigned long depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
    unsigned long mipMapCount;
    unsigned long reserved1[11];
    DDS_PIXELFORMAT ddspf;
    unsigned long caps;
    unsigned long caps2;
    unsigned long caps3;
    unsigned long caps4;
    unsigned long reserved2;
} DDS_HEADER;

#pragma pack(pop)

static_assert(sizeof(DDS_PIXELFORMAT) == 32, "DDS pixel format size mismatch");
static_assert(sizeof(DDS_HEADER) == 124, "DDS Header size mismatch");
constexpr size_t DDS_MIN_HEADER_SIZE = sizeof(unsigned long) + sizeof(DDS_HEADER);

#endif //IMAGE_DDS_H
