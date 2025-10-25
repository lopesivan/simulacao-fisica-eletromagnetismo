#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <stdio.h>

#define X_KM_MAX 100.0f
#define Y_KM_MAX 100.0f

void armazenar_posicao_mouse(float  mouse_x,
                             float  mouse_y,
                             int    screen_width,
                             int    screen_height,
                             float* pos_km_x,
                             float* pos_km_y,
                             int    index);

void converter_km_para_pixel(float  km_x,
                             float  km_y,
                             int    screen_width,
                             int    screen_height,
                             float* px,
                             float* py);

float calcular_modulo_vetor(float x0, float y0, float x1, float y1);

int main()
{
    if(!al_init())
    {
        fprintf(stderr, "Falha ao iniciar Allegro.\n");
        return -1;
    }

    al_install_mouse();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(0, &disp_data);
    int screen_width  = disp_data.width;
    int screen_height = disp_data.height;

    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    ALLEGRO_DISPLAY* display =
        al_create_display(screen_width, screen_height);

    /* DEBUG: Tamanho da tela: 1920 x 1080 */
    printf("TRACE: Tamanho da tela: %d x %d\n",
           screen_width,
           screen_height);
    /* Tamanho da tela: 1920 x 1080 */

    if(!display)
    {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }

    ALLEGRO_FONT* font =
        al_load_font("SauceCodeProNerdFontMono-Regular.ttf", 20, 0);

    if(!font)
    {
        fprintf(stderr,
                "Fonte personalizada não encontrada. Usando fonte "
                "padrão.\n");
        font = al_create_builtin_font();
        if(!font)
        {
            fprintf(stderr, "Falha ao carregar fonte padrão.\n");
            return -1;
        }
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue,
                             al_get_mouse_event_source());
    al_register_event_source(event_queue,
                             al_get_keyboard_event_source());

    bool running = true;
    bool redraw  = true;

    float mouse_x = 0, mouse_y = 0;
    int   points_stored = 0;
    float pos_km_x[2] = {0}, pos_km_y[2] = {0};
    float px = 0, py = 0;

    ALLEGRO_EVENT ev;

    while(running)
    {
        al_wait_for_event(event_queue, &ev);

        switch(ev.type)
        {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
            mouse_x = ev.mouse.x;
            mouse_y = ev.mouse.y;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = false;
            }
            else if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                if(points_stored < 2)
                {
                    armazenar_posicao_mouse(mouse_x,
                                            mouse_y,
                                            screen_width,
                                            screen_height,
                                            pos_km_x,
                                            pos_km_y,
                                            points_stored);
                    /* Esses arrays são ponteiros
                     * automaticamente quando passados como
                     argumentos. Ou
                     * seja, isso aqui:
                     armazenar_posicao_mouse(mouse_x, mouse_y,
                     screen_width, screen_height, &pos_km_x[0],
                     &pos_km_y[0],
                     points_stored);
                     */

                    points_stored++;
                }
                else
                {
                    points_stored = 0;
                }
                redraw = true;
            }
            break;
        }

        if(redraw)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            char buffer[128];

            for(int i = 0; i < points_stored; i++)
            {
                converter_km_para_pixel(pos_km_x[i],
                                        pos_km_y[i],
                                        screen_width,
                                        screen_height,
                                        &px,
                                        &py);

                al_draw_filled_circle(
                    px, py, 10, al_map_rgb(255, 0, 0));

                snprintf(buffer,
                         sizeof(buffer),
                         "Posição %d: X = %.2f km, Y = %.2f km",
                         i + 1,
                         pos_km_x[i],
                         pos_km_y[i]);

                al_draw_text(font,
                             al_map_rgb(255, 255, 255),
                             20,
                             20 + i * 30,
                             0,
                             buffer);
            }

            // Desenhar linha conectando os dois pontos
            if(points_stored == 2)
            {
                float x1, y1, x2, y2;
                converter_km_para_pixel(pos_km_x[0],
                                        pos_km_y[0],
                                        screen_width,
                                        screen_height,
                                        &x1,
                                        &y1);
                converter_km_para_pixel(pos_km_x[1],
                                        pos_km_y[1],
                                        screen_width,
                                        screen_height,
                                        &x2,
                                        &y2);

                al_draw_line(x1,
                             y1,
                             x2,
                             y2,
                             al_map_rgb(0, 255, 0),
                             2.0f);  // linha verde

                int id = 2;

                snprintf(buffer,
                         sizeof(buffer),
                         "Delta   %d: dX = %.2f km, dY = %.2f km",
                         id + 1,
                         pos_km_x[1] - pos_km_x[0],
                         pos_km_y[1] - pos_km_y[0]);

                al_draw_text(font,
                             al_map_rgb(255, 255, 255),
                             20,
                             20 + id * 30,
                             0,
                             buffer);

                id = 3;
                snprintf(buffer,
                         sizeof(buffer),
                         "Modulo   %d: u = %.2f km",
                         id + 1,
                         calcular_modulo_vetor(pos_km_x[0],
                                               pos_km_y[0],
                                               pos_km_x[1],
                                               pos_km_y[1]));

                al_draw_text(font,
                             al_map_rgb(255, 255, 255),
                             20,
                             20 + id * 30,
                             0,
                             buffer);

                // zerando as posiçoes
                pos_km_x[0] = 0;
                pos_km_y[0] = 0;
                pos_km_x[1] = 0;
                pos_km_y[1] = 0;
            }

            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}

void armazenar_posicao_mouse(float  mouse_x,
                             float  mouse_y,
                             int    screen_width,
                             int    screen_height,
                             float* pos_km_x,
                             float* pos_km_y,
                             int    index)
{
    float km_x = (mouse_x / screen_width) * X_KM_MAX;
    float km_y = Y_KM_MAX - ((mouse_y / screen_height) * Y_KM_MAX);
    pos_km_x[index] = km_x;
    pos_km_y[index] = km_y;
}

void converter_km_para_pixel(float  km_x,
                             float  km_y,
                             int    screen_width,
                             int    screen_height,
                             float* px,
                             float* py)
{
    *px = (km_x / X_KM_MAX) * screen_width;
    *py = screen_height - (km_y / Y_KM_MAX) * screen_height;
}

float calcular_modulo_vetor(float x0, float y0, float x1, float y1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    return sqrtf(dx * dx + dy * dy);
}
