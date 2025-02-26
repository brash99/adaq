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

import java.awt.*;
import java.applet.*;
import java.util.*;

//import ClassConnection;

/* This is a class used internally by the ClassRegistry to store
   information about a single object that is registered with it. */

class ClassData {
  String name;            // Name the object is registered under.
  ClassConnection conn;   // The interface to send commands to.
}

/* The Class Registry.

   This class should never be instantiated.  Instead, it is used by
   calling the static methods of the class, to register objects with
   it and send messages to the currently registered objects.

   Because the methods and data are static, they are shared between
   all objects that use the class: thus an object in one applet or
   document can "see" the objects registered by another.

   Note that, for this to work, your objects must all be accessing the
   same *class*: i.e., they must use the registry at the same URL.  The
   practical implication of this is that the applets that wish to
   communcate with each should be stored in the same directory on your
   server, along with the ClassRegistry classes themselves.
   */

public class ClassRegistry {

  static final int DEBUG = 0;

  static Hashtable classes = new Hashtable();
  static String RESULT_NOCLASS = "No Class";

  /* Add an object with the given name and interface to the registry. */

  static synchronized public int addClass(String name, ClassConnection conn) {
    Vector vec = get_class_vector(name);
    ClassData cd = new ClassData();
    cd.name = name;
    cd.conn = conn;
    vec.addElement(cd);
    return vec.size();
  }

  /* Remove an object that was previously added to the registry */

  static synchronized public boolean remClass(String name,
					      ClassConnection conn) {
    Vector vec = get_class_vector(name);
    if( vec != null ) {
      int last = vec.size();
      for( int i=0; i<last; i++ ) {
	ClassData cd = (ClassData)vec.elementAt(i);
	if( cd != null && cd.conn == conn ) {
	  vec.removeElementAt(i);
	  return true;
	}
      }
    }
    return false;
  }

  /* Send a command to all objects that are registered with the given
     name.  Returns RESULT_NOCLASS if there are none with that
     name, otherwise returns the result returned by the last object
     the command was sent to. */

  static synchronized public Object doCommand(String name,
					      String cmd, Object data) {
    Vector vec = get_class_vector(name);
    Object result = RESULT_NOCLASS;
    if( vec != null ) {
      int last = vec.size();
      for( int i=0; i<last; i++ ) {
	ClassData cd = (ClassData)vec.elementAt(i);
	if( cd != null ) result = cd.conn.doCommand(cmd,data);
      }
    }
    return result;
  }

  static private Vector get_class_vector(String name) {
    if( DEBUG > 0 ) System.out.println("Getting classes for: " + name);
    Vector vec = (Vector)classes.get(name);
    if( vec == null ) {
      vec = new Vector();
      classes.put(name,vec);
    }
    return vec;
  }
}
