/*
  event.cc -- implement Event

  source file of the GNU LilyPond music typesetter

  (c) 1996--2004 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#include "event.hh"

#include "warn.hh"

Moment
Event::get_length () const
{
  Duration *d = unsmob_duration (get_property ("duration"));
  if (!d)
    {
      Moment m ;
      return m;
    }
  return d->get_length ();
}

void
Event::compress (Moment m)
{
  Duration *d =  unsmob_duration (get_property ("duration"));
  if (d)
    set_property ("duration", d ->compressed (m.main_part_).smobbed_copy ());
}


Pitch
Event::to_relative_octave (Pitch last)
{
  Pitch *old_pit = unsmob_pitch (get_property ("pitch"));
  if (old_pit)
    {
      Pitch new_pit = *old_pit;
      new_pit = new_pit.to_relative_octave (last);

      SCM check = get_property ("absolute-octave");
      if (scm_is_number (check) &&
	  new_pit.get_octave () != scm_to_int (check))
	{
	  Pitch expected_pit (scm_to_int (check),
			      new_pit.get_notename (),
			      new_pit.get_alteration ());
	  origin ()->warning (_f ("octave check failed; expected %s, found: %s",
				  expected_pit.to_string (),
				  new_pit.to_string ()));
	  new_pit = expected_pit;
	}
      
      set_property ("pitch", new_pit.smobbed_copy ());
  
      return new_pit;
    }
  return last;
}
  
Event::Event ()
  : Music ()
{
}

ADD_MUSIC (Event);

LY_DEFINE (ly_music_duration_length, "ly:music-duration-length", 1, 0,0,
	  (SCM mus),
	  "Extract the duration field from @var{mus}, and return the length.")
{
  Music* m =   unsmob_music (mus);
  SCM_ASSERT_TYPE (m, mus, SCM_ARG1, __FUNCTION__, "Music");
  
  Duration *d = unsmob_duration (m->get_property ("duration"));

  Moment l ;
  
  if (d)
    {
      l = d->get_length ();  
    }
  else
    programming_error ("Music has no duration");
  return l.smobbed_copy ();
  
}


LY_DEFINE (ly_music_duration_compress, "ly:music-duration-compress", 2, 0,0,
	  (SCM mus, SCM fact),
	  "Compress @var{mus} by factor @var{fact}, which is a @code{Moment}.")
{
  Music* m =   unsmob_music (mus);
  Moment * f = unsmob_moment (fact);
  SCM_ASSERT_TYPE (m, mus, SCM_ARG1, __FUNCTION__, "Music");
  SCM_ASSERT_TYPE (f, fact, SCM_ARG2, __FUNCTION__, "Moment");
  
  Duration *d = unsmob_duration (m->get_property ("duration"));
  if (d)
    m->set_property ("duration", d->compressed (f->main_part_).smobbed_copy ());
  return SCM_UNSPECIFIED;
}



/*
  This is hairy, since the scale in a key-change event may contain
  octaveless notes.


  TODO: this should use ly:pitch. 
 */
LY_DEFINE (ly_transpose_key_alist, "ly:transpose-key-alist",
	  2, 0, 0, (SCM l, SCM pit),
	  "Make a new key alist of @var{l} transposed by pitch @var{pit}")
{
  SCM newlist = SCM_EOL;
  Pitch *p = unsmob_pitch (pit);
  
  for (SCM s = l; scm_is_pair (s); s = scm_cdr (s))
    {
      SCM key = scm_caar (s);
      SCM alter = scm_cdar (s);
      if (scm_is_pair (key))
	{
	  Pitch orig (scm_to_int (scm_car (key)),
		      scm_to_int (scm_cdr (key)),
		      scm_to_int (alter));

	  orig = orig.transposed (*p);

	  SCM key = scm_cons (scm_int2num (orig.get_octave ()),
			     scm_int2num (orig.get_notename ()));

	  newlist = scm_cons (scm_cons (key, scm_int2num (orig.get_alteration ())),
			     newlist);
	}
      else if (scm_is_number (key))
	{
	  Pitch orig (0, scm_to_int (key), scm_to_int (alter));
	  orig = orig.transposed (*p);

	  key = scm_int2num (orig.get_notename ());
	  alter = scm_int2num (orig.get_alteration ());
	  newlist = scm_cons (scm_cons (key, alter), newlist);
	}
    }
  return scm_reverse_x (newlist, SCM_EOL);
}

void
Key_change_ev::transpose (Pitch p)
{
  SCM pa = get_property ("pitch-alist");
  set_property ("pitch-alist", ly_transpose_key_alist (pa, p.smobbed_copy ()));

  Event::transpose (p);
}

bool
alist_equal_p (SCM a, SCM b)
{
  for (SCM s = a;
       scm_is_pair (s); s = scm_cdr (s))
    {
      SCM key = scm_caar (s);
      SCM val = scm_cdar (s);
      SCM l = scm_assoc (key, b);

      if (l == SCM_BOOL_F
	  || !ly_c_equal_p ( scm_cdr (l), val))

	return false;
    }
  return true;
}
ADD_MUSIC (Key_change_ev);
