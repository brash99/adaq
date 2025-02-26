/*
 * WebTerm
 * ~~~~~~~
 *
 * RubberTextField class: a simple subclass of the AWT TextField class
 * that is not picky about its width.
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

public class RubberTextField extends TextField {
  int pref_size = 0;
  int min_size = 2;

  public RubberTextField() { super(); }
  public RubberTextField(int cols) { super(cols); }
  public RubberTextField(int cols, int pref, int min) {
    super(cols); pref_size = pref; min_size = min;
  }
  public RubberTextField(String text) { super(text); }
  public RubberTextField(String text, int cols) { super(text,cols); }
  public RubberTextField(String text, int cols, int pref, int min) {
    super(text,cols); pref_size = pref; min_size = min;
  }

  public Dimension preferredSize() {
    if( pref_size > 0 ) return preferredSize(pref_size);
    return super.preferredSize();
  }
  public Dimension minimumSize() { return minimumSize(min_size); }
}
