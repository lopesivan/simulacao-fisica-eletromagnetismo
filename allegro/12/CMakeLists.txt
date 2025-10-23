cmake_minimum_required(VERSION 3.10)
project(mouse_pos C)

# Define o padrão C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Adiciona flags de compilação
add_compile_options(-Wall)

# Encontra o pacote Allegro5
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO5 REQUIRED allegro-5)
pkg_check_modules(ALLEGRO5_FONT REQUIRED allegro_font-5)
pkg_check_modules(ALLEGRO5_TTF REQUIRED allegro_ttf-5)
pkg_check_modules(ALLEGRO5_PRIMITIVES REQUIRED allegro_primitives-5)
pkg_check_modules(ALLEGRO5_MAIN REQUIRED allegro_main-5)

# Adiciona os diretórios de include
include_directories(
  ${ALLEGRO5_INCLUDE_DIRS} ${ALLEGRO5_FONT_INCLUDE_DIRS}
  ${ALLEGRO5_TTF_INCLUDE_DIRS} ${ALLEGRO5_PRIMITIVES_INCLUDE_DIRS}
  ${ALLEGRO5_MAIN_INCLUDE_DIRS} /usr/include/x86_64-linux-gnu)

# Define o executável
add_executable(mouse_pos mouse_pos.c)

# Linka as bibliotecas
target_link_libraries(
  mouse_pos
  ${ALLEGRO5_LIBRARIES}
  ${ALLEGRO5_FONT_LIBRARIES}
  ${ALLEGRO5_TTF_LIBRARIES}
  ${ALLEGRO5_PRIMITIVES_LIBRARIES}
  ${ALLEGRO5_MAIN_LIBRARIES}
  m)
