/*
 * WebTerm
 * ~~~~~~~
 *
 * EmulatorOptions class: a GUI window that allows the user to view
 * and change an emulator's (and the underlying terminal's) settings.
 *
 * This class should not be subclassed by new emulations -- instead,
 * the Emulator class has a method that returns an AWT container
 * of the subclass's custom options.
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
 * 1.4: Created this file.
 *
 */

import java.awt.*; 
import java.io.*;
import java.util.*;
import java.applet.*;

//import Emulator;
//import Parameters;
//import RubberTextField;

/* This is being prepared for future excitement...

class Ridge extends Canvas {
  public static final int ORIENT_HORIZONTAL = 0;
  public static final int ORIENT_VERTICAL = 1;

  int orientation = ORIENT_HORIZONTAL;

  public Ridge(int orientation) {
    this.orientation = orientation;
  }

  public Dimension minimumSize() {
    if( orientation == ORIENT_VERTICAL ) {
      return new Dimension(4,4);
    }
    return new Dimension(4,4);
  }

  public Dimension preferredSize() {
    if( orientation == ORIENT_VERTICAL ) {
      return new Dimension(8,4);
    }
    return new Dimension(4,8);
  }

  public void layout() {
  }

  public void paint(Graphics g) {
    Dimension d = size();
    if( orientation == ORIENT_VERTICAL ) {
      int x = (d.width/2)-2;
      g.setColor(Color.white);
      g.draw3DRect(x,0,x+3,d.height-1,true);
    } else {
      int y = (d.height/2)-2;
      g.setColor(Color.white);
      g.draw3DRect(0,y,d.width-1,y+3,true);
    }
  }

  public final synchronized void update (Graphics g) {
    paint(g);
  }
}

*/

public class EmulatorOptions extends Frame {

  static final int debug = 0;

  static final String ACTION_TERMINATE = new String("ACTION_TERMINATE");
  static final String ACTION_EMULATION = new String("ACTION_EMULATION");

    //  Applet applet;
  Frame applet;
  Emulator emulator;
  Parameters params;

  MenuBar menubar;
  Menu file, help;
  MenuItem file_quit;
  MenuItem help_about;

  GridBagLayout layout;

  TextField class_txt;

  Choice mode_choice = null;
  GridBagConstraints mode_cn;
  // Mapping from positions in choice widget to emulator type IDs.
  int[] mode_types = new int[256];

  Label status_lab;

    //  public EmulatorOptions(Applet app, Emulator emu, Telnet tel,
    //			 Parameters parm) {
  public EmulatorOptions(Frame app, Emulator emu, Telnet tel,
			 Parameters parm) {
    super("WebTerm Emulator Options");

    Label lab;
    Component comp;

    applet = app;
    emulator = emu;
    params = parm;

    menubar = new MenuBar();
    this.setMenuBar(menubar);
    file = new Menu("File");
    file_quit = new MenuItem("Close");
    file.add(file_quit);
    menubar.add(file);

    /* This isn't implemented...
    help = new Menu("Help");
    help_about = new MenuItem("About");
    help.add(help_about);
    menubar.add(help);
    menubar.setHelpMenu(help);
    */

    GridBagConstraints cn = new GridBagConstraints();
    layout = new GridBagLayout();
    this.setLayout(layout);

    // Create the top row of emulation type controls.

    cn.gridx = 0;
    cn.gridy = 0;
    cn.gridwidth = 1;
    cn.gridheight = 1;
    cn.fill = cn.NONE;
    cn.anchor = cn.WEST;
    cn.weightx = 0;
    cn.weighty = 0;

    lab = new Label("Emulation ");
    layout.setConstraints(lab,cn);
    this.add(lab);
    cn.gridx++;

    cn.anchor = cn.EAST;

    lab = new Label("Class:");
    layout.setConstraints(lab,cn);
    this.add(lab);
    cn.gridx++;

    cn.fill = cn.HORIZONTAL;
    cn.anchor = cn.WEST;
    cn.weightx = 3;

    String str = "";
    if( emulator != null ) {
      str = emulator.getClass().getName();
    }
    class_txt = new RubberTextField(str,200,32,4);
    layout.setConstraints(class_txt,cn);
    this.add(class_txt);
    cn.gridx++;

    cn.fill = cn.NONE;
    cn.anchor = cn.EAST;
    cn.weightx = 0;

    lab = new Label("Mode:");
    layout.setConstraints(lab,cn);
    this.add(lab);
    cn.gridx++;

    mode_cn = new GridBagConstraints();
    mode_cn.gridx = cn.gridx;
    mode_cn.gridy = 0;
    mode_cn.gridwidth = 1;
    mode_cn.gridheight = 1;
    mode_cn.fill = cn.HORIZONTAL;
    mode_cn.anchor = cn.WEST;
    mode_cn.weightx = 0;
    mode_cn.weighty = 0;

    update_mode();

    cn.gridx=0;
    cn.gridy++;

    // Create the second row of emulation status info.

    cn.gridwidth = 2;
    cn.anchor = cn.EAST;

    lab = new Label("Status:");
    layout.setConstraints(lab,cn);
    this.add(lab);
    cn.gridx+=2;

    cn.gridwidth = cn.REMAINDER;
    cn.anchor = cn.WEST;
    cn.fill = cn.HORIZONTAL;
    cn.weightx = 1;

    status_lab = new Label("(Initializing)",Label.LEFT);
    layout.setConstraints(status_lab,cn);
    this.add(status_lab);
    cn.gridx++;

    /*
    cn.gridx=0;
    cn.gridy++;
    cn.gridwidth = cn.REMAINDER;
    cn.anchor = cn.CENTER;
    cn.fill = cn.HORIZONTAL;
    cn.weightx = 1;

    comp = new Ridge(Ridge.ORIENT_HORIZONTAL);
    layout.setConstraints(comp,cn);
    this.add(comp);
    cn.gridx++;
    */

    this.layout();
  }

  public void setEmulator(Emulator emu) {
    emulator = emu;
    update_mode();
    this.layout();
  }

  public void setEmulatorStatus(String stat) {
    if( status_lab != null ) status_lab.setText(stat);
  }

  public String getEmulationName() {
    return class_txt.getText();
  }

  void update_mode() {
    if( mode_choice != null ) {
      this.remove(mode_choice);
      mode_choice = null;
    }

    mode_choice = new Choice();
    if( emulator != null ) {
      int orig_type = emulator.getCurType();
      int last_type = -1;
      int cur_pos = 0;
      for( int i=0; emulator.getTypeName(i) != null; i++ ) {
	if( emulator.getCurType() != last_type ) {
	  mode_choice.addItem(emulator.getTypeName(i));
	  last_type = emulator.getCurType();
	  mode_types[cur_pos++] = last_type;
	}
      }
      mode_choice.select(emulator.getTypeName(orig_type));
      mode_types[cur_pos] = -1;
    } else {
      mode_types[0] = -1;
    }
    layout.setConstraints(mode_choice,mode_cn);
    this.add(mode_choice);
  }

  int get_menu_width() {
    int num = menubar.countMenus();
    int width = 0;
    for( int i=0; i<num; i++ ) {
      Menu mn = menubar.getMenu(i);
      if( mn != null ) {
	FontMetrics fm = getFontMetrics(mn.getFont());
	width += ( fm.stringWidth(mn.getLabel()) * 3 ) / 2;
	width += fm.stringWidth("  ");
      }
    }
    return width;
  }

  public Dimension minimumSize() {
    Dimension dim = super.minimumSize();
    int width = get_menu_width();
    if( dim.width < width ) dim.width = width;
    return dim;
  }

  public Dimension preferredSize() {
    Dimension dim = super.preferredSize();
    int width = get_menu_width();
    if( dim.width < width ) dim.width = width;
    return dim;
  }

  public void show() {
    super.show();
    // show other windows here...
  }

  public void hide() {
    // hide other windows here...
    super.hide();
  }

  public void dispose() {
    // dispose other windows here...
    super.dispose();
  }

  public boolean action(Event event, Object what) {
    if( event.target == file_quit ) {
      Event e = new Event(this,Event.WINDOW_DESTROY,null);
      if( debug > 0 )
	System.out.println("WebTerm: Posting EmulatorOptions exit...");
      this.postEvent(e);
      return true;
    } else if( event.target == help_about ) {
      return true;
    } else if( event.target == class_txt ) {
      Event e = new Event(this,Event.ACTION_EVENT,ACTION_EMULATION);
      applet.postEvent(e);
      return true;
    } else if( event.target == mode_choice ) {
      if( emulator != null )
	emulator.getTypeName(mode_types[mode_choice.getSelectedIndex()]);
      return true;
    }
    return super.action(event,what);
  }

  public boolean handleEvent(Event event) {
    switch(event.id) {
    case Event.WINDOW_DESTROY:
      if( event.target == this ) {
	Event e = new Event(this,Event.ACTION_EVENT,ACTION_TERMINATE);
	applet.postEvent(e);
	return true;
      } else {
	// dispose other windows:
	// window.dispose();
	// window = null;
	// window_menu.setState(false);
      }
    }
    return super.handleEvent(event);
  }


}
