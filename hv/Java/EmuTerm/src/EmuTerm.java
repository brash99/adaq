/*
 * WebTerm
 * ~~~~~~~
 *
 * Telnet session client, VT100 terminal emulator, and terminal screen
 * driver.
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
 * 1.4: Added "Options..." button and control of EmulatorOptions window.
 *      Moved RubberTextField into its own file.
 *      Added scroll bar for Terminal's new display history.
 *
 * 1.3: Divided classes into separate files.
 * 
 * 1.2: Added support for text styles.
 * 
 * 1.1: Applet arguments are now handed to the Telnet class, which
 *      parses the new prompt[n], reply[n], and endprompt[n] arguments.
 * 
 * 1.0: First release.
 *
 */

//import java.applet.Applet;    
import java.awt.*; 
import java.io.*;
import java.util.*;
import java.awt.event.KeyEvent;
import java.awt.Robot;
import java.awt.event.InputEvent;

//import Terminal;
//import Emulator;
//import EmulatorOptions;
//import Telnet;
//import Parameters;
//import RubberTextField;

// This module is automatically created by the Makefile, and contains
// data on the last build.  We dynamically load the module,
// so it doesn't have to exist.
// import WebTerm__Build;

public class EmuTerm extends Frame implements Parameters {

  static final boolean DEBUG = true;

  static public String VersionString = "2.0";

  Terminal terminal;
  Telnet telnet;
  GridBagLayout layout;
  Scrollbar history_bar;
  Button conn_but;
  Button dis_but;
  Button opt_but;
  Button save_but;
  Button test_but;

  TextField host_txt;
  TextField port_txt;

  EmulatorOptions opt_win = null;

  String emulation = "Emulator";
  String emu_status = "Unknown";
  Emulator req_emulator;
  Emulator emulator;

  String buildTag = "";

  boolean initialized = false;

    final Toolkit toolkit = Toolkit.getDefaultToolkit();
    final EventQueue eventQueue = toolkit.getSystemEventQueue();

  public EmuTerm() {
    try {
      Class newcls = Class.forName(getClass().getName() + "__Build");
      if( newcls != null ) {
	Hashtable hash = (Hashtable)newcls.newInstance();
	if( hash != null ) {
	  buildTag = " (" + hash.get("dateString") + ")";
	  if( DEBUG ) System.out.println("WebTerm: Got build tag: " + buildTag);
	  VersionString = "" + hash.get("versionString") + " build " +
	    hash.get("build");
	  if( DEBUG ) System.out.println("WebTerm: Got version tag: " + VersionString);
	}
      }
    } catch(Exception e) {
      if( DEBUG ) System.out.println(e.toString());
    }
  }

  public String getAppletInfo() {
    return ( "WebTerm v" + VersionString + buildTag + "\n\n" +
	     "The design and implementation of WebTerm are available for\n" +
	     "royalty-free adoption and use for non-commercial purposes,\n" +
	     "by an public or private organization.  Copyright is retained\n" +
	     "by the Northwest Alliance for Computational Science and\n" +
	     "Engineering and Oregon State University.  Redistribution\n" +
	     "of any part of WebTerm or any derivative works must include\n" +
	     "this notice.\n" );
  }

  public String[][] getParameterInfo() {
    String[][] telnet_info = Telnet.getParameterInfo();
    String[][] terminal_info = Terminal.getParameterInfo();
    String[][] applet_info = {
      { "host", "string", "initial name of telnet host" },
      { "port", "integer", "initial port number (Telnet is 23)" },
      { "emulator", "string", "initial emulator class" }
    };
    String[][] info =
      new String[telnet_info.length + terminal_info.length +
		applet_info.length][];

    int i = 0;
    
    for( int pos=0; i<telnet_info.length; i++,pos++ ) {
      info[i] = telnet_info[pos];
    }
    for( int pos=0; i<terminal_info.length; i++,pos++ ) {
      info[i] = terminal_info[pos];
    }
    for( int pos=0; i<applet_info.length; i++,pos++ ) {
      info[i] = applet_info[pos];
    }

    return info;
  }

  /* Implement our parameter interface */
  public String getParameter(String name) {
      //    return super.getParameter(name);
      if(name=="register") {
	  return "terminal";
      }
      if(name=="fontsize") {
	  return "12";
      }
      if(name=="background") {
	  return "default";
      }
      if(name=="foreground") {
	  return "default";
      }
      if(name=="history") {
	  return "50";
      }
      if(name=="outputecho") {
	  return null;
      }
      if(name=="port") {
	  return "2008";
      }
      if(name=="host") {
	  return "cdetts2";
      }

      return null;
  }

  public Emulator setEmulator(String name) {
    req_emulator = null;

    if( name != null ) {
      try {
	if( opt_win != null ) opt_win.setEmulatorStatus("Retrieving: " + name);
	Class emuclass = Class.forName(name);
	if( opt_win != null ) opt_win.setEmulatorStatus("Starting: " + name);
	if( emuclass != null ) req_emulator = (Emulator)emuclass.newInstance();
      } catch( ClassNotFoundException e ) {
	emu_status = "Unable to retrieve: " + name;
      } catch( InstantiationException e ) {
	emu_status = "Unable to instantiate: " + name;
      } catch( IllegalAccessException e ) {
	emu_status = "Unable to access: " + name;
      } catch( ClassCastException e ) {
	emu_status = "Not an emulator: " + name;
      } catch( Exception e ) {
	emu_status = "Error: " + e.toString();
      }
    }

    if( req_emulator != null ) {
      if( emulator != null ) {
	emulator.setTelnet(null);
	emulator.setTerminal(null);
      }
      emulator = req_emulator;
      req_emulator.setTerminal(terminal);
      req_emulator.setTelnet(telnet);
      emulation = name;
      emu_status = "Emulator loaded and active.";
      if( opt_win != null ) opt_win.setEmulatorStatus(emu_status);
      return emulator;
    }

    req_emulator = null;
    if( opt_win != null ) opt_win.setEmulatorStatus(emu_status);

    if( emulator == null ) {
      emulator = new Emulator();
      emulator.setTerminal(terminal);
      emulator.setTelnet(telnet);
    }

    return null;
  }

  public void paint(Graphics g) {
    if( !initialized ) {
      Dimension d = size();
      FontMetrics m = g.getFontMetrics(g.getFont());
      String str = "WebTerm is loading...";
      int strWidth = m.stringWidth(str);
      g.drawString(str,(d.width/2)-(strWidth/2),(d.height/2));
    }
  }

  // Initialize applet -- create GUI components, emulator, telnet client.
  public void init() {
    String host = getParameter("host");
    String port = getParameter("port");
    emulation = getParameter("emulator");
    Label lab;

    //    super.init();

    repaint();

    if( DEBUG ) System.out.println("WebTerm: Requested host: " + host);
    if( DEBUG ) System.out.println("WebTerm: Requested port: " + port);
    if( DEBUG ) System.out.println("WebTerm: Requested emulator: " + emulation);

    if( host == null ) host="";// host = getCodeBase().getHost();
    if( port == null ) port = "23";

    GridBagConstraints cn = new GridBagConstraints();
    layout = new GridBagLayout();
    this.setLayout(layout);

    // Create the top row of controls.

    cn.gridx = cn.RELATIVE;
    cn.gridy = 0;
    cn.gridwidth = 2;
    cn.gridheight = 1;
    cn.fill = cn.NONE;
    cn.anchor = cn.CENTER;
    cn.weightx = 0;
    cn.weighty = 0;

    conn_but = new Button("Connect");
    layout.setConstraints(conn_but,cn);
    this.add(conn_but);

    cn.gridwidth = 1;

    dis_but = new Button("Disconnect");
    layout.setConstraints(dis_but,cn);
    this.add(dis_but);
    dis_but.disable();

    opt_but = new Button("Options...");
    layout.setConstraints(opt_but,cn);
    this.add(opt_but);

    lab = new Label("Host:");
    layout.setConstraints(lab,cn);
    this.add(lab);

    cn.fill = cn.HORIZONTAL;
    cn.weightx = 3;
    host_txt = new RubberTextField(host,100,32,2);
    layout.setConstraints(host_txt,cn);
    this.add(host_txt);

    cn.fill = cn.NONE;
    cn.weightx = 0;
    lab = new Label("Port:");
    layout.setConstraints(lab,cn);
    this.add(lab);

    cn.fill = cn.HORIZONTAL;
    cn.weightx = 0;
    port_txt = new RubberTextField(port,5,5,4);
    layout.setConstraints(port_txt,cn);
    this.add(port_txt);

    // create bottom control
 /*** 13 Feb 2015 ***/
    cn.gridx = cn.RELATIVE;
    cn.gridy = 2;
    cn.gridwidth = 2;
    cn.gridheight = 1;
    cn.fill = cn.NONE;
    cn.anchor = cn.CENTER;
    cn.weightx = 0.2;
    cn.weighty = 0;

    save_but = new Button("Save");
    layout.setConstraints(save_but,cn);
    this.add(save_but);
    
    //    cn.gridx = 1;
    //    cn.gridy = 2;
    cn.gridwidth = 1;

    test_but = new Button("Test");
    layout.setConstraints(test_but,cn);
    this.add(test_but);

    /***   ***/

    // Create the terminal screen area.

    cn.gridx = 0;
    cn.gridy = 1;
    cn.gridwidth = 1;
    cn.gridheight = 1;
    cn.fill = cn.VERTICAL;
    cn.anchor = cn.CENTER;
    cn.weightx = 0;
    cn.weighty = 1;

    history_bar = new Scrollbar(Scrollbar.VERTICAL,0,1,0,0);
    layout.setConstraints(history_bar,cn);
    this.add(history_bar);

    cn.gridx = 1;
    cn.gridwidth = cn.REMAINDER;
    cn.gridheight = 1;
    cn.fill = cn.BOTH;
    cn.anchor = cn.CENTER;
    cn.weightx = 1;
    cn.weighty = 1;


    terminal = new Terminal();
    //terminal.resize(this.size());
    layout.setConstraints(terminal,cn);
    this.add(terminal);

    terminal.parseParameters((Parameters)this);
    terminal.init();

    //this.resize(this.size());
    this.layout();

    telnet = new Telnet(null);
    telnet.parseParameters((Parameters)this);

    // Try to load a nice initial emulator
    if( emulation == null || setEmulator(emulation) == null ) {
      if( setEmulator("VT200") == null ) {
	if( setEmulator("VT100Emulator") == null ) {
	  if( setEmulator("Emulator") == null ) {
	    emulation = "Emulator";
	    System.out.println("WebTerm: Unable to find any emulator!");
	  }
	}
      }
    }

    initialized = true;

    /*
    emulator = new VT100Emulator();
    emulator.setTerminal(terminal);
    emulator.setTelnet(telnet);
    */
  }

  public void destroy() {
    if( telnet != null ) telnet.disconnect();
    if( opt_win != null ) opt_win.dispose();
  }

  boolean written_copyright = true;
  public void start() {
    if( !written_copyright && terminal != null ) {
      written_copyright = true;
      terminal.setStyle(Terminal.STYLE_BOLD);
      terminal.setForeColor(5);
      terminal.write("\r\nWebTerm ",0);
      terminal.setStyle(Terminal.STYLE_PLAIN);
      terminal.setForeColor(0);
      terminal.write("v" + VersionString + buildTag + "\r\n",0);
      terminal.setStyle(Terminal.STYLE_ITALIC);
      terminal.setForeColor(2);
      terminal.write("VT220/VT100/VT52/XTerm terminal emulator " +
		     "and Telnet client\r\n",0);
      //"written by Dianne Hackborn and Melanie Johnson.\r\n",0);
      terminal.setStyle(Terminal.STYLE_PLAIN);
      terminal.setForeColor(0);
      terminal.write("\r\nCopyright 1996 by " +
		     "the Northwest Alliance for Computational Science " +
		     "and Engineering and Oregon State University.\r\n\r\n",0);
      /*
      terminal.write("\r\nThe design and implementation of WebTerm are\r\n" +
		     "available for royalty-free adoption and use for\r\n" +
		     "non-commercial purposes, by any public or\r\n" +
		     "private organization.  Copyright is retained by\r\n" +
		     "the Northwest Alliance for Computational Science\r\n" +
		     "and Engineering and Oregon State University.\r\n" +
		     "Redistribution of any part of WebTerm or any\r\n" +
		     "derivative works must include this notice.\r\n" +
		     "All Rights Reserved.\r\n",0);
      terminal.write("\r\nPlease address correspondence to " +
		     "nacse-questions@nacse.org.\r\n\r\n",
		     0);
		     */
      // Special code for checking Unicode symbols...
      if( false ) {
	int[] sections = {
	  0x0000, 0x0040, 0x0080, 0x00c0,
	  0x0100, 0x0140, 0x0180, 0x01c0,
	  0x2500, 0x2540, 0x2580, 0x25c0,
	  0x2600, 0x2640, 0x2680, 0x26c0
	};
	StringBuffer str = new StringBuffer(80);
	for( int i=0; i<sections.length; i++ ) {
	  str.setLength(0);
	  String numstr = Integer.toString(sections[i],16);
	  for( int j=4; j>numstr.length(); j-- ) str.append('0');
	  str.append(numstr);
	  str.append(": ");
	  for( int j=0; j<0x40; j++ ) str.append((char)(sections[i]+j));
	  terminal.write(str.toString(),Terminal.OUTF_RAW);
	  terminal.write("\r\n",0);
	}
      }
      if( false ) {
	for( int i=0; i<9; i++ ) {
	  terminal.setForeColor(i);
	  terminal.write("  abcdefghijklmnopqrstuvwxyz123456789  \r\n",0);
	}
	terminal.setForeColor(0);
	for( int i=0; i<9; i++ ) {
	  terminal.setBackColor(i);
	  terminal.write("  abcdefghijklmnopqrstuvwxyz123456789  \r\n",0);
	}
	terminal.setBackColor(0);
      }
    }
    if( opt_win != null ) opt_win.show();
  }

  public void stop() {
    if( opt_win != null ) opt_win.hide();
  }
    /*
     * action()
     */
  public boolean action(Event event, Object arg) {
    if( DEBUG ) System.out.println("WebTerm: Applet: " + event);
    if( event.target == conn_but ) {
      if( telnet != null ) {
	try {
	  telnet.connect(host_txt.getText(),
			 Integer.parseInt(port_txt.getText()));
	}
	catch (NumberFormatException ex) {
	  if( terminal != null )
	    terminal.write("\r\nPort must be a number.\r\n",0);
	}
      }
      return true;
    } else if( event.target == dis_but ) {
      if( telnet != null ) telnet.disconnect();
      return true;
    } else if( event.target == opt_but ) {
      if( opt_win == null ) {
	opt_win = new EmulatorOptions(this,emulator,telnet,(Parameters)this);
	opt_win.pack();
	opt_win.show();
	opt_win.setEmulatorStatus(emu_status);
	if( opt_but != null ) opt_but.disable();
      }
      if( terminal != null && telnet != null && telnet.getHost() != null )
	terminal.requestFocus();
      return true;
  /*** save ***/
    } else if( event.target == save_but ) {
	if( DEBUG ) System.out.println("WebTerm: action: SAVE button");
	if( terminal != null && telnet != null && telnet.getHost() != null )
	terminal.requestFocus();
      return true;
  /*** test ***/
    } else if( event.target == test_but ) {
	if( DEBUG ) System.out.println("WebTerm: action: TEST button");
	if( terminal != null && telnet != null && telnet.getHost() != null )
	terminal.requestFocus();
	//pressSimKey(KeyEvent.VK_UP);
	//	pressKeySim(KeyEvent.VK_RIGHT);
	//	emulator.send((char) KeyEvent.VK_U);
	//printTermScreen();
	String str=getString(3);
	StringTokenizer st = new StringTokenizer(str);
	System.out.println("+++token3:");
	while (st.hasMoreTokens()) {
	    System.out.println(st.nextToken());
	}	
	System.out.println("---token3");
	
	for(int i=0;i<1; i++) {
	    str=getString(4);
	    st = new StringTokenizer(str);
	    System.out.println("["+i+"]:");
	    while (st.hasMoreTokens()) {
		System.out.print(" : ");
		System.out.print(st.nextToken());
	    }	
	    System.out.println("\n******");
	    terminal.requestFocus();
	    pressSimKey(KeyEvent.VK_N); //n (ascii:110) - Next
	    Sleep(3000);
	    String upd = "";
	    str=getString(0);
	    System.out.println("str0= " + str);
	    System.out.println("str23= " + getString(23));
	    //printTermScreen();
	    //while (str.length() < 70) {

	    if(false)	    while (!upd.equals("A932A")) {
		str=getString(0);
		//	System.out.println("str= " + str);
		
		upd=str.substring(69,76);	
		if(upd!=null)
		    System.out.println("upd="+upd);	
		Sleep(1000);
	    }
	    terminal.requestFocus();
	} // end for
	terminal.requestFocus();

      return true;
  /********************/
    } else if( event.target == opt_win ) {
      if( arg == EmulatorOptions.ACTION_EMULATION ) {
	Emulator emu = setEmulator(opt_win.getEmulationName());
	if( emu != null ) opt_win.setEmulator(emu);
	return true;
      } else if( arg == EmulatorOptions.ACTION_TERMINATE ) {
	if( opt_but != null ) {
	  opt_but.enable();
	}
	if( opt_win != null ) opt_win.dispose();
	opt_win = null;
	return true;
      }
    } else if( event.target == terminal && event.id == Event.ACTION_EVENT ) {
      if( arg == Terminal.ACTION_DISCONNECT ) {
	disconnect();
	return true;
      } else if( arg == Terminal.ACTION_CONNECT ) {
	connect();
	//	pressSimKey(KeyEvent.VK_U); // u - Update
	emulator.send((char) KeyEvent.VK_D); // D - display menu
	emulator.send((char) KeyEvent.VK_D); 
	emulator.send((char) KeyEvent.VK_U);
	terminal.requestFocus();

	return true;
      } else if( arg == Terminal.ACTION_VIEW ) {
	if( history_bar != null ) {
	  if( DEBUG ) System.out.println("WebTerm: View pos: " +
					 terminal.getDataPos() +
					 ", view: " +
					 terminal.getDataView() +
					 ", total: " +
					 terminal.getDataTotal());
	  history_bar.setValues(terminal.getDataPos(),
				terminal.getDataView(),
				0, terminal.getDataTotal() -
				terminal.getDataView());
	  history_bar.setLineIncrement(1);
	  history_bar.setPageIncrement(terminal.getDataView()-1);
	  return true;
	}
      }
    }
    return super.action(event,arg);
  }


  public void showStatus (String msg) {
	//void
    }

  public boolean handleEvent(Event e) {
    if (e.id == Event.MOUSE_ENTER) {
      String tmp = null;
      if( telnet != null ) {
	tmp = telnet.getHost();
	if( tmp != null ) tmp = "WebTerm Connected: <" + tmp +
			    "> port #" + telnet.getPort();
      }
      if( tmp == null )
	tmp = "WebTerm (c)1996 NACSE and Oregon State University.";
      showStatus(tmp);
      return true;
    } else if (e.id == Event.MOUSE_EXIT) {
      showStatus("");
      return true;
    } else if( e.target == history_bar ) {
      if( DEBUG ) System.out.println("WebTerm: History bar: " + e.toString());
      if( terminal != null ) {
	terminal.setDataPos(history_bar.getValue());
	return true;
      }
    }
    return super.handleEvent(e);
  }

  public void connect() {
    conn_but.disable();
    dis_but.enable();
    host_txt.disable();
    port_txt.disable();
    if( terminal != null ) terminal.requestFocus();
  }

  public void disconnect() {
    conn_but.enable();
    dis_but.disable();
    host_txt.enable();
    port_txt.enable();
    conn_but.requestFocus();
    if( terminal != null ) terminal.setStyle(terminal.STYLE_PLAIN);
  }

    /** 16 Feb 2015 **/
  public String  getString(int row) {
      StringBuffer sb = new StringBuffer();
      for(int i=0; i<terminal.getNumCols(); i++) { 
	  sb.append(terminal.getChar(row, i));
      }
      //System.out.println("row="+row+" : "+ sb.toString());
      return sb.toString(); 
  }
  public void printTermScreen() {
      System.out.println("*****");
      for(int k=0; k<terminal.getNumRows(); k++) { 
	  System.out.println(getString(k));	
     }
       System.out.println("******************");
  }  

   /** 13 Feb 2014 **/
   /** Simulate a key press/ release routines
   *  pressSimKey(int keycode) - uses Robot to simulate KEy pressed/released
   *  pressKeySim(int keycode) - uses toolkit.getSystemEventQueue().postEvent()
   *  Both methods are work! But, terminal need tobe in focus (terminal.requestFocus()) after routine call. 
   ***/
    public boolean pressSimKey(int keycode) {
	try {
	    Robot robot = new Robot();
	    // Simulate a key press
	    //	    robot.keyPress(KeyEvent.VK_A);
	    robot.keyPress(keycode);
	    robot.keyRelease(keycode);
	    
	} catch (AWTException e) {
	    e.printStackTrace();
	    return false;
	}
	return true;
    } 

    public boolean pressKeySim(int keycode) {
	// press
	eventQueue.postEvent( new KeyEvent( this
					    , KeyEvent.KEY_PRESSED
					    , System.currentTimeMillis()
					    , 0 // InputEvent.CTRL_DOWN_MASK 
					    //, KeyEvent.VK_LEFT.
					    , keycode
					    , KeyEvent.CHAR_UNDEFINED
					    )
			      );
	// release
	eventQueue.postEvent( new KeyEvent( this
					    , KeyEvent.KEY_RELEASED
					    , System.currentTimeMillis()
					    , 0 // InputEvent.CTRL_DOWN_MASK 
					    //, KeyEvent.VK_LEFT.
					    , keycode
					    , KeyEvent.CHAR_UNDEFINED
					    )
			      );
	return true;
    }
    
    public void Sleep(int ms) {
	  try {
	      Thread.sleep(ms);                 //1000 milliseconds is one second.
	  } catch(InterruptedException ex) {
	      Thread.currentThread().interrupt();
	  }
    };

    /*
     * 13 Jan 2015
     * For test purpose 
     */
  public static void main(String[] args) {
      EmuTerm f = new EmuTerm();
      f.addWindowListener(new java.awt.event.WindowAdapter() {
	      public void windowClosing(java.awt.event.WindowEvent e) {
		  System.exit(0);
	      };
	  });

      f.pack();
      f.init();
      f.setSize(800,600 + 20); // add 20, seems enough for the Frame title,
      f.show();
      f.terminal.requestFocus();

      System.out.println("Press: "+f.pressKeySim(1006));

      if(false)
      for(int i=0; i<3;i++) {     
	  try {
	      Thread.sleep(3000);                 //3000 milliseconds is one second.
	  } catch(InterruptedException ex) {
	      Thread.currentThread().interrupt();
	  }
	  //f.pressSimKey(KeyEvent.VK_RIGHT);
	  f.printTermScreen();
      }
      
      //      if(f.emulator!=null)
      //	  f.emulator.send((char) KeyEvent.VK_U); // U - update
      System.out.println("Done");   
  }



}
