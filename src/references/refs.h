#pragma once

#include "stdafx.h"

namespace sun_magic {
	namespace refs {

		namespace tracer {
			static const std::string HIRAGANA_TRACE = "assets/models/hiragana_learn.txt";
			static const std::string HIRAGANA_OUTPUT_BASE = "strokes_";
		}

		namespace zinnia {
			static const std::string ZINNIA_MODEL = "assets/models/hiragana.model";
		}

		namespace fonts {
			static const std::string MSMINCHO = "assets/fonts/msmincho.ttc";
		}

		namespace textures {

			namespace backgrounds {
				static const std::string POSTER_CLOSE = "assets/backgrounds/bg_dummy1_1.jpg";
				static const std::string POSTER_AWAY =  "assets/backgrounds/bg_dummy1.jpg";
				static const std::string WINDOW = "assets/backgrounds/bg_dummy2.jpg";
				static const std::string DESK = "assets/backgrounds/bg_dummy3.jpg";
				static const std::string DOOR = "assets/backgrounds/bg_dummy4.jpg";

				static const std::string OFFICE = "assets/backgrounds/office.jpg";
			}

			namespace objects {
				static const std::string NEKO = "assets/objects/neko.png";
				static const std::string NEKO_HAPPY = "assets/objects/neko-happy.png";
				static const std::string STUPID_LOVE = "assets/objects/stupid-love.png";
				static const std::string SPRITES = "assets/objects/hiragana-spritesheet.png";
			}

		}

	}
}