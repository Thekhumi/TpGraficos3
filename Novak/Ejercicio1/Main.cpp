#ifndef _DEBUG
#define ALLEGRO_STATICLINK
#endif // !_DEBUG
#define PLAYER_FILE "player.png"
#define ENEMY_FILE "enemy.png"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
/*constantes*/
const float FPS = 60;		//tasa de fps
const int SCREEN_W = 640;	//ancho de pantalla
const int SCREEN_H = 480;	//largo de pantalla
const int PLAYER_SIZE = 32;	//tamaño de imagen de  jugador
const int ENEMY_SIZE = 32;	// tamaño de imagen de enemigo
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *display = NULL;					// pantalla
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;			//cola de eventos
	ALLEGRO_TIMER *timer = NULL;						//timer de juego
	ALLEGRO_BITMAP *player = NULL;						//bitmap de player
	ALLEGRO_BITMAP *enemy = NULL;						// bitmap de enemigo
	float player_x = SCREEN_W / 2.0 - PLAYER_SIZE / 2.0;//posicion en x de player
	float player_y = SCREEN_H / 2.0 - PLAYER_SIZE / 2.0;//posicion en y de player
	float enemy_x = 0;									//posicion en x de enemigo
	float enemy_y = 0;									//posicion en y de enemigo
	bool redraw = true;
	bool doexit = false;
	bool key[4] = { false, false, false, false };		//arreglo de teclas
	/*inicializo allegro*/
	if (!al_init()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	/*inicializo addon para cargar imagenes*/
	if (!al_init_image_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	/*inicializo el timer del juego*/
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize timer!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	/*inicializo la ventana*/
	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_timer(timer);
		return -1;
	}
	/*inicializo teclado*/
	if (!al_install_keyboard()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize keyboard!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	/*cargo la imagen del player*/
	player = al_load_bitmap(PLAYER_FILE);
	if (!player) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load bouncer!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	/*cargo la imagen del enemigo*/
	enemy = al_load_bitmap(ENEMY_FILE);
	if (!enemy) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load bouncer!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_clear_to_color(al_map_rgb(255, 0, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	/*inicializo cola de eventos*/
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize event queue!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_bitmap(player);
		al_destroy_bitmap(enemy);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display)); //inicializo eventos de ventana

	al_register_event_source(event_queue, al_get_timer_event_source(timer)); //inicializo eventos de timer

	al_register_event_source(event_queue, al_get_keyboard_event_source()); //inicializo eventos de teclado

	al_clear_to_color(al_map_rgb(0, 0, 0)); //pongo pantalla en negro

	al_flip_display();

	al_start_timer(timer); //inicio el timer
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (key[KEY_UP] && player_y >= 4.0) {
				player_y -= 4.0;
			}

			if (key[KEY_DOWN] && player_y <= SCREEN_H - PLAYER_SIZE - 4.0) {
				player_y += 4.0;
			}

			if (key[KEY_LEFT] && player_x >= 4.0) {
				player_x -= 4.0;
			}

			if (key[KEY_RIGHT] && player_x <= SCREEN_W - PLAYER_SIZE - 4.0) {
				player_x += 4.0;
			}

			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_bitmap(player, player_x, player_y, 0);

			al_draw_bitmap(enemy, enemy_x, enemy_y, 0);

			al_flip_display();
		}
	}

	al_destroy_bitmap(player);
	al_destroy_bitmap(enemy);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}