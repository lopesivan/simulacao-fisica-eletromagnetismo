// main.cpp — C++17

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
#include <memory>  // <-- FALTAVA

// -------------------- Constantes / Config --------------------
namespace cfg
{
constexpr float X_KM_MAX = 100.0f;
constexpr float Y_KM_MAX = 100.0f;
}  // namespace cfg

// -------------------- RAII para recursos Allegro
// --------------------
struct AllegroDeleter
{
    void operator()(ALLEGRO_DISPLAY* p) const noexcept
    {
        if(p)
            al_destroy_display(p);
    }
    void operator()(ALLEGRO_EVENT_QUEUE* p) const noexcept
    {
        if(p)
            al_destroy_event_queue(p);
    }
    void operator()(ALLEGRO_FONT* p) const noexcept
    {
        if(p)
            al_destroy_font(p);
    }
};

using DisplayUPtr =
    std::unique_ptr<ALLEGRO_DISPLAY, AllegroDeleter>;
using EventQueueUPtr =
    std::unique_ptr<ALLEGRO_EVENT_QUEUE, AllegroDeleter>;
using FontUPtr = std::unique_ptr<ALLEGRO_FONT, AllegroDeleter>;

DisplayUPtr display{al_create_display(screen_width, screen_height)};
if(!display)
{
    std::cerr << "Falha ao criar display.\n";
    return EXIT_FAILURE;
}
// -------------------- Utilidades de domínio --------------------
namespace util
{

// Representa uma posição em quilômetros no plano
struct KmPos
{
    float x{};
    float y{};
};

// Converte pixel -> km e km -> pixel
inline KmPos mouse_pixel_to_km(float mouse_x,
                               float mouse_y,
                               int   screen_w,
                               int   screen_h)
{
    const float km_x =
        (mouse_x / static_cast<float>(screen_w)) * cfg::X_KM_MAX;
    const float km_y =
        cfg::Y_KM_MAX -
        ((mouse_y / static_cast<float>(screen_h)) * cfg::Y_KM_MAX);
    return {km_x, km_y};
}

inline std::pair<float, float>
km_to_pixel(float km_x, float km_y, int screen_w, int screen_h)
{
    const float px =
        (km_x / cfg::X_KM_MAX) * static_cast<float>(screen_w);
    const float py =
        static_cast<float>(screen_h) -
        ((km_y / cfg::Y_KM_MAX) * static_cast<float>(screen_h));
    return {px, py};
}

inline float vector_mod(float x0, float y0, float x1, float y1)
{
    const float dx = x1 - x0;
    const float dy = y1 - y0;
    return std::sqrt(dx * dx + dy * dy);
}

}  // namespace util

// -------------------- Aplicação --------------------
int main()
{
    // Inicialização Allegro
    if(!al_init())
    {
        std::cerr << "Falha ao iniciar Allegro.\n";
        return EXIT_FAILURE;
    }
    if(!al_install_mouse())
    {
        std::cerr << "Falha ao instalar mouse.\n";
    }
    if(!al_install_keyboard())
    {
        std::cerr << "Falha ao instalar teclado.\n";
    }
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    // Modo de vídeo
    ALLEGRO_DISPLAY_MODE screen_data{};
    al_get_display_mode(0, &screen_data);
    const int screen_width  = screen_data.width;
    const int screen_height = screen_data.height;

    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    DisplayUPtr display{
        al_create_display(screen_width, screen_height)};
    if(!display)
    {
        std::cerr << "Falha ao criar display.\n";
        return EXIT_FAILURE;
    }

    std::cout << "TRACE: Tamanho da tela: " << screen_width << " x "
              << screen_height << '\n';

    // Fonte (tenta personalizada; cai para built-in)
    FontUPtr font{al_load_font(
        "SauceCodeProNerdFontMono-Regular.ttf", 20, 0)};
    if(!font)
    {
        std::cerr << "Fonte personalizada não encontrada. Usando "
                     "fonte padrão.\n";
        font.reset(al_create_builtin_font());
        if(!font)
        {
            std::cerr << "Falha ao carregar fonte padrão.\n";
            return EXIT_FAILURE;
        }
    }

    // Fila de eventos
    EventQueueUPtr event_queue{al_create_event_queue()};
    if(!event_queue)
    {
        std::cerr << "Falha ao criar fila de eventos.\n";
        return EXIT_FAILURE;
    }
    al_register_event_source(
        event_queue.get(),
        al_get_display_event_source(display.get()));
    al_register_event_source(event_queue.get(),
                             al_get_mouse_event_source());
    al_register_event_source(event_queue.get(),
                             al_get_keyboard_event_source());

    // Paleta de cores (constantes)
    const ALLEGRO_COLOR COLOR_GREEN  = al_map_rgb(0, 255, 0);
    const ALLEGRO_COLOR COLOR_RED    = al_map_rgb(255, 0, 0);
    const ALLEGRO_COLOR COLOR_YELLOW = al_map_rgb(255, 255, 2);
    const ALLEGRO_COLOR COLOR_CYAN   = al_map_rgb(2, 255, 255);
    const ALLEGRO_COLOR COLOR_WHITE  = al_map_rgb(255, 255, 255);
    const ALLEGRO_COLOR COLOR_BLACK  = al_map_rgb(0, 0, 0);

    // Estado
    float mouse_x = 0.0f, mouse_y = 0.0f;
    int   points_stored = 0;

    std::array<float, 2> pos_km_x{0.0f, 0.0f};
    std::array<float, 2> pos_km_y{0.0f, 0.0f};

    bool running = true;
    bool redraw  = true;

    ALLEGRO_EVENT ev{};

    while(running)
    {
        al_wait_for_event(event_queue.get(), &ev);

        switch(ev.type)
        {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
            mouse_x = static_cast<float>(ev.mouse.x);
            mouse_y = static_cast<float>(ev.mouse.y);
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
                    const auto km =
                        util::mouse_pixel_to_km(mouse_x,
                                                mouse_y,
                                                screen_width,
                                                screen_height);
                    pos_km_x[static_cast<std::size_t>(
                        points_stored)] = km.x;
                    pos_km_y[static_cast<std::size_t>(
                        points_stored)] = km.y;
                    ++points_stored;
                }
                else
                {
                    // reseta ao apertar espaço novamente
                    points_stored = 0;
                    pos_km_x      = {0.0f, 0.0f};
                    pos_km_y      = {0.0f, 0.0f};
                }
                redraw = true;
            }
            break;

        default:
            break;
        }

        if(redraw)
        {
            al_clear_to_color(COLOR_BLACK);

            // desenha pontos armazenados
            for(int i = 0; i < points_stored; ++i)
            {
                auto [px, py] = util::km_to_pixel(
                    pos_km_x[static_cast<std::size_t>(i)],
                    pos_km_y[static_cast<std::size_t>(i)],
                    screen_width,
                    screen_height);

                al_draw_filled_circle(px, py, 10.0f, COLOR_RED);

                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2)
                    << "Posição " << (i + 1) << ": X = "
                    << pos_km_x[static_cast<std::size_t>(i)]
                    << " km, "
                    << "Y = "
                    << pos_km_y[static_cast<std::size_t>(i)]
                    << " km";

                al_draw_text(font.get(),
                             COLOR_CYAN,
                             20.0f,
                             20.0f + i * 30.0f,
                             0,
                             oss.str().c_str());
            }

            // linha e métricas quando há dois pontos
            if(points_stored == 2)
            {
                auto [x1, y1] = util::km_to_pixel(pos_km_x[0],
                                                  pos_km_y[0],
                                                  screen_width,
                                                  screen_height);
                auto [x2, y2] = util::km_to_pixel(pos_km_x[1],
                                                  pos_km_y[1],
                                                  screen_width,
                                                  screen_height);

                al_draw_line(x1, y1, x2, y2, COLOR_GREEN, 2.0f);

                {
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(2)
                        << "Delta   3: dX = "
                        << (pos_km_x[1] - pos_km_x[0])
                        << " km, dY = "
                        << (pos_km_y[1] - pos_km_y[0]) << " km";
                    al_draw_text(font.get(),
                                 COLOR_YELLOW,
                                 20.0f,
                                 20.0f + 2 * 30.0f,
                                 0,
                                 oss.str().c_str());
                }
                {
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(2)
                        << "Modulo  4: u = "
                        << util::vector_mod(pos_km_x[0],
                                            pos_km_y[0],
                                            pos_km_x[1],
                                            pos_km_y[1])
                        << " km";
                    al_draw_text(font.get(),
                                 COLOR_WHITE,
                                 20.0f,
                                 20.0f + 3 * 30.0f,
                                 0,
                                 oss.str().c_str());
                }
            }

            al_flip_display();
            redraw = false;
        }
    }

    // Recursos são destruídos automaticamente pelos unique_ptrs
    return EXIT_SUCCESS;
}
