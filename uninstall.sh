#!/bin/bash
# ASCII Image Converter - Uninstall Script
# 
# Copyright (c) 2025 danko12
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}============================================${NC}"
echo -e "${YELLOW}ASCII Image Converter - Uninstaller${NC}"
echo -e "${YELLOW}============================================${NC}"
echo ""

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo -e "${RED}Error: This script must be run as root (use sudo)${NC}"
    echo "Usage: sudo ./uninstall.sh"
    exit 1
fi

# Remove binary
INSTALL_DIR="/usr/local/bin"

if [ -f "${INSTALL_DIR}/ascii" ]; then
    echo "Removing ${INSTALL_DIR}/ascii..."
    rm -f "${INSTALL_DIR}/ascii"
    echo -e "${GREEN}✓ Removed successfully${NC}"
else
    echo -e "${YELLOW}Binary not found at ${INSTALL_DIR}/ascii${NC}"
    echo "Nothing to uninstall."
fi

echo ""
echo -e "${GREEN}Uninstallation complete!${NC}"
