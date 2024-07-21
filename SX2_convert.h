// SX2_convert.h: plik dołączany dla standardowych systemowych plików dołączanych,
// lub pliki dołączane specyficzne dla projektu.

#pragma once

#include <iostream>
#include <cstdint>

#include <d3d9.h>
#include <D3DX9Mesh.h>
#include <windows.h>
#include <bitsery/traits/core/traits.h>

#include <bitsery/traits/array.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/bitsery.h>



struct QD_SX {
	char magic[16];
	std::uint16_t nVertices;
	std::uint16_t nIndices;
	std::uint32_t dwFWF;
	std::uint32_t vertexSize;
	char DXVersion[4];
};

template <typename S>
void serialize(S& s, QD_SX& o) {
	s.container1b(o.magic);
	s.value2b(o.nVertices);
	s.value2b(o.nIndices);
	if (std::strncmp(o.magic,"Simplified_X00  ",16)==0) {
		o.dwFWF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
		o.vertexSize = 40;
		o.DXVersion[0] = '0';
		o.DXVersion[1] = '8';
		o.DXVersion[2] = '0';
		o.DXVersion[3] = '0';
	}
	else {
		s.value4b(o.dwFWF);
		s.value4b(o.vertexSize);
		s.container1b(o.DXVersion);
	}
}

// TODO: W tym miejscu przywołaj dodatkowe nagłówki wymagane przez program.
