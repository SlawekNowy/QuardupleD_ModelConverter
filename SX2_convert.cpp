// SX2_convert.cpp: definiuje punkt wejścia dla aplikacji.
//

#include <fstream>
#include <array>
#include "SX2_convert.h"
#include <filesystem>



#pragma comment(lib,"D3D9.lib")
//
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

using namespace std;
using Buffer = std::array<char, 256>;


static TCHAR szWindowClass[] = "win32app";


// The string that appears in the application's title bar.
static TCHAR szTitle[] = "DirectX9App";
using InputAdapter = bitsery::InputBufferAdapter<Buffer>;
int main(int argc, char** argv)
{
	if (argc > 1) {
		char* file = argv[1];
		QD_SX header;
		auto fileStr = std::ifstream(file,std::ios::binary);
		std::array<char, 256> headerBuffer = { 0 };
		fileStr.read(headerBuffer.data(), 256);
		bitsery::quickDeserialization<InputAdapter, QD_SX>({headerBuffer.begin(),headerBuffer.end()}, header);

		std::vector<char> vertexBuffer(header.nVertices * header.vertexSize);
		std::vector<char> indexBuffer(2 * header.nIndices);
		fileStr.read(vertexBuffer.data(), vertexBuffer.size());
		fileStr.read(indexBuffer.data(), indexBuffer.size());
		LPD3DXMESH mesh;
		WNDCLASSEX wcex;
		std::memset(&wcex, 0, sizeof(WNDCLASSEX));

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DefWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hIcon = LoadIcon(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				"Call to RegisterClassEx failed!",
				"Win32 Guided Tour",
				NULL);

			return 1;
		}
		HWND hWnd = CreateWindow(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			500, 100,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			NULL
		);
		if (!hWnd)
		{
			MessageBox(NULL,
				"Call to CreateWindow failed!",
				"Win32 Guided Tour",
				NULL);

			return 1;
		}
		IDirect3D9* g_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		LPDIRECT3DDEVICE9 d3dDevice = NULL;

		D3DPRESENT_PARAMETERS d3dpp;
		LPDIRECT3DVERTEXBUFFER9 vertexBufInter;
		LPDIRECT3DINDEXBUFFER9 indexBufInter;

		std::memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;

		g_d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &d3dDevice);

		D3DXCreateMeshFVF(header.nIndices / 3, header.nVertices, 0, header.dwFWF,d3dDevice , &mesh);
#ifndef NDEBUG
		D3DVERTEXELEMENT9 fvfStruct[MAX_FVF_DECL_SIZE] = { 0 };
		D3DXDeclaratorFromFVF(header.dwFWF, fvfStruct);
		mesh->GetVertexBuffer(&vertexBufInter);
		mesh->GetIndexBuffer(&indexBufInter);
		D3DVERTEXBUFFER_DESC vertexBufferDesc;
		D3DINDEXBUFFER_DESC indexBufferDesc;
		vertexBufInter->GetDesc(&vertexBufferDesc);
		indexBufInter->GetDesc(&indexBufferDesc);
#endif
		char* vertexBuf, *indexBuf;

		vertexBufInter->Lock(0, 0, (void**)&vertexBuf, 0);
		std::memcpy(vertexBuf, vertexBuffer.data(), header.vertexSize * header.nVertices);
		vertexBufInter->Unlock();
		indexBufInter->Lock(0, 0, (void**)&indexBuf, 0);
		std::memcpy(indexBuf, indexBuffer.data(), 2 * header.nIndices);
		indexBufInter->Unlock();
		/*
		int textureNum = (D3DFVF_TEXCOUNT_MASK & header.dwFWF) >> D3DFVF_TEXCOUNT_SHIFT;
		D3DXMATERIAL* materials = new D3DXMATERIAL[textureNum]{ 0 };
		
		for (int i = 0; i < textureNum; i++) {
			materials[i].MatD3D.Power = 10;
			materials[i].MatD3D.Diffuse = materials[i].MatD3D.Specular = materials[i].MatD3D.Specular = { 1,1,1,1 };
			materials[i].MatD3D.Emissive = { 0,0,0,1 };
			materials[i].pTextureFilename = const_cast<char*>("");
		}
		*/
		std::filesystem::path pathObj(file);
			std::string pathOut;
			std::filesystem::create_directory(pathObj.parent_path().string() + "/orig");

			pathOut = pathObj.parent_path().string() + "\\" +"orig" + "\\" + pathObj.stem().string() + ".x";
		D3DXSaveMeshToX(pathOut.data(), mesh, NULL, NULL, NULL, 0, D3DXF_FILEFORMAT_TEXT);


		vertexBufInter->Release();
		indexBufInter->Release();
		mesh->Release();
		d3dDevice->Release();
		g_d3d9->Release();




	}
	else
	{
		std::cout << "Usage SX2_convert.exe [absolute path to SX file]";
		}
}

