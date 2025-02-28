/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "src/test-ringbuf.cm" instead.
 */

#include <check.h>

#line 1 "src/test-ringbuf.cm"
/* -*- c -*- */
/* test-ringbuf.cm: tests for ringbuf.c. */

/*
    Copyright (C) 2008 Micah Cowan

    This file is part of GNU teseq.

    GNU teseq is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GNU teseq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "teseq.h"

#include <stdio.h>

#include "ringbuf.h"

struct ringbuf *the_buffer;

START_TEST(empty)
{
#line 32
        fail_unless (ringbuf_get (the_buffer) == EOF);

}
END_TEST

START_TEST(read_test)
{
#line 35
        fail_unless (ringbuf_put (the_buffer, 'a') == 0);
        fail_unless (ringbuf_put (the_buffer, 'b') == 0);
        fail_unless (ringbuf_put (the_buffer, 'c') == 0);
        fail_unless (ringbuf_get (the_buffer) == 'a');
        fail_unless (ringbuf_get (the_buffer) == 'b');
        fail_unless (ringbuf_get (the_buffer) == 'c');
        fail_unless (ringbuf_get (the_buffer) == EOF);
        fail_unless (ringbuf_get (the_buffer) == EOF);

}
END_TEST

START_TEST(wraparound)
{
#line 45
        fail_unless (ringbuf_putmem (the_buffer, "abc", 3) == 0);
        fail_unless (ringbuf_get (the_buffer) == 'a');
        fail_unless (ringbuf_putmem (the_buffer, "defg", 4) == 1);
        fail_unless (ringbuf_putmem (the_buffer, "def", 3) == 0);
        fail_unless (ringbuf_put (the_buffer, 'g') == 1);
        fail_unless (ringbuf_get (the_buffer) == 'b');
        fail_unless (ringbuf_get (the_buffer) == 'c');
        fail_unless (ringbuf_get (the_buffer) == 'd');
        fail_unless (ringbuf_get (the_buffer) == 'e');
        fail_unless (ringbuf_get (the_buffer) == 'f');
        fail_unless (ringbuf_get (the_buffer) == EOF);

}
END_TEST

START_TEST(reader)
{
#line 58
        struct ringbuf_reader *reader1;
        struct ringbuf_reader *reader2;
        ringbuf_put (the_buffer, 'x');
        ringbuf_put (the_buffer, 'x');
        ringbuf_get (the_buffer);
        ringbuf_get (the_buffer);
        ringbuf_putmem (the_buffer, "abcde", 5);
        reader1 = ringbuf_reader_new (the_buffer);
        fail_unless (ringbuf_reader_get (reader1) == 'a');
        fail_unless (ringbuf_reader_get (reader1) == 'b');
        fail_unless (ringbuf_reader_get (reader1) == 'c');
        fail_unless (ringbuf_reader_get (reader1) == 'd');
        reader2 = ringbuf_reader_new (the_buffer);
        fail_unless (ringbuf_reader_get (reader2) == 'a');
        ringbuf_reader_consume (reader1);
        fail_unless (ringbuf_get (the_buffer) == 'e');

}
END_TEST

START_TEST(clear)
{
#line 76
        ringbuf_put (the_buffer, 'a');
        ringbuf_put (the_buffer, 'b');
        ringbuf_put (the_buffer, 'c');
        fail_unless (ringbuf_get (the_buffer) == 'a');
        ringbuf_clear (the_buffer);
        fail_unless (ringbuf_get (the_buffer) == EOF);

}
END_TEST

START_TEST(putback)
{
#line 84
        ringbuf_put (the_buffer, 'a');
        ringbuf_put (the_buffer, 'b');
        fail_unless (ringbuf_putback (the_buffer, 'c') == 0);
        fail_unless (ringbuf_get (the_buffer) == 'c');
        fail_unless (ringbuf_get (the_buffer) == 'a');
        fail_unless (ringbuf_putback (the_buffer, 'd') == 0);
        fail_unless (ringbuf_get (the_buffer) == 'd');
        fail_unless (ringbuf_get (the_buffer) == 'b');
        fail_unless (ringbuf_get (the_buffer) == EOF);

}
END_TEST

START_TEST(reader_reset)
{
#line 95
        struct ringbuf_reader *reader;
        ringbuf_putmem (the_buffer, "abcd", 4);
        reader = ringbuf_reader_new (the_buffer);
        fail_unless (ringbuf_reader_get (reader) == 'a');
        fail_unless (ringbuf_reader_get (reader) == 'b');
        ringbuf_reader_reset (reader);
        fail_unless (ringbuf_reader_get (reader) == 'a');
        fail_unless (ringbuf_reader_get (reader) == 'b');
        fail_unless (ringbuf_reader_get (reader) == 'c');
        fail_unless (ringbuf_reader_get (reader) == 'd');
        fail_unless (ringbuf_reader_get (reader) == EOF);

}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    /* User-specified pre-run code */
#line 108
        the_buffer = ringbuf_new (5);

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, empty);
    tcase_add_test(tc1_1, read_test);
    tcase_add_test(tc1_1, wraparound);
    tcase_add_test(tc1_1, reader);
    tcase_add_test(tc1_1, clear);
    tcase_add_test(tc1_1, putback);
    tcase_add_test(tc1_1, reader_reset);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
