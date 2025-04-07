#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 200

int main()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao iniciar Allegro.\n");
        return -1;
    }

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display)
    {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }

    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_FONT* font =
        al_load_font("SauceCodeProNerdFontMono-Regular.ttf", 20, 0);
    if (!font)
    {
        fprintf(stderr, "Fonte 'SauceCodeProNerdFontMono-Regular.ttf' não "
                        "encontrada. Usando fonte padrão.\n");
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

    bool running = true;
    float mouse_x = 0, mouse_y = 0;

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

        // Conversão para km
        float pos_km_x = (mouse_x / SCREEN_WIDTH) * 100.0f;
        float pos_km_y = ((SCREEN_HEIGHT - mouse_y) / SCREEN_HEIGHT) * 20.0f;

        al_clear_to_color(al_map_rgb(0, 0, 0));

        char buffer[128];
        snprintf(buffer, sizeof(buffer),
                 "Posição do mouse: X = %.2f km, Y = %.2f km", pos_km_x,
                 pos_km_y);
        al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, buffer);

        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
