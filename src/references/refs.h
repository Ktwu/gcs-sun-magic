#pragma once

#include "stdafx.h"
#define ref_t static const std::string

namespace sun_magic {
	namespace refs {

		namespace tracer {
			ref_t HIRAGANA_TRACE = "assets/models/hiragana_learn.txt";
			ref_t HIRAGANA_OUTPUT_BASE = "strokes_";
		}

		namespace zinnia {
			ref_t ZINNIA_MODEL = "assets/models/custom_hiragana.txt";
		}

		namespace fonts {
			/* Both Hiragana and English, although the Hiragana are flamboyant */
			ref_t MSMINCHO = "assets/fonts/msmincho.ttc";

			/* Hiragana only, but simple enough for new learners */
			ref_t KAORI = "assets/fonts/KaoriGel.ttf";
		}
	
		namespace sounds {
			ref_t SAMPLE = "assets/sounds/a.au";
		}

		namespace textures {

			namespace backgrounds {
				ref_t TITLE = "assets/backgrounds/title.jpg";
				ref_t POSTER_CLOSE = "assets/backgrounds/bg_dummy1_1.jpg";
				ref_t POSTER_AWAY =  "assets/backgrounds/bg_dummy1.jpg";
				ref_t WINDOW = "assets/backgrounds/bg_dummy2.jpg";
				ref_t DESK = "assets/backgrounds/bg_dummy3.jpg";
				ref_t DOOR = "assets/backgrounds/bg_dummy4.jpg";

				ref_t OFFICE = "assets/backgrounds/office.jpg";
			}

			namespace objects {
				ref_t AHIRU = "assets/objects/ahiru.png";
				ref_t EBI = "assets/objects/ebi.png";
				ref_t FOODBOWL = "assets/objects/foodbowl.png";
				ref_t INU = "assets/objects/inu.png";
				ref_t NEKO = "assets/objects/neko.png";
				ref_t NEKO_HAPPY = "assets/objects/neko-happy.png";
				ref_t OBAKE = "assets/objects/obake.png";
				ref_t STUPID_LOVE = "assets/objects/stupid-love.png";
				ref_t SUSHI = "assets/objects/sushi.png";
				ref_t USAGI = "assets/objects/usagi.png";

				ref_t SPRITES_MEH = "assets/objects/hiragana-meh.png";
				ref_t SPRITES_HAPPY = "assets/objects/hiragana-happy.png";
				ref_t SPRITES_ANGRY = "assets/objects/hiragana-angry.png";
				ref_t SPRITES_OUTLINE = "assets/objects/hiragana-outline.png";
			}

			namespace ui {
				ref_t BUTTON = "assets/ui/button.png";
				ref_t BUTTON_HOVER = "assets/ui/button_hover.png";
				ref_t BUTTON_PRESS = "assets/ui/button_pressed.png";
				ref_t NOTE_PAD = "assets/ui/notepad.png";
				ref_t NOTE_PAD2 = "assets/ui/notepad2.png";
			}

		}

	}
}