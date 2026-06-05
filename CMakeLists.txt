cmake_minimum_required(VERSION 3.20)
project(PGK3d)

set(CMAKE_CXX_STANDARD 17)

# ==============================================================================
# 1. AUTOMATYCZNE POBIERANIE BIBLIOTEKI GLM (Header-only)
# ==============================================================================
include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# ==============================================================================
# 2. KONFIGURACJA ŚCIEŻEK FREEGLUT (Twoja obecna konfiguracja)
# ==============================================================================
set(FREEGLUT_ROOT "C:/Users/kubam/CLionProjects/freeglut-3.8.0")
set(FREEGLUT_BUILD "${FREEGLUT_ROOT}/cmake-build-debug")

include_directories("${FREEGLUT_ROOT}/include")
link_directories("${FREEGLUT_BUILD}/lib")

# ==============================================================================
# 3. DEFINICJA PLIKÓW PROJEKTU
# ==============================================================================
add_executable(PGK3d main.cpp
        GeometryProvider.cpp
        GeometryProvider.h
        Engine.cpp
        Engine.h
        Camera.cpp
        Camera.h
        Player.cpp
        Player.h
        stb_image.h
        trawa.png
        gracz_lewo.png
        gracz_prawo.png
        gracz_przod.png
        gracz_tyl.png
        BitmapHandler.cpp
        BitmapHandler.h)

# ==============================================================================
# 4. LINKOWANIE BIBLIOTEK (Dodano target glm::glm)
# ==============================================================================
target_link_libraries(PGK3d freeglut opengl32 glu32 glm::glm)

# ==============================================================================
# 5. KOPIOWANIE PLIKU DLL PO ZBUDOWANIU PROJEKTU
# ==============================================================================
add_custom_command(TARGET PGK3d POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FREEGLUT_BUILD}/bin/libfreeglut.dll"
        "$<TARGET_FILE_DIR:PGK3d>/libfreeglut.dll")