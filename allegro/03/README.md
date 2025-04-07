# posição do mouse

Para fazer isso funcionar corretamente com Allegro,
onde a **origem padrão** é o **canto superior esquerdo
(0,0)**, a gente precisa **inverter o eixo Y** para que o
**canto inferior esquerdo vire a origem (0,0)**, como num
gráfico cartesiano tradicional.

---

## Objetivo:
- **Canto inferior esquerdo:** (0 km, 0 km)
- **Canto inferior direito:** (1000 km, 0 km)
- **Canto superior esquerdo:** (0 km, 20 km)

---

## Etapas:

1. **Obter resolução da tela**
2. **Converter as coordenadas do mouse para coordenadas "reais" em km**
3. **Inverter o Y (pois Allegro conta do topo para baixo)**

## Resultado:
Agora o programa:
- Mostra a posição do mouse em km,
- Usa **(0, 0)** no **canto inferior esquerdo da tela**,
- E o **eixo Y cresce para cima** (como em gráficos matemáticos).
