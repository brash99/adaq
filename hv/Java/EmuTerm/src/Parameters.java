/*
 * WebTerm
 * ~~~~~~~
 *
 * Parameters interface: used to communicate program parameters between
 * different classes.
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
 * History
 * ~~~~~~~
 *
 * 1.3: Created this file.
 *
 */

public interface Parameters {
  public String getParameter(String name);
};
