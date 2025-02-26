/*
 * ClassRegistry version 1.0
 * ~~~~~~~~~~~~~
 *
 * A set of classes for communicating between objects that exist in
 * different applets in different frames, windows, or documents.
 *
 * Written by Dianne Hackborn
 *
 * Copyright (C) 1996 by Oregon State University and the
 * Northwest Alliance for Computational Science and Engineering (NACSE).
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
 * 1.0: First release.
 *
 */

import java.applet.Applet;    
import java.awt.*; 
import java.io.*;

//import ClassConnection;
//import ClassRegistry;

/* This applet implements a simple button that sends a message
   to a ClassRegistry every time it is pressed by the user.
   There are four parameters used to control its behavior:

   REGISTER: A string defining the registry name to send the
             command to.  The default is "DemoApplet".

   LABEL:    A string defining the text to draw on the button's
             face.  The default is "Demo".

   COMMAND:  A string with the name of the command to send
             to the registry.  The default is "send".

   TEXT:     A string with the data to send to the registry.
             The default is "Demo".
*/

public class RegistryButton extends Applet {

  static final boolean DEBUG = false;

  static public String VersionString = "1.0";

  String register = null;
  String command = null;
  String send_text = null;

  GridBagLayout layout;
  Button send_but;

  public String getAppletInfo() {
    return ( "ClassRegistry Demo Button v" + VersionString + ".\n\n" +
	    "This program is Copyright (C) 1996 by\r\n" +
	    "Oregon State University, Corvallis, Oregon and the\n" +
	    "Northwest Alliance for Computational Science\n" +
	    "and Engineering (NACSE).  All Rights Reserved.\n" );
  }

  public String[][] getParameterInfo() {
    String[][] applet_info = {
      { "register", "string", "registry name to send commands to" },
      { "label", "string", "label on button face" },
      { "command", "string", "command to send to registry" },
      { "text", "string", "text to send to registry" }
    };

    return applet_info;
  }

  String parseString(String in) {
    if( in == null ) return null;
    StringBuffer out = new StringBuffer();
    for( int i=0; i<in.length(); i++ ) {
      if( in.charAt(i) != '\\' ) out.append(in.charAt(i));
      else if( ++i < in.length() ) {
	switch( in.charAt(i) ) {
	case '\\':
	  out.append('\\');
	  break;
	case 'n':
	  out.append('\n');
	  break;
	case 'r':
	  out.append('\r');
	  break;
	case 't':
	  out.append('\t');
	  break;
	case 'f':
	  out.append('\f');
	  break;
	case 'b':
	  out.append('\b');
	  break;
	case 'u':
	  {
	    int c = 0;
	    for( int j=0; j<4 && i<in.length(); j++,i++ ) {
	      c<<=4;
	      c = c+(int)(in.charAt(i)-'0');
	    }
	    out.append((char)c);
	  } break;
	case 'x':
	  {
	    int c = 0;
	    for( int j=0; j<2 && i<in.length(); j++,i++ ) {
	      c<<=4;
	      c = c+(int)(in.charAt(i)-'0');
	    }
	    out.append((char)c);
	  } break;
	default:
	  if( in.charAt(i) >= '0' && in.charAt(i) <= '9' ) {
	    int c = 0;
	    for( int j=0; j<3 && i<in.length(); j++,i++ ) {
	      c<<=3;
	      c = c+(int)(in.charAt(i)-'0');
	    }
	    out.append((char)c);
	  }
	}
      }
    }
    return out.toString();
  }

  public void init() {
    register = getParameter("register");
    String label = getParameter("label");
    command = getParameter("command");
    send_text = getParameter("text");

    if( register == null ) register = "DemoApplet";
    if( label == null ) label = "Demo";
    if( command == null ) command = "send";
    if( send_text == null ) send_text = "Demo\n";

    send_text = parseString(send_text);

    super.init();

    GridBagConstraints cn = new GridBagConstraints();
    layout = new GridBagLayout();
    this.setLayout(layout);

    cn.gridx = cn.RELATIVE;
    cn.gridy = 0;
    cn.gridwidth = 1;
    cn.gridheight = 1;
    cn.fill = cn.NONE;
    cn.anchor = cn.CENTER;
    cn.weightx = 1;
    cn.weighty = 1;

    send_but = new Button(label);
    layout.setConstraints(send_but,cn);
    this.add(send_but);
    
    this.layout();
  }

  public boolean action(Event event, Object arg) {
    if( DEBUG ) System.out.println("Applet: " + event);
    if( event.target == send_but ) {
      ClassRegistry.doCommand(register,command,send_text);
      return true;
    }
    return super.action(event,arg);
  }
}

