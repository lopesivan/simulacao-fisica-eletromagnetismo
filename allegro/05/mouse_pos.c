#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#define X_KM_MAX 1000.0f
#define Y_KM_MAX 20.0f

int main()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao iniciar Allegro.\n");
        return -1;
    }

    al_install_mouse();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon(); // Necessário para desenhar círculos

    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(0, &disp_data);
    int screen_width = disp_data.width;
    int screen_height = disp_data.height;

    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    ALLEGRO_DISPLAY* display = al_create_display(screen_width, screen_height);
    if (!display)
    {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }

    ALLEGRO_FONT* font =
        al_load_font("SauceCodeProNerdFontMono-Regular.ttf", 20, 0);
    if (!font)
    {
        fprintf(stderr,
                "Fonte personalizada não encontrada. Usando fonte padrão.\n");
        font = al_create_builtin_font();
        if (!font)
        {
            fprintf(stderr, "Falha ao carregar fonte padrão.\n");
            return -1;
        }
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    bool running = true;
    float mouse_x = 0, mouse_y = 0;

    int points_stored = 0;
    float pos_km_x[2] = {0}, pos_km_y[2] = {0};

    while (running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_x = ev.mouse.x;
            mouse_y = ev.mouse.y;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = false;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && points_stored < 2)
            {
                // Armazena posição atual do mouse em km
                float km_x = (mouse_x / screen_width) * X_KM_MAX;
                float km_y = Y_KM_MAX - ((mouse_y / screen_height) * Y_KM_MAX);

                pos_km_x[points_stored] = km_x;
                pos_km_y[points_stored] = km_y;
                points_stored++;
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        for (int i = 0; i < points_stored; i++)
        {
            // Converter km para pixel
            float px = (pos_km_x[i] / X_KM_MAX) * screen_width;
            float py = screen_height - (pos_km_y[i] / Y_KM_MAX) * screen_height;

            al_draw_filled_circle(px, py, 10, al_map_rgb(255, 0, 0)); // círculo vermelho

            char buffer[128];
            snprintf(buffer, sizeof(buffer),
                     "Posição %d: X = %.2f km, Y = %.2f km", i + 1, pos_km_x[i], pos_km_y[i]);
            al_draw_text(font, al_map_rgb(255, 255, 255), 20, 20 + i * 30, 0, buffer);
        }

        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}

