
# Projeto com Allegro 5 – Uso de `PKG_CONFIG_PATH` no Makefile e no CMake

## Diretório `10` – Makefile

Neste diretório, utilizamos um **Makefile tradicional** para
compilar o programa `mouse_pos.c` com suporte à biblioteca
**Allegro 5**.

Para garantir que o `pkg-config` encontrasse os arquivos
`.pc` corretamente, foi necessário **definir o caminho das
bibliotecas manualmente** usando a variável `PKG_CONFIG_PATH`:

```make
PKG_CONFIG_PATH := /usr/lib/x86_64-linux-gnu/pkgconfig
export PKG_CONFIG_PATH
```

Esse caminho é necessário em algumas distribuições Linux
(como o Ubuntu) onde as bibliotecas de desenvolvimento ficam
nesse diretório.

As bibliotecas utilizadas foram:

- `allegro-5`
- `allegro_font-5`
- `allegro_ttf-5`
- `allegro_main-5`
- `allegro_primitives-5`

E foram passadas ao compilador com auxílio do `pkg-config`:

```make
ALLEGRO_CFLAGS = $(shell /usr/bin/pkg-config --cflags $(ALLEGRO_LIBS))
ALLEGRO_LDLIBS = $(shell /usr/bin/pkg-config --libs $(ALLEGRO_LIBS))
```

---

## Mudanças no `CMakeLists.txt`

Para obter o mesmo comportamento no CMake, foram realizadas
**duas adaptações principais**:

### 1. Verificação das bibliotecas com mensagem amigável

Antes de compilar, o `CMakeLists.txt` verifica se todas as
bibliotecas Allegro estão disponíveis. Se alguma estiver
ausente, o processo é abortado com uma sugestão clara
de instalação:

```cmake
if(NOT _exit_code EQUAL 0)
    message(FATAL_ERROR
        "\n[ERRO] A biblioteca '${lib}' não foi encontrada.\n"
        "Você pode instalá-la com:\n\n"
        "  sudo apt install liballegro5-dev\n"
    )
endif()
```

### 2. Forçar `PKG_CONFIG_PATH` dentro do CMake (opcional)

Para garantir que o `pkg-config` use o diretório correto
mesmo quando não exportado no ambiente, foi inserido o
seguinte trecho:

```cmake
if(NOT DEFINED ENV{PKG_CONFIG_PATH})
    set(ENV{PKG_CONFIG_PATH} "/usr/lib/x86_64-linux-gnu/pkgconfig")
endif()
```

> Isso evita problemas caso o CMake seja invocado por uma IDE
ou outro ambiente que não propague variáveis de ambiente
do terminal.

---

## Considerações finais

Ambas as abordagens (Makefile e CMake) foram ajustadas para
garantir a correta localização das bibliotecas do **Allegro
5**, mantendo a portabilidade e fornecendo mensagens claras
ao usuário sobre dependências ausentes.
