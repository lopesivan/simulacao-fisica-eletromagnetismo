# Posição do Mouse

Você está **absolutamente no caminho certo**, Ivan! Vamos
dissecar essa conversão com calma — como se fosse mesmo
um gráfico cartesiano com escala.

---

## 🎯 Objetivo do código

Você quer converter **coordenadas em pixel** (ex: `mouse_x`,
`mouse_y`) que vêm da tela do computador para **coordenadas
em quilômetros**, num espaço ideal de:

- **Eixo X**: de `0 km` a `X_KM_MAX` (1000 km)
- **Eixo Y**: de `0 km` a `Y_KM_MAX` (20 km), **com 0 no rodapé e 20 no topo**

---

## 📐 A matemática por trás

### 🧮 1. Conversão para `km_x` (eixo horizontal)

```c
km_x = (mouse_x / screen_width) * X_KM_MAX;
```

Aqui temos:

- `mouse_x / screen_width` → calcula a **razão da posição do mouse na horizontal**
    - Ex: se `mouse_x == 500` e `screen_width == 1000` → razão = `0.5`
- Multiplicamos essa razão pelo tamanho total em km:
    - `0.5 * 1000 km = 500 km`

✅ Resultado: convertemos da **posição em pixel para posição real em km**.

---

### 🧮 2. Conversão para `km_y` (eixo vertical invertido)

```c
km_y = Y_KM_MAX - ((mouse_y / screen_height) * Y_KM_MAX);
```

Aqui é parecido, mas com **inversão do eixo Y**, porque:

- No Allegro (como em quase todos os sistemas gráficos),
o **Y aumenta de cima pra baixo**:
    - `(0, 0)` é o **canto superior esquerdo**
    - `(0, screen_height)` é o **canto inferior esquerdo**
- Já em coordenadas cartesianas (como em gráficos
matemáticos), o **Y aumenta de baixo pra cima**

### Etapas:

1. **`mouse_y / screen_height`** → pega a fração de altura do mouse
2. **Multiplica por `Y_KM_MAX`** → transforma em km
3. **Subtrai de `Y_KM_MAX`** → inverte o eixo

---

### 🔍 Exemplo prático:

Se sua tela tem `screen_height = 1080`, e o mouse está no
pixel `mouse_y = 810`, e `Y_KM_MAX = 20 km`:

```c
(mouse_y / screen_height) = 810 / 1080 = 0.75
km_y = 20 - (0.75 * 20) = 20 - 15 = 5 km
```

✅ Significa que o mouse está **5 km acima da base da tela**.

---

## ✅ Resumo visual

| Posição (px)        | Razão       | Valor (km)         |
|---------------------|-------------|--------------------|
| `mouse_x = 0`       | `0`         | `0 km`             |
| `mouse_x = width/2` | `0.5`       | `500 km`           |
| `mouse_x = width`   | `1.0`       | `1000 km`          |

| `mouse_y = 0`       | `0`         | `20 km` (topo)     |
| `mouse_y = height/2`| `0.5`       | `10 km`            |
| `mouse_y = height`  | `1.0`       | `0 km` (rodapé)    |

---

