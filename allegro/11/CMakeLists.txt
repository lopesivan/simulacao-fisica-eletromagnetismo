# PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig cmake -B build
cmake_minimum_required(VERSION 3.10)
project(mouse_pos C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Lista de bibliotecas Allegro
set(ALLEGRO_COMPONENTS allegro-5 allegro_font-5 allegro_ttf-5 allegro_main-5
                       allegro_primitives-5)

# Checa cada uma com pkg-config
foreach(lib ${ALLEGRO_COMPONENTS})
  execute_process(COMMAND pkg-config --exists ${lib} RESULT_VARIABLE _exit_code)
  if(NOT _exit_code EQUAL 0)
    message(
      FATAL_ERROR
        "\n[ERRO] A biblioteca '${lib}' não foi encontrada.\n"
        "Você pode instalar tudo com:\n\n"
        "  sudo apt install liballegro5-dev\n")
  endif()
endforeach()

# Pega flags do pkg-config
execute_process(
  COMMAND /usr/bin/pkg-config --cflags ${ALLEGRO_COMPONENTS}
  OUTPUT_VARIABLE ALLEGRO_CFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
  COMMAND /usr/bin/pkg-config --libs ${ALLEGRO_COMPONENTS}
  OUTPUT_VARIABLE ALLEGRO_LDLIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Aplica as flags
add_executable(mouse_pos mouse_pos.c)
target_compile_options(mouse_pos PRIVATE ${ALLEGRO_CFLAGS})
target_link_libraries(mouse_pos PRIVATE ${ALLEGRO_LDLIBS} m)
