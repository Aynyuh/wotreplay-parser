#include "wotreplay_c.h"

#include "parser.h"
#include "game.h"
#include "packet.h"

#include <fstream>

using namespace wotreplay;

void* wotreplay_game_parse(const char* filename) {
	std::ifstream in(filename, std::ios::binary);

	parser_t parser(load_data_mode_t::on_demand);
	game_t* game = new game_t;

	parser.set_debug(false);
	parser.parse(in, *game);

	return (void*) game;
}

game_packet wotreplay_game_get_packet(void* game, int index) {
	const auto &packet = ((game_t*) game)->get_packets().at(index);

	if (packet.has_property(property_t::position)) {
		return game_packet{
			packet.type(),
			packet.clock(),
			packet.player_id(),
			{ 
				std::get<0>(packet.position()),
				std::get<1>(packet.position()),
				std::get<2>(packet.position()),
			},
		};
	}

	return game_packet{
		0,
		0,
		0,
		{0.f, 0.f, 0.f}
	};
}

int wotreplay_game_get_packet_count(void* game) {
	return ((game_t*) game)->get_packets().size();
}

void wotreplay_game_free(void* game) {
	delete (game_t*) game;
}
