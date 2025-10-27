/*
 * ASCII-MEDIA - Rust Build Script
 * Copyright (c) 2025 danko12
 * Licensed under MIT License
 * 
 * This build script compiles the C/C++ components and links them with Rust
 */

use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:rerun-if-changed=src/");
    println!("cargo:rerun-if-changed=include/");

    // Compile C files
    cc::Build::new()
        .file("src/image.c")
        .file("src/argparse.c")
        .file("src/print_image.c")
        .include("include")
        .flag("-std=c99")
        .flag("-D_GNU_SOURCE")
        .warnings(true)
        .compile("ascii_c");

    // Compile C++ files
    cc::Build::new()
        .file("src/ascii_processor.cpp")
        .include("include")
        .cpp(true)
        .flag_if_supported("-std=c++17")
        .flag("-D_GNU_SOURCE")
        .warnings(true)
        .compile("ascii_cpp");

    // Link math library
    println!("cargo:rustc-link-lib=m");
    println!("cargo:rustc-link-lib=stdc++");

    // Set library search path
    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());
    println!("cargo:rustc-link-search=native={}", out_dir.display());
    
    println!("cargo:warning=C/C++ components compiled successfully");
}
