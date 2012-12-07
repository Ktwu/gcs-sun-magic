#pragma once

#include "stdafx.h"

namespace sun_magic {
	namespace refs {

		namespace tracer {
			static const std::string HIRAGANA_TRACE = "assets/models/hiragana_learn.txt";
			static const std::string HIRAGANA_OUTPUT_BASE = "strokes_";
		}

		namespace zinnia {
			static const std::string ZINNIA_MODEL = "assets/models/custom_hiragana.txt";//"assets/models/hiragana.model";
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
				static const std::string AHIRU = "assets/objects/ahiru.png";
				static const std::string EBI = "assets/objects/ebi.png";
				static const std::string FOODBOWL = "assets/objects/foodbowl.png";
				static const std::string INU = "assets/objects/inu.png";
				static const std::string NEKO = "assets/objects/neko.png";
				static const std::string NEKO_HAPPY = "assets/objects/neko-happy.png";
				static const std::string OBAKE = "assets/objects/obake.png";
				static const std::string STUPID_LOVE = "assets/objects/stupid-love.png";
				static const std::string SUSHI = "assets/objects/sushi.png";
				static const std::string USAGI = "assets/objects/usagi.png";

				static const std::string SPRITES_MEH = "assets/objects/hiragana-meh.png";
				static const std::string SPRITES_HAPPY = "assets/objects/hiragana-happy.png";
				static const std::string SPRITES_ANGRY = "assets/objects/hiragana-angry.png";
			}

		}

	}
}