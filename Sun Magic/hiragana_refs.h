#pragma once

#include "stdafx.h"
#include "create_map.h"

namespace sun_magic {
	namespace hiragana {

	enum id {
		A = 0, I, U, E, O,
		KA,	KI,	KU,	KE,	KO,
		SA,	SHI, SU, SE, SO,
		TA, CHI, TSU, TE, TO,
		NA,	NI,	NU,	NE,	NO,
		HA, HI, FU, HE, HO,
		MA,	MI,	MU,	ME,	MO,
		YA,	YU,	YO,
		RA,	RI,	RU,	RE,	RO,
		WA,	WO,	N,
		NUM_HIRAGANA
	};

	static std::map<id, sf::String> refs = tools::CreateMap<id, sf::String>
		(A, L"あ")	(I, L"い")	(U, L"う")	(E, L"え")	(O, L"お")
		(KA, L"か")	(KI, L"き")	(KU, L"く")	(KE, L"け")	(KO, L"こ")
		(SA, L"さ")	(SHI, L"し")	(SU, L"す")	(SE, L"せ")	(SO, L"そ")
		(TA, L"た")	(CHI, L"ち")	(TSU, L"つ")	(TE, L"て")	(TO, L"と")
		(NA, L"な")	(NI, L"に")	(NU, L"ぬ")	(NE, L"ね")	(NO, L"の")
		(HA, L"は")	(HI, L"ひ")	(FU, L"ふ")	(HE, L"へ")	(HO, L"ほ")
		(MA, L"ま")	(ME, L"み")	(MU, L"む")	(ME, L"め")	(MO, L"も")
		(YA, L"や")				(YU, L"ゆ")				(YO, L"よ")
		(RA, L"ら")	(RI, L"り")	(RU, L"る")	(RE, L"れ")	(RO, L"ろ")
		(WA, L"わ")										(WO, L"を")
		(N, L"ん");

	}
}