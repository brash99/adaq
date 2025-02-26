/*
 * WebTerm
 * ~~~~~~~
 *
 * Emulator class: an abstract class that defines the interface
 * to a terminal emulator.  It typically sits between a Terminal class,
 * where it interacts with the user, and a Telnet class, which it
 * uses to communicate with a remote machine.
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
 * ----------------------------------------------------------------------
 *
 * History
 * ~~~~~~~
 *
 * 1.3: Created this file.
 *
 */

import java.awt.*; 
import java.io.*;
import java.util.*;

//import Terminal;
//import Telnet;

public class Emulator {

  static final boolean DEBUG = false;

  /* Standard ASCII control characters. */

  static final char NUL = (char)0x00;
  static final char SOH = (char)0x01;
  static final char STX = (char)0x02;
  static final char ETX = (char)0x03;
  static final char EOT = (char)0x04;
  static final char ENQ = (char)0x05;
  static final char ACK = (char)0x06;
  static final char BEL = (char)0x07;
  static final char BS  = (char)0x08;
  static final char HT  = (char)0x09;
  static final char LF  = (char)0x0a;
  static final char VT  = (char)0x0b;
  static final char FF  = (char)0x0c;
  static final char CR  = (char)0x0d;
  static final char SO  = (char)0x0e;
  static final char SI  = (char)0x0f;
  static final char DLE = (char)0x10;
  static final char DC1 = (char)0x11;
  static final char DC2 = (char)0x12;
  static final char DC3 = (char)0x13;
  static final char DC4 = (char)0x14;
  static final char NAK = (char)0x15;
  static final char SYN = (char)0x16;
  static final char ETB = (char)0x17;
  static final char CAN = (char)0x18;
  static final char EM  = (char)0x19;
  static final char SUB = (char)0x1a;
  static final char ESC = (char)0x1b;
  static final char FS  = (char)0x1c;
  static final char GS  = (char)0x1d;
  static final char RS  = (char)0x1e;
  static final char US  = (char)0x1f;
  static final char DEL = (char)0x7f;
  static final char IND = (char)0x84;
  static final char NEL = (char)0x85;
  static final char SSA = (char)0x86;
  static final char ESA = (char)0x87;
  static final char HTS = (char)0x88;
  static final char HTJ = (char)0x89;
  static final char VTS = (char)0x8a;
  static final char PLD = (char)0x8b;
  static final char PLU = (char)0x8c;
  static final char RI  = (char)0x8d;
  static final char SS2 = (char)0x8e;
  static final char SS3 = (char)0x8f;
  static final char DCS = (char)0x90;
  static final char PU1 = (char)0x91;
  static final char PU2 = (char)0x92;
  static final char STS = (char)0x93;
  static final char CCH = (char)0x94;
  static final char MW  = (char)0x95;
  static final char SPA = (char)0x96;
  static final char EPA = (char)0x97;
  static final char CSI = (char)0x9b;
  static final char ST  = (char)0x9c;
  static final char OSC = (char)0x9d;
  static final char PM  = (char)0x9e;
  static final char APC = (char)0x9f;

  Terminal terminal;
  Telnet telnet;

  public Emulator() {
  }

  public int getCurType() {
    return 0;
  }

  public String getTypeName(int num) {
    if( num == 0 ) return "DUMB";
    return null;
  }

  public synchronized void setTerminal(Terminal terminal) {
    this.terminal = terminal;
    if( terminal != null ) {
      terminal.setEmulator(this);
    }
  }

  public synchronized void setTelnet(Telnet telnet) {
    this.telnet = telnet;
    if( telnet != null ) {
      telnet.setEmulator(this);
      if( terminal != null )  {
	setWindowSize(terminal.getNumCols(),terminal.getNumRows());
      }
    }
  }

  public Terminal getTerminal() {
    return this.terminal;
  }

  public Telnet getTelnet() {
    return this.telnet;
  }

  public synchronized void setWindowSize(int cols, int rows) {
    if( telnet != null ) {
      telnet.setWindowSize(getNumCols(),getNumRows());
    }
  }

  public synchronized int getNumRows() {
    if( terminal != null ) return terminal.getNumRows();
    return 0;
  }

  public synchronized int getNumCols() {
    if( terminal != null ) return terminal.getNumCols();
    return 0;
  }

  public synchronized void connect(String host, int port) {
    if( terminal != null ) terminal.connect(host,port);
  }

  public synchronized void disconnect() {
    if( terminal != null ) terminal.disconnect();
  }

  public synchronized void receive(char[] d, int off, int len) {
    int cnt = 0;
    while( len > 0 ) {
      if( !checkChar(d[off+cnt]) ) {
	if( cnt > 0 ) {
	  if( DEBUG && cnt > 0 ) {
	    System.out.println("WebTerm: Text: '" +
			       sequenceToString(new String(d,off,cnt)) + "'");
	  }
	  if( terminal != null ) terminal.write(d,off,cnt,
						terminal.OUTF_PARTIAL);
	  off += cnt;
	  cnt = 0;
	}
	doChar(d[off]);
	off++;
      } else {
	cnt++;
      }
      len--;
    }
    if( DEBUG && cnt > 0 ) {
      System.out.println("WebTerm: Text: '" +
			 sequenceToString(new String(d,off,cnt)) + "'");
    }
    if( terminal != null ) terminal.write(d,off,cnt,0);
  }

  public synchronized void receive(String str) {
    receive(str.toCharArray(),0,str.length());
  }

  public synchronized void receive(char c) {
    if( checkChar(c) ) {
      if( DEBUG ) System.out.println("WebTerm: Text: '" + charToString(c) + "'");
      if( terminal != null ) terminal.write(c,0);
    } else {
      doChar(c);
      //if( terminal != null ) terminal.write(null,0,0,terminal.OUTF_PARTIAL);
      if( terminal != null ) terminal.write(null,0,0,0);
    }
  }

  public synchronized void send(char[] d, int off, int len) {
    if( telnet != null ) {
      byte[] b = new byte[len];
      int i = 0;
      while( len > 0 ) {
	b[i] = (byte)d[off];
	i++;
	off++;
	len--;
      }
      try {
	telnet.write(b);
      }
      catch (IOException ex) {
	if( terminal != null ) {
	  terminal.write("\r\n" + ex.toString(),0);
	}
      }
    }
  }

  public synchronized void send(String str) {
    if( telnet != null ) {
      try {
	telnet.write(str);
      }
      catch (IOException ex) {
	if( terminal != null ) {
	  terminal.write("\r\n" + ex.toString(),0);
	}
      }
    }
  }

  public synchronized void send(char c) {
    if( telnet != null ) {
      try {
	telnet.write((byte)c);
      }
      catch (IOException ex) {
	if( terminal != null ) {
	  terminal.write("\r\n" + ex.toString(),0);
	}
      }
    }
  }

  /* Override this method to handle non-ASCII events -- e.g., cursor
     keys, mouse buttons, etc. */
  public synchronized boolean handleEvent(Event e) {
    return false;
  }

  /* Override this method to implement a particular emulator.  Return
     true to pass the given character on the the terminal untouched,
     or false if doing your own processing of it. */
  public boolean doChar(char c) {
    return true;
  }

  /* This method is called by the root emulator to check if its subclass
     needs to do its own processing of the character -- it should return
     true if it can be directly sent to the terminal, and false if
     the subclass needs to look at it.
     While it is not strictly necessary, overriding this method can
     dramatically increase the performance of the emulation. */
  public boolean checkChar(char c) {
    return true;
  }

  /* ------------------------------------------------------------
     HELPER FUNCTIONS FOR DISPLAYING STRINGS
     ------------------------------------------------------------ */

  static final String[] codes_00_20 = {
    "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
    "BS",  "HT",  "LF",  "VT",  "FF",  "CR",  "SO",  "SI",
    "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
    "CAN", "EM",  "SUB", "ESC", "FS",  "GS",  "RS",  "US",
    "[]"
  };

  static final String[] codes_7f_9f = {
    "DEL",
    "$80", "$81", "$82", "$83", "IND", "NEL", "SSA", "ESA",
    "HTS", "HTJ", "VTS", "PLD", "PLU", "RI",  "SS2", "SS3",
    "DCS", "PU1", "PU2", "STS", "CCH", "MW",  "SPA", "EPA",
    "$98", "$99", "$9a", "CSI", "ST",  "OSC", "PM",  "APC"
  };

  static final public String charToString(char c) {
    if( c <= (char)0x20 ) {
      return codes_00_20[(int)c];
    } else if( c >= (char)0x7f && c <= (char)0x9f ) {
      return codes_7f_9f[(int)c-0x7f];
    } else if( c == (char)0xff ) {
      return "$ff";
    } else if( c >= (char)0x100 ) {
      return "$" + Integer.toString((int)c,16);
    }
    return String.valueOf(c);
  }

  static final public String sequenceToString(String seq) {
    StringBuffer res = new StringBuffer(128);
    for( int i=0; i<seq.length(); i++ ) {
      if( i > 0 ) res.append(' ');
      res.append(charToString(seq.charAt(i)));
    }
    return res.toString();
  }
}
