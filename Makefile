# Top Level Makefile (MousePaw Media Build System)
# Version: 2.1.0
# Tailored For: Goldilocks
#
# Author(s): Jason C. McDonald

# LICENSE (BSD-3-Clause)
# Copyright (c) 2018 MousePaw Media.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
# CONTRIBUTING
# See https://www.mousepawmedia.com/developers for information
# on how to contribute to our projects.

MK_DIR = @cmake -E make_directory
CH_DIR = @cmake -E chdir
CP = @cmake -E copy
CP_DIR = @cmake -E copy_directory
RM = @cmake -E remove
RM_DIR = @cmake -E remove_directory
ECHO = @cmake -E echo
LN = @cmake -E create_symlink

none: help

help:
	$(ECHO) "=== Goldilocks 1.3 ==="
	$(ECHO) "Select a build target:"
	$(ECHO) "  make ready         Build Goldilocks and bundles it for distribution."
	$(ECHO) "  make clean         Clean up Goldilocks and Tester."
	$(ECHO) "  make cleandebug    Clean up Goldilocks and Tester Debug."
	$(ECHO) "  make cleanrelease  Clean up Goldilocks and Tester Release."
	$(ECHO) "  make docs          Generate HTML docs."
	$(ECHO) "  make docs_pdf      Generate PDF docs."
	$(ECHO) "  make goldilocks        Build Goldilocks as release."
	$(ECHO) "  make goldilocks_debug  Build Goldilocks as debug."
	$(ECHO) "  make tester        Build Goldilocks Tester (+Goldilocks) as release."
	$(ECHO) "  make tester_debug  Build Goldilocks Tester (+Goldilocks) as debug."
	$(ECHO) "  make all           Build everything."
	$(ECHO) "  make allfresh      Clean and rebuild everything."
	$(ECHO)
	$(ECHO) "Clang Sanitizers (requires Debug build and Clang.)"
	$(ECHO) "  SAN=address     Use AddressSanitizer"
	$(ECHO) "  SAN=leak        Use LeakSanitizer w/o AddressSanitizer (Linux only)"
	$(ECHO) "  SAN=memory      Use MemorySanitizer"
	$(ECHO) "  SAN=thread      Use ThreadSanitizer"
	$(ECHO) "  SAN=undefined   Use UndefiniedBehaviorSanitizer"
	$(ECHO)
	$(ECHO) "Optional Architecture"
	$(ECHO) "  ARCH=32         Make x86 build (-m32)"
	$(ECHO) "  ARCH=64         Make x64 build (-m64)"
	$(ECHO)
	$(ECHO) "Use Configuration File"
	$(ECHO) "  CONFIG=foo      Uses the configuration file 'foo.config'"
	$(ECHO) "                  in the root of this repository."
	$(ECHO) "  When unspecified, default.config will be used."
	$(ECHO)
	$(ECHO) "For other build options, see the 'make' command in 'docs/', 'goldilocks-source/', and 'goldilocks-tester/'."

clean:
	$(MAKE) clean -C goldilocks-source
	$(MAKE) clean -C goldilocks-tester
	$(RM) tester_debug
	$(RM) tester

cleanall: clean
	$(MAKE) clean -C docs

cleandebug:
	$(MAKE) cleandebug -C goldilocks-source
	$(MAKE) cleandebug -C goldilocks-tester
	$(RM) tester_debug

cleanrelease:
	$(MAKE) cleanrelease -C goldilocks-source
	$(MAKE) cleanrelease -C goldilocks-tester
	$(RM) tester

docs:
	$(RM_DIR) docs/build/html
	$(MAKE) html -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/html/index.html'."
	$(ECHO) "-------------"

docs_pdf:
	$(MAKE) latexpdf -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/latex/Goldilocks.pdf'."
	$(ECHO) "-------------"

goldilocks:
	$(MAKE) release -C goldilocks-source
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "Goldilocks is in 'goldilocks-source/lib/Release'."
	$(ECHO) "-------------"

goldilocks_debug:
	$(MAKE) debug -C goldilocks-source
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO)  on "Goldilocks is in 'goldilocks-source/lib/Debug'."
	$(ECHO) "-------------"

minimal:
	$(RM_DIR) goldilocks
	$(ECHO) "Creating file structure..."
	$(MK_DIR) goldilocks/include/goldilocks
	$(ECHO) "Copying Goldilocks Minimal..."
	$(CP) goldilocks-source/include/goldilocks/core_types.hpp goldilocks/include/goldilocks/core_types.hpp
	$(ECHO) "Copying README and LICENSE..."
	$(CP) README.md goldilocks/README.md
	$(CP) LICENSE.md goldilocks/LICENSE.md
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "The header-only library is in 'goldilocks'."
	$(ECHO) "-------------"

ready: goldilocks
	$(RM_DIR) goldilocks
	$(ECHO) "Creating file structure..."
	$(MK_DIR) goldilocks
	$(ECHO) "Copying Goldilocks..."
	$(CP_DIR) goldilocks-source/include/ goldilocks/include/
	$(CP_DIR) goldilocks-source/lib/Release/ goldilocks/lib/
	$(ECHO) "Copying README and LICENSE..."
	$(CP) README.md goldilocks/README.md
	$(CP) LICENSE.md goldilocks/LICENSE.md
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "The libraries are in 'goldilocks'."
	$(ECHO) "-------------"

tester: goldilocks
	$(MAKE) release -C goldilocks-tester
	$(RM) tester
	$(LN) goldilocks-tester/bin/Release/goldilocks-tester tester
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "Goldilocks Tester is in 'goldilocks-tester/bin/Release'."
	$(ECHO) "The link './tester' has been created for convenience."
	$(ECHO) "-------------"


tester_debug: goldilocks_debug
	$(MAKE) debug -C goldilocks-tester
	$(RM) tester_debug
	$(LN) goldilocks-tester/bin/Debug/goldilocks-tester tester_debug
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "Goldilocks Tester is in 'goldilocks-tester/bin/Debug'."
	$(ECHO) "The link './tester_debug' has been created for convenience."
	$(ECHO) "-------------"

all: docs tester

allfresh: cleanall all

.PHONY: all allfresh clean cleanall cleandebug cleanrelease docs docs_pdf goldilocks goldilocks_debug ready tester tester_debug
