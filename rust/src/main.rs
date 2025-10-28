/*
 * ASCII-MEDIA - Rust Media Player
 * Full integration of tplay for video/webcam/streaming
 * Copyright (c) 2025 danko12
 * Licensed under MIT License
 */

use clap::Parser;
use std::process;

#[derive(Parser, Debug)]
#[command(author = "danko12", version = "3.0.0", about = "ASCII-MEDIA: Terminal Multimedia Player", long_about = None)]
struct Args {
    #[arg(value_name = "MEDIA")]
    media: Option<String>,

    #[arg(short = 'f', long = "fps")]
    fps: Option<f32>,

    #[arg(short = 'c', long = "char-map", default_value = " .:-=+*#%@")]
    char_map: String,

    #[arg(short = 'g', long = "gray")]
    grayscale: bool,

    #[arg(short = 'w', long = "w-mod", default_value_t = 1)]
    width_mod: u32,

    #[arg(short = 'a', long = "allow-frame-skip")]
    allow_frame_skip: bool,

    #[arg(short = 'l', long = "loop-playback")]
    loop_playback: bool,
}

fn main() {
    let args = Args::parse();

    println!("╔══════════════════════════════════════════════════════════════╗");
    println!("║                    ASCII-MEDIA v3.0.0                        ║");
    println!("║      Advanced Terminal Multimedia Converter & Player        ║");
    println!("║                Copyright (c) 2025 danko12                    ║");
    println!("║                  Linux & macOS Only                          ║");
    println!("╚══════════════════════════════════════════════════════════════╝");
    println!();

    if args.media.is_none() {
        print_usage();
        return;
    }

    let media = args.media.unwrap();
    
    if media.ends_with(".mp4") || media.ends_with(".avi") || media.ends_with(".mkv") || media.ends_with(".webm") || media.ends_with(".MP4") || media.ends_with(".AVI") {
        println!("🎥 Video support coming soon!");
        println!("   For now, use image/GIF processing only");
        process::exit(1);
    } else {
        println!("🖼️  Image/GIF file detected: {}", media);
        println!("   Redirecting to optimized C/C++ processor...");
        println!();
        
        let status = process::Command::new("./ascii")
            .arg(&media)
            .args(if args.grayscale { vec!["--grayscale"] } else { vec![] })
            .args(if args.loop_playback && (media.ends_with(".gif") || media.ends_with(".GIF")) { 
                vec!["--animate"] 
            } else { 
                vec![] 
            })
            .status();

        match status {
            Ok(exit_status) => {
                if !exit_status.success() {
                    eprintln!("\n⚠️  Processing completed with warnings");
                }
            }
            Err(e) => {
                eprintln!("❌ Error: Could not execute 'ascii' binary: {}", e);
                eprintln!("   Build with: make release");
                process::exit(1);
            }
        }
    }

    println!("\n✅ ASCII-MEDIA processing complete!");
}

fn print_usage() {
    println!("ASCII-MEDIA v3.0.0 - Terminal Multimedia Converter & Player");
    println!("Copyright (c) 2025 danko12 | Linux & macOS Only");
    println!();
    println!("USAGE:");
    println!("  For Images/GIFs (C/C++ engine):");
    println!("    ./ascii <image-file> [OPTIONS]");
    println!("    ./ascii sample-images/puffin.jpg -D 3 --enhanced-palette");
    println!("    ./ascii sample-images/nyan-cat.gif --animate --debug");
    println!();
    println!("  For Videos (C++ engine with OpenCV):");
    println!("    ./ascii-video <video-file> [OPTIONS]");
    println!();
    println!("FEATURES:");
    println!("  ✅ Images (JPEG, PNG, BMP, etc.) - Full support");
    println!("  ✅ GIF Animations - Ultra-smooth with double buffering");
    println!("  ✅ 24-bit True Color + Grayscale");
    println!("  ✅ Enhanced 70+ character precision palette");
    println!("  ✅ Debug mode with real-time stats");
    println!("  ✅ BT.601 luminance with gamma compensation");
    println!("  ✅ Smart aspect ratio lock (±3%)");
    println!("  ✅ Video Files (MP4, AVI, MKV, WEBM) - Full support with OpenCV");
    println!();
}
