/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// CryHam: threw out layouts, added more media keys, from:
//         https://deskthority.net/wiki/Media_key

#pragma once
#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C"{
#endif

//#define LAYOUT_US_ENGLISH
//#define LAYOUT_US_INTERNATIONAL
//#define LAYOUT_UNITED_KINGDOM	



// http://en.wikipedia.org/wiki/Keyboard_layout


#define MODIFIERKEY_CTRL        ( 0x01 | 0xE000 )
#define MODIFIERKEY_SHIFT       ( 0x02 | 0xE000 )
#define MODIFIERKEY_ALT         ( 0x04 | 0xE000 )
#define MODIFIERKEY_GUI         ( 0x08 | 0xE000 )
#define MODIFIERKEY_LEFT_CTRL   ( 0x01 | 0xE000 )
#define MODIFIERKEY_LEFT_SHIFT  ( 0x02 | 0xE000 )
#define MODIFIERKEY_LEFT_ALT    ( 0x04 | 0xE000 )
#define MODIFIERKEY_LEFT_GUI    ( 0x08 | 0xE000 )
#define MODIFIERKEY_RIGHT_CTRL  ( 0x10 | 0xE000 )
#define MODIFIERKEY_RIGHT_SHIFT ( 0x20 | 0xE000 )
#define MODIFIERKEY_RIGHT_ALT   ( 0x40 | 0xE000 )
#define MODIFIERKEY_RIGHT_GUI   ( 0x80 | 0xE000 )


#define KEY_A                   (   4  | 0xF000 )
#define KEY_B                   (   5  | 0xF000 )
#define KEY_C                   (   6  | 0xF000 )
#define KEY_D                   (   7  | 0xF000 )
#define KEY_E                   (   8  | 0xF000 )
#define KEY_F                   (   9  | 0xF000 )
#define KEY_G                   (  10  | 0xF000 )
#define KEY_H                   (  11  | 0xF000 )
#define KEY_I                   (  12  | 0xF000 )
#define KEY_J                   (  13  | 0xF000 )
#define KEY_K                   (  14  | 0xF000 )
#define KEY_L                   (  15  | 0xF000 )
#define KEY_M                   (  16  | 0xF000 )
#define KEY_N                   (  17  | 0xF000 )
#define KEY_O                   (  18  | 0xF000 )
#define KEY_P                   (  19  | 0xF000 )
#define KEY_Q                   (  20  | 0xF000 )
#define KEY_R                   (  21  | 0xF000 )
#define KEY_S                   (  22  | 0xF000 )
#define KEY_T                   (  23  | 0xF000 )
#define KEY_U                   (  24  | 0xF000 )
#define KEY_V                   (  25  | 0xF000 )
#define KEY_W                   (  26  | 0xF000 )
#define KEY_X                   (  27  | 0xF000 )
#define KEY_Y                   (  28  | 0xF000 )
#define KEY_Z                   (  29  | 0xF000 )

#define KEY_1                   (  30  | 0xF000 )
#define KEY_2                   (  31  | 0xF000 )
#define KEY_3                   (  32  | 0xF000 )
#define KEY_4                   (  33  | 0xF000 )
#define KEY_5                   (  34  | 0xF000 )
#define KEY_6                   (  35  | 0xF000 )
#define KEY_7                   (  36  | 0xF000 )
#define KEY_8                   (  37  | 0xF000 )
#define KEY_9                   (  38  | 0xF000 )
#define KEY_0                   (  39  | 0xF000 )

#define KEY_ENTER               (  40  | 0xF000 )
#define KEY_ESC                 (  41  | 0xF000 )
#define KEY_BACKSPACE           (  42  | 0xF000 )
#define KEY_TAB                 (  43  | 0xF000 )
#define KEY_SPACE               (  44  | 0xF000 )
#define KEY_MINUS               (  45  | 0xF000 )
#define KEY_EQUAL               (  46  | 0xF000 )
#define KEY_LEFT_BRACE          (  47  | 0xF000 )
#define KEY_RIGHT_BRACE         (  48  | 0xF000 )
#define KEY_BACKSLASH           (  49  | 0xF000 )
#define KEY_NON_US_NUM          (  50  | 0xF000 )
#define KEY_SEMICOLON           (  51  | 0xF000 )
#define KEY_QUOTE               (  52  | 0xF000 )
#define KEY_TILDE               (  53  | 0xF000 )
#define KEY_COMMA               (  54  | 0xF000 )
#define KEY_PERIOD              (  55  | 0xF000 )
#define KEY_SLASH               (  56  | 0xF000 )
#define KEY_CAPS_LOCK           (  57  | 0xF000 )

#define KEY_F1                  (  58  | 0xF000 )
#define KEY_F2                  (  59  | 0xF000 )
#define KEY_F3                  (  60  | 0xF000 )
#define KEY_F4                  (  61  | 0xF000 )
#define KEY_F5                  (  62  | 0xF000 )
#define KEY_F6                  (  63  | 0xF000 )
#define KEY_F7                  (  64  | 0xF000 )
#define KEY_F8                  (  65  | 0xF000 )
#define KEY_F9                  (  66  | 0xF000 )
#define KEY_F10                 (  67  | 0xF000 )
#define KEY_F11                 (  68  | 0xF000 )
#define KEY_F12                 (  69  | 0xF000 )

#define KEY_PRINTSCREEN         (  70  | 0xF000 )
#define KEY_SCROLL_LOCK         (  71  | 0xF000 )
#define KEY_PAUSE               (  72  | 0xF000 )
#define KEY_INSERT              (  73  | 0xF000 )
#define KEY_HOME                (  74  | 0xF000 )
#define KEY_PAGE_UP             (  75  | 0xF000 )
#define KEY_DELETE              (  76  | 0xF000 )
#define KEY_END                 (  77  | 0xF000 )
#define KEY_PAGE_DOWN           (  78  | 0xF000 )
#define KEY_RIGHT               (  79  | 0xF000 )
#define KEY_LEFT                (  80  | 0xF000 )
#define KEY_DOWN                (  81  | 0xF000 )
#define KEY_UP                  (  82  | 0xF000 )

#define KEY_NUM_LOCK            (  83  | 0xF000 )
#define KEYPAD_SLASH            (  84  | 0xF000 )
#define KEYPAD_ASTERIX          (  85  | 0xF000 )
#define KEYPAD_MINUS            (  86  | 0xF000 )
#define KEYPAD_PLUS             (  87  | 0xF000 )
#define KEYPAD_ENTER            (  88  | 0xF000 )
#define KEYPAD_1                (  89  | 0xF000 )
#define KEYPAD_2                (  90  | 0xF000 )
#define KEYPAD_3                (  91  | 0xF000 )
#define KEYPAD_4                (  92  | 0xF000 )
#define KEYPAD_5                (  93  | 0xF000 )
#define KEYPAD_6                (  94  | 0xF000 )
#define KEYPAD_7                (  95  | 0xF000 )
#define KEYPAD_8                (  96  | 0xF000 )
#define KEYPAD_9                (  97  | 0xF000 )
#define KEYPAD_0                (  98  | 0xF000 )
#define KEYPAD_PERIOD           (  99  | 0xF000 )
#define KEY_NON_US_BS           ( 100  | 0xF000 )
#define KEY_MENU                ( 101  | 0xF000 )

#define KEY_F13                 ( 104  | 0xF000 )
#define KEY_F14                 ( 105  | 0xF000 )
#define KEY_F15                 ( 106  | 0xF000 )
#define KEY_F16                 ( 107  | 0xF000 )
#define KEY_F17                 ( 108  | 0xF000 )
#define KEY_F18                 ( 109  | 0xF000 )
#define KEY_F19                 ( 110  | 0xF000 )
#define KEY_F20                 ( 111  | 0xF000 )
#define KEY_F21                 ( 112  | 0xF000 )
#define KEY_F22                 ( 113  | 0xF000 )
#define KEY_F23                 ( 114  | 0xF000 )
#define KEY_F24                 ( 115  | 0xF000 )

#define KEY_SYSTEM_POWER_DOWN   ( 0x81 | 0xE200 )
#define KEY_SYSTEM_SLEEP        ( 0x82 | 0xE200 )
#define KEY_SYSTEM_WAKE_UP      ( 0x83 | 0xE200 )

#define KEY_MEDIA_PLAY          ( 0xB0 | 0xE400 )
#define KEY_MEDIA_PAUSE         ( 0xB1 | 0xE400 )
#define KEY_MEDIA_RECORD        ( 0xB2 | 0xE400 )
#define KEY_MEDIA_FAST_FORWARD  ( 0xB3 | 0xE400 )
#define KEY_MEDIA_REWIND        ( 0xB4 | 0xE400 )
#define KEY_MEDIA_NEXT_TRACK    ( 0xB5 | 0xE400 )
#define KEY_MEDIA_PREV_TRACK    ( 0xB6 | 0xE400 )
#define KEY_MEDIA_STOP          ( 0xB7 | 0xE400 )
#define KEY_MEDIA_EJECT         ( 0xB8 | 0xE400 )
#define KEY_MEDIA_RANDOM_PLAY   ( 0xB9 | 0xE400 )
#define KEY_MEDIA_PLAY_PAUSE    ( 0xCD | 0xE400 )
#define KEY_MEDIA_PLAY_SKIP     ( 0xCE | 0xE400 )
#define KEY_MEDIA_MUTE          ( 0xE2 | 0xE400 )
#define KEY_MEDIA_VOLUME_INC    ( 0xE9 | 0xE400 )
#define KEY_MEDIA_VOLUME_DEC    ( 0xEA | 0xE400 )

#define KEY_MEDIA_CALC          ( 0x192 | 0xE400 )

// todo more-


// Everything below this line is not intended for use in "normal" programs.
// These private symbols populate lookup tables, which are used to translate
// ascii and UTF8 unicode into keystroke sequences.


#ifdef LAYOUT_US_ENGLISH

#define SHIFT_MASK		0x40
#define KEYCODE_TYPE		uint8_t
#define KEYCODE_MASK		0x007F

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_QUOTE + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_QUOTE				// 39 '
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 (
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_SLASH				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_SLASH + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE				// 91 [
#define ASCII_5C	KEY_BACKSLASH				// 92
#define ASCII_5D	KEY_RIGHT_BRACE				// 93 ]
#define ASCII_5E	KEY_6 + SHIFT_MASK			// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	KEY_TILDE				// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_BACKSLASH + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK		// 125 }
#define ASCII_7E	KEY_TILDE + SHIFT_MASK			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#endif // LAYOUT_US_ENGLISH



#ifdef LAYOUT_US_INTERNATIONAL

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define DEADKEYS_MASK		0x0700
#define	CIRCUMFLEX_BITS		0x0100
#define ACUTE_ACCENT_BITS	0x0200
#define GRAVE_ACCENT_BITS	0x0300
#define TILDE_BITS		0x0400
#define DIAERESIS_BITS		0x0500
#define KEYCODE_TYPE		uint16_t
#define KEYCODE_MASK		0x07FF
#define DEADKEY_CIRCUMFLEX	KEY_6 + SHIFT_MASK
#define DEADKEY_ACUTE_ACCENT	KEY_QUOTE
#define DEADKEY_GRAVE_ACCENT	KEY_TILDE
#define DEADKEY_TILDE		KEY_TILDE + SHIFT_MASK
#define DEADKEY_DIAERESIS	KEY_QUOTE + SHIFT_MASK

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	DIAERESIS_BITS + KEY_SPACE		// 34 "
#define ASCII_23	KEY_3 + SHIFT_MASK			// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	ACUTE_ACCENT_BITS + KEY_SPACE		// 39 '
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 (
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_SLASH				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_SLASH + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_2 + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE				// 91 [
#define ASCII_5C	KEY_BACKSLASH				// 92
#define ASCII_5D	KEY_RIGHT_BRACE				// 93 ]
#define ASCII_5E	CIRCUMFLEX_BITS + KEY_SPACE		// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	GRAVE_ACCENT_BITS + KEY_SPACE		// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_BACKSLASH + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK		// 125 }
#define ASCII_7E	TILDE_BITS + KEY_SPACE			// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127
#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	KEY_1 + ALTGR_MASK			// 161 Âˇ     Inverted Exclamation
#define ISO_8859_1_A2	KEY_C + ALTGR_MASK + SHIFT_MASK		// 162 Â�?     Cent SIGN
#define ISO_8859_1_A3	KEY_4 + ALTGR_MASK + SHIFT_MASK		// 163 Â�?     Pound Sign
#define ISO_8859_1_A4	KEY_4 + ALTGR_MASK			// 164 Â¤     Currency or Euro Sign
#define ISO_8859_1_A5	KEY_MINUS + ALTGR_MASK			// 165 ÂĄ     YEN SIGN
#define ISO_8859_1_A6	KEY_BACKSLASH + ALTGR_MASK + SHIFT_MASK	// 166 Â¦     BROKEN BAR			??
#define ISO_8859_1_A7	KEY_S + ALTGR_MASK + SHIFT_MASK		// 167 Â§     SECTION SIGN
#define ISO_8859_1_A8	KEY_QUOTE + ALTGR_MASK + SHIFT_MASK	// 168 Â¨     DIAERESIS
#define ISO_8859_1_A9	KEY_C + ALTGR_MASK			// 169 Â©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ÂŞ     FEMININE ORDINAL
#define ISO_8859_1_AB	KEY_LEFT_BRACE + ALTGR_MASK		// 171 Â«     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_BACKSLASH + ALTGR_MASK		// 172 Â¬     NOT SIGN			??
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	KEY_R + ALTGR_MASK			// 174 Â®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ÂŻ     MACRON
#define ISO_8859_1_B0	KEY_SEMICOLON + ALTGR_MASK + SHIFT_MASK	// 176 Â°     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 Â±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	KEY_2 + ALTGR_MASK			// 178 Â˛     SUPERSCRIPT TWO
#define ISO_8859_1_B3	KEY_3 + ALTGR_MASK			// 179 Âł     SUPERSCRIPT THREE
#define ISO_8859_1_B4	KEY_QUOTE + ALTGR_MASK			// 180 Â´     ACUTE ACCENT
#define ISO_8859_1_B5	KEY_M + ALTGR_MASK			// 181 Âµ     MICRO SIGN
#define ISO_8859_1_B6	KEY_SEMICOLON + ALTGR_MASK		// 182 Â¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 Â·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 Â¸     CEDILLA
#define ISO_8859_1_B9	KEY_1 + ALTGR_MASK + SHIFT_MASK		// 185 Âą     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 Âş     MASCULINE ORDINAL
#define ISO_8859_1_BB	KEY_RIGHT_BRACE + ALTGR_MASK		// 187 Â»     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	KEY_6 + ALTGR_MASK			// 188 ÂĽ     FRACTION ONE QUARTER
#define ISO_8859_1_BD	KEY_7 + ALTGR_MASK			// 189 Â˝     FRACTION ONE HALF
#define ISO_8859_1_BE	KEY_8 + ALTGR_MASK			// 190 Âľ     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	KEY_SLASH + ALTGR_MASK			// 191 Âż     INVERTED QUESTION MARK
#define ISO_8859_1_C0	GRAVE_ACCENT_BITS + KEY_A + SHIFT_MASK	// 192 Ă€     A GRAVE
#define ISO_8859_1_C1	KEY_A + ALTGR_MASK + SHIFT_MASK		// 193 Ă�     A ACUTE
#define ISO_8859_1_C2	CIRCUMFLEX_BITS	+ KEY_A + SHIFT_MASK	// 194 Ă‚     A CIRCUMFLEX
#define ISO_8859_1_C3	TILDE_BITS + KEY_A + SHIFT_MASK		// 195 Ă�     A TILDE
#define ISO_8859_1_C4	KEY_Q + ALTGR_MASK + SHIFT_MASK		// 196 Ă„     A DIAERESIS
#define ISO_8859_1_C5	KEY_W + ALTGR_MASK + SHIFT_MASK		// 197 Ă…     A RING ABOVE
#define ISO_8859_1_C6	KEY_Z + ALTGR_MASK + SHIFT_MASK		// 198 Ă†     AE
#define ISO_8859_1_C7	KEY_COMMA + ALTGR_MASK + SHIFT_MASK	// 199 Ă‡     C CEDILLA
#define ISO_8859_1_C8	GRAVE_ACCENT_BITS + KEY_E + SHIFT_MASK	// 200 Ă�     E GRAVE
#define ISO_8859_1_C9	KEY_E + ALTGR_MASK + SHIFT_MASK		// 201 Ă‰     E ACUTE
#define ISO_8859_1_CA	CIRCUMFLEX_BITS + KEY_E + SHIFT_MASK	// 202 ĂŠ     E CIRCUMFLEX
#define ISO_8859_1_CB	DIAERESIS_BITS + KEY_E + SHIFT_MASK	// 203 Ă‹     E DIAERESIS
#define ISO_8859_1_CC	GRAVE_ACCENT_BITS + KEY_I + SHIFT_MASK	// 204 ĂŚ     I GRAVE
#define ISO_8859_1_CD	KEY_I + ALTGR_MASK + SHIFT_MASK		// 205 ĂŤ     I ACUTE
#define ISO_8859_1_CE	CIRCUMFLEX_BITS + KEY_I + SHIFT_MASK	// 206 ĂŽ     I CIRCUMFLEX
#define ISO_8859_1_CF	DIAERESIS_BITS + KEY_I + SHIFT_MASK	// 207 ĂŹ     I DIAERESIS
#define ISO_8859_1_D0	KEY_D + ALTGR_MASK + SHIFT_MASK		// 208 Ă�     ETH
#define ISO_8859_1_D1	KEY_N + ALTGR_MASK + SHIFT_MASK		// 209 Ă�?     N TILDE
#define ISO_8859_1_D2	GRAVE_ACCENT_BITS + KEY_O + SHIFT_MASK	// 210 Ă’     O GRAVE
#define ISO_8859_1_D3	KEY_O + ALTGR_MASK + SHIFT_MASK		// 211 Ă“     O ACUTE
#define ISO_8859_1_D4	CIRCUMFLEX_BITS + KEY_O + SHIFT_MASK	// 212 Ă”     O CIRCUMFLEX
#define ISO_8859_1_D5	TILDE_BITS + KEY_O + SHIFT_MASK		// 213 Ă•     O TILDE
#define ISO_8859_1_D6	KEY_P + ALTGR_MASK + SHIFT_MASK		// 214 Ă–     O DIAERESIS
#define ISO_8859_1_D7	KEY_EQUAL + ALTGR_MASK			// 215 Ă—     MULTIPLICATION
#define ISO_8859_1_D8	KEY_L + ALTGR_MASK + SHIFT_MASK		// 216 Ă�     O STROKE
#define ISO_8859_1_D9	GRAVE_ACCENT_BITS + KEY_U + SHIFT_MASK	// 217 Ă™     U GRAVE
#define ISO_8859_1_DA	KEY_U + ALTGR_MASK + SHIFT_MASK		// 218 Ăš     U ACUTE
#define ISO_8859_1_DB	CIRCUMFLEX_BITS + KEY_U + SHIFT_MASK	// 219 Ă›     U CIRCUMFLEX
#define ISO_8859_1_DC	KEY_Y + ALTGR_MASK + SHIFT_MASK		// 220 Ăś     U DIAERESIS
#define ISO_8859_1_DD	ACUTE_ACCENT_BITS + KEY_Y + SHIFT_MASK	// 221 Ăť     Y ACUTE
#define ISO_8859_1_DE	KEY_T + ALTGR_MASK + SHIFT_MASK		// 222 Ăž     THORN
#define ISO_8859_1_DF	KEY_S + ALTGR_MASK			// 223 Ăź     SHARP S
#define ISO_8859_1_E0	GRAVE_ACCENT_BITS + KEY_A		// 224 Ă      a GRAVE
#define ISO_8859_1_E1	KEY_A + ALTGR_MASK			// 225 Ăˇ     a ACUTE
#define ISO_8859_1_E2	CIRCUMFLEX_BITS + KEY_A			// 226 Ă�?     a CIRCUMFLEX
#define ISO_8859_1_E3	TILDE_BITS + KEY_A			// 227 Ă�?     a TILDE
#define ISO_8859_1_E4	DIAERESIS_BITS + KEY_A			// 228 Ă¤     a DIAERESIS
#define ISO_8859_1_E5	KEY_W + ALTGR_MASK			// 229 ĂĄ     a RING ABOVE
#define ISO_8859_1_E6	KEY_Z + ALTGR_MASK			// 230 Ă¦     ae
#define ISO_8859_1_E7	KEY_COMMA + ALTGR_MASK			// 231 Ă§     c CEDILLA
#define ISO_8859_1_E8	GRAVE_ACCENT_BITS + KEY_E		// 232 Ă¨     e GRAVE
#define ISO_8859_1_E9	ACUTE_ACCENT_BITS + KEY_E		// 233 Ă©     e ACUTE
#define ISO_8859_1_EA	CIRCUMFLEX_BITS + KEY_E			// 234 ĂŞ     e CIRCUMFLEX
#define ISO_8859_1_EB	DIAERESIS_BITS + KEY_E			// 235 Ă«     e DIAERESIS
#define ISO_8859_1_EC	GRAVE_ACCENT_BITS + KEY_I		// 236 Ă¬     i GRAVE
#define ISO_8859_1_ED	KEY_I + ALTGR_MASK			// 237 Ă­     i ACUTE
#define ISO_8859_1_EE	CIRCUMFLEX_BITS + KEY_I			// 238 Ă®     i CIRCUMFLEX
#define ISO_8859_1_EF	DIAERESIS_BITS + KEY_I			// 239 ĂŻ     i DIAERESIS
#define ISO_8859_1_F0	KEY_D + ALTGR_MASK			// 240 Ă°     ETH
#define ISO_8859_1_F1	KEY_N + ALTGR_MASK			// 241 Ă±     n TILDE
#define ISO_8859_1_F2	GRAVE_ACCENT_BITS + KEY_O		// 242 Ă˛     o GRAVE
#define ISO_8859_1_F3	KEY_O + ALTGR_MASK			// 243 Ăł     o ACUTE
#define ISO_8859_1_F4	CIRCUMFLEX_BITS + KEY_O			// 244 Ă´     o CIRCUMFLEX
#define ISO_8859_1_F5	TILDE_BITS + KEY_O 			// 245 Ăµ     o TILDE
#define ISO_8859_1_F6	KEY_P + ALTGR_MASK			// 246 Ă¶     o DIAERESIS
#define ISO_8859_1_F7	KEY_EQUAL + ALTGR_MASK + SHIFT_MASK	// 247 Ă·     DIVISION
#define ISO_8859_1_F8	KEY_L + ALTGR_MASK			// 248 Ă¸     o STROKE
#define ISO_8859_1_F9	GRAVE_ACCENT_BITS + KEY_U		// 249 Ăą     u GRAVE
#define ISO_8859_1_FA	KEY_U + ALTGR_MASK			// 250 Ăş     u ACUTE
#define ISO_8859_1_FB	CIRCUMFLEX_BITS + KEY_U			// 251 Ă»     u CIRCUMFLEX
#define ISO_8859_1_FC	KEY_Y + ALTGR_MASK			// 252 ĂĽ     u DIAERESIS
#define ISO_8859_1_FD	ACUTE_ACCENT_BITS + KEY_Y		// 253 Ă˝     y ACUTE
#define ISO_8859_1_FE	KEY_T + ALTGR_MASK			// 254 Ăľ     THORN
#define ISO_8859_1_FF	DIAERESIS_BITS + KEY_Y			// 255 Ăż     y DIAERESIS
#define UNICODE_20AC	KEY_5 + ALTGR_MASK			//     â‚¬     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_5 + ALTGR_MASK			// 20AC â‚¬    Euro Sign

#endif // LAYOUT_US_INTERNATIONAL



#ifdef LAYOUT_UNITED_KINGDOM

#define SHIFT_MASK		0x0040
#define ALTGR_MASK		0x0080
#define KEYCODE_TYPE		uint8_t
#define KEYCODE_MASK		0x00FF
#define KEY_NON_US_100		63

#define ASCII_20	KEY_SPACE				// 32  
#define ASCII_21	KEY_1 + SHIFT_MASK			// 33 !
#define ASCII_22	KEY_2 + SHIFT_MASK			// 34 "
#define ASCII_23	KEY_BACKSPACE				// 35 #
#define ASCII_24	KEY_4 + SHIFT_MASK			// 36 $
#define ASCII_25	KEY_5 + SHIFT_MASK			// 37 %
#define ASCII_26	KEY_7 + SHIFT_MASK			// 38 &
#define ASCII_27	KEY_QUOTE				// 39 '
#define ASCII_28	KEY_9 + SHIFT_MASK			// 40 (
#define ASCII_29	KEY_0 + SHIFT_MASK			// 41 )
#define ASCII_2A	KEY_8 + SHIFT_MASK			// 42 *
#define ASCII_2B	KEY_EQUAL + SHIFT_MASK			// 43 +
#define ASCII_2C	KEY_COMMA				// 44 ,
#define ASCII_2D	KEY_MINUS				// 45 -
#define ASCII_2E	KEY_PERIOD				// 46 .
#define ASCII_2F	KEY_SLASH				// 47 /
#define ASCII_30	KEY_0					// 48 0
#define ASCII_31	KEY_1					// 49 1
#define ASCII_32	KEY_2					// 50 2
#define ASCII_33	KEY_3					// 51 3
#define ASCII_34	KEY_4					// 52 4
#define ASCII_35	KEY_5					// 53 5
#define ASCII_36	KEY_6					// 54 6
#define ASCII_37	KEY_7					// 55 7
#define ASCII_38	KEY_8					// 55 8
#define ASCII_39	KEY_9					// 57 9
#define ASCII_3A	KEY_SEMICOLON + SHIFT_MASK		// 58 :
#define ASCII_3B	KEY_SEMICOLON				// 59 ;
#define ASCII_3C	KEY_COMMA + SHIFT_MASK			// 60 <
#define ASCII_3D	KEY_EQUAL				// 61 =
#define ASCII_3E	KEY_PERIOD + SHIFT_MASK			// 62 >
#define ASCII_3F	KEY_SLASH + SHIFT_MASK			// 63 ?
#define ASCII_40	KEY_QUOTE + SHIFT_MASK			// 64 @
#define ASCII_41	KEY_A + SHIFT_MASK			// 65 A
#define ASCII_42	KEY_B + SHIFT_MASK			// 66 B
#define ASCII_43	KEY_C + SHIFT_MASK			// 67 C
#define ASCII_44	KEY_D + SHIFT_MASK			// 68 D
#define ASCII_45	KEY_E + SHIFT_MASK			// 69 E
#define ASCII_46	KEY_F + SHIFT_MASK			// 70 F
#define ASCII_47	KEY_G + SHIFT_MASK			// 71 G
#define ASCII_48	KEY_H + SHIFT_MASK			// 72 H
#define ASCII_49	KEY_I + SHIFT_MASK			// 73 I
#define ASCII_4A	KEY_J + SHIFT_MASK			// 74 J
#define ASCII_4B	KEY_K + SHIFT_MASK			// 75 K
#define ASCII_4C	KEY_L + SHIFT_MASK			// 76 L
#define ASCII_4D	KEY_M + SHIFT_MASK			// 77 M
#define ASCII_4E	KEY_N + SHIFT_MASK			// 78 N
#define ASCII_4F	KEY_O + SHIFT_MASK			// 79 O
#define ASCII_50	KEY_P + SHIFT_MASK			// 80 P
#define ASCII_51	KEY_Q + SHIFT_MASK			// 81 Q
#define ASCII_52	KEY_R + SHIFT_MASK			// 82 R
#define ASCII_53	KEY_S + SHIFT_MASK			// 83 S
#define ASCII_54	KEY_T + SHIFT_MASK			// 84 T
#define ASCII_55	KEY_U + SHIFT_MASK			// 85 U
#define ASCII_56	KEY_V + SHIFT_MASK			// 86 V
#define ASCII_57	KEY_W + SHIFT_MASK			// 87 W
#define ASCII_58	KEY_X + SHIFT_MASK			// 88 X
#define ASCII_59	KEY_Y + SHIFT_MASK			// 89 Y
#define ASCII_5A	KEY_Z + SHIFT_MASK			// 90 Z
#define ASCII_5B	KEY_LEFT_BRACE				// 91 [
#define ASCII_5C	KEY_NON_US_100				// 92
#define ASCII_5D	KEY_RIGHT_BRACE				// 93 ]
#define ASCII_5E	KEY_6 + SHIFT_MASK			// 94 ^
#define ASCII_5F	KEY_MINUS + SHIFT_MASK			// 95 _
#define ASCII_60	KEY_TILDE				// 96 `
#define ASCII_61	KEY_A					// 97 a
#define ASCII_62	KEY_B					// 98 b
#define ASCII_63	KEY_C					// 99 c
#define ASCII_64	KEY_D					// 100 d
#define ASCII_65	KEY_E					// 101 e
#define ASCII_66	KEY_F					// 102 f
#define ASCII_67	KEY_G					// 103 g
#define ASCII_68	KEY_H					// 104 h
#define ASCII_69	KEY_I					// 105 i
#define ASCII_6A	KEY_J					// 106 j
#define ASCII_6B	KEY_K					// 107 k
#define ASCII_6C	KEY_L					// 108 l
#define ASCII_6D	KEY_M					// 109 m
#define ASCII_6E	KEY_N					// 110 n
#define ASCII_6F	KEY_O					// 111 o
#define ASCII_70	KEY_P					// 112 p
#define ASCII_71	KEY_Q					// 113 q
#define ASCII_72	KEY_R					// 114 r
#define ASCII_73	KEY_S					// 115 s
#define ASCII_74	KEY_T					// 116 t
#define ASCII_75	KEY_U					// 117 u
#define ASCII_76	KEY_V					// 118 v
#define ASCII_77	KEY_W					// 119 w
#define ASCII_78	KEY_X					// 120 x
#define ASCII_79	KEY_Y					// 121 y
#define ASCII_7A	KEY_Z					// 122 z
#define ASCII_7B	KEY_LEFT_BRACE + SHIFT_MASK		// 123 {
#define ASCII_7C	KEY_NON_US_100 + SHIFT_MASK		// 124 |
#define ASCII_7D	KEY_RIGHT_BRACE + SHIFT_MASK		// 125 }
#define ASCII_7E	KEY_BACKSLASH + SHIFT_MASK		// 126 ~
#define ASCII_7F	KEY_BACKSPACE				// 127

#define ISO_8859_1_A0	KEY_SPACE				// 160       Nonbreakng Space
#define ISO_8859_1_A1	0					// 161 Âˇ     Inverted Exclamation
#define ISO_8859_1_A2	0					// 162 Â�?     Cent SIGN
#define ISO_8859_1_A3	KEY_3 + SHIFT_MASK			// 163 Â�?     Pound Sign
#define ISO_8859_1_A4	0					// 164 Â¤     Currency or Euro Sign
#define ISO_8859_1_A5	0					// 165 ÂĄ     YEN SIGN
#define ISO_8859_1_A6	KEY_TILDE + ALTGR_MASK			// 166 Â¦     BROKEN BAR
#define ISO_8859_1_A7	0					// 167 Â§     SECTION SIGN
#define ISO_8859_1_A8	0					// 168 Â¨     DIAERESIS
#define ISO_8859_1_A9	0					// 169 Â©     COPYRIGHT SIGN
#define ISO_8859_1_AA	0					// 170 ÂŞ     FEMININE ORDINAL
#define ISO_8859_1_AB	0					// 171 Â«     LEFT DOUBLE ANGLE QUOTE
#define ISO_8859_1_AC	KEY_TILDE + SHIFT_MASK			// 172 Â¬     NOT SIGN
#define ISO_8859_1_AD	0					// 173       SOFT HYPHEN
#define ISO_8859_1_AE	0					// 174 Â®     REGISTERED SIGN
#define ISO_8859_1_AF	0					// 175 ÂŻ     MACRON
#define ISO_8859_1_B0	0					// 176 Â°     DEGREE SIGN
#define ISO_8859_1_B1	0					// 177 Â±     PLUS-MINUS SIGN
#define ISO_8859_1_B2	0					// 178 Â˛     SUPERSCRIPT TWO
#define ISO_8859_1_B3	0					// 179 Âł     SUPERSCRIPT THREE
#define ISO_8859_1_B4	0					// 180 Â´     ACUTE ACCENT
#define ISO_8859_1_B5	0					// 181 Âµ     MICRO SIGN
#define ISO_8859_1_B6	0					// 182 Â¶     PILCROW SIGN
#define ISO_8859_1_B7	0					// 183 Â·     MIDDLE DOT
#define ISO_8859_1_B8	0					// 184 Â¸     CEDILLA
#define ISO_8859_1_B9	0					// 185 Âą     SUPERSCRIPT ONE
#define ISO_8859_1_BA	0					// 186 Âş     MASCULINE ORDINAL
#define ISO_8859_1_BB	0					// 187 Â»     RIGHT DOUBLE ANGLE QUOTE
#define ISO_8859_1_BC	0					// 188 ÂĽ     FRACTION ONE QUARTER
#define ISO_8859_1_BD	0					// 189 Â˝     FRACTION ONE HALF
#define ISO_8859_1_BE	0					// 190 Âľ     FRACTION THREE QUARTERS
#define ISO_8859_1_BF	0					// 191 Âż     INVERTED QUESTION MARK
#define ISO_8859_1_C0	0					// 192 Ă€     A GRAVE
#define ISO_8859_1_C1	KEY_A + ALTGR_MASK + SHIFT_MASK		// 193 Ă�     A ACUTE
#define ISO_8859_1_C2	0					// 194 Ă‚     A CIRCUMFLEX
#define ISO_8859_1_C3	0					// 195 Ă�     A TILDE
#define ISO_8859_1_C4	0					// 196 Ă„     A DIAERESIS
#define ISO_8859_1_C5	0					// 197 Ă…     A RING ABOVE
#define ISO_8859_1_C6	0					// 198 Ă†     AE
#define ISO_8859_1_C7	0					// 199 Ă‡     C CEDILLA
#define ISO_8859_1_C8	0					// 200 Ă�     E GRAVE
#define ISO_8859_1_C9	KEY_E + ALTGR_MASK + SHIFT_MASK		// 201 Ă‰     E ACUTE
#define ISO_8859_1_CA	0					// 202 ĂŠ     E CIRCUMFLEX
#define ISO_8859_1_CB	0					// 203 Ă‹     E DIAERESIS
#define ISO_8859_1_CC	0					// 204 ĂŚ     I GRAVE
#define ISO_8859_1_CD	KEY_I + ALTGR_MASK + SHIFT_MASK		// 205 ĂŤ     I ACUTE
#define ISO_8859_1_CE	0					// 206 ĂŽ     I CIRCUMFLEX
#define ISO_8859_1_CF	0					// 207 ĂŹ     I DIAERESIS
#define ISO_8859_1_D0	0					// 208 Ă�     ETH
#define ISO_8859_1_D1	0					// 209 Ă�?     N TILDE
#define ISO_8859_1_D2	0					// 210 Ă’     O GRAVE
#define ISO_8859_1_D3	KEY_O + ALTGR_MASK + SHIFT_MASK		// 211 Ă“     O ACUTE
#define ISO_8859_1_D4	0					// 212 Ă”     O CIRCUMFLEX
#define ISO_8859_1_D5	0					// 213 Ă•     O TILDE
#define ISO_8859_1_D6	0					// 214 Ă–     O DIAERESIS
#define ISO_8859_1_D7	0					// 215 Ă—     MULTIPLICATION
#define ISO_8859_1_D8	0					// 216 Ă�     O STROKE
#define ISO_8859_1_D9	0					// 217 Ă™     U GRAVE
#define ISO_8859_1_DA	KEY_U + ALTGR_MASK + SHIFT_MASK		// 218 Ăš     U ACUTE
#define ISO_8859_1_DB	0					// 219 Ă›     U CIRCUMFLEX
#define ISO_8859_1_DC	0					// 220 Ăś     U DIAERESIS
#define ISO_8859_1_DD	0					// 221 Ăť     Y ACUTE
#define ISO_8859_1_DE	0					// 222 Ăž     THORN
#define ISO_8859_1_DF	0					// 223 Ăź     SHARP S
#define ISO_8859_1_E0	0					// 224 Ă      a GRAVE
#define ISO_8859_1_E1	KEY_A + ALTGR_MASK			// 225 Ăˇ     a ACUTE
#define ISO_8859_1_E2	0					// 226 Ă�?     a CIRCUMFLEX
#define ISO_8859_1_E3	0					// 227 Ă�?     a TILDE
#define ISO_8859_1_E4	0					// 228 Ă¤     a DIAERESIS
#define ISO_8859_1_E5	0					// 229 ĂĄ     a RING ABOVE
#define ISO_8859_1_E6	0					// 230 Ă¦     ae
#define ISO_8859_1_E7	0					// 231 Ă§     c CEDILLA
#define ISO_8859_1_E8	0					// 232 Ă¨     e GRAVE
#define ISO_8859_1_E9	KEY_E + ALTGR_MASK			// 233 Ă©     e ACUTE
#define ISO_8859_1_EA	0					// 234 ĂŞ     e CIRCUMFLEX
#define ISO_8859_1_EB	0					// 235 Ă«     e DIAERESIS
#define ISO_8859_1_EC	0					// 236 Ă¬     i GRAVE
#define ISO_8859_1_ED	KEY_I + ALTGR_MASK			// 237 Ă­     i ACUTE
#define ISO_8859_1_EE	0					// 238 Ă®     i CIRCUMFLEX
#define ISO_8859_1_EF	0					// 239 ĂŻ     i DIAERESIS
#define ISO_8859_1_F0	0					// 240 Ă°     ETH
#define ISO_8859_1_F1	0					// 241 Ă±     n TILDE
#define ISO_8859_1_F2	0					// 242 Ă˛     o GRAVE
#define ISO_8859_1_F3	KEY_O + ALTGR_MASK			// 243 Ăł     o ACUTE
#define ISO_8859_1_F4	0					// 244 Ă´     o CIRCUMFLEX
#define ISO_8859_1_F5	0					// 245 Ăµ     o TILDE
#define ISO_8859_1_F6	0					// 246 Ă¶     o DIAERESIS
#define ISO_8859_1_F7	0					// 247 Ă·     DIVISION
#define ISO_8859_1_F8	0					// 248 Ă¸     o STROKE
#define ISO_8859_1_F9	0					// 249 Ăą     u GRAVE
#define ISO_8859_1_FA	KEY_U + ALTGR_MASK			// 250 Ăş     u ACUTE
#define ISO_8859_1_FB	0					// 251 Ă»     u CIRCUMFLEX
#define ISO_8859_1_FC	0					// 252 ĂĽ     u DIAERESIS
#define ISO_8859_1_FD	0					// 253 Ă˝     y ACUTE
#define ISO_8859_1_FE	0					// 254 Ăľ     THORN
#define ISO_8859_1_FF	0					// 255 Ăż     y DIAERESIS
#define UNICODE_20AC	KEY_4 + ALTGR_MASK			//     â‚¬     Euro Sign
// not yet implemented
#define UNICODE_EXTRA00	0x20AC
#define KEYCODE_EXTRA00	KEY_4 + ALTGR_MASK			// 20AC â‚¬    Euro Sign

#endif // LAYOUT_UNITED_KINGDOM



extern const KEYCODE_TYPE keycodes_ascii[];
extern const KEYCODE_TYPE keycodes_iso_8859_1[];

#ifdef __cplusplus
} // extern "C"
#endif
