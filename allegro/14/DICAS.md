Você tem várias opções para definir cores reutilizáveis:

## **Opção 1: Constantes globais (Recomendado)**

```c
// No topo do arquivo, após os #define
static const ALLEGRO_COLOR COR_VERDE    = {0, 1, 0, 1};  // RGB normalizado
static const ALLEGRO_COLOR COR_VERMELHO = {1, 0, 0, 1};
static const ALLEGRO_COLOR COR_AMARELO  = {1, 1, 0, 1};
static const ALLEGRO_COLOR COR_CIANO    = {0, 1, 1, 1};
static const ALLEGRO_COLOR COR_BRANCO   = {1, 1, 1, 1};
```

**Uso:**
```c
al_draw_line(x1, y1, x2, y2, COR_VERDE, 2.0f);
```

---

## **Opção 2: Macros (Mais flexível)**

```c
// No topo do arquivo
#define COR_VERDE    al_map_rgb(0, 255, 0)
#define COR_VERMELHO al_map_rgb(255, 0, 0)
#define COR_AMARELO  al_map_rgb(255, 255, 2)
#define COR_CIANO    al_map_rgb(2, 255, 255)
#define COR_BRANCO   al_map_rgb(255, 255, 255)
```

**Uso:**
```c
al_draw_line(x1, y1, x2, y2, COR_VERDE, 2.0f);
```

---

## **Opção 3: Variáveis locais no main()**

```c
int main()
{
    // ... inicializações ...
    
    // Definir paleta de cores
    ALLEGRO_COLOR cor_verde    = al_map_rgb(0, 255, 0);
    ALLEGRO_COLOR cor_vermelho = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR cor_amarelo  = al_map_rgb(255, 255, 2);
    ALLEGRO_COLOR cor_ciano    = al_map_rgb(2, 255, 255);
    ALLEGRO_COLOR cor_branco   = al_map_rgb(255, 255, 255);
    
    // ... resto do código ...
}
```

---

## **Recomendação**

Use **macros** (Opção 2) - é o padrão em código Allegro e evita chamadas repetidas de `al_map_rgb()` em cada frame:

```c
#define X_KM_MAX 100.0f
#define Y_KM_MAX 100.0f

// Paleta de cores
#define COR_VERDE    al_map_rgb(0, 255, 0)
#define COR_VERMELHO al_map_rgb(255, 0, 0)
#define COR_AMARELO  al_map_rgb(255, 255, 2)
#define COR_CIANO    al_map_rgb(2, 255, 255)
#define COR_BRANCO   al_map_rgb(255, 255, 255)
#define COR_PRETO    al_map_rgb(0, 0, 0)
```

Assim você pode usar em qualquer lugar do código de forma limpa e eficiente!
