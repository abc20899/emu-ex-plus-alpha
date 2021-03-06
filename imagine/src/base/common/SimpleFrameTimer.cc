/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#define LOGTAG "FrameTimer"
#include <imagine/base/Screen.hh>
#include <imagine/time/Time.hh>
#include <imagine/logger/logger.h>
#include "SimpleFrameTimer.hh"

namespace Base
{

FrameTimer::~FrameTimer() {}

bool SimpleFrameTimer::init(EventLoop loop)
{
	eventLoop = loop;
	return true;
}

void SimpleFrameTimer::deinit()
{
	timer.deinit();
}

void SimpleFrameTimer::scheduleVSync()
{
	cancelled = false;
	if(requested)
	{
		return;
	}
	requested = true;
	if(timer)
	{
		return; // timer already armed
	}
	constexpr double NSEC_PER_SEC = 1000000000.;
	timer.callbackAfterNSec(
		[this]()
		{
			requested = false;
			if(cancelled)
			{
				cancelled = false;
				timer.cancel();
				return; // frame request was cancelled
			}
			uint64_t timestamp = IG::Time::now().nSecs();
			auto s = Screen::screen(0);
			if(s->isPosted())
			{
				s->frameUpdate(timestamp);
				s->prevFrameTimestamp = timestamp;
			}
			if(!requested)
			{
				cancel();
			}
		}, 1, std::round(Screen::screen(0)->frameTime() * NSEC_PER_SEC), eventLoop, Timer::HINT_REUSE);
}

void SimpleFrameTimer::cancel()
{
	cancelled = true;
}

}
