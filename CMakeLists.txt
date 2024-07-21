# CMakeList.txt: projekt CMake dla elementu SX2_convert, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.8)

# Włącz przeładowywanie kodu XAML na gorąco dla kompilatorów MSVC, jeśli jest to obsługiwane.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SX2_convert")

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (SX2_convert "SX2_convert.cpp" "SX2_convert.h")



find_package(dxsdk-d3dx CONFIG REQUIRED)
target_link_libraries(SX2_convert PRIVATE Microsoft::D3DX9 Microsoft::D3DX10 Microsoft::D3DX11)
target_include_directories(SX2_convert PRIVATE Microsoft::D3DX9 Microsoft::D3DX10 Microsoft::D3DX11)


target_include_directories(SX2_convert PRIVATE Bitsery::bitsery)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SX2_convert PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
