#!/bin/bash
# ASCII Image Converter - System-Wide Installation Script
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
#
# Supports: Linux (Debian/Ubuntu, Fedora, Arch), macOS
# Usage: sudo ./install.sh

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}ASCII Image Converter v2.1.0 - Installer${NC}"
echo -e "${BLUE}Copyright (c) 2025 danko12${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo -e "${RED}Error: This script must be run as root (use sudo)${NC}"
    echo "Usage: sudo ./install.sh"
    exit 1
fi

# Detect OS
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if [ -f /etc/debian_version ]; then
            OS="debian"
            echo -e "${GREEN}Detected: Debian/Ubuntu${NC}"
        elif [ -f /etc/redhat-release ]; then
            OS="fedora"
            echo -e "${GREEN}Detected: Fedora/RHEL/CentOS${NC}"
        elif [ -f /etc/arch-release ]; then
            OS="arch"
            echo -e "${GREEN}Detected: Arch Linux${NC}"
        else
            OS="linux"
            echo -e "${YELLOW}Detected: Generic Linux${NC}"
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        echo -e "${GREEN}Detected: macOS${NC}"
    else
        echo -e "${RED}Error: Unsupported operating system: $OSTYPE${NC}"
        exit 1
    fi
}

# Install dependencies
install_dependencies() {
    echo ""
    echo -e "${BLUE}Installing dependencies...${NC}"
    
    case $OS in
        debian)
            apt-get update
            apt-get install -y build-essential gcc make
            ;;
        fedora)
            dnf install -y gcc make
            ;;
        arch)
            pacman -Sy --noconfirm base-devel gcc make
            ;;
        macos)
            if ! command -v brew &> /dev/null; then
                echo -e "${YELLOW}Homebrew not found. Install it from https://brew.sh${NC}"
                echo "Or continue with Xcode Command Line Tools..."
            fi
            # Check for Xcode Command Line Tools
            if ! command -v gcc &> /dev/null; then
                echo "Installing Xcode Command Line Tools..."
                xcode-select --install
            fi
            ;;
        *)
            echo -e "${YELLOW}Please ensure gcc and make are installed${NC}"
            ;;
    esac
    
    echo -e "${GREEN}✓ Dependencies installed${NC}"
}

# Build the project
build_project() {
    echo ""
    echo -e "${BLUE}Building ASCII Image Converter...${NC}"
    
    # Clean previous builds
    make clean 2>/dev/null || true
    
    # Build optimized release version
    make release
    
    if [ ! -f "./ascii" ]; then
        echo -e "${RED}Error: Build failed. Executable not found.${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✓ Build successful${NC}"
}

# Install system-wide
install_system_wide() {
    echo ""
    echo -e "${BLUE}Installing system-wide...${NC}"
    
    # Installation directories
    INSTALL_DIR="/usr/local/bin"
    
    # Install binary
    echo "Installing binary to ${INSTALL_DIR}/ascii"
    install -m 755 ascii "${INSTALL_DIR}/ascii"
    
    # Verify installation
    if [ -f "${INSTALL_DIR}/ascii" ]; then
        echo -e "${GREEN}✓ Installed to ${INSTALL_DIR}/ascii${NC}"
    else
        echo -e "${RED}Error: Installation failed${NC}"
        exit 1
    fi
}

# Main installation process
main() {
    detect_os
    install_dependencies
    build_project
    install_system_wide
    
    echo ""
    echo -e "${GREEN}============================================${NC}"
    echo -e "${GREEN}Installation Complete!${NC}"
    echo -e "${GREEN}============================================${NC}"
    echo ""
    echo -e "You can now run: ${BLUE}ascii${NC}"
    echo ""
    echo "Examples:"
    echo "  ascii image.jpg -D 3"
    echo "  ascii photo.png --sharpen 1.2 -D 4"
    echo "  ascii animation.gif -D 3 --animate"
    echo ""
    echo "For help: ascii --help"
    echo "For version: ascii --version"
    echo ""
}

# Run main installation
main
