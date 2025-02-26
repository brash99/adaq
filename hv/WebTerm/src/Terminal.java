/*
 * WebTerm
 * ~~~~~~~
 *
 * Terminal class: a low-level terminal screen driver, encapsulated as
 * a widget.
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
 * - The page buffer will occasionally get null pointers placed into it,
 *   causing an error report to the console.  This doesn't cause any
 *   long-term harm to the applet's execution, however.
 *
 * ----------------------------------------------------------------------
 *
 * History
 * ~~~~~~~
 *
 * 1.4: Added screenSetChars(), screenDeleteChars(), screenInsertChars()
 *      Added fill char and fill style params to other screen methods.
 *      Added modes: MODE_SMOOTH, MODE_NEWLINE, MODE_NOWRAP, MODE_INVERSE.
 *      Improved display updating speed with dirty[] array.
 *      Added display history.
 *      Wasn't catching exceptions when creating fonts.  Oops!
 *      Added synchronized to most public methods, to fix many potential
 *      race conditions.
 *      Added basic support for color.
 *
 * 1.3: Created this file.
 *      Added REGISTRY parameter.
 *      Cleaned up font allocation in init(), fixed bugs.
 *      Added FONTSIZE parameter.
 *      preferredSize() now just returns minimumSize().
 *
 */

//import java.applet.Applet;    
import java.awt.*; 
import java.io.*;
import java.net.*;

//import Emulator;
//import ClassRegistry;
//import ClassConnection;

public class Terminal extends Canvas implements ClassConnection {

  static final boolean DEBUG = true;

  /* ------------------------------------------------------------
     CONSTANT VALUES SECTION
     ------------------------------------------------------------ */

  /* Mode flags.  These are global operating modes of the terminal;
     the initial state of all modes is off. */

  public static final int MODE_SMOOTH = 1<<0;    // Smooth scroll display?
  public static final int MODE_NEWLINE = 1<<1;   // Go to left column on NL?
  public static final int MODE_NOWRAP = 1<<2;    // Wrap at right of screen?
  public static final int MODE_INVERSE = 1<<3;   // Inverse video display?

  /* Output flags used with the write() methods. */

  public static final int OUTF_RAW     = 1<<0;  // Don't interpret \n \b etc.?
  public static final int OUTF_PARTIAL = 1<<1;  // Don't update screen?

  /* Style flags.  Use with setStyle() and getStyle(); each character can
     have its own style, and that style is taken from the current style
     setting when it was written to the terminal. */

  public static final int STYLE_PLAIN      = 0;
  public static final int STYLE_INVERSE    = 1<<0;
  public static final int STYLE_BOLD       = 1<<1;
  public static final int STYLE_ITALIC     = 1<<2;
  public static final int STYLE_UNDERSCORE = 1<<3;
  public static final int STYLE_MASK       = 0xff;

  public static final int STYLE_COLOR_MASK = 0xff;
  public static final int STYLE_FOREGROUND_POS = 8;
  public static final int STYLE_BACKGROUND_POS = 16;

  /* Colors */

  public static final int MAX_COLORS = 9;

  static final int init_foregrounds[] = {
    ((0x00)<<16) | ((0x00)<<8) | 0x00,      // the default foreground
    ((0x00)<<16) | ((0x00)<<8) | 0x00,      // black
    ((0xff)<<16) | ((0x00)<<8) | 0x00,      // red
    ((0x00)<<16) | ((0xff)<<8) | 0x00,      // green
    ((0xff)<<16) | ((0xff)<<8) | 0x00,      // yellow
    ((0x00)<<16) | ((0x00)<<8) | 0xff,      // blue
    ((0xff)<<16) | ((0x00)<<8) | 0xff,      // magenta
    ((0x00)<<16) | ((0xff)<<8) | 0xff,      // cyan
    ((0xff)<<16) | ((0xff)<<8) | 0xff       // white
  };

  static final int init_backgrounds[] = {
    ((0xe0)<<16) | ((0xe0)<<8) | 0xe0,      // the default foreground
    ((0x00)<<16) | ((0x00)<<8) | 0x00,      // black
    ((0xff)<<16) | ((0x00)<<8) | 0x00,      // red
    ((0x00)<<16) | ((0xff)<<8) | 0x00,      // green
    ((0xff)<<16) | ((0xff)<<8) | 0x00,      // yellow
    ((0x00)<<16) | ((0x00)<<8) | 0xff,      // blue
    ((0xff)<<16) | ((0x00)<<8) | 0xff,      // magenta
    ((0x00)<<16) | ((0xff)<<8) | 0xff,      // cyan
    ((0xff)<<16) | ((0xff)<<8) | 0xff       // white
  };

  /* EVENT_ACTION types that the terminal reports to its parent. */

  public static final Integer ACTION_DISCONNECT = new Integer(0);
  public static final Integer ACTION_CONNECT = new Integer(1);
  public static final Integer ACTION_VIEW = new Integer(2);

  static final int INNER_SIZE = 4;
  static final int ACTIVE_SIZE = 2;

  static final int BORDER_WIDTH = (INNER_SIZE+ACTIVE_SIZE);
  static final int BORDER_HEIGHT = (INNER_SIZE+ACTIVE_SIZE);

  /* ------------------------------------------------------------
     INSTANCE DATA SECTION
     ------------------------------------------------------------ */

  // Global mode flags.
  int curMode = 0;

  // Terminal emulator to communicate with.
  Emulator emulator;

  // If not null, this is the name of the ClassRegistry we register with.
  String register = null;

  // User's requested font size.  -1 is "use the default".
  int fontsize = -1;

  // The fonts being used for rendering and useful info about them.
  Font fontPlain, fontBold, fontItalic, fontBoldItalic;
  int charWidth, charHeight;
  int fontAscent, fontDescent;

  // The default background and foreground colors of the widget.
  Color defBackground = null, defForeground = null;

  // The current background and foreground colors being used.
  Color[] textBackground = new Color[MAX_COLORS];
  Color[] textForeground = new Color[MAX_COLORS];

  // Color for doing 3D ridges and stuff.  Not really used, yet.
  Color embossIn = null, embossOut = null;

  // Used to sync() the display.
  Toolkit toolkit;

  // Do we currently have input focus?
  boolean have_focus = false;

  /* Translation table for translating written characters to screen codes. */

  char[] defaultTranslation;   // The standard translation table.
  char[] curTranslation;       // The current table being used.

  boolean outputecho = false;

  /* Cursor and rendering state information. */

  int numRows, numCols;        // Current width/height of screen in characters.
  int curRow, curCol;          // Current cursor location.
  int curStyle = STYLE_PLAIN;  // Current character style.
  boolean cursorVisible = true;// Show the cursor?

  int regionTop, regionBottom, // Current bounding region.
      regionLeft, regionRight;

  int lastRow, lastCol;        // Last postion cursor was drawn at.
  boolean overChar = false;    // Flag if at far right of screen.
  char[][] page;               // The actual character information.
  int[][] style;               // The styles of the characters.

  int dirtyTop = -1,           // First and last dirty rows.
      dirtyBottom = -1;
  boolean[] dirty;             // Rows which need to be redrawn.
  int scrollCount = 0;         // Optimize scrolling (not implemented).

  /* History buffer information.  */

  char[][] history_char;         // Characters in history buffer
  int[][] history_style;         // Styles in history buffer
  int history_top, history_len;  // Top position and amount in history arrays
  boolean history_changed;       // Has position or data changed?
  int history_view = 0;          // Current display view.  0 => no hist visible
  int history_last_len = 0;      // Last length of history we displayed
  int history_last_view = 0;     // Last view on history we displayed

  /* ------------------------------------------------------------
     CONSTRUCTOR AND GLOBAL CONTROL METHODS.
     ------------------------------------------------------------ */

  public Terminal() {
    defaultTranslation = new char[256];
    for( int i=0; i<0x100; i++ ) defaultTranslation[i] = (char)i;
    for( int i=0; i<0x20; i++ ) defaultTranslation[i] = '?';
    for( int i=0x7f; i<0xa0; i++ ) defaultTranslation[i] = '?';
    defaultTranslation[0xff] = '?';
    setTranslation(null);
    for( int i=0; i<MAX_COLORS; i++ ) {
      textForeground[i] = textBackground[i] = null;
    }
    init_history(500);
  }

  public static String[][] getParameterInfo() {
    String[][] info = {
      { "register", "string", "name to register the terminal as" },
      { "fontsize", "int", "font point size to use in terminal" },
      { "background", "string", "terminal background color or \"default\"" },
      { "foreground", "string", "terminal foreground color or \"default\"" },
      { "history", "int", "maximum number of lines in history buffer" },
      { "outputecho", "", "if given, echo terminal output to console" },
    };

    return info;
  }

  public void setEmulator(Emulator emulator) {
    this.emulator = emulator;
    if( emulator != null ) {
      emulator.setWindowSize(numCols,numRows);
    }
  }

  public Emulator getEmulator() {
    return this.emulator;
  }

  public synchronized void parseParameters(Parameters p) {
    String tmp;

    if( register != null ) ClassRegistry.remClass(register,this);
    register = p.getParameter("register");
    if( register != null ) ClassRegistry.addClass(register,this);

    fontsize = -1;
    String fs_str = p.getParameter("fontsize");
    if( fs_str != null ) {
      fontsize = Integer.parseInt(fs_str);
    }

    tmp = p.getParameter("background");
    if( tmp != null ) {
      textBackground[0] = null;
      try {
	if( !tmp.equals("default") ) {
	  Color bg = Color.getColor(tmp);
	  if( bg != null ) textBackground[0] = bg;
	}
      } catch (Exception e) {
	System.out.println("WebTerm: Background Color Format Error: " + tmp);
      }
    } else {
      textBackground[0] = new Color(init_backgrounds[0]);
    }

    tmp = p.getParameter("foreground");
    if( tmp != null ) {
      textForeground[0] = null;
      try {
	if( !tmp.equals("default") ) {
	  Color fg = Color.getColor(tmp);
	  if( fg != null ) textForeground[0] = fg;
	}
      } catch (Exception e) {
	System.out.println("WebTerm: Foreground Color Format Error: " + tmp);
      }
    } else {
      textForeground[0] = new Color(init_foregrounds[0]);
    }

    tmp = p.getParameter("history");
    if( tmp != null ) {
      try {
	init_history(Integer.parseInt(tmp));
      }
      catch (NumberFormatException ex) {
	System.out.println("WebTerm: History must be a number.");
      }
    }

    tmp = p.getParameter("outputecho");
    if( tmp != null ) outputecho = true;
  }

  public synchronized void init() {
    Graphics g = getGraphics();
    Font f = g.getFont();
    int size = f.getSize();
    if( fontsize > 0 ) size = fontsize;
    fontPlain = f;
    try {
      fontPlain = new Font("Courier",Font.PLAIN,size);
    } catch( Exception e ) {
      System.out.println("WebTerm: Unable to get font Courier PLAIN " + size);
    }
    if( fontPlain == null ) fontPlain = f;
    if( DEBUG ) {
      System.out.println("WebTerm: StdFont name=" + fontPlain.getName()
			 + " size=" + fontPlain.getSize()
			 + " style=" + fontPlain.getStyle());
    }
    fontBold = fontPlain;
    try {
      fontBold = new Font(fontPlain.getName(),Font.BOLD,
			  fontPlain.getSize());
    } catch( Exception e ) {
      System.out.println("WebTerm: Unable to get font Courier BOLD " + size);
    }
    fontItalic = fontBoldItalic = fontBold;
    try {
      fontBoldItalic = new Font(fontPlain.getName(),Font.BOLD+Font.ITALIC,
				fontPlain.getSize());
    } catch( Exception e ) {
      System.out.println("WebTerm: Unable to get font Courier ITALIC " + size);
    }
    try {
      fontItalic = new Font(fontPlain.getName(),Font.ITALIC,
			    fontPlain.getSize());
    } catch( Exception e ) {
      System.out.println("WebTerm: Unable to get font Courier BOLD ITALIC " +
			 size);
    }
    if( DEBUG ) {
      System.out.println("WebTerm: BldFont name=" + fontBold.getName()
			 + " size=" + fontBold.getSize()
			 + " style=" + fontBold.getStyle());
      System.out.println("WebTerm: ItlFont name=" + fontItalic.getName()
			 + " size=" + fontItalic.getSize()
			 + " style=" + fontItalic.getStyle());
    }
    g.setFont(fontPlain);
    setFont(fontPlain);

    defBackground = getBackground();
    defForeground = getForeground();
    if( textBackground[0] == null ) textBackground[0] = defBackground;
    setTextBackground(textBackground[0],0);
    if( textForeground[0] == null ) textForeground[0] = defForeground;
    setTextForeground(textForeground[0],0);

    for( int i=0; i<MAX_COLORS; i++ ) {
      if( textForeground[i] == null )
	textForeground[i] = new Color(init_foregrounds[i]);
      if( textBackground[i] == null )
	textBackground[i] = new Color(init_backgrounds[i]);
    }

    embossIn = new Color(0,0,0);
    embossOut = new Color(255,255,255);

    toolkit = getToolkit();

    screenInit();
    repaint();
  }

  public synchronized void reset(boolean hard) {
    if( DEBUG ) System.out.println("WebTerm: Doing reset of terminal.");
    curStyle = STYLE_PLAIN;
    curMode = 0;
    setTranslation(null);
    if( !hard ) {
      curCol = 0;
      curRow = 0;
      lastCol = 0;
      lastRow = 0;
      scrollCount = 0;
      setRegion();
    } else {
      numCols--;
      screenInit();
    }
    for( int i=0; i<numRows; i++ ) dirty[i] = true;
    dirtyTop = 0;
    dirtyBottom = numRows-1;
  }

  public void connect(String host, int port) {
    Event e = new Event(this,Event.ACTION_EVENT,ACTION_CONNECT);
    postEvent(e);
  }

  public void disconnect() {
    Event e = new Event(this,Event.ACTION_EVENT,ACTION_DISCONNECT);
    postEvent(e);
  }

  /* ------------------------------------------------------------
     CLASS CONNECTION INTERFACE.
     ------------------------------------------------------------ */

  public synchronized Object doCommand(String cmd, Object data) {
    if( cmd.compareTo("send") == 0 && data instanceof String ) {
      if( emulator != null ) emulator.send((String)data);
      return new Integer(0);
    }
    return new Integer(1);
  }

  /* ------------------------------------------------------------
     HISTORY MANIPULATION METHODS.
     ------------------------------------------------------------ */

  public synchronized int getDataTotal() {
    return numRows + history_len;
  }

  public synchronized int getDataPos() {
    return history_len+history_view;
  }

  public synchronized void setDataPos(int pos) {
    int viewpos = pos-history_len;
    if( viewpos > 0 ) viewpos = 0;
    if( viewpos < (-history_len) ) viewpos = -history_len;
    if( history_view != viewpos ) {
      history_view = history_last_view = viewpos;
      history_changed = true;
      screenRedraw(getGraphics());
    }
  }

  public synchronized int getDataView() {
    return numRows;
  }

  void init_history(int size) {
    if( size < 0 ) size = 0;
    history_char = new char[size][];
    history_style = new int[size][];
    history_top = 0;
    history_len = 0;
    history_changed = true;
    history_view = 0;
    history_last_len = 0;
    history_last_view = 0;
  }

  void page_to_history(int top, int bottom) {
    if( top <= bottom ) {
      int maxlen = history_char.length;
      int offset = (bottom-top+1);
      int add_pos = (history_top+history_len)%maxlen;

      if( DEBUG ) {
	System.out.println("WebTerm: page_to_history(" + top + "," + bottom + ")");
	System.out.println("WebTerm: maxlen=" + maxlen + ", offset=" + offset +
			   ", add_pos=" + add_pos + ", histtop=" +
			   history_top + ", histlen=" + history_len);
      }

      // Increase amount of data in history.
      history_len += offset;
      if( history_len > maxlen ) history_len = maxlen;

      // Copy new page data to history.
      while( top <= bottom ) {
	history_char[add_pos] = page[top];
	history_style[add_pos] = style[top];
	add_pos++;
	top++;
	if( add_pos >= maxlen ) add_pos = 0;
      }

      // Set the history top, to overwrite old data.
      history_top = add_pos - history_len;
      if( history_top < 0 ) history_top += maxlen;

      // If we are looking at any of the history, make the view
      // stay stable.
      if( DEBUG ) System.out.println("WebTerm: Current history view: " + history_view +
				     ", len: " + history_len);
      if( history_view < 0 ) {
	history_view -= offset;
      }
      if( history_view < (-history_len) ) {
	history_view = -history_len;
	history_changed = true;
      }
      if( DEBUG ) System.out.println("WebTerm: New history view: " + history_view);
    }
  }

  /* ------------------------------------------------------------
     SCREEN LAYOUT METHODS.
     ------------------------------------------------------------ */

  FontMetrics get_metrics(boolean install) {
    Graphics g;
    FontMetrics m;
    Dimension d;

    g = getGraphics();
    m = g.getFontMetrics(fontPlain);
    if( install ) {
      charWidth = m.charWidth('W');
      charHeight = m.getHeight();
      fontAscent = m.getAscent();
      fontDescent = m.getDescent();
    }

    return m;
  }

  public Dimension minimumSize() {
    FontMetrics m = get_metrics(false);
    Dimension dim = new Dimension((m.charWidth('W')*4) + BORDER_WIDTH*2,
				  (m.getHeight()*2) + BORDER_HEIGHT*2);
    if( DEBUG ) System.out.println("WebTerm: Minimum dim: " + dim);
    return dim;
  }

  public Dimension preferredSize() {
    // When the requested size is smaller than both preferred dimensions,
    // the layout manager seems to just use the preferred size, rather
    // than falling back to the minimum size.  Yuck.
    return minimumSize();
    /*
    FontMetrics m = get_metrics(false);
    Dimension dim = new Dimension((m.charWidth('W')*80) + BORDER_WIDTH*2,
				  (m.getHeight()*24) + BORDER_HEIGHT*2);
    System.out.println("WebTerm: Preferred dim: " + dim);
    return dim;
    */
  }

  public void layout() {
    if( DEBUG ) System.out.println("WebTerm: Doing layout of terminal.");
    screenInit();
  }

  public synchronized void screenInit() {
    Dimension d;
    int rows,cols;

    get_metrics(true);

    if( DEBUG ) System.out.println("WebTerm: Initializing screen...");

    d = size();
    cols = (d.width-(BORDER_WIDTH*2)) / charWidth;
    rows = (d.height-(BORDER_HEIGHT*2)) / charHeight;
    if( cols < 4 ) cols = 80;
    if( rows < 2 ) rows = 24;

    if( numRows == rows && numCols == cols ) return;

    numRows = rows;
    numCols = cols;
    curCol = 0;
    curRow = 0;
    lastCol = 0;
    lastRow = 0;
    scrollCount = 0;
    setRegion();

    if( DEBUG ) System.out.println("WebTerm: Width = " + d.width +
				   ", Height = " + d.height);
    if( DEBUG ) System.out.println("WebTerm: Columns = " + numCols +
				   ", Rows = " + numRows);
    page = new char[numRows][numCols];
    style = new int[numRows][numCols];
    dirty = new boolean[numRows];

    for( int r = 0; r < numRows; r++ ) {
      for( int c = 0; c < numCols; c++ ) {
	page[r][c] = ' ';
	style[r][c] = STYLE_PLAIN;
      }
      dirty[r] = true;
    }

    dirtyTop = 0;
    dirtyBottom = numRows-1;
    history_changed = true;
    history_last_len = -1;

    if( emulator != null ) {
      emulator.setWindowSize(numCols,numRows);
    }
  }

  /* ------------------------------------------------------------
     SIMPLE MANIPULATION METHODS.
     ------------------------------------------------------------ */

  final public synchronized int getCursorRow() {
    return curRow;
  }

  final public synchronized int getCursorCol() {
    return curCol;
  }

  final public synchronized int getNumRows() {
    return numRows;
  }

  final public synchronized int getNumCols() {
    return numCols;
  }

  final public synchronized void setCursorPos(int row, int col) {
    if( row < 0 ) row = 0;
    if( row >= numRows ) row = numRows-1;
    if( col < 0 ) col = 0;
    if( col >= numCols ) col = numCols-1;
    curRow = row;
    curCol = col;
  }

  final public synchronized int xToCol(int x) {
    int col = (x-BORDER_WIDTH)/charWidth;
    if( col < 0 ) col = 0;
    if( col >= numCols ) col = numCols-1;
    return col;
  }

  final public synchronized int yToRow(int y) {
    int row = ((y-BORDER_HEIGHT)/charHeight) + history_view;
    if( row < 0 ) row = 0;
    if( row >= numRows ) row = numRows-1;
    return row;
  }

  final public synchronized void setCursorVisible(boolean state) {
    cursorVisible = state;
  }

  final public synchronized boolean getCursorVisible() {
    return cursorVisible;
  }

  final public synchronized void setRegion() {
    regionTop = 0;
    regionBottom = numRows-1;
    regionLeft = 0;
    regionRight = numCols-1;
  }

  final public synchronized void setRegion(int top, int bottom) {
    setRegion(top,bottom,0,numCols-1);
  }

  final public synchronized void setRegion(int top, int bottom,
					   int left, int right) {
    if( top < 0 ) top = 0;
    if( top >= numRows ) top = numRows-1;
    if( bottom < 0 ) bottom = 0;
    if( bottom >= numRows ) bottom = numRows-1;
    if( top >= bottom-2 ) {
      if( top > 2 ) top = 0;
      else bottom = numRows-1;
    }
    if( left < 0 ) left = 0;
    if( left >= numCols ) left = numCols-1;
    if( right < 0 ) right = 0;
    if( right >= numCols ) right = numCols-1;
    if( left >= right-2 ) {
      if( left > 2 ) left = 0;
      else right = numCols-1;
    }
    regionTop = top;
    regionBottom = bottom;
    regionLeft = left;
    regionRight = right;
  }

  final public synchronized int getRegionTop() {
    return regionTop;
  }

  final public synchronized int getRegionBottom() {
    return regionBottom;
  }

  final public synchronized int getRegionLeft() {
    return regionLeft;
  }

  final public synchronized int getRegionRight() {
    return regionRight;
  }

  final public synchronized void setStyle(int style) {
    curStyle = (style&STYLE_MASK) | (curStyle&~STYLE_MASK);
  }

  final public synchronized void setForeColor(int col) {
    if( col < 0 ) col = 0;
    else if( col >= MAX_COLORS ) col = MAX_COLORS-1;
    curStyle = ((col&STYLE_COLOR_MASK)<<STYLE_FOREGROUND_POS)
      | (curStyle&~(STYLE_COLOR_MASK<<STYLE_FOREGROUND_POS));
  }

  final public synchronized void setBackColor(int col) {
    if( col < 0 ) col = 0;
    else if( col >= MAX_COLORS ) col = MAX_COLORS-1;
    curStyle = ((col&STYLE_COLOR_MASK)<<STYLE_BACKGROUND_POS)
      | (curStyle&~(STYLE_COLOR_MASK<<STYLE_BACKGROUND_POS));
  }

  final public synchronized int getStyle() {
    return curStyle;
  }

  final public synchronized int getForeColor() {
    return (curStyle>>STYLE_FOREGROUND_POS) & STYLE_COLOR_MASK;
  }

  final public synchronized int getBackColor() {
    return (curStyle>>STYLE_BACKGROUND_POS) & STYLE_COLOR_MASK;
  }

  final public synchronized void setMode(int mode) {
    if( (curMode&MODE_INVERSE) != (mode&MODE_INVERSE) ) {
      screenDirty(0,numRows-1);
    }
    curMode = mode;
  }

  final public synchronized int getMode() {
    return curMode;
  }

  final public synchronized void setTranslation(char[] t) {
    if( t != null ) curTranslation = t;
    else curTranslation = defaultTranslation;
  }

  final public synchronized char[] getTranslation() {
    return curTranslation;
  }

  final public synchronized void setTextBackground(Color bg, int num) {
    if( num >= 0 && num < MAX_COLORS ) {
      textBackground[num] = bg;
      if( num == 0 ) setBackground(bg);
    }
    screenDirty(0,numRows-1);
  }

  final public synchronized void setTextForeground(Color fg, int num) {
    if( num >= 0 && num < MAX_COLORS ) {
      textForeground[num] = fg;
      if( num == 0 ) setForeground(fg);
    }
    screenDirty(0,numRows-1);
  }

  final public synchronized Color getTextBackground(int num) {
    if( num >= 0 && num < MAX_COLORS ) return textBackground[num];
    return null;
  }

  final public synchronized Color getForeBackground(int num) {
    if( num >= 0 && num < MAX_COLORS ) return textForeground[num];
    return null;
  }

  // Does not go through translation table!
  final public synchronized void setChar(char c) {
    page[curRow][curCol] = c;
    style[curRow][curCol] = curStyle;
  }

  // Does not go through translation table!
  final public synchronized void setChar(char c, int row, int col) {
    page[row][col] = c;
    style[row][col] = curStyle;
  }

  final public synchronized char getChar() {
    return page[curRow][curCol];
  }

  final public synchronized char getChar(int row, int col) {
    return page[row][col];
  }

  final public synchronized int getCharStyle() {
    return style[curRow][curCol];
  }

  final public synchronized int getCharStyle(int row, int col) {
    return style[row][col];
  }

  /* ------------------------------------------------------------
     HIGH-LEVEL MANIPULATION METHODS.
     ------------------------------------------------------------ */

  public synchronized void screenScrollRegion(int top, int bottom,
					      int amount) {
    if( DEBUG ) System.out.println("WebTerm: screenScrollRegion(" + top +
				   "," + bottom + "," + amount + ")");
    if( top >= bottom ) return;
    if( (curMode&MODE_SMOOTH) != 0 ) screenClean(getGraphics());
    if( amount < 0 ) {
      amount = -amount;
      page_to_history(top,top+amount-1);
      for( int i = top; i < (bottom-amount+1); i++ ) {
	page[i] = page[i+amount];
	style[i] = style[i+amount];
      }
      for( int i = bottom-amount+1; i < bottom+1; i++ ) {
	page[i] = new char[numCols];
	style[i] = new int[numCols];
	for( int j = 0; j < numCols; j++ ) {
	  page[i][j] = ' ';
	  style[i][j] = STYLE_PLAIN;
	}
      }
      /*
      cursorVisible = false;
      drawRowCol(g,curRow,curCol);
      g.copyArea(0,amount*charHeight,
		 numCols*charWidth,(numRows-amount)*charHeight,
		 0,-amount*charHeight);
      if( dirtyTop >= 0 ) {
	dirtyTop-=amount;
	if( dirtyTop < 0 ) dirtyTop = 0;
      }
      if( dirtyBottom >= 0 ) {
	dirtyBottom-=amount;
	if( dirtyBottom < 0 ) dirtyBottom = 0;
      }
      screenDirty(numRows-amount,numRows-1);
      cursorVisible = true;
      drawRowCol(g,curRow,curCol);
      */
      screenDirty(top,bottom);
    } else if( amount > 0 ) {
      for( int i = bottom; i >= top+amount; i-- ) {
	page[i] = page[i-amount];
	style[i] = style[i-amount];
      }
      for( int i = top; i < top+amount; i++ ) {
	page[i] = new char[numCols];
	style[i] = new int[numCols];
	for( int j = 0; j < numCols; j++ ) {
	  page[i][j] = ' ';
	  style[i][j] = STYLE_PLAIN;
	}
      }
      /*
      cursorVisible = false;
      drawRowCol(g,curRow,curCol);
      g.copyArea(0,0,
		 numCols*charWidth,(numRows-amount)*charHeight,
		 0,amount*charHeight);
      if( dirtyTop >= 0 ) {
	dirtyTop+=amount;
	if( dirtyTop >= numRows ) dirtyTop = numRows-1;
      }
      if( dirtyBottom >= 0 ) {
	dirtyBottom+=amount;
	if( dirtyBottom >= numRows ) dirtyBottom = numRows-1;
      }
      screenDirty(0,amount-1);
      cursorVisible = true;
      drawRowCol(g,curRow,curCol);
      */
      screenDirty(top,bottom);
    }
    /* toolkit.sync(); */
  }

  public synchronized void screenScroll(int amount) {
    screenScrollRegion(regionTop,regionBottom,amount);
  }

  public synchronized void screenSetChars(int left, int right,
					  char fillc, int fillstyle) {
    if( left < regionLeft ) left = regionLeft;
    if( right > regionRight ) right = regionRight;
    while( left <= right ) {
      page[curRow][left] = fillc;
      style[curRow][left] = fillstyle;
    }
    screenDirty(curRow,curRow);
  }

  public synchronized void screenDeleteChars(int left, int right,
					     char fillc, int fillstyle) {
    if( left < regionLeft ) left = regionLeft;
    if( right > regionRight ) right = regionRight;
    int over = right+1;
    while( over <= regionRight && left <= regionRight ) {
      page[curRow][left] = page[curRow][over];
      style[curRow][left] = style[curRow][over];
      left++;
      over++;
    }
    while( left <= regionRight ) {
      page[curRow][left] = fillc;
      style[curRow][left] = fillstyle;
      left++;
    }
    screenDirty(curRow,curRow);
  }

  public synchronized void screenInsertChars(int left, int right,
					     char fillc, int fillstyle) {
    if( left < regionLeft ) left = regionLeft;
    if( right > regionRight ) right = regionRight;
    if( left == right ) return;
    int inner = regionRight-(right-left);
    right = regionRight;
    while( inner >= left && right >= left ) {
      page[curRow][right] = page[curRow][inner];
      style[curRow][right] = style[curRow][inner];
      inner--;
      right--;
    }
    while( right >= left ) {
      page[curRow][right] = fillc;
      style[curRow][right] = fillstyle;
      right--;
    }
    screenDirty(curRow,curRow);
  }

  public synchronized void screenClearEOL(char fillc, int fillstyle) {
    for( int i=curCol; i<=regionRight; i++ ) {
      page[curRow][i] = fillc;
      style[curRow][i] = fillstyle;
    }
    screenDirty(curRow,curRow);
  }

  public synchronized void screenClearBOL(char fillc, int fillstyle) {
    for( int i=regionLeft; i<=curCol; i++ ) {
      page[curRow][i] = fillc;
      style[curRow][i] = fillstyle;
    }
    screenDirty(curRow,curRow);
  }

  public synchronized void screenClearLine(char fillc, int fillstyle) {
    for( int i=regionLeft; i<=regionRight; i++ ) {
      page[curRow][i] = fillc;
      style[curRow][i] = fillstyle;
    }
    screenDirty(curRow,curRow);
  }

  public synchronized void screenClearEOD(char fillc, int fillstyle) {
    int left = curCol;
    for( int j=curRow; j<=regionBottom; j++ ) {
      for( int i=left; i<=regionRight; i++ ) {
	page[j][i] = fillc;
	style[j][i] = fillstyle;
      }
      left = regionLeft;
    }
    screenDirty(curRow,regionBottom);
  }

  public synchronized void screenClearBOD(char fillc, int fillstyle) {
    for( int j=regionTop; j<curRow; j++ ) {
      for( int i=regionLeft; i<=regionRight; i++ ) {
	page[j][i] = fillc;
	style[j][i] = fillstyle;
      }
    }
    for( int i=regionLeft; i<curCol; i++ ) {
      page[curRow][i] = fillc;
      style[curRow][i] = fillstyle;
    }
    screenDirty(regionTop,curRow);
  }

  public synchronized void screenClear(char fillc, int fillstyle) {
    for( int j=regionTop; j<=regionBottom; j++ ) {
      for( int i=regionLeft; i<=regionRight; i++ ) {
	page[j][i] = fillc;
	style[j][i] = fillstyle;
      }
    }
    screenDirty(regionTop,regionBottom);
  }

  /* ------------------------------------------------------------
     SCREEN REFRESHING METHODS.
     ------------------------------------------------------------ */

  public synchronized void screenRedraw(Graphics g, int top, int bot,
					boolean full) {
    if( top < 0 ) top = 0;
    if( bot >= numRows ) bot = numRows-1;
    g.setFont(fontPlain);
    if( DEBUG ) System.out.println("WebTerm: Redrawing: " + top + " to " + bot);
    if( history_view < 0 && (history_changed || full) ) {
      int i = history_view + numRows - 1;
      if( i >= 0 ) i = -1;
      while( i>=history_view ) {
	if( DEBUG ) System.out.println("WebTerm: Drawing: " + i + ", history_view: " +
				       history_view);
	drawRow(g,i--);
      }
      top = 0;
      bot = numRows+history_view-1;
      if( bot < 0 ) top = bot = -1;
      full = true;
    }
    int maxbot = numRows+history_view-1;
    if( bot > maxbot ) bot = maxbot;
    if( top > bot ) top = bot = -1;
    if( top >= 0 ) {
      for( int i = top; i <= bot; i++ ) {
	if( full || dirty[i] ) {
	  if( DEBUG ) System.out.println("WebTerm: Drawing: " + i + ", curRow: " +
					 curRow);
	  drawRow(g,i);
	  dirty[i] = false;
	}
/*
      g.setColor(getBackground());
      g.fillRect(BORDER_WIDTH,BORDER_HEIGHT+(i*charHeight),
		 (numCols*charWidth),
		 charHeight);
      g.setColor(getForeground());
      g.drawChars(page[i],0,numCols,BORDER_WIDTH,
		  BORDER_HEIGHT + (i*charHeight + fontAscent));
      if( cursorVisible && i == curRow ) {
	if( DEBUG ) System.out.println("WebTerm: Drawing cursor: row=" + curRow +
				       ", col=" + curCol);
	drawRowCol(g,curRow,curCol);
      }
*/
      }
    }

    history_changed = false;
    dirtyTop = -1;
    dirtyBottom = -1;

    if( full ) toolkit.sync();
    scrollCount = 0;
  }

  public void screenRedraw(Graphics g) {
    screenRedraw(g,0,numRows-1,true);
  }

  public synchronized void screenClean(Graphics g) {
    if( DEBUG ) System.out.println("WebTerm: Cleaning screen: " + dirtyTop +
				   " to " + dirtyBottom);
    if( history_last_view != history_view
	|| history_last_len != history_len ) {
      Event e = new Event(this,Event.ACTION_EVENT,ACTION_VIEW);
      postEvent(e);
    }
    history_last_view = history_view;
    history_last_len = history_len;
    if( (dirtyTop >= 0 && dirtyBottom >= 0) || history_changed ) {
      drawRowCol(g,lastRow,lastCol);
      screenRedraw(g,dirtyTop,dirtyBottom,false);
    }
    if( lastRow != curRow || lastCol != curCol ) {
      drawRowCol(g,lastRow,lastCol);
      drawRowCol(g,curRow,curCol);
    }
    lastRow = curRow;
    lastCol = curCol;
    toolkit.sync();
  }

  /* ------------------------------------------------------------
     CHARACTER-STREAM METHODS.
     ------------------------------------------------------------ */

  public synchronized void write(char[] d, int off, int len, int flags) {
    if( outputecho && d != null ) System.out.print(new String(d,off,len));
    if( DEBUG ) {
      if( d == null ) d = new char[0];
      System.out.println("WebTerm: Write: " + new String(d,off,len));
    }
    if( len == 1 ) {
      write(d[off],flags);
    } else if( len > 1 ) {
      while( len > 0 ) {
	int last_row = curRow;
	boolean do_redraw;
	if( (flags&OUTF_RAW) != 0 ) do_redraw = put_char(d[off]);
	else do_redraw = put_std_char(d[off]);
	if( do_redraw ) {
	  if( last_row < curRow ) screenDirty(last_row,curRow);
	  else screenDirty(curRow,last_row);
	}
	off++;
	len--;
      }
      if( (flags&OUTF_PARTIAL) == 0 ) screenClean(getGraphics());
    } else if( (flags&OUTF_PARTIAL) == 0 ) screenClean(getGraphics());
  }

  public synchronized void write(String str, int flags) {
    write(str.toCharArray(),0,str.length(),flags);
  }

  public synchronized void write(char c, int flags) {
    if( outputecho ) System.out.print(c);
    int last_col = curCol, last_row = curRow;
    boolean do_redraw;
    if( (flags&OUTF_RAW) != 0 ) do_redraw = put_char(c);
    else do_redraw = put_std_char(c);
    if( do_redraw ) {
      if( last_row < curRow ) screenDirty(last_row,curRow);
      else screenDirty(curRow,last_row);
    }
    if( (flags&OUTF_PARTIAL) == 0 ) screenClean(getGraphics());
  }

  boolean put_char(char c) {
    if( c < curTranslation.length ) c = curTranslation[(int)c];
    if( curCol >= regionRight && !overChar ) {
      page[curRow][curCol] = c;
      style[curRow][curCol] = curStyle;
      curCol = regionRight;
      overChar = true;
      if( DEBUG ) System.out.println("WebTerm: Hit margin, overChar=" + overChar);
    } else if (curCol >= regionRight && overChar) {
      if( (curMode&MODE_NOWRAP) == 0 ) {
	overChar = false;
	put_std_char('\r');
	put_std_char('\n');
	/*
	  screenScroll(-1);
	  curRow = regionBottom;
	*/
	page[curRow][curCol] = c;
	style[curRow][curCol] = curStyle;
	curCol++;
	screenDirty(curRow-1,curRow);
	if( DEBUG ) System.out.println("WebTerm: Past margin, overChar=" + overChar);
      } else {
	page[curRow][curCol] = c;
	style[curRow][curCol] = curStyle;
	if( DEBUG ) System.out.println("WebTerm: Stay margin, overChar=" + overChar);
      }
    } else {
      page[curRow][curCol] = c;
      style[curRow][curCol] = curStyle;
      curCol++;
      overChar = false;
    }
    return true;
  }

  static final char CTRL_A = (char)1;
  static final char CTRL_B = (char)2;
  static final char CTRL_C = (char)3;
  static final char CTRL_D = (char)4;
  static final char CTRL_E = (char)5;
  static final char CTRL_F = (char)6;
  static final char CTRL_G = (char)7;
  static final char CTRL_H = (char)8;
  static final char CTRL_I = (char)9;
  static final char CTRL_J = (char)10;
  static final char CTRL_K = (char)11;
  static final char CTRL_L = (char)12;
  static final char CTRL_M = (char)13;
  static final char CTRL_N = (char)14;
  static final char CTRL_O = (char)15;
  static final char CTRL_P = (char)16;
  static final char CTRL_Q = (char)17;
  static final char CTRL_R = (char)18;
  static final char CTRL_S = (char)19;
  static final char CTRL_T = (char)20;
  static final char CTRL_U = (char)21;
  static final char CTRL_V = (char)22;
  static final char CTRL_W = (char)23;
  static final char CTRL_X = (char)24;
  static final char CTRL_Y = (char)25;
  static final char CTRL_Z = (char)26;

  boolean put_std_char(char c) {
    boolean do_redraw = false;

    switch( c ) {
    case Emulator.NUL:
      break;
    case Emulator.BEL:     // Bell
      break;
    case Emulator.BS:     // Backspace
      curCol--;
      if(curCol < regionLeft) {
	curCol = regionLeft;
      }
      //do_redraw = true;
      break;
    case Emulator.HT:     // Tab
      int newcol = (curCol&0xfff8) + 8;
      if( newcol > regionRight ) newcol = regionRight;
      curCol = newcol;
      break;
    case Emulator.LF:
      if( curRow >= regionBottom ) {
	screenScroll(-1);
	curRow = regionBottom;
	screenDirty(regionBottom,regionBottom);
      } else {
	curRow++;
      }
      if( (curMode&MODE_NEWLINE) != 0 ) curCol=regionLeft;
      overChar = false;
      //do_redraw = true;
      break;
    case Emulator.CR:
      curCol = regionLeft;
      //do_redraw = true;
      break;
    case Emulator.FF:
      screenScroll(-(regionBottom-regionTop+1));
      break;
    case Emulator.DEL:
      for( int i=curCol; i<regionRight; i++ ) {
	page[curRow][i] = page[curRow][i+1];
	style[curRow][i] = style[curRow][i+1];
      }
      page[curRow][regionRight] = ' ';
      style[curRow][regionRight] = curStyle;
      screenDirty(curRow,curRow);
      do_redraw = true;
      break;
    default:
      do_redraw = put_char(c);
      break;
    }

    return do_redraw;
  }

  /* ------------------------------------------------------------
     EVENT HANDLING METHODS.
     ------------------------------------------------------------ */

  static final boolean DEBUG_EVT = false;

  public boolean handleEvent(Event e) {
    if( e.id == e.KEY_ACTION ) {
      if( DEBUG_EVT || DEBUG ) System.out.println("WebTerm: Key event: " + e);
      if( emulator != null ) return emulator.handleEvent(e);
    }
    return super.handleEvent(e);
  }

  public boolean keyDown(Event e, int key ) {
 
    if( DEBUG_EVT || DEBUG ) System.out.println("WebTerm: Key down: " + e);
    if( emulator != null ) {
      if( emulator.handleEvent(e) ) return true;
      if( key == ' ' && e.modifiers == e.CTRL_MASK ) key = '\000';
      else if( key == '@' && e.modifiers == (e.CTRL_MASK|e.SHIFT_MASK) )
	key = '\000';

      if( key == '\n' ) emulator.send('\r');
      emulator.send((char)key);
      return true;
    }
    return false;

  }

  boolean getting_focus = false;

  public boolean mouseDown(Event e, int x, int y) {
    if( !have_focus ) {
      getting_focus = true;
      requestFocus();
      return true;
    } else {
      if( DEBUG_EVT || DEBUG ) System.out.println("WebTerm: Mouse down: " + e);
      if( emulator != null ) return emulator.handleEvent(e);
    }
    return false;
  }

  public boolean mouseUp(Event e, int x, int y) {
    if( getting_focus ) {
      getting_focus = false;
      return true;
    } else {
      if( DEBUG_EVT || DEBUG ) System.out.println("WebTerm: Mouse up: " + e);
      if( emulator != null ) return emulator.handleEvent(e);
    }
    return false;
  }

  public void paint(Graphics g) {
    draw_border(g);
    screenRedraw(g);
  }

  public synchronized boolean gotFocus(Event evt, Object what) {
    Graphics g = getGraphics();
    have_focus = true;
    if( g != null ) draw_border(g);
    return true;
  }

  public synchronized boolean lostFocus(Event evt, Object what) {
    Graphics g = getGraphics();
    have_focus = false;
    if( g != null ) draw_border(g);
    return true;
  }

  public final synchronized void update (Graphics g) {
    paint(g);
  }

  /* ------------------------------------------------------------
     PRIVATE RENDERING METHODS.
     ------------------------------------------------------------ */

  Font get_style_font(int style) {
    switch( style&(STYLE_BOLD|STYLE_ITALIC) ) {
    case STYLE_PLAIN:
      return fontPlain;
    case STYLE_BOLD:
      return fontBold;
    case STYLE_ITALIC:
      return fontItalic;
    case STYLE_BOLD|STYLE_ITALIC:
      return fontBoldItalic;
    default:
      return fontPlain;
    }
  }

  void drawRowCol(Graphics g, int row, int col) {
    if( row >= (numRows+history_view) ) return;
    int x = ( col * charWidth ) + BORDER_WIDTH;
    int y = ( (row-history_view) * charHeight ) + BORDER_HEIGHT;
    int st = style[row][col];
    g.setFont(get_style_font(style[row][col]));
    if( ( ( curRow != row || curCol != col || !cursorVisible )
	  ^ ( (st&STYLE_INVERSE) == STYLE_INVERSE) )
	^ ( (curMode&MODE_INVERSE) != 0 ) ) {
      g.setColor(textBackground[(st>>STYLE_BACKGROUND_POS)&STYLE_COLOR_MASK]);
      g.fillRect(x,y,charWidth,charHeight);
      //g.clearRect(x,y,charWidth,charHeight);
      g.setColor(textForeground[(st>>STYLE_FOREGROUND_POS)&STYLE_COLOR_MASK]);
      g.drawChars(page[row],col,1,x,y+fontAscent);
    } else {
      g.setColor(textForeground[(st>>STYLE_FOREGROUND_POS)&STYLE_COLOR_MASK]);
      g.fillRect(x,y,charWidth,charHeight);
      g.setColor(textBackground[(st>>STYLE_BACKGROUND_POS)&STYLE_COLOR_MASK]);
      g.drawChars(page[row],col,1,x,y+fontAscent);
    }
    if( (style[row][col]&STYLE_UNDERSCORE) != 0 ) {
      g.drawLine(x,y+fontAscent,x+charWidth-1,y+fontAscent);
    }
  }

  void drawRow(Graphics g, int row) {
    int col=0;
    int y = BORDER_HEIGHT+((row-history_view)*charHeight);

    char[] rowChars;
    int[] rowStyles;
    if( row < 0 ) {
      int off = (history_top+history_len+row)
	% history_char.length;
      rowChars = history_char[off];
      rowStyles = history_style[off];
    } else {
      rowChars = page[row];
      rowStyles = style[row];
    }

    if( rowStyles == null || rowChars == null ) {
      System.out.println("WebTerm: *** null entry at row " + row);
      return;
    }

    while( col < numCols ) {
      int first=col;
      int cur_style = rowStyles[col];
      boolean inverse =  ( ( (cur_style&STYLE_INVERSE) != 0 )
			   ^ ( (curMode&MODE_INVERSE) != 0 ) );
      while( col < numCols && cur_style == rowStyles[col] ) {
	col++;
      }
      int x = BORDER_WIDTH+(first*charWidth);
      g.setFont(get_style_font(cur_style));

      //System.out.println("Style = " + Integer.toString(cur_style,16));
      if( inverse ) g.setColor(textForeground[(cur_style>>STYLE_FOREGROUND_POS)
					     &STYLE_COLOR_MASK]);
      else g.setColor(textBackground[(cur_style>>STYLE_BACKGROUND_POS)
				    &STYLE_COLOR_MASK]);

      g.fillRect(x,y,((col-first)*charWidth),charHeight);

      if( inverse ) g.setColor(textBackground[(cur_style>>STYLE_BACKGROUND_POS)
					     &STYLE_COLOR_MASK]);
      else g.setColor(textForeground[(cur_style>>STYLE_FOREGROUND_POS)
				    &STYLE_COLOR_MASK]);

      g.drawChars(rowChars,first,col-first,x,y+fontAscent);

      if( (cur_style&STYLE_UNDERSCORE) != 0 ) {
	g.drawLine(x,y+fontAscent,x+((col-first)*charWidth),y+fontAscent);
      }
    }
    if( cursorVisible && row == curRow ) {
      if( DEBUG ) System.out.println("WebTerm: Drawing cursor: row=" + curRow +
				     ", col=" + curCol);
      drawRowCol(g,curRow,curCol);
    }
  }

  void draw_border(Graphics g) {
    Dimension d = size();
    if( have_focus ) {
      g.setColor(defForeground);
    } else {
      g.setColor(defBackground);
    }
    g.drawRect(0,0,d.width-1,d.height-1);
    g.drawRect(1,1,d.width-3,d.height-3);
    g.setColor(embossOut);
    if( ( (textBackground[0].getBlue() + textBackground[0].getGreen() +
	   textBackground[0].getRed())
	  / 3 ) >= 128 ) {
      g.draw3DRect(2,2,d.width-5,d.height-5,false);
      g.draw3DRect(3,3,d.width-7,d.height-7,false);
    } else {
      g.draw3DRect(2,2,d.width-5,d.height-5,true);
      g.draw3DRect(3,3,d.width-7,d.height-7,true);
    }
  }

  void screenDirty(int top, int bottom) {
    if( dirtyTop < 0 || top < dirtyTop ) dirtyTop = top;
    if( bottom > dirtyBottom ) dirtyBottom = bottom;
    for( int i=top; i<=bottom; i++ ) dirty[i] = true;
  }
}
