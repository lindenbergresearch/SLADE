
/*******************************************************************
 * SLADE - It's a Doom Editor
 * Copyright (C) 2008-2014 Simon Judd
 *
 * Email:       sirjuddington@gmail.com
 * Web:         http://slade.mancubus.net
 * Filename:    CodePages.cpp
 * Description: Stuff to handle conversion of bytes into Unicode
 *				characters according to various code pages.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************/


/*******************************************************************
 * INCLUDES
 *******************************************************************/
#include "CodePages.h"


/*******************************************************************
 * DATA TABLES
 *******************************************************************/
string asciitable[128] =
    {
        "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "TAB", "LF", "VT", "FF", "CR", "SO", "SI",
        "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US",
        "Space", "!", "\"", "#", "$", "%", "&", "\'", "(", ")", "*", "+", ",", "-", ".", "/",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
        "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
        "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
        "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
        "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~", "DEL",
    };

// Those are the UTF-8 values for the characters in the IBM Code Page 437
uint8_t cp437table[256][3] =
    {
        { 0xc2, 0xb7 }, // NULL represented by a middle dot (which isn't actually part of CP437)
        { 0xe2, 0x98, 0xba },
        { 0xe2, 0x98, 0xbb },
        { 0xe2, 0x99, 0xa5 },
        { 0xe2, 0x99, 0xa6 },
        { 0xe2, 0x99, 0xa3 },
        { 0xe2, 0x99, 0xa0 },
        { 0xe2, 0x80, 0xa2 },
        { 0xe2, 0x97, 0x98 },
        { 0xe2, 0x97, 0x8b },
        { 0xe2, 0x97, 0x99 },
        { 0xe2, 0x99, 0x82 },
        { 0xe2, 0x99, 0x80 },
        { 0xe2, 0x99, 0xaa },
        { 0xe2, 0x99, 0xab },
        { 0xe2, 0x98, 0xbc },
        { 0xe2, 0x96, 0xba },
        { 0xe2, 0x97, 0x84 },
        { 0xe2, 0x86, 0x95 },
        { 0xe2, 0x80, 0xbc },
        { 0xc2, 0xb6 },
        { 0xc2, 0xa7 },
        { 0xe2, 0x96, 0xac },
        { 0xe2, 0x86, 0xa8 },
        { 0xe2, 0x86, 0x91 },
        { 0xe2, 0x86, 0x93 },
        { 0xe2, 0x86, 0x92 },
        { 0xe2, 0x86, 0x90 },
        { 0xe2, 0x88, 0x9f },
        { 0xe2, 0x86, 0x94 },
        { 0xe2, 0x96, 0xb2 },
        { 0xe2, 0x96, 0xbc },
        // Boring ASCII characters begin here
        { ' ' },
        { '!' },
        { '"' },
        { '#' },
        { '$' },
        { '%' },
        { '&' },
        { '\'' },
        { '(' },
        { ')' },
        { '*' },
        { '+' },
        { ',' },
        { '-' },
        { '.' },
        { '/' },
        { '0' },
        { '1' },
        { '2' },
        { '3' },
        { '4' },
        { '5' },
        { '6' },
        { '7' },
        { '8' },
        { '9' },
        { ':' },
        { ';' },
        { '<' },
        { '=' },
        { '>' },
        { '?' },
        { '@' },
        { 'A' },
        { 'B' },
        { 'C' },
        { 'D' },
        { 'E' },
        { 'F' },
        { 'G' },
        { 'H' },
        { 'I' },
        { 'J' },
        { 'K' },
        { 'L' },
        { 'M' },
        { 'N' },
        { 'O' },
        { 'P' },
        { 'Q' },
        { 'R' },
        { 'S' },
        { 'T' },
        { 'U' },
        { 'V' },
        { 'W' },
        { 'X' },
        { 'Y' },
        { 'Z' },
        { '[' },
        { '\\' },
        { ']' },
        { '^' },
        { '_' },
        { '`' },
        { 'a' },
        { 'b' },
        { 'c' },
        { 'd' },
        { 'e' },
        { 'f' },
        { 'g' },
        { 'h' },
        { 'i' },
        { 'j' },
        { 'k' },
        { 'l' },
        { 'm' },
        { 'n' },
        { 'o' },
        { 'p' },
        { 'q' },
        { 'r' },
        { 's' },
        { 't' },
        { 'u' },
        { 'v' },
        { 'w' },
        { 'x' },
        { 'y' },
        { 'z' },
        { '{' },
        { '|' },
        { '}' },
        { '~' },
        // We now return to the regularly-scheduled UTF-8 value mess
        { 0xe2, 0x8c, 0x82 },
        { 0xc3, 0x87 },
        { 0xc3, 0xbc },
        { 0xc3, 0xa9 },
        { 0xc3, 0xa2 },
        { 0xc3, 0xa4 },
        { 0xc3, 0xa0 },
        { 0xc3, 0xa5 },
        { 0xc3, 0xa7 },
        { 0xc3, 0xaa },
        { 0xc3, 0xab },
        { 0xc3, 0xa8 },
        { 0xc3, 0xaf },
        { 0xc3, 0xae },
        { 0xc3, 0xac },
        { 0xc3, 0x84 },
        { 0xc3, 0x85 },
        { 0xc3, 0x89 },
        { 0xc3, 0xa6 },
        { 0xc3, 0x86 },
        { 0xc3, 0xb4 },
        { 0xc3, 0xb6 },
        { 0xc3, 0xb2 },
        { 0xc3, 0xbb },
        { 0xc3, 0xb9 },
        { 0xc3, 0xbf },
        { 0xc3, 0x96 },
        { 0xc3, 0x9c },
        { 0xc2, 0xa2 },
        { 0xc2, 0xa3 },
        { 0xc2, 0xa5 },
        { 0xe2, 0x82, 0xa7 },
        { 0xc6, 0x92 },
        { 0xc3, 0xa1 },
        { 0xc3, 0xad },
        { 0xc3, 0xb3 },
        { 0xc3, 0xba },
        { 0xc3, 0xb1 },
        { 0xc3, 0x91 },
        { 0xc2, 0xaa },
        { 0xc2, 0xba },
        { 0xc2, 0xbf },
        { 0xe2, 0x8c, 0x90 },
        { 0xc2, 0xac },
        { 0xc2, 0xbd },
        { 0xc2, 0xbc },
        { 0xc2, 0xa1 },
        { 0xc2, 0xab },
        { 0xc2, 0xbb },
        { 0xe2, 0x96, 0x91 },
        { 0xe2, 0x96, 0x92 },
        { 0xe2, 0x96, 0x93 },
        { 0xe2, 0x94, 0x82 },
        { 0xe2, 0x94, 0xa4 },
        { 0xe2, 0x95, 0xa1 },
        { 0xe2, 0x95, 0xa2 },
        { 0xe2, 0x95, 0x96 },
        { 0xe2, 0x95, 0x95 },
        { 0xe2, 0x95, 0xa3 },
        { 0xe2, 0x95, 0x91 },
        { 0xe2, 0x95, 0x97 },
        { 0xe2, 0x95, 0x9d },
        { 0xe2, 0x95, 0x9c },
        { 0xe2, 0x95, 0x9b },
        { 0xe2, 0x94, 0x90 },
        { 0xe2, 0x94, 0x94 },
        { 0xe2, 0x94, 0xb4 },
        { 0xe2, 0x94, 0xac },
        { 0xe2, 0x94, 0x9c },
        { 0xe2, 0x94, 0x80 },
        { 0xe2, 0x94, 0xbc },
        { 0xe2, 0x95, 0x9e },
        { 0xe2, 0x95, 0x9f },
        { 0xe2, 0x95, 0x9a },
        { 0xe2, 0x95, 0x94 },
        { 0xe2, 0x95, 0xa9 },
        { 0xe2, 0x95, 0xa6 },
        { 0xe2, 0x95, 0xa0 },
        { 0xe2, 0x95, 0x90 },
        { 0xe2, 0x95, 0xac },
        { 0xe2, 0x95, 0xa7 },
        { 0xe2, 0x95, 0xa8 },
        { 0xe2, 0x95, 0xa4 },
        { 0xe2, 0x95, 0xa5 },
        { 0xe2, 0x95, 0x99 },
        { 0xe2, 0x95, 0x98 },
        { 0xe2, 0x95, 0x92 },
        { 0xe2, 0x95, 0x93 },
        { 0xe2, 0x95, 0xab },
        { 0xe2, 0x95, 0xaa },
        { 0xe2, 0x94, 0x98 },
        { 0xe2, 0x94, 0x8c },
        { 0xe2, 0x96, 0x88 },
        { 0xe2, 0x96, 0x84 },
        { 0xe2, 0x96, 0x8c },
        { 0xe2, 0x96, 0x90 },
        { 0xe2, 0x96, 0x80 },
        { 0xce, 0xb1 },
        { 0xc3, 0x9f },
        { 0xce, 0x93 },
        { 0xcf, 0x80 },
        { 0xce, 0xa3 },
        { 0xcf, 0x83 },
        { 0xc2, 0xb5 },
        { 0xcf, 0x84 },
        { 0xce, 0xa6 },
        { 0xce, 0x98 },
        { 0xce, 0xa9 },
        { 0xce, 0xb4 },
        { 0xe2, 0x88, 0x9e },
        { 0xcf, 0x86 },
        { 0xce, 0xb5 },
        { 0xe2, 0x88, 0xa9 },
        { 0xe2, 0x89, 0xa1 },
        { 0xc2, 0xb1 },
        { 0xe2, 0x89, 0xa5 },
        { 0xe2, 0x89, 0xa4 },
        { 0xe2, 0x8c, 0xa0 },
        { 0xe2, 0x8c, 0xa1 },
        { 0xc3, 0xb7 },
        { 0xe2, 0x89, 0x88 },
        { 0xc2, 0xb0 },
        { 0xe2, 0x88, 0x99 },
        { 0xc2, 0xb7 },
        { 0xe2, 0x88, 0x9a },
        { 0xe2, 0x81, 0xbf },
        { 0xc2, 0xb2 },
        { 0xe2, 0x96, 0xa0 }
    };

uint8_t cp437len[256] =
    {
        2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        // Boring ASCII characters begin here
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // We now return to the regularly-scheduled UTF-8 value mess
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 3, 3, 2, 3, 3, 3, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3
    };

uint8_t ansicolors[16][3] =
    {
        { 0x00, 0x00, 0x00 }, // Black
        { 0x00, 0x00, 0xAA }, // Blue
        { 0x00, 0xAA, 0x00 }, // Green
        { 0x00, 0xAA, 0xAA }, // Cyan
        { 0xAA, 0x00, 0x00 }, // Red
        { 0xAA, 0x00, 0xAA }, // Magenta
        { 0xAA, 0x55, 0x00 }, // Brown
        { 0xAA, 0xAA, 0xAA }, // Light Gray
        { 0x55, 0x55, 0x55 }, // Dark Gray
        { 0x55, 0x55, 0xFF }, // Light Blue
        { 0x55, 0xFF, 0x55 }, // Light Green
        { 0x55, 0xFF, 0xFF }, // Light Cyan
        { 0xFF, 0x55, 0x55 }, // Light Red
        { 0xFF, 0x55, 0xFF }, // Light Magenta
        { 0xFF, 0xFF, 0x55 }, // Yellow
        { 0xFF, 0xFF, 0xFF }, // White
    };


/*******************************************************************
 * FUNCTIONS
 *******************************************************************/
string CodePages::fromASCII(uint8_t val) {
    if (val < 128) return asciitable[val];
    else return "";
}


string CodePages::fromCP437(uint8_t val) {
    return wxString::FromUTF8((const char *) cp437table[val], cp437len[val]);
}


rgba_t CodePages::ansiColor(uint8_t val) {
    if (val >= 16) val = ((val >> 4) & 7);
    return rgba_t(ansicolors[val][0], ansicolors[val][1], ansicolors[val][2]);
}
