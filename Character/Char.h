/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Look\CharLook.h"
#include "Look\PetLook.h"
#include "Gameplay\Maplemap\Mapobject.h"
#include "IO\Components\ChatBalloon.h"
#include "Graphics\EffectLayer.h"
#include "Util\rectangle2d.h"

namespace Character
{
	using IO::ChatBalloon;
	using Gameplay::MapObject;
	using Graphics::EffectLayer;

	// Base for characters, e.g. the player and other clients on the same map.
	class Char : public MapObject
	{
	public:
		// Player states which determine animation and state. 
		// Values are used in movement packets (add 1 if facing left).
		enum State : int8_t
		{
			WALK = 2,
			STAND = 4,
			FALL = 6,
			ALERT = 8,
			PRONE = 10,
			SWIM = 12,
			LADDER = 14,
			ROPE = 16,
			DIED = 18,
			SIT = 20
		};

		static State byvalue(int8_t value)
		{
			return static_cast<State>(value);
		}

		// Character effects from Effect.wz
		enum Effect
		{
			LEVELUP,
			JOBCHANGE,
			SCROLL_SUCCESS,
			SCROLL_FAILURE
		};

		// Draw look, nametag, effects and chat bubble.
		void draw(Point<int16_t> viewpos, float inter) const override;
		// Update look and movements.
		int8_t update(const Physics& physics) override;
		// Return the current map layer, or 7 if on a ladder or rope.
		int8_t getlayer() const override;

		// Display an animation as an effect ontop of the character.
		void showeffect(Animation toshow);
		// Display an animation as an effect ontop of the character.
		void showeffectbyid(Effect toshow);
		// Display a chat bubble with the specified line in it.
		void speak(string line);
		// Change a part of the character's look.
		void changelook(Maplestat::Value stat, int32_t id);

		// Add a pet with the specified stats.
		void addpet(uint8_t index, int32_t iid, string name, 
			int32_t uniqueid, Point<int16_t> pos, uint8_t stance, int32_t fhid);
		// Remove a pet with the specified index and reason.
		void removepet(uint8_t index, bool hunger);

		// Change the character's face expression by id.
		void sendface(int32_t expression);
		// Set if the character sprite is mirrored (true = facing left)
		virtual void setflip(bool flipped);
		// Change the character's state.
		virtual void setstate(State newstate);

		// Return if the character is facing left.
		bool getflip() const;
		// Return the name of this character.
		string getname() const;

		// Return if the char is in the Char::SIT state.
		bool issitting() const;
		// Return if the char is in the Char::LADDER or Char::ROPE state.
		bool isclimbing() const;
		// Return wether the character sprite uses stances for two-handed weapons.
		bool istwohanded() const;

		// Obtain a reference to this character's look.
		const CharLook& getlook() const;

		// Initialize character effects.
		static void init();

	protected:
		Char(int32_t oid, CharLook look, string name);

		bool update(const Physics& physics, float speed);

		CharLook look;
		ChatBalloon chatballoon;
		EffectLayer effects;
		Text namelabel;
		PetLook pets[3];

		State state;
		bool flip;

	private:
		static map<Effect, Animation> effectdata;
	};
}