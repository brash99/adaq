/*
 * WebTerm
 * ~~~~~~~
 *
 * VT200 class: a subclass of Emulator that implements
 * standard XTerm, VT200, VT100, and VT52 emulations.
 *
 * Written by Dianne Hackborn and Melanie Johnson.
 *
 * The design and implementation of WebTerm are available for
 * royalty-free adoption and use for non-commercial purposes, by any
 * public or private organization.  Copyright is retained by the
 * Northwest Alliance for Computational Science and Engineering and
 * Oregon State University.  Redistribution of any part of WebTerm or
 * any derivative works must include this notice.
 *
 * All Rights Reserved.
 *
 * Please address correspondence to nacse-questions@nacse.org.
 *
 * ----------------------------------------------------------------------
 *
 * Known Bugs
 * ~~~~~~~~~~
 *
 * None.
 *
 * ----------------------------------------------------------------------
 *
 * History
 * ~~~~~~~
 *
 * 1.4: Completely re-wrote emulation.  Now much more ANSI-compliant,
 *      and should be much better at handling unexpected escape sequences.
 *      Added "VT220" and "VT52" synonyms for emulation.
 *      Changed name of class.
 *
 * 1.3: Created this file.
 *      Added "XTERM" and "VT100" synonyms for emulation.
 *
 */

/* VT Emulation info:

   One cost-effective document is "VT220 Programmer Pocket Guide", part
   number EK-VT220-HR-001, $21.00 (US). From the 48-state United States,
   call 1-800/DIGITAL to order this. From elsewhere, dial +1 603/884-6660.
*/

import java.applet.Applet;    
import java.awt.*;
import java.awt.Event;
import java.io.*;
import java.net.*;

//import Emulator;

/* This class encapsulates all of the information about a parsed
   command, and some basic methods to help the parser.  See the
   VT200 class for what these values mean and details on
   the format of commands. */

class Command {

  final static boolean DEBUG = false;

  // Types of commands.
  public static final int TYPE_NONE = 0;    // Non-command.
  public static final int TYPE_ESC =  1;    // ESC-term
  public static final int TYPE_CSI =  2;    // ESC-[-params-inter-term
  public static final int TYPE_DCS =  3;    // ESC-P-params-inter-term-data
  public static final int TYPE_OSC =  4;    // ESC-]-params-inter-term-data
  public static final int TYPE_PM =   5;    // ESC-^-params-inter-term-data
  public static final int TYPE_APC =  6;    // ESC-_-params-inter-term-data

  // This one is used for constructing commands that don't follow
  // a standard ASCII command structure.
  public static final int TYPE_RAW = 7;     // Just use raw data string.

  // Flags encountered during parameter parsing.
  // These are bit values.

  public static final int FLAG_LT = 0;    // '<' encountered in params.
  public static final int FLAG_LT_MASK = (1<<FLAG_LT);
  public static final int FLAG_EQ = 1;    // '=' encountered in params.
  public static final int FLAG_EQ_MASK = (1<<FLAG_EQ);
  public static final int FLAG_GT = 2;    // '>' encountered in params.
  public static final int FLAG_GT_MASK = (1<<FLAG_GT);
  public static final int FLAG_QUES = 3;  // '?' encountered in params.
  public static final int FLAG_QUES_MASK = (1<<FLAG_QUES);

  // The maximum number of parameters we can handle at one time.
  public static final int MAX_PARAMS = 64;

  // The maximum number of intermediate characters we can handle at one time.
  public static final int MAX_INTERS = 16;

  public int type = 0;         // One of VT100Emulator's state codes.

  public int cur_param = 0;    // Current parameters
  public int[] params          // Parsed parameter values
    = new int[MAX_PARAMS];
  public boolean[] has_params  // Was a value supplied?
    = new boolean[MAX_PARAMS];
  public int flags;            // Param flags: <, =, >, ?

  public int cur_inter = 0;    // Current intermediate character
  public char[] inters         // Intermediate characters supplied
    = new char[MAX_INTERS];

  public StringBuffer data     // Data string
    = new StringBuffer(64);

  public char term = 0;        // Terminating character

  public Command() {
  }

  // Reset the command.
  public void init() {
    type = 0;
    for(int i=0; i<=cur_param && i <MAX_PARAMS; i++ ) {
      params[i] = 0;
      has_params[i] = false;
    }
    cur_param = 0;
    flags = 0;
    cur_inter = 0;
    data.setLength(0);
    term = 0;
  }

  // Set the command type, e.g. TYPE_CSI.
  public boolean setType(int type) {
    this.type = type;
    return true;
  }

  /* This is the programmic interface to the command: it allows commands
     to be constructed in their logical units. */

  // Directly add a paramter to the command.
  public boolean addParam(int param,int flags) {
    if( cur_param < MAX_PARAMS ) {
      params[cur_param] = param;
      has_params[cur_param] = true;
      this.flags |= flags;
      cur_param++;
      return true;
    }
    return false;
  }

  // Skip [set to default] next parameter in command.
  public boolean skipParam() {
    if( cur_param < MAX_PARAMS ) {
      params[cur_param] = 0;
      has_params[cur_param] = false;
      cur_param++;
      return true;
    }
    return false;
  }

  // Directly add intermediate characters to the command.
  public boolean addIntermediates(String inter) {
    int i=0;
    while( i < inter.length() && cur_inter < MAX_INTERS ) {
      parseInterChar(inter.charAt(i));
      i++;
    }
    if( i >= inter.length() ) return true;
    return false;
  }

  // Directly add string data to the command.
  public boolean addData(String data) {
    this.data.append(data);
    return true;
  }
  public boolean addData(char data) {
    this.data.append(data);
    return true;
  }

  // Directly set the terminating character
  public boolean setTerminator(char t) {
    term = t;
    return true;
  }

  /* This is the parser interface to the command: it allows commands
     to be constructed one character at a time, as they occur in the
     command sequence. */

  // Parse the next character in a parameter stream or flag character.
  public boolean parseParamChar(char c) {
    if( DEBUG ) System.out.println("WebTerm: Parsing seq char " + c);
    if( c >= '0' && c <= '9' ) {
      if( cur_param < MAX_PARAMS ) {
	params[cur_param] *= 10;
	params[cur_param] += (int)(c-'0');
	has_params[cur_param] = true;
	if( DEBUG ) System.out.println("WebTerm: Param #" + cur_param +
				       " now " + params[cur_param]);
      }
      return true;
    } else if( c == ';' || c == ':' ) {
      cur_param++;
      if( cur_param > MAX_PARAMS ) cur_param = MAX_PARAMS;
      if( cur_param < MAX_PARAMS ) has_params[cur_param] = true;
      if( DEBUG ) System.out.println("WebTerm: Switch to param #" + cur_param);
      return true;
    } else if( c >= '<' && c <= '?' ) {
      flags |= 1<<((int)(c-'<'));
      return true;
    }
    return false;
  }

  // Add the next intermediate character.
  public boolean parseInterChar(char c) {
    if( cur_inter < MAX_INTERS ) {
      inters[cur_inter] = c;
      cur_inter++;
    }
    return true;
  }

  // Add the next data character.
  public boolean parseDataChar(char c) {
    data.append(c);
    return true;
  }

  // Set the terminating character.
  public boolean parseTermChar(char c) {
    if( DEBUG ) System.out.println("WebTerm: Last param: " + cur_param);
    if( cur_param >= MAX_PARAMS ) cur_param = MAX_PARAMS-1;
    if( cur_param < MAX_PARAMS ) {
      if( has_params[cur_param] ) cur_param++;
    }
    term = c;
    return true;
  }

  public String paramsToString() {
    int i;
    String str;
    str = "Params:";
    for(i=0; i<cur_param; i++) {
      if( has_params[i] )
	str = str + " " + params[i];
      else
	str = str + " <DEF>";
    }
    return str;
  }

  public String toSequence() {
    StringBuffer str = new StringBuffer(64);
    switch( type ) {
    case TYPE_ESC:
      str.append("\033");
      break;
    case TYPE_CSI:
      str.append("\033[");
      break;
    case TYPE_DCS:
      str.append("\033P");
      break;
    case TYPE_OSC:
      str.append("\033]");
      break;
    case TYPE_PM:
      str.append("\033^");
      break;
    case TYPE_APC:
      str.append("\033_");
      break;
    case TYPE_RAW:
      return data.toString();
    }
    
    if( (flags&FLAG_LT_MASK) != 0 ) str.append('<');
    if( (flags&FLAG_EQ_MASK) != 0 ) str.append('=');
    if( (flags&FLAG_GT_MASK) != 0 ) str.append('>');
    if( (flags&FLAG_QUES_MASK) != 0 ) str.append('?');

    for( int i=0; i<cur_param; i++ ) {
      if( has_params[i] ) {
	str.append(params[i]);
      }
      if( i < (cur_param-1) ) str.append(';');
    }

    if( cur_inter > 0 ) {
      str.append(inters,0,cur_inter);
    }

    str.append(term);

    if( data.length() > 0 ) {
      str.append(data.toString());
      str.append("\033\\");
    }

    return str.toString();
  }

  public String toString() {
    return Emulator.sequenceToString(toSequence());
  }
}

public class VT200 extends Emulator {

  static final boolean DEBUG = false;

  /* ------------------------------------------------------------
     CONSTANT VALUES SECTION
     ------------------------------------------------------------ */

  /* Basic types of characters:
     
     A "control character" is of the range 0x00-0x1f or 0x80-0x9f;
     the DEC control characters are defined below.

     An "intermediate character" is in the range 0x20-0x2f; they may
     occure multiple times before the last character of an escape
     sequence or CSI command.

     A "parameter character" is in the range 0x30-0x39 or 0x3b; it
     appears as "23;7;45".

     A "flag character" is in the range 0x3c-0x3f; it sets a boolean
     flag if it appears anywhere.

     A "final character" is in the range 0x40-0x7e; it terminates an
     escape sequence or CSI command.
   */

  // State of character parser.

  /* In the normal state, non-control characters is simply rendered to
     the screen. */
  static final int STATE_ASCII = Command.TYPE_NONE; // Rendering characters

  /* An escape sequence has the following form:
     1. ESC
     2. Zero or more intermediate characters
     3. Final character */
  static final int STATE_ESC = Command.TYPE_ESC;    // In escape sequence

  /* A control sequence has the following form:
     1. CSI (or ESC-[)
     2. Zero or more parameter characters
     3. Zero or more intermediate characters
     4. Final character */
  static final int STATE_CSI = Command.TYPE_CSI;    // In control seqeuence

  /* A device control sequence has the following form:
     1. DCS (or ESC-P)
     2. Zero or more parameter characters
     3. Zero or more intermediate characters
     4. Final character
     5. Data string
     6. ST (or ESC-\) */
  static final int STATE_DCS = Command.TYPE_DCS;  // In device control sequence

  /* An operating system command has the following form:
     1. OSC (or ESC-])
     2. Data string
     3. ST (or ESC-\) */
  static final int STATE_OSC = Command.TYPE_OSC; // In operating system command

  /* A privacy message has the following form:
     1. PM (or ESC-^)
     2. Data string
     3. ST (or ESC-\) */
  static final int STATE_PM = Command.TYPE_PM;      // In privacy message

  /* An application program command has the following form:
     1. APC (or ESC-_)
     2. Data string
     3. ST (or ESC-\) */
  static final int STATE_APC = Command.TYPE_APC;    // In app. program command

  /* The state when processing a command's final data string */
  static final int STATE_DATA = STATE_APC+1;        // Ends with ST.

  /* The first state in a VT52 direct cursor address; the next
     character received is the line, with ' ' being the top line. */
  static final int STATE_DCA1 = STATE_APC+2;

  /* The second state in a VT52 direct cursor address; the next
     character received is the column, with ' ' being the far left. */
  static final int STATE_DCA2 = STATE_APC+3;

  /* Operating modes.  We assign an ID to each mode we understand,
     then create arrays to help us map these IDs to particular command
     sequences and values. */

  static final int MODE_KAM = 0;      //  2  Keyboard locked / unlocked
  static final int MODE_IRM = 1;      //  4  Character insert / replace mode
  static final int MODE_SRM = 2;      //  12 Send-receive on / off
  static final int MODE_LNM = 3;      //  20 LF is new-line / linefeed
  static final int MODE_DECCKM = 4;   // ?1  Cursor mode: application / cursor
  static final int MODE_DECANM = 5;   // ?2  Emulation: (NA) / VT52
  static final int MODE_DECCOLM = 6;  // ?3  132 column / 80 column
  static final int MODE_DECSCLM = 7;  // ?4  Scrolling smooth / jump
  static final int MODE_DECSCNM = 8;  // ?5  Screen reverse / normal
  static final int MODE_DECOM = 9;    // ?6  Origin mode
  static final int MODE_DECAWM = 10;  // ?7  Autowrap on / off
  static final int MODE_DECARM = 11;  // ?8  Autorepeat on / off
  static final int MODE_MITMRM = 12;  // ?9  Send MIT mouse rep on / off
  static final int MODE_DECPFF = 13;  // ?18 Print form feed on / off
  static final int MODE_DECPEX = 14;  // ?19 Print extent screen / region
  static final int MODE_DECTCEM = 15; // ?25 Text cursor on / off
  static final int MODE_DECTEK = 16;  // ?38 Enter Tektronix mode
  static final int MODE_XTMACOL = 17; // ?40 Allow 80<->132 switch on / off
  static final int MODE_XTMCFIX = 18; // ?41 Curses(5) fix on / off
  static final int MODE_DECNRCM = 19; // ?42 Char set national / multinational
  static final int MODE_XTMMBM = 20;  // ?44 Margin bell on / off
  static final int MODE_XTMRWM = 21;  // ?45 Reverse-wrap on / off
  static final int MODE_XTMLOGM = 22; // ?46 Logging on / off
  static final int MODE_XTMASM = 23;  // ?47 Alternate / normal screen buffer
  static final int MODE_XTMMRM = 24;  // ?1000 VT200 mouse report on / off
  static final int MODE_XTMHMRM = 25; // ?1001 VT200 hilight mouse rep on / off
  // The rest are not actually mapped to CSI h and CSI l values.
  static final int MODE_DECKPAM = 26; //     Keypad application / numeric
  static final int MODE_DECELR = 27;  //     Enable DEC locator reports
  static final int MODE_DECOLR = 28;  //     This is a one-shot locator report
  static final int MODE_DECPLR = 29;  //     Locator reports in pixels / chars
  static final int _NUM_MODE = 30;

  // Flag status of each mode's command sequence.

  static final int[] mode_flags = {
    0, 0, 0, 0,                                      // KAM, IRM, SRM, LNM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // DECCKM, DECANM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // DECCOLM, DECSCLM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // DECSCNM, DECOM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // DECAWM, DECARM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // MITMRM, DECPFF
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // DECPEX, DECTCEM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // DECTEX, XTMACOL
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // XTMCFIX, DECNRCM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // XTMMBM, XTMRWM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // XTMLOGM, XTMASM
    Command.FLAG_QUES_MASK, Command.FLAG_QUES_MASK,  // XTMMRM, XTMHMRM
    0, 0,                                            // DECKPAM, DECELR
    0, 0                                             // DECOLR, DECPLR
  };

  // Parameter value of each mode's command sequence.

  static final int[] mode_values = {
     2,  4, 12, 20,    // KAM, IRM, SRM, LNM
     1,  2,  3,  4,    // DECCKM, DECANM, DECCOLM, DECSCLM
     5,  6,  7,  8,    // DECSCNM, DECOM, DECAWM, DECARM
     9, 18, 19, 25,    // MITMRM, DECPFF, DECPEX, DECTCEM
    38, 40, 41, 42,    // DECTEX, XTMACOL, XTMCFIX, DECNRCM
    44, 45, 46, 47,    // XTMMBM, XTMRWM, XTMLOGM, XTMASM,
    1000, 1001,        // XTMMRM, XTMHMRM
     // These are not standard mode syntax.
     2,  2,  2,  2     // DECKPAM, DECELR, DECOLR, DECPLR
  };

  // Initial values of modes.

  static final boolean[] mode_inits = {
    false, false, false, false, // KAM, IRM, SRM, LNM
    false, true,  false, false, // DECCKM, DECANM, DECCOLM, DECSCLM
    false, false, true,  true,  // DECSCNM, DECOM, DECAWM, DECARM
    false, false, true,  true,  // MITMRM, DECPFF, DECPEX, DECTCEM
    false, false, false, true,  // DECTEX, XTMACOL, XTMCFIX, DECNRCM
    false, false, false, false, // XTMMBM, XTMRWM, XTMLOGM, XTMASM,
    false, false,               // XTMMRM, XTMHMRM
    false, false, false, false  // DECKPAM, DECELR, DECOLR, DECPLR
  };

  // Names of the modes
  static final String[] mode_names = {
    "Keyboard action", "Insertion", "Send-receive",
    "Linefeed", "Cursor key", "ANSI/VT52", "Column",
    "Scrolling", "Screen", "Origin", "Wraparound",
    "Auto-repeat", "Send MIT mouse", "Print formfeed",
    "Print extent", "Text cursor", "Tektronix",
    "Allow column switch", "Curses fix", "Character set",
    "Margin bell", "Reverse-wraparound", "Logging",
    "Alternate screen", "VT200 mouse report",
    "VT200 hilight mouse report",
    "Keypad", "Enable locator reports", "One-shot locator report",
    "Pixel-unit locator reports"
  };

  // Abbreviations of mode names
  static final String[] mode_abbrvs = {
    "KAM", "IRM", "SRM", "LNM",
    "DECCKM", "DECANM", "DECCOLM", "DECSCLM",
    "DECSCNM", "DECOM", "DECAWM", "DECARM",
    "MITMRM", "DECPFF", "DECPEX", "DECTCEM",
    "DECTEX", "XTMACOL", "XTMCFIX", "DECNRCM",
    "XTMMBM", "XTMRWM", "XTMLOGM", "XTMASM",
    "XTMMWM", "XTMHMRM",
    "DECKPAM", "DECELR", "DECOLR", "DECPLR"
  };

  // Mode operations that can be performed
  static final int MOP_RESET = 0;    // CSI Pn l
  static final int MOP_SET = 1;      // CSI Pn h
  static final int MOP_SAVE = 2;     // CSI Pn r
  static final int MOP_RESTORE = 3;  // CSI Pn s

  // Character codes that need to be directly handled by emulator.
  final char[] special_chars = {
    ENQ, BS, HT, LF, VT, FF, SO, SI, DC1, DC3, CAN, SUB, ESC, DEL,
    IND, NEL, HTS, RI, SS2, SS3, DCS, CSI, ST, OSC, PM, APC };

  /* Types of emulations available. */
  static final int EMU_XTERM = 0;
  static final int EMU_VT220 = 1;
  static final int EMU_VT100 = 2;
  static final int EMU_VT52 = 3;

  /* ------------------------------------------------------------
     INSTANCE DATA SECTION
     ------------------------------------------------------------ */

  // The current parsing state.
  int state = STATE_ASCII;

  // Current command being parsed.
  Command cmd = new Command();

  /* Sub-command being executed (e.g., IND within CSI).
     We need this because we transform 8-bit codes to their
     ESC-Fe counterpart, and call doESC to execute them. */
  Command sub_cmd = new Command();

  // 'true' for characters we need to directly handle.
  // (this is constructed from the 'special_chars' array.)
  boolean[] char_flags = new boolean[256];

  // Current state of modes.
  boolean[] mode_states = new boolean[_NUM_MODE];

  // Saved state of modes.
  boolean[] mode_saves = new boolean[_NUM_MODE];

  // Current emulation type
  int emulation = EMU_XTERM;

  // Current columns where there are tabstops -- true is a tabstop.
  boolean[] tabs = new boolean[512];  // lots of room!

  // The line value processed in a VT52 direct cursor address.
  int dca_line = 0;

  // ** NOTE: We also need to save the rendering mode and some flags.
  int saved_row = 0;
  int saved_col = 0;
  int saved_style = Terminal.STYLE_PLAIN;

  /* ------------------------------------------------------------
     CONSTRUCTOR AND EMULATOR METHODS WE OVERRIDE.
     ------------------------------------------------------------ */

  public VT200() {
    // Flag the ASCII characters we need to directly deal with.
    for( int i=0; i<char_flags.length; i++ ) {
      char_flags[i] = false;
    }
    for( int i=0; i<special_chars.length; i++ ) {
      char_flags[(int)special_chars[i]] = true;
    }

    resetTerminal(false);
  }

  public int getCurType() {
    switch( emulation ) {
    case EMU_XTERM:
      return 0;
    case EMU_VT220:
      return 1;
    case EMU_VT100:
      return 3;
    case EMU_VT52:
      return 5;
    }
    return -1;
  }

  public String getTypeName(int num) {
    switch(num) {
    case 0:
      mode_states[MODE_DECANM] = true;
      emulation = EMU_XTERM;
      return "XTERM";
    case 1:
      mode_states[MODE_DECANM] = true;
      emulation = EMU_VT220;
      return "DEC-VT220";
    case 2:
      mode_states[MODE_DECANM] = true;
      emulation = EMU_VT220;
      return "VT220";
    case 3:
      mode_states[MODE_DECANM] = true;
      emulation = EMU_VT100;
      return "DEC-VT100";
    case 4:
      mode_states[MODE_DECANM] = true; 
      emulation = EMU_VT100;
      return "VT100";
    case 5:
      mode_states[MODE_DECANM] = false;
      emulation = EMU_VT52;
      return "DEC-VT52";
    case 6:
      mode_states[MODE_DECANM] = false;
      emulation = EMU_VT52;
      return "VT52";
    }
    return null;
  }

  public boolean checkChar(char c) {
    if( DEBUG ) if( c == HT ) System.out.println("WebTerm: SPEC Char: HT");
    if( state == STATE_ASCII && !char_flags[(int)c] ) {
      return true;
    }
    return false;
  }

  /* Handling of complex (non-character) types of events. */

  void build_lr(Command cmd, Event e) {
    // XXX not implemented.
  }

  public boolean handleEvent(Event e) {
    Command scmd = null;
    if( e.id == e.KEY_ACTION ) {
      scmd = new Command();
      if( e.key == e.LEFT ) {
	if( mode_states[MODE_DECCKM] && mode_states[MODE_DECANM] ) {
	  scmd.setType(Command.TYPE_ESC);
	  scmd.addIntermediates("O");
	  scmd.setTerminator('D');
	} else {
	  scmd.setType(Command.TYPE_CSI);
	  scmd.setTerminator('D');
	}
      } else if( e.key == e.RIGHT ) {
	if( mode_states[MODE_DECCKM] && mode_states[MODE_DECANM] ) {
	  scmd.setType(Command.TYPE_ESC);
	  scmd.addIntermediates("O");
	  scmd.setTerminator('C');
	} else {
	  scmd.setType(Command.TYPE_CSI);
	  scmd.setTerminator('C');
	}
      } else if( e.key == e.UP ) {
	if( mode_states[MODE_DECCKM] && mode_states[MODE_DECANM] ) {
	  scmd.setType(Command.TYPE_ESC);
	  scmd.addIntermediates("O");
	  scmd.setTerminator('A');
	} else {
	  scmd.setType(Command.TYPE_CSI);
	  scmd.setTerminator('A');
	}
      } else if( e.key == e.DOWN ) {
	if( mode_states[MODE_DECCKM] && mode_states[MODE_DECANM] ) {
	  scmd.setType(Command.TYPE_ESC);
	  scmd.addIntermediates("O");
	  scmd.setTerminator('B');
	} else {
	  scmd.setType(Command.TYPE_CSI);
	  scmd.setTerminator('B');
	}
      } else if( e.key == e.HOME ) {
	// XXX currently ignore it.
	return false;
      } else if( e.key == e.END ) {
	// XXX currently ignore it.
	return false;
      } else {
	scmd.setType(Command.TYPE_CSI);

	switch( e.key ) {
	case Event.PGUP: scmd.addParam(5,0); break;
	case Event.PGDN: scmd.addParam(6,0); break;
	case Event.F1: scmd.addParam(11,0); break;
	case Event.F2: scmd.addParam(12,0); break;
	case Event.F3: scmd.addParam(13,0); break;
	case Event.F4: scmd.addParam(14,0); break;
	case Event.F5: scmd.addParam(15,0); break;
	case Event.F6: scmd.addParam(17,0); break;
	case Event.F7: scmd.addParam(18,0); break;
	case Event.F8: scmd.addParam(19,0); break;
	case Event.F9: scmd.addParam(20,0); break;
	case Event.F10: scmd.addParam(21,0); break;
	case Event.F11: scmd.addParam(23,0); break;
	case Event.F12: scmd.addParam(24,0); break;
	default: return false;
	}

	scmd.setTerminator('~');
      }

    } else if( e.id == e.MOUSE_DOWN || e.id == e.MOUSE_UP ) {
      scmd = new Command();
      if( mode_states[MODE_DECELR] ) {
	build_lr(scmd,e);
      } else if( mode_states[MODE_MITMRM] || mode_states[MODE_XTMMRM] ||
	  mode_states[MODE_XTMHMRM] ) {
	char button = ' ';
	if( (e.modifiers&e.META_MASK) != 0 ) {
	  button = '"';
	} else if( (e.modifiers&e.ALT_MASK) != 0 ) {
	  button = '!';
	}
	if( e.id == e.MOUSE_UP ) {
	  if( mode_states[MODE_MITMRM] ) return false;
	  button = '#';
	}
	char x = '!';
	char y = '!';
	if( terminal != null ) {
	  x = (char)(terminal.xToCol(e.x)+'!');
	  y = (char)(terminal.yToRow(e.y)+'!');
	}
	scmd.setType(Command.TYPE_RAW);
	scmd.addData(ESC);
	scmd.addData('[');
	scmd.addData('M');
	scmd.addData(button);
	scmd.addData(x);
	scmd.addData(y);
      }
      if( scmd.type == Command.TYPE_NONE ) return false;
    }

    if( scmd != null && scmd.type != Command.TYPE_NONE ) {
      if( DEBUG ) System.out.println("WebTerm: Sending event: " +
				     scmd.toString());
      send(scmd.toSequence());
      return true;
    }
    return false;
  }

  /* Main character parsing function: handles control characters and the
     main state machine; dispatches command sequences to approriate
     methods for executing them. */

  public boolean doChar(char c) {

    // Handle an Xterm non-ANSIness in the DATA state --
    // the data is terminated by -any- non-printing character.
    if( emulation == EMU_XTERM && state == STATE_DATA ) {
      if( c <= (char)0x1f || ( c >= (char)0x80 && c <= (char)0x9f) ) c = ST;
    }

    // First, process control characters.

    switch( c ) {

      // These first two cancel the current command.
    case CAN:
      if( DEBUG ) System.out.println("WebTerm: CAN (Cancel)");
      reset_state();
      return false;
    case SUB:
      if( DEBUG ) System.out.println("WebTerm: SUB (Substitute)");
      reset_state();
      if( terminal != null ) {
	terminal.setStyle(terminal.getStyle() ^ terminal.STYLE_INVERSE);
	terminal.write('?',terminal.OUTF_PARTIAL);
	terminal.setStyle(terminal.getStyle() ^ terminal.STYLE_INVERSE);
      }
      return false;

      // ENQ immediately sends an ACK.
    case ENQ:
      if( DEBUG ) System.out.println("WebTerm: ENQ (Enquire)");
      if( telnet != null ) send(ACK);
      return false;
      
      // BS moves cursor to the left.
    case BS:
      if( DEBUG ) System.out.println("WebTerm: BS (Backspace)");
      if( terminal != null ) {
	int col = terminal.getCursorCol();
	if( col > 0 ) col--;
	terminal.setCursorPos(terminal.getCursorRow(),col);
      }
      // An Xterm non-ANSIness.
      if( emulation == EMU_XTERM ) reset_state();
      return false;

      // HT moves to next horizontal tab stop.
      // XXX tabs aren't fully implemented!
    case HT:
      if( DEBUG ) System.out.println("WebTerm: HT (Horizontal Tab)");
      if( DEBUG ) System.out.println("WebTerm: Doing HI...");
      if( terminal != null ) {
	int col = terminal.getCursorCol()+1;
	int last = terminal.getRegionRight();
	while( col < tabs.length && col <= last && !tabs[col] ) col++;
	if( col > last ) col = last;
	terminal.setCursorPos(terminal.getCursorRow(),col);
      }
      // An Xterm non-ANSIness.
      if( emulation == EMU_XTERM ) reset_state();
      return false;

      // LF, VT, and FF all move to next line.
    case LF:
    case VT:
    case FF:
      //if( DEBUG ) System.out.println("WebTerm: LF, VT, or FF");
      if( terminal != null ) {
	terminal.write('\n',terminal.OUTF_PARTIAL);
      }
      // An Xterm non-ANSIness.
      if( emulation == EMU_XTERM ) reset_state();
      return false;

      // Character sets are not currently implented.  Awww...
    case SO:
    case SI:
      if( DEBUG ) System.out.println("WebTerm: SO or SI");
      // An Xterm non-ANSIness.
      if( emulation == EMU_XTERM ) reset_state();
      return false;

      // DC1 and DC3 are ignored.
    case DC1:
    case DC3:
      if( DEBUG ) System.out.println("WebTerm: DC1 or DC3");
      return false;

      // Escape cancels the current command, and starts a new one.
      // However, be aware that ESC-\ is ST, the command data terminator!
      // We thus can't just initialize our state right now...
    case ESC:
      if( DEBUG ) System.out.println("WebTerm: Entered ESC");
      state = STATE_ESC;
      return false;

      // Transform 8-bit escapes into their respective escape sequence.
    default:
      if( c >= (char)0x80 && c <= (char)0x9f ) {
	if( DEBUG ) System.out.println("WebTerm: 8bit: " + charToString(c));
	sub_cmd.init();
	sub_cmd.type = STATE_ESC;
	sub_cmd.term = (char)( c - ( (char)0x80 - '@' ) );
	doESC(sub_cmd);
	return false;
      }
    }

    switch( state ) {

      /* Process normally -- display the characters. */
    case STATE_ASCII:
      return true;

      /* Process a basic escape sequence -- intermediate characters
	 followed by a terminal character. */
    case STATE_ESC:
      if( c == '\\' ) {
	sub_cmd.init();
	sub_cmd.type = STATE_ESC;
	sub_cmd.term = '\\';
	doESC(sub_cmd);
	return false;
      } else if( mode_states[MODE_DECANM] &&
		 c >= (char)0x20 && c <= (char)0x2f ) {
	cmd.parseInterChar(c);
	return false;
      } else if( (c >= (char)0x20 && c <= (char)0x7e) || c >= (char)0xa0 ) {
	cmd.parseTermChar(c);
	cmd.type = Command.TYPE_ESC;
	doESC(cmd);
	if( state == STATE_ESC ) {
	  reset_state();
	}
	return false;
      }
      return true;

      /* Process control sequence -- parameters followed by intermediate
	 characters followed by terminal character. */
    case STATE_CSI:
      if( c >= (char)0x30 && c <= (char)0x3f ) {
	cmd.parseParamChar(c);
	return false;
      } else if( c >= (char)0x20 && c <= (char)0x2f ) {
	cmd.parseInterChar(c);
	return false;
      } else if( (c >= (char)0x20 && c <= (char)0x7e) || c >= (char)0xa0 ) {
	// We let all printable characters terminate the control sequence,
	// to handle some non-ANSI sequences.
	cmd.parseTermChar(c);
	doCSI(cmd);
	reset_state();
	return false;
      }
      return true;

      /* Process other sequence -- parameters followed by intermediate
	 characters followed by terminal character, then go into
	 data processing state. */
    case STATE_DCS:
    case STATE_OSC:
    case STATE_PM:
    case STATE_APC:
      // Another Xterm non-ANSIness.  *sigh*
      if( emulation == EMU_XTERM ) {
	if( c < '0' || c > '9' ) {
	  cmd.parseTermChar(c);
	  state = STATE_DATA;
	  return false;
	}
      }
      if( c >= (char)0x30 && c <= (char)0x3f ) {
	cmd.parseParamChar(c);
	return false;
      } else if( c >= (char)0x20 && c <= (char)0x2f ) {
	cmd.parseInterChar(c);
	return false;
      } else if( (c >= (char)0x20 && c <= (char)0x7e) || c >= (char)0xa0 ) {
	cmd.parseTermChar(c);
	state = STATE_DATA;
	return false;
      }

      return true;

      /* Collect command data.  ST will be intercepted above, to cause
	 the final command to actually be executed. */
    case STATE_DATA:
      if( (c >= (char)0x20 && c <= (char)0x7e) || c >= (char)0xa0 ) {
	cmd.parseDataChar(c);
	return false;
      }

      return true;

      /* Process a VT52 direct-cursor address command. */
    case STATE_DCA1:
      dca_line = (int)(c-' ');
      state = STATE_DCA2;
      return false;
    case STATE_DCA2:
      if( terminal != null ) {
	terminal.setCursorPos(dca_line,(int)(c-' '));
      }
      reset_state();
      return false;

    default:
      state = STATE_ASCII;
      System.out.println("WebTerm: *** Unknown VT100 emulator state!");
      return false;
    }
  }

  /* ------------------------------------------------------------
     INITIALIZATION AND GLOBAL CONTROL.
     ------------------------------------------------------------ */

  /* Set modes to initial states. */

  void init_modes() {
    for( int i=0; i<_NUM_MODE; i++ ) {
      mode_states[i] = mode_saves[i] = mode_inits[i];
    }
  }

  /* Set tabs to defaults -- every eight characters. */

  void init_tabs() {
    for( int i=0; i<tabs.length; i+=1 ) {
      tabs[i] = (i%8)==0;
    }
  }

  /* Reset the state machine back to displaying ASCII characters. */

  void reset_state() {
    if( state != STATE_ASCII ) {
      cmd.init();
    }
    state = STATE_ASCII;
  }

  /* Reflect the emulation mode back into the Terminal's settings. */

  void update_terminal_mode() {
    if( terminal != null ) {
      int mode = 0;
      if( mode_states[MODE_DECSCLM] ) mode |= Terminal.MODE_SMOOTH;
      if( !mode_states[MODE_DECAWM] ) mode |= Terminal.MODE_NOWRAP;
      if( mode_states[MODE_DECSCNM] ) mode |= Terminal.MODE_INVERSE;
      if( mode_states[MODE_LNM] ) mode |= Terminal.MODE_NEWLINE;
      terminal.setMode(mode);
      terminal.setCursorVisible(mode_states[MODE_DECTCEM]);
    }
  }
  
  /* Do a reset of the entire terminal.  If "hard" is true, this is a
     full hard reset. */

  public void resetTerminal(boolean hard) {
    init_modes();
    init_tabs();
    reset_state();
    if( terminal != null ) {
      terminal.reset(hard);
      update_terminal_mode();
    }
  }

  /* ------------------------------------------------------------
     ESC PARSING
     ------------------------------------------------------------ */

  public void doESC(Command mycmd) {

    if( mycmd.cur_inter <= 0 ) {
      switch( mycmd.term ) {

	// VT52 cursor up sequence
      case 'A':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Cursor up.");
	if( terminal != null ) {
	  int row = terminal.getCursorRow()-1;
	  if( row < 0 ) row = 0;
	  terminal.setCursorPos(row,terminal.getCursorCol());
	}
	return;

	// VT52 cursor down sequence
      case 'B':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Cursor down.");
	if( terminal != null ) {
	  int row = terminal.getCursorRow()+1;
	  if( row >= terminal.getNumRows() ) row = terminal.getNumRows()-1;
	  terminal.setCursorPos(row,terminal.getCursorCol());
	}
	return;

	// VT52 cursor right sequence
      case 'C':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Cursor right.");
	if( terminal != null ) {
	  int col = terminal.getCursorCol()+1;
	  if( col >= terminal.getNumCols() ) col = terminal.getNumCols()-1;
	  terminal.setCursorPos(terminal.getCursorRow(),col);
	}
	return;

	// IND moves cursor down, possibly scrolling.
	// In VT52 mode, this is cursor left.
      case 'D':
	if( mode_states[MODE_DECANM] ) {
	  if( DEBUG ) System.out.println("WebTerm: IND (Index)");
	  if( terminal != null ) {
	    terminal.write('\n',terminal.OUTF_PARTIAL);
	  }
	} else {
	  if( DEBUG ) System.out.println("WebTerm: (VT52) Cursor left.");
	  if( terminal != null ) {
	    int col = terminal.getCursorCol()-1;
	    if( col < 0 ) col = 0;
	    terminal.setCursorPos(terminal.getCursorRow(),col);
	  }
	}
	return;

	// NEL moves cursor down and to column zero, possibly scrolling.
      case 'E':
	if( DEBUG ) System.out.println("WebTerm: NEL (Next Line)");
	if( terminal != null ) {
	  int row = terminal.getCursorRow();
	  if( row < terminal.getNumRows() ) row++;
	  else terminal.screenScroll(-1);
	  terminal.setCursorPos(row,0);
	}
	return;

	// HTS sets a tab stop.
	// In VT52 mode, this is cursor home.
      case 'H':
	if( mode_states[MODE_DECANM] ) {
	  if( DEBUG ) System.out.println("WebTerm: HTS (Horizontal Tab Set)");
	  if( terminal != null ) {
	    int col = terminal.getCursorCol();
	    if( col < tabs.length ) tabs[col] = true;
	  }
	} else {
	  if( DEBUG ) System.out.println("WebTerm: (VT52) Cursor home");
	  if( terminal != null ) {
	    terminal.setCursorPos(0,0);
	  }
	}
	return;

	// HTJ.  Not a VT220 code.
	// In VT52 mode, this is reverse line feed.
      case 'I':
	if( mode_states[MODE_DECANM] ) {
	  if( DEBUG )
	    System.out.println("WebTerm: HTJ (Horizontal Tab W/Justification)");
	} else {
	  if( DEBUG ) System.out.println("WebTerm: (VT52) Reverse line feed");
	  if( terminal != null ) {
	    int row = terminal.getCursorRow();
	    if( row > 0 ) row--;
	    else terminal.screenScroll(1);
	    terminal.setCursorPos(row,terminal.getCursorCol());
	  }
	}
	return;

	// VT52 Erase to end of screen sequence
      case 'J':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Erase to end of screen.");
	if( terminal != null ) terminal.screenClearEOD(' ',
						       terminal.getStyle());
	return;

	// PLD and PLU.  Not a VT220 code.
	// In VT52 mode, this is erase to end of line.
      case 'K':
	if( mode_states[MODE_DECANM] ) {
	  if( DEBUG ) System.out.println("WebTerm: PLD (Partial Line Down)");
	} else {
	  if( DEBUG ) System.out.println("WebTerm: (VT52) Erase to end of line.");
	  if( terminal != null ) terminal.screenClearEOL(' ',
							 terminal.getStyle());
	}
	return;
      case 'L':
	if( DEBUG ) System.out.println("WebTerm: PLU (Partial Line Up)");
	return;

	// RI moves cursor up, possible scrolling
      case 'M':
	if( DEBUG ) System.out.println("WebTerm: RI (Reverse Index)");
	if( terminal != null ) {
	  int row = terminal.getCursorRow();
	  if( row > 0 ) row--;
	  else terminal.screenScroll(1);
	  terminal.setCursorPos(row,terminal.getCursorCol());
	}
	return;

	// SS2 and SS3.  XXX Character sets are not implemented!
      case 'N':
	if( DEBUG ) System.out.println("WebTerm: SS2 (Single Shift G2)");
	return;
      case 'O':
	if( DEBUG ) System.out.println("WebTerm: SS3 (Single Shift G3)");
	return;

	// DCS cancels the current command and starts a new
	// device control string.
      case 'P':
	reset_state();
	if( DEBUG ) System.out.println("WebTerm: Entered DCS");
	cmd.type = state = STATE_DCS;
	return;

	// VT52 print cursor line: a do-nothing for this emulation.
      case 'V':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Print cursor line");
	return;

	// VT52 printer controller modes: a do-nothing for this emulation.
      case 'W':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Enter printer controller mode");
	return;
      case 'X':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Exit printer controller mode");
	return;

	// VT52 direct cursor address sequence.
      case 'Y':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Direct cursor address");
	state = STATE_DCA1;
	return;

	// VT52 identify sequence.  XXX Not implemented!
      case 'Z':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Identify");
	sendDA(DA_VT52);
	return;

	// VT52 enter ANSI sequence.
      case '<':
	doMode(MODE_DECANM,MOP_SET);
	return;

	// VT52 enter autoprint mode: a do-nothing for this emulation.
      case ' ':
	if( DEBUG ) System.out.println("WebTerm: (VT52) Enter autoprint mode");
	return;

	// RIS resets the terminal.
      case 'c':
	if( DEBUG ) System.out.println("WebTerm: RIS (Reset to Initial State)");
	reset_state();
	resetTerminal(true);
	return;

	// CSI cancels the current command and starts a new
	// control sequence.
      case '[':
	reset_state();
	if( DEBUG ) System.out.println("WebTerm: Entered CSI");
	cmd.type = state = STATE_CSI;
	return;

	// OSC cancels the current command and starts a new
	// operating system command.
	// In VT52 mode, this is print screen: a do-nothing for this emulation.
      case ']':
	if( mode_states[MODE_DECANM] ) {
	  reset_state();
	  if( DEBUG ) System.out.println("WebTerm: Entered OSC");
	  cmd.type = state = STATE_OSC;
	} else {
	  if( DEBUG ) System.out.println("WebTerm: (VT52) Print screen");
	}
	return;

	// PM cancels the current command and starts a new
	// privacy message.
      case '^':
	reset_state();
	if( DEBUG ) System.out.println("WebTerm: Entered PM");
	cmd.type = state = STATE_PM;
	return;

	// APC cancels the current command and starts a new
	// application program command.
	// In VT52 mode, this is the exit autoprint sequence.
      case '_':
	if( mode_states[MODE_DECANM] ) {
	  reset_state();
	  if( DEBUG ) System.out.println("WebTerm: Entered APC");
	  cmd.type = state = STATE_APC;
	} else {
	  if( DEBUG ) System.out.println("WebTerm: (VT52) Exit autoprint mode");
	}
	return;

	// ST terminates the collection of a command's data,
	// and executes the final command.
      case '\\':
	if( DEBUG ) System.out.println("WebTerm: ST (String Terminator)");
	if( state == STATE_DATA ) {
	  switch(cmd.type) {
	  case STATE_DCS:
	    doDCS(cmd);
	    reset_state();
	    return;
	  case STATE_OSC:
	    doOSC(cmd);
	    reset_state();
	    return;
	  case STATE_PM:
	    doPM(cmd);
	    reset_state();
	    return;
	  case STATE_APC:
	    doAPC(cmd);
	    reset_state();
	    return;
	  default:
	    System.out.println("WebTerm: Unknown state at string terminator.");
	    reset_state();
	    return;
	  }
	}
	System.out.println("WebTerm: String terminator outside of string.");
	return;

	// The rest are non-ANSI escape sequences.

      case '>':        // Normal keypad mode
	doMode(MODE_DECKPAM,MOP_RESET);
	return;
      case '=':        // Application keypad mode
	doMode(MODE_DECKPAM,MOP_SET);
	return;
      case '8':        // Restore cursor position
	if( DEBUG ) System.out.println("WebTerm: DECRC: Restore cursor.");
	if( terminal != null ) {
	  terminal.setCursorPos(saved_row,saved_col);
	  terminal.setStyle(saved_style);
	}
	return;
      case '7':        // Save cursor position
	if( DEBUG ) System.out.println("WebTerm: DECSC: Save cursor.");
	if( terminal != null ) {
	  saved_row = terminal.getCursorRow();
	  saved_col = terminal.getCursorCol();
	  saved_style = terminal.getStyle();
	}
	return;
      }

    }

    // Commands with "#" intermediate character.
    if( mycmd.cur_inter == 1 && mycmd.inters[0] == '#' ) {
      switch( mycmd.term ) {
      case '3':
	if( DEBUG ) System.out.println("WebTerm: DECDHL: Double-height (top)");
	return;
      case '4':
	if( DEBUG ) System.out.println("WebTerm: DECDHL: Double-height (bottom)");
	return;
      case '5':
	if( DEBUG ) System.out.println("WebTerm: DECSWL: Single-width");
	return;
      case '6':
	if( DEBUG ) System.out.println("WebTerm: DECDWL: Double-width");
	return;
      }
    }

    mycmd.setType(Command.TYPE_ESC);
    System.out.println("WebTerm: Unknown ESC: " + mycmd.toString());
  }

  /* ------------------------------------------------------------
     CSI PARSING
     ------------------------------------------------------------ */

  public void doCSI(Command mycmd) {

    int mop = -1;

    switch( mycmd.term ) {
    case 'h':
      mop = MOP_SET;
      break;
    case 'l':
      mop = MOP_RESET;
      break;
    case 's':
      if( mycmd.flags == Command.FLAG_QUES_MASK ) mop = MOP_SAVE;
      break;
    case 'r':
      if( mycmd.flags == Command.FLAG_QUES_MASK ) mop = MOP_RESTORE;
      break;
    }

    if( mop >= 0 ) {
      int flags = mycmd.flags;

      // Cycle through each parameter
      for( int i=0; i<mycmd.cur_param; i++ ) {
	int param = mycmd.params[i];

	// Look for a mode matching the parameter.
	int j = 0;
	while( j<_NUM_MODE ) {
	  if( mode_flags[j] == flags && mode_values[j] == param ) {
	    doMode(j,mop);
	    break;
	  }
	  j++;
	}

	if( j >= _NUM_MODE ) {
	  System.out.println("WebTerm: Unknown Mode: " + param +
			     "(Flags=" + flags + ", Op=" + mop + ")");
	}

      }
      return;
    }

    // Standard commands.
    if( mycmd.flags == 0 && mycmd.cur_inter <= 0 ) {
      int cnt = mycmd.cur_param;
      int[] args = mycmd.params;
      boolean[] given = mycmd.has_params;

      switch( mycmd.term ) {
      case '@':                     // ICH Insert blanks
	if( DEBUG ) System.out.println("WebTerm: ICH: Insert characters.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  if( !given[0] ) num = 1;
	  terminal.screenInsertChars(terminal.getCursorCol(),
				     terminal.getCursorCol()+num,
				     ' ', terminal.STYLE_PLAIN);
	}
	break;
      case 'A':                     // CUU: Cursor up
	if( DEBUG ) System.out.println("WebTerm: CUU: Cursor up.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  int row;
	  if( !given[0] ) num = 1;
	  row = terminal.getCursorRow()-num;
	  terminal.setCursorPos(row,terminal.getCursorCol());
	}
	break;
      case 'B':                     // CUD: Cursor down
	if( DEBUG ) System.out.println("WebTerm: CUD: Cursor down.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  int row;
	  if( !given[0] ) num = 1;
	  row = terminal.getCursorRow()+num;
	  terminal.setCursorPos(row,terminal.getCursorCol());
	}
	break;
      case 'C':                     // CUF: Cursor right
	if( DEBUG ) System.out.println("WebTerm: CUF: Cursor forward.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  int col;
	  if( !given[0] ) num = 1;
	  col = terminal.getCursorCol()+num;
	  terminal.setCursorPos(terminal.getCursorRow(),col);
	}
	break;
      case 'D':                     // CUB: Cursor left
	if( DEBUG ) System.out.println("WebTerm: CUB: Cursor backward.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  int col;
	  if( !given[0] ) num = 1;
	  col = terminal.getCursorCol()-num;
	  terminal.setCursorPos(terminal.getCursorRow(),col);
	}
	break;
      case 'E':                     // CNL: Cursor to next line
	if( DEBUG ) System.out.println("WebTerm: CNL: Cursor next line.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  // XXX Don't know the correct implementation...
	  int num = args[0];
	  int row, last;
	  if( !given[0] ) num = 1;
	  row = terminal.getCursorRow()+num;
	  last = terminal.getRegionBottom();
	  if( row > last ) {
	    terminal.screenScroll(last-row);
	    row = last;
	  }
	  terminal.setCursorPos(row,0);
	}
	break;
      case 'F':                     // CPL: Cursor to previous line
	if( DEBUG ) System.out.println("WebTerm: CPL: Cursor previous line.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  // XXX Don't know the correct implementation...
	  int num = args[0];
	  int row, first;
	  if( !given[0] ) num = 1;
	  row = terminal.getCursorRow()-num;
	  first = terminal.getRegionTop();
	  if( row < first ) {
	    terminal.screenScroll(first-row);
	    row = first;
	  }
	  terminal.setCursorPos(row,0);
	}
	break;
      case 'H':                     // CUP or HVP: Set cursor pos
      case 'f':
	if( DEBUG ) {
	  if( mycmd.term == 'H' ) {
	    System.out.println("WebTerm: CUP: Cursor position.");
	  } else {
	    System.out.println("WebTerm: HVP: Horiz/vert position.");
	  }
	}
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int row = terminal.getCursorRow();
	  int col = terminal.getCursorCol();
	  // Yeach, this is not written well!
	  if( cnt == 0 ) {
	    if( mode_states[MODE_DECOM] ) {
	      row = terminal.getRegionTop();
	      col = terminal.getRegionLeft();
	    } else {
	      row = col = 0;
	    }
	  } else {
	    if( given[0] && cnt >= 1 ) {
	      row = args[0]-1;
	      if( mode_states[MODE_DECOM] ) {
		row += terminal.getRegionTop();
	      }
	    }
	    if( given[1] && cnt >= 2 ) {
	      col = args[1]-1;
	      if( mode_states[MODE_DECOM] ) {
		col += terminal.getRegionLeft();
	      }
	    }
	  }
	  if( mode_states[MODE_DECOM] ) {
	    if( row > terminal.getRegionBottom() ) {
	      row = terminal.getRegionBottom();
	    }
	    if( col > terminal.getRegionRight() ) {
	      col = terminal.getRegionRight();
	    }
	  }
	  terminal.setCursorPos(row,col);
	}
	break;
      case 'I':                     // CHT: Move to tabstop N
	if( DEBUG ) System.out.println("WebTerm: CHT: Cursor horizontal tab.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	// XXX this may not be completely right...
	if( terminal != null ) {
	  int i=0;
	  int right = terminal.getRegionRight();
	  if( right >= tabs.length ) right = tabs.length-1;
	  for( int num=0; i<=right && num<=mycmd.cur_param; i++ ) {
	    if( tabs[i] ) num++;
	  }
	  terminal.setCursorPos(terminal.getCursorRow(),i);
	}
	break;
      case 'J':                     // ED: Erase in display
	if( DEBUG ) System.out.println("WebTerm: ED: Erase in display.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  if( !given[0] || args[0] == 0 )
	    terminal.screenClearEOD(' ',terminal.getStyle());
	  else if( args[0] == 1 )
	    terminal.screenClearBOD(' ',terminal.getStyle());
	  else if( args[0] == 2 )
	    terminal.screenScroll(-(terminal.getRegionBottom()-
				    terminal.getRegionTop()+1));
	}
	break;
      case 'K':                     // EL: Erase in line
	if( DEBUG ) System.out.println("WebTerm: EL: Erase in line.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  if( !given[0] || args[0] == 0 )
	    terminal.screenClearEOL(' ',terminal.getStyle());
	  else if( args[0] == 1 )
	    terminal.screenClearBOL(' ',terminal.getStyle());
	  else if( args[0] == 2 )
	    terminal.screenClearLine(' ',terminal.getStyle());
	}
	break;
      case 'L':                     // IL: Insert lines at cursor
	if( DEBUG ) System.out.println("WebTerm: IL: Insert line.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  if( !given[0] ) num = 1;
	  int row = terminal.getCursorRow();
	  if( row < 0 ) row = 0;
	  terminal.screenScrollRegion(row,terminal.getRegionBottom(),num);
	}
	break;
      case 'M':                     // DL: Delete lines at cursor
	if( DEBUG ) System.out.println("WebTerm: DL: Delete line.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  if( !given[0] ) num = 1;
	  terminal.screenScrollRegion(terminal.getCursorRow(),
				      terminal.getRegionBottom(),-num);
	}
	break;
      case 'P':                     // DCH: Delete character
	if( DEBUG ) System.out.println("WebTerm: DCH: Delete character");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  if( !given[0] ) num = 0;
	  terminal.screenDeleteChars(terminal.getCursorCol(),
				     terminal.getCursorCol(),
				     ' ', Terminal.STYLE_PLAIN);
	}
	break;
      case 'X':                     // ECH: Erase character
	if( DEBUG ) System.out.println("WebTerm: ECH: Erase character");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int num = args[0];
	  if( !given[0] ) num = 0;
	  terminal.screenSetChars(terminal.getCursorCol(),
				  terminal.getCursorCol()+num,
				  ' ', Terminal.STYLE_PLAIN);
	}
	break;
      case 'c':                     // DA: Device attr report
	if( cnt <= 0 || (cnt == 1 && args[0] == 0) ) {
	  sendDA(DA_PRIMARY);
	} else {
	  System.out.println("WebTerm: Unknown DA: " + mycmd.toString());
	}
	break;
      case 'g':
	if( DEBUG ) System.out.println("WebTerm: TBC: Tabulation clear.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  if( mycmd.cur_param <= 0 || mycmd.params[0] == 0 ) {
	    for( int i=0; i<tabs.length; i++ ) tabs[i] = false;
	  } else if( mycmd.params[0] == 3 ) {
	    int col = terminal.getCursorCol();
	    if( col < tabs.length ) tabs[col] = false;
	  }
	}
	break;
      case 'm':
	if( DEBUG ) System.out.println("WebTerm: SGR: Set graphic rendition.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( cnt <= 0 ) {
	  if( DEBUG ) System.out.println("WebTerm: Plain style.");
	  terminal.setStyle(terminal.STYLE_PLAIN);
	  terminal.setForeColor(0);
	  terminal.setBackColor(0);
	} else {
	  for( int i=0; i<cnt; i++ ) {
	    switch( args[i] ) {
	    case 0:
	      if( DEBUG ) System.out.println("WebTerm: Plain style.");
	      terminal.setStyle(terminal.STYLE_PLAIN);
	      terminal.setForeColor(0);
	      terminal.setBackColor(0);
	      break;
	    case 1:
	      if( DEBUG ) System.out.println("WebTerm: Bold style.");
	      terminal.setStyle(terminal.getStyle() | terminal.STYLE_BOLD);
	      break;
	    case 4:
	      if( DEBUG ) System.out.println("WebTerm: Underscore style.");
	      terminal.setStyle(terminal.getStyle() | terminal.STYLE_UNDERSCORE);
	      break;
	    case 5:
	      if( DEBUG ) System.out.println("WebTerm: Italic style.");
	      terminal.setStyle(terminal.getStyle() | terminal.STYLE_ITALIC);
	      break;
	    case 7:
	      if( DEBUG ) System.out.println("WebTerm: Inverse style.");
	      terminal.setStyle(terminal.getStyle() | terminal.STYLE_INVERSE);
	      break;
	      /* VT220 control code */
	    case 22:
	      if( DEBUG ) System.out.println("WebTerm: Bold style off.");
	      terminal.setStyle(terminal.getStyle() & ~terminal.STYLE_BOLD);
	      break;
	    case 24:
	      if( DEBUG ) System.out.println("WebTerm: Underscore style off.");
	      terminal.setStyle(terminal.getStyle()
				& ~terminal.STYLE_UNDERSCORE);
	      break;
	    case 25:
	      if( DEBUG ) System.out.println("WebTerm: Italic style off.");
	      terminal.setStyle(terminal.getStyle() & ~terminal.STYLE_ITALIC);
	      break;
	    case 27:
	      if( DEBUG ) System.out.println("WebTerm: Inverse style off.");
	      terminal.setStyle(terminal.getStyle() & ~terminal.STYLE_INVERSE);
	      break;
	    default:
	      if( args[i] >= 30 && args[i] <= 39 )
		terminal.setForeColor(args[i]-30+1);
	      else if( args[i] >= 40 && args[i] <= 49 )
		terminal.setBackColor(args[i]-40+1);
	      else if( DEBUG )
		System.out.println("WebTerm: Unknown style: " + args[i]);
	      break;
	    }
	  }
	}
	break;
      case 'n':                     // Device status report
	doDSR(mycmd);
	break;
      case 'q':                     // DECLL: Load LEDs
	if( DEBUG ) System.out.println("WebTerm: DECLL: Load LEDs.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( DEBUG ) {
	  for( int p=0; p<mycmd.cur_param; p++ ) {
	    String str = "       ";
	    for( int i=0; i<4; i++ ) {
	      if( (mycmd.params[p]&(1<<i)) != 0 ) str = str + "*** ";
	      else str = str + "--- ";
	    }
	    System.out.println(str);
	  }
	}
	break;
      case 'r':                     // DECSTBM: Set scroll region
	if( DEBUG ) System.out.println("WebTerm: DECSTBM: Set scroll region.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( terminal != null ) {
	  int top = 0;
	  int bottom = terminal.getNumRows()-1;
	  if( given[0] && cnt >= 1 ) top = args[0]-1;
	  if( given[1] && cnt >= 2 ) bottom = args[1]-1;
	  terminal.setRegion(top,bottom);
	  if( mode_states[MODE_DECOM] ) {
	    terminal.setCursorPos(terminal.getRegionTop(),
				  terminal.getRegionLeft());
	  } else {
	    terminal.setCursorPos(0,0);
	  }
	}
	break;
      case 'x':                     // DECREQTPARM: Request term params
	if( DEBUG ) System.out.println("WebTerm: DECREQTPARM: Request term params.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( telnet != null ) {
	  Command scmd = new Command();
	  scmd.setType(Command.TYPE_CSI);
	  scmd.addParam(2,0);   // Only reporting on request
	  scmd.addParam(1,0);   // No parity set
	  scmd.addParam(1,0);   // 8 bits per character
	  scmd.addParam(120,0); // 19200 bps transmit speed
	  scmd.addParam(120,0); // 19200 bps receive speed
	  scmd.addParam(1,0);   // Bit rate multiplier is 16
	  scmd.addParam(0,0);   // Switch flags.   XXX not implemented.
	  scmd.setTerminator('x');
	  if( DEBUG ) System.out.println("WebTerm: Sending DECREPTPARM: " +
					 scmd.toString());
	  send(scmd.toSequence());
	}
	break;
      case 'y':                     // DECTST: Invoke confidence test
	if( DEBUG ) System.out.println("WebTerm: DECTST: Invoke confidence test.");
	if( DEBUG ) System.out.println(mycmd.paramsToString());
	if( mycmd.cur_param == 2 && mycmd.params[0] == 2
	    && mycmd.params[1] == 0 ) {
	  // This one does a reset.
	  reset_state();
	  resetTerminal(true);
	}
	break;
      default:
	{
	  System.out.println("WebTerm: Unknown CSI: " + mycmd.toString());
	}
      }
      return;
    }

    // Command with intermediate sequence '' and '?' flag
    if( mycmd.flags == Command.FLAG_QUES_MASK && mycmd.cur_inter <= 0 ) {
      switch( mycmd.term ) {

	// DSR - device status report.
      case 'n':
	doDSR(cmd);
	return;
      }
    }

    // Command with intermediate sequence '' and '>' flag
    if( mycmd.flags == Command.FLAG_GT_MASK && mycmd.cur_inter <= 0 ) {
      switch( mycmd.term ) {

	// DA - secondary device attribute report.
      case 'c':
	if( mycmd.cur_param <= 0 ||
	    (mycmd.cur_param == 1 && mycmd.params[0] == 0) ) {
	  sendDA(DA_SECONDARY);
	} else {
	  System.out.println("WebTerm: Unknown DA: " + mycmd.toString());
	}
	return;
      }
    }

    // Command with intermediate sequence '!' and no flags
    if( mycmd.flags == 0 && mycmd.cur_inter == 1 && mycmd.inters[0] == '!' ) {
      switch( mycmd.term ) {

	// DECSTR does a soft-reset of the terminal
      case 'p':
	if( DEBUG ) System.out.println("WebTerm: DECSTR (Soft reset)");
	reset_state();
	resetTerminal(false);
	return;

      }
    }

    // Command with intermediate sequence ''' and no flags
    if( mycmd.flags == 0 && mycmd.cur_inter == 1 && mycmd.inters[0] == '\'' ) {
      switch( mycmd.term ) {

	/* These aren't implemented yet...

	// DECELR - enable locator reporting.
      case 'z':
	if( DEBUG ) System.out.println("WebTerm: DECELR (Enable Locator Reports)");
	mode_states[MODE_DECELR] = false;
	mode_states[MODE_DECOLR] = false;
	mode_states[MODE_DECPLR] = false;
	if( mycmd.cur_param > 0 && mycmd.params[0] == 1 ) {
	  mode_states[MODE_DECELR] = true;
	} else if( mycmd.cur_param > 0 && mycmd.params[0] == 2 ) {
	  mode_states[MODE_DECELR] = true;
	  mode_states[MODE_DECOLR] = true;
	}
	if( mycmd.cur_param > 1 && mycmd.params[1] == 1 ) {
	  mode_states[MODE_DECPLR] = true;
	}
	return;

	// DECRLP - request current locator position.
      case '|':
	if( DEBUG ) System.out.println("WebTerm: DECRLP (Request Locator Position)");
	if( mycmd.cur_param <= 0 || mycmd.params[0] <= 1 ) {
	  Command scmd = new Command();
	  build_lr(scmd,null);
	  if( DEBUG ) System.out.println("WebTerm: Sending event: " +
					 scmd.toString());
	  send(scmd.toSequence());
	  return;
	}
	*/
      }
    }

    System.out.println("WebTerm: Unknown CSI: " + mycmd.toString());
  }

  /* ------------------------------------------------------------
     DSR PARSING.
     ------------------------------------------------------------ */

  public boolean doDSR(Command mycmd) {
    Command scmd = new Command();
    if( mycmd.cur_param >= 1 && mycmd.cur_inter <= 0 ) {
      if( mycmd.flags == 0 && mycmd.params[0] == 6 ) {
	if( DEBUG ) System.out.println("WebTerm: DSR - cursor position.");
	scmd.setType(Command.TYPE_CSI);
	int row=1, col=1;
	if( terminal != null ) {
	  row = terminal.getCursorRow()+1;
	  col = terminal.getCursorCol()+1;
	  if( mode_states[MODE_DECOM] ) {
	    row -= terminal.getRegionTop();
	    col -= terminal.getRegionLeft();
	  }
	}
	if( row < 1 ) row = 1;
	if( col < 1 ) col = 1;
	scmd.addParam(row,0);
	scmd.addParam(col,0);
	scmd.setTerminator('R');
      } else if( mycmd.flags == 0 && mycmd.params[0] == 5 ) {
	if( DEBUG ) System.out.println("WebTerm: DSR - terminal status.");
	scmd.setType(Command.TYPE_CSI);
	scmd.addParam(0,0);
	scmd.setTerminator('n');
      } else if( mycmd.flags == Command.FLAG_QUES_MASK &&
		 mycmd.params[0] == 15 ) {
	if( DEBUG ) System.out.println("WebTerm: DSR - printer status.");
	scmd.setType(Command.TYPE_CSI);
	scmd.addParam(13,Command.FLAG_QUES_MASK);  // No printer.
	scmd.setTerminator('n');
      } else if( mycmd.flags == Command.FLAG_QUES_MASK &&
		 mycmd.params[0] == 25 ) {
	if( DEBUG ) System.out.println("WebTerm: DSR - user defined keys.");
	scmd.setType(Command.TYPE_CSI);
	scmd.addParam(21,Command.FLAG_QUES_MASK);  // Locked
	scmd.setTerminator('n');
      } else {
	System.out.println("WebTerm: Unknown DSR: " + mycmd.toString());
      }
    } else {
      System.out.println("WebTerm: Unknown DSR: " + mycmd.toString());
    }

    if( scmd.type != Command.TYPE_NONE && telnet != null ) {
      if( DEBUG ) System.out.println("WebTerm: Sending DSR report: " +
				     scmd.toString());
      send(scmd.toSequence());
      return true;
    }
    return false;
  }

  /* ------------------------------------------------------------
     DCS PARSING.
     ------------------------------------------------------------ */

  public void doDCS(Command mycmd) {

    // Standard commands.
    if( mycmd.flags == 0 && mycmd.cur_inter <= 0 ) {
      switch( mycmd.term ) {
      default:
	{
	  System.out.println("WebTerm: Unknown DCS: " + mycmd.toString());
	}
      }
      return;
    }

    System.out.println("WebTerm: Unknown DCS: " + mycmd.toString());
  }

  /* ------------------------------------------------------------
     OSC PARSING.
     ------------------------------------------------------------ */

  public void doOSC(Command mycmd) {

    // Standard commands.
    if( mycmd.flags == 0 && mycmd.cur_inter <= 0 ) {
      switch( mycmd.term ) {
      default:
	{
	  System.out.println("WebTerm: Unknown OSC: " + mycmd.toString());
	}
      }
      return;
    }

    System.out.println("WebTerm: Unknown OSC: " + mycmd.toString());
  }

  /* ------------------------------------------------------------
     PM PARSING.
     ------------------------------------------------------------ */

  public void doPM(Command mycmd) {

    // Standard commands.
    if( mycmd.flags == 0 && mycmd.cur_inter <= 0 ) {
      switch( mycmd.term ) {
      default:
	{
	  System.out.println("WebTerm: Unknown PM: " + mycmd.toString());
	}
      }
      return;
    }

    System.out.println("WebTerm: Unknown PM: " + mycmd.toString());
  }

  /* ------------------------------------------------------------
     APC PARSING.
     ------------------------------------------------------------ */

  public void doAPC(Command mycmd) {

    // Standard commands.
    if( mycmd.flags == 0 && mycmd.cur_inter <= 0 ) {
      switch( mycmd.term ) {
      default:
	{
	  System.out.println("WebTerm: Unknown APC: " + mycmd.toString());
	}
      }
      return;
    }

    System.out.println("WebTerm: Unknown APC: " + mycmd.toString());
  }

  /* ------------------------------------------------------------
     EMULATION SUPPORT.
     ------------------------------------------------------------ */

  /* Send a Device Attribute string, of the given type, to the remote
     host. */

  static final int DA_PRIMARY = 0;
  static final int DA_SECONDARY = 1;
  static final int DA_VT52 = 2;

  public boolean sendDA(int type) {
    Command scmd = new Command();
    scmd.setType(Command.TYPE_CSI);
    scmd.setTerminator('c');

    if( type == DA_PRIMARY || type == DA_VT52 ) {
      if( emulation == EMU_VT220 ) {
	scmd.addParam(62,Command.FLAG_QUES_MASK);  // VT 220
	if( terminal != null && terminal.getNumCols() >= 132 ) {
	  scmd.addParam(1,Command.FLAG_QUES_MASK);   // 132 columns
	}
	//scmd.addParam(2,Command.FLAG_QUES_MASK);   // printer port
	//scmd.addParam(6,Command.FLAG_QUES_MASK);   // selective erase
	//scmd.addParam(7,Command.FLAG_QUES_MASK);   // DRCS
	//scmd.addParam(8,Command.FLAG_QUES_MASK);   // UDK
	//scmd.addParam(9,Command.FLAG_QUES_MASK);   // 7-bit nat. repl. chars
      } else if( emulation == EMU_VT52 && type == DA_VT52 ) {
	scmd.setType(Command.TYPE_ESC);
	scmd.addIntermediates("/");
	scmd.setTerminator('Z');
      } else {
	scmd.addParam(6,Command.FLAG_QUES_MASK);     // VT 102
	// Others:
	// ESC [ ? 1; 2 c     -- VT 100 with AVO
	// ESC [ ? 1; 0 c     -- VT 101
	//            ^--------- 1 = STP,      2 = AVO,  4 = GO
        //                       processor op, video op, graphics op
      }

    } else if( type == DA_SECONDARY ) {

      scmd.addParam(1,Command.FLAG_GT_MASK);    // VT 220
      scmd.addParam(10,Command.FLAG_GT_MASK);   // Firmware version 1.0
      scmd.addParam(0,Command.FLAG_GT_MASK);    // No options

    }
      
    if( scmd.type != Command.TYPE_NONE && telnet != null ) {
      if( DEBUG ) System.out.println("WebTerm: Sending DA report: " +
				     scmd.toString());
      send(scmd.toSequence());
      return true;
    }
    return false;
  }

  /* Do the given operation 'op' on the mode state 'mode'. */

  public void doMode(int mode, int op) {
    if( DEBUG ) System.out.println(mode_names[mode] + " mode (" +
				   mode_abbrvs[mode] + "): Op=" + op);
    switch( op ) {
    case MOP_RESET:
      mode_states[mode] = false;
      break;
    case MOP_SET:
      mode_states[mode] = true;
      break;
    case MOP_SAVE:
      mode_saves[mode] = mode_states[mode];
      return;
    case MOP_RESTORE:
      mode_states[mode] = mode_saves[mode];
      break;
    default:
      System.out.println("WebTerm: Unknown mode operation: " + op);
    }

    if( mode == MODE_DECOM ) {
      if( terminal != null ) {
	if( mode_states[MODE_DECOM] ) {
	  terminal.setCursorPos(terminal.getRegionTop(),
				terminal.getRegionLeft());
	} else {
	  terminal.setCursorPos(0,0);
	}
      }
    } else if( mode == MODE_MITMRM && op == MOP_SET ) {
      mode_states[MODE_MITMRM] = false;
      mode_states[MODE_XTMHMRM] = false;
    } else if( mode == MODE_XTMMRM && op == MOP_SET ) {
      mode_states[MODE_MITMRM] = false;
      mode_states[MODE_XTMHMRM] = false;
    } else if( mode == MODE_XTMHMRM && op == MOP_SET ) {
      mode_states[MODE_MITMRM] = false;
      mode_states[MODE_XTMMRM] = false;
    } else if( mode == MODE_DECSCLM || mode == MODE_DECAWM ||
	       mode == MODE_DECSCNM || mode == MODE_LNM ||
	       mode == MODE_DECTCEM ) {
      update_terminal_mode();
    }
  }

}
