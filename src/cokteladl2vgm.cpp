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

/** @file cokteladl2vgm.cpp
 *  The project's main entry point.
 */

#include <cstdio>
#include <cstring>

#include <vector>
#include <string>

#include "common/util.hpp"
#include "common/version.hpp"
#include "common/error.hpp"
#include "common/file.hpp"

#include "adlplayer.hpp"

struct Job;

void printUsage(const char *name);
void printVersion();

Job parseCommandLine(int argc, char **argv);

void convertADL(const std::string &adlFile);
void convertMDY(const std::string &mdyFile, const std::string &tbrFile);


/** Type for all operations this tool can do. */
enum Operation {
	kOperationInvalid = 0, ///< Invalid command line.
	kOperationHelp       , ///< Show the help text.
	kOperationVersion    , ///< Show version information.
	kOperationADL        , ///< Convert an ADL file.
	kOperationMDY          ///< Convert a MDY+TBR file.
};

/** Full description of the job this tool will be doing. */
struct Job {
	Operation operation; ///< The operation to perform.
	std::vector<std::string> files; ///< The files to manipulate.

	Job() : operation(kOperationInvalid) {
	}
};


int main(int argc, char **argv) {
	// Find out what we're supposed to do
	Job job = parseCommandLine(argc, argv);

	try {
		// Handle the job
		switch (job.operation) {
			case kOperationHelp:
				printUsage(argv[0]);
				break;

			case kOperationVersion:
				printVersion();
				break;

			case kOperationADL:
				convertADL(job.files[0]);
				break;

			case kOperationMDY:
				convertMDY(job.files[0], job.files[1]);
				break;

			case kOperationInvalid:
			default:
				printUsage(argv[0]);
				return -1;
		}
	} catch (Common::Exception &e) {
		Common::printException(e);
		return -2;
	} catch (std::exception &e) {
		Common::Exception se(e);

		Common::printException(se);
		return -2;
	}

	return 0;
}

/** Print usage/help text. */
void printUsage(const char *name) {
	std::printf("%s - Tool to convert Coktel Vision's AdLib music to VGM\n", ADL2VGM_NAME);
	std::printf("Usage: %s [options] <file.adl>\n", name);
	std::printf("       %s [options] <file.mdy> <file.tbr>\n\n", name);
	std::printf("  -h      --help              Display this text and exit.\n");
	std::printf("  -v      --version           Display version information and exit.\n");
}

/** Print the tool's version. */
void printVersion() {
	std::printf("%s\n", ADL2VGM_NAMEVERSION);
}

Job parseCommandLine(int argc, char **argv) {
	Job job;

	// Go through all arguments
	for (int i = 1; i < argc; i++) {
		// Find --help and --version
		if        (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			job.operation = kOperationHelp;
			break;
		} else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			job.operation = kOperationVersion;
			break;
		}

		// Everything else is assumed to be a filename
		job.files.push_back(argv[i]);
	}

	// Already found an operation => return it
	if (job.operation != kOperationInvalid)
		return job;

	// One file is assumed to be an ADL, two files MDY+TBR. Everything else is invalid
	if      (job.files.size() == 1)
		job.operation = kOperationADL;
	else if (job.files.size() == 2)
		job.operation = kOperationMDY;
	else
		job.operation = kOperationInvalid;

	return job;
}

/** Convert an ADL file into VGM. */
void convertADL(const std::string &adlFile) {
	status("Converting ADL \"%s\" to VGM...", adlFile.c_str());

	// Open the input file
	Common::File adl(adlFile);
	ADLPlayer adlPlayer(adl);

	status("Loaded...");
	adlPlayer.convert();
}

/** Convert a MDY+TBR file into VGM. */
void convertMDY(const std::string &mdyFile, const std::string &tbrFile) {
	status("Converting MDY \"%s\" with TBR \"%s\" to VGM...", mdyFile.c_str(), tbrFile.c_str());

	// Open the input files
	Common::File mdy(mdyFile);
	Common::File tbr(tbrFile);
}
