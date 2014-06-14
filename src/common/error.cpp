/* CoktelADL2VGM - Tool to convert Coktel Vision's AdLib music to VGM
 *
 * CoktelADL2VGM is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * CoktelADL2VGM is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with CoktelADL2VGM. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file common/error.cpp
 *  Basic exceptions to throw.
 */

#include <cstdarg>
#include <cstdio>

#include "common/error.hpp"
#include "common/util.hpp"

namespace Common {

StackException::StackException() {
}

StackException::StackException(const char *s, ...) {
	char buf[STRINGBUFLEN];
	va_list va;

	va_start(va, s);
	vsnprintf(buf, STRINGBUFLEN, s, va);
	va_end(va);

	_stack.push(buf);
}

StackException::StackException(const StackException &e) : _stack(e._stack) {
}

StackException::StackException(const std::exception &e) {
	add(e);
}

StackException::~StackException() throw() {
}

void StackException::add(const char *s, ...) {
	char buf[STRINGBUFLEN];
	va_list va;

	va_start(va, s);
	vsnprintf(buf, STRINGBUFLEN, s, va);
	va_end(va);

	_stack.push(buf);
}

void StackException::add(const std::exception &e) {
	_stack.push(e.what());
}

const char *StackException::what() const throw() {
	if (_stack.empty())
		return "";

	return _stack.top().c_str();
}

StackException::Stack &StackException::getStack() {
	return _stack;
}


const Exception kOpenError("Can't open file");
const Exception kReadError("Read error");
const Exception kSeekError("Seek error");
const Exception kWriteError("Write error");


void printException(Exception &e, const std::string &prefix) {
	Exception::Stack &stack = e.getStack();

	try {
		if (stack.empty()) {
			status("FATAL ERROR");
			return;
		}

		status("%s%s", prefix.c_str(), stack.top().c_str());

		stack.pop();

		while (!stack.empty()) {
			status("    Because: %s", stack.top().c_str());
			stack.pop();
		}
	} catch (...) {
		status("FATAL ERROR: Exception while printing exception stack");
	}
}

} // End of namespace Common
