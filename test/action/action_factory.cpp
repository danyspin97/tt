/*
 * Copyright (c) 2020 Danilo Spinella <oss@danyspin97.org>.
 *
 * This file is part of tt
 * (see https://github.com/danyspin97/tt).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tt/action/action_factory.hpp"

#include "catch2/catch.hpp"

#include "nngpp/buffer.h" // for buffer

#include "tt/action/notify_up_action.hpp" // for NotifyUpAction
#include "tt/action/pack_action.hpp"      // for PackAction

TEST_CASE("ActionFactory") {
    SECTION("NotifyUpAction") {
        tt::NotifyUpAction action("dummy", true);
        auto buffer = tt::PackAction(action);

        auto deserialized_action =
            tt::ActionFactory::GetActionFromBuffer(buffer);

        REQUIRE(deserialized_action);
        CHECK(deserialized_action->name() == "notify_up");
        CHECK_NOTHROW(
            dynamic_cast<tt::NotifyUpAction *>(deserialized_action.get()));
    }
}
