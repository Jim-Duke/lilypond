/*
  This file is part of LilyPond, the GNU music typesetter.

  Copyright (C) 2013--2015 Mike Solomon <mike@mikesolomon.org>

  LilyPond is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LilyPond is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "engraver.hh"

#include "context.hh"
#include "directional-element-interface.hh"
#include "international.hh"
#include "note-column.hh"
#include "pointer-group-interface.hh"
#include "slur.hh"
#include "slur-proto-engraver.hh"
#include "spanner.hh"
#include "stream-event.hh"
#include "warn.hh"

#include "translator.icc"

void
Slur_proto_engraver::derived_mark () const
{
  for (vsize i = start_events_.size (); i--;)
    {
      scm_gc_mark (start_events_[i].slur_->self_scm ());
      if (start_events_[i].note_)
        scm_gc_mark (start_events_[i].note_->self_scm ());
    }
  for (vsize i = stop_events_.size (); i--;)
    {
      scm_gc_mark (stop_events_[i].slur_->self_scm ());
      if (stop_events_[i].note_)
        scm_gc_mark (stop_events_[i].note_->self_scm ());
    }
}

void
Slur_proto_engraver::listen_slur (Stream_event *ev, Stream_event *note)
{
  Direction d = to_dir (ev->get_property ("span-direction"));
  if (d == START)
    start_events_.push_back (Event_info (ev, note));
  else if (d == STOP)
    stop_events_.push_back (Event_info (ev, note));
  else ev->origin ()->warning (_f ("direction of %s invalid: %d",
                                     event_name_, int (d)));
}

void
Slur_proto_engraver::listen_note (Stream_event *ev)
{
  for (SCM arts = ev->get_property ("articulations");
       scm_is_pair (arts); arts = scm_cdr (arts))
    {
      Stream_event *art = unsmob<Stream_event> (scm_car (arts));
      if (art->in_event_class (event_symbol ()))
        listen_slur (art, ev);
    }
}

void
Slur_proto_engraver::acknowledge_note_column (Grob_info info)
{
  Grob *e = info.grob ();
  for (vsize i = slurs_.size (); i--;)
    Slur::add_column (slurs_[i], e);
  for (vsize i = end_slurs_.size (); i--;)
    Slur::add_column (end_slurs_[i], e);
  // Now cater for slurs starting/ending at a notehead: those override
  // the column bounds
  if (note_slurs_[START].empty () && note_slurs_[STOP].empty ())
    return;
  extract_grob_set (e, "note-heads", heads);
  for (vsize i = heads.size (); i--;)
    {
      if (Stream_event *ev =
          unsmob<Stream_event> (heads[i]->get_property ("cause")))
        for (LEFT_and_RIGHT (d))
          {
            std::pair<Note_slurs::const_iterator, Note_slurs::const_iterator> its
              = note_slurs_[d].equal_range (ev);
            for (Note_slurs::const_iterator it = its.first;
                 it != its.second;
                 ++it)
              it->second->set_bound (d, heads[i]);
          }
    }
}

void
Slur_proto_engraver::acknowledge_extra_object (Grob_info info)
{
  objects_to_acknowledge_.push_back (info);
}

void
Slur_proto_engraver::acknowledge_inline_accidental (Grob_info info)
{
  acknowledge_extra_object (info);
}

void
Slur_proto_engraver::acknowledge_dots (Grob_info info)
{
  acknowledge_extra_object (info);
}

void
Slur_proto_engraver::acknowledge_fingering (Grob_info info)
{
  acknowledge_extra_object (info);
}

void
Slur_proto_engraver::acknowledge_tuplet_number (Grob_info info)
{
  acknowledge_extra_object (info);
}

void
Slur_proto_engraver::acknowledge_script (Grob_info info)
{
  if (!info.grob ()->internal_has_interface (ly_symbol2scm ("dynamic-interface")))
    acknowledge_extra_object (info);
}

void
Slur_proto_engraver::acknowledge_text_script (Grob_info info)
{
  acknowledge_extra_object (info);
}

void
Slur_proto_engraver::acknowledge_end_tie (Grob_info info)
{
  acknowledge_extra_object (info);
}

void
Slur_proto_engraver::finalize ()
{
  for (vsize i = 0; i < slurs_.size (); i++)
    {
      slurs_[i]->warning (_f ("unterminated %s", object_name_));
      slurs_[i]->suicide ();
    }
  slurs_.clear ();
}

void
Slur_proto_engraver::create_slur (const string &spanner_id, Event_info evi, Grob *g_cause, Direction dir, bool left_broken)
{
  Grob *ccc = left_broken
    ? unsmob<Grob> (get_property ("currentCommandColumn"))
    : 0; // efficiency
  SCM cause = evi.slur_ ? evi.slur_->self_scm () : g_cause->self_scm ();
  Spanner *slur = make_spanner (grob_name_, cause);
  slur->set_property ("spanner-id", ly_string2scm (spanner_id));
  if (dir)
    set_grob_direction (slur, dir);
  if (left_broken)
    slur->set_bound (LEFT, ccc);
  slurs_.push_back (slur);
  if (evi.note_)
    note_slurs_[START].insert (Note_slurs::value_type (evi.note_, slur));

  if (double_property_name_
      && to_boolean (get_property (double_property_name_)))
  {
    set_grob_direction (slur, DOWN);
    slur = make_spanner (grob_name_, cause);
    slur->set_property ("spanner-id", ly_string2scm (spanner_id));
    set_grob_direction (slur, UP);
    if (left_broken)
      slur->set_bound (LEFT, ccc);
    slurs_.push_back (slur);
    if (evi.note_)
      note_slurs_[START].insert(Note_slurs::value_type (evi.note_, slur));
  }

}

bool
Slur_proto_engraver::can_create_slur (const string &id, vsize old_slurs, vsize *event_idx, Stream_event *ev)
{
  for (vsize j = slurs_.size (); j--;)
    {
      Grob *slur = slurs_[j];
      Direction updown = to_dir (ev->get_property ("direction"));

      // Check if we already have a slur with the same spanner-id.
      if (id == robust_scm2string (slur->get_property ("spanner-id"), ""))
        {
          if (j < old_slurs)
            {
              // We already have an old slur, so give a warning
              // and completely ignore the new slur.
              ev->origin ()->warning (_f ("already have %s", object_name_));
              if (event_idx)
                start_events_.erase (start_events_.begin () + (*event_idx));
              return false;
            }

          // If this slur event has no direction, it will not
          // contribute anything new to the existing slur(s), so
          // we can ignore it.

          if (!updown)
            return false;

          Stream_event *c = unsmob<Stream_event> (slur->get_property ("cause"));

          if (!c)
            {
              slur->programming_error (_f ("%s without a cause", object_name_));
              return true;
            }

          Direction slur_dir = to_dir (c->get_property ("direction"));

          // If the existing slur does not have a direction yet,
          // we'd rather take the new one.

          if (!slur_dir)
            {
              slur->suicide ();
              slurs_.erase (slurs_.begin () + j);
              return true;
            }

          // If the existing slur has the same direction as ours, drop ours

          if (slur_dir == updown)
            return false;
        }
    }
  return true;
}

bool
Slur_proto_engraver::try_to_end (Event_info evi)
{
  string id = robust_scm2string (evi.slur_->get_property ("spanner-id"), "");

  // Find the slurs that are ended with this event (by checking the spanner-id)
  bool ended = false;
  for (vsize j = slurs_.size (); j--;)
    {
      if (id == robust_scm2string (slurs_[j]->get_property ("spanner-id"), ""))
        {
          ended = true;
          end_slurs_.push_back (slurs_[j]);
          if (evi.note_)
            note_slurs_[STOP].insert
              (Note_slurs::value_type
               (evi.note_, dynamic_cast <Spanner *> (slurs_[j])));
          slurs_.erase (slurs_.begin () + j);
        }
    }
  return ended;
}

void
Slur_proto_engraver::process_music ()
{
  for (vsize i = 0; i < stop_events_.size (); i++)
    {
      string id = robust_scm2string
        (stop_events_[i].slur_->get_property ("spanner-id"), "");
      bool ended = try_to_end (stop_events_[i]);
      if (ended)
        {
          // Ignore redundant stop events for this id
          for (vsize j = stop_events_.size (); --j > i;)
            {
              if (id == robust_scm2string
                  (stop_events_[j].slur_->get_property ("spanner-id"), ""))
                stop_events_.erase (stop_events_.begin () + j);
            }
        }
      else
        stop_events_[i].slur_->origin ()->warning (_f ("cannot end %s", object_name_));
    }

  vsize old_slurs = slurs_.size ();
  for (vsize i = start_events_.size (); i--;)
    {
      Stream_event *ev = start_events_[i].slur_;
      string id = robust_scm2string (ev->get_property ("spanner-id"), "");
      Direction updown = to_dir (ev->get_property ("direction"));

      if (can_create_slur (id, old_slurs, &i, ev))
        create_slur (id, start_events_[i], 0, updown, false);
    }

  set_melisma (slurs_.size ());
}

void
Slur_proto_engraver::set_melisma (bool)
{
}

void
Slur_proto_engraver::stop_translation_timestep ()
{
  if (Grob *g = unsmob<Grob> (get_property ("currentCommandColumn")))
    {
      for (vsize i = 0; i < end_slurs_.size (); i++)
        Slur::add_extra_encompass (end_slurs_[i], g);

      if (!start_events_.size ())
        for (vsize i = 0; i < slurs_.size (); i++)
          Slur::add_extra_encompass (slurs_[i], g);
    }

  for (vsize i = 0; i < end_slurs_.size (); i++)
    {
      Spanner *s = dynamic_cast<Spanner *> (end_slurs_[i]);
      if (!s->get_bound (RIGHT))
        s->set_bound (RIGHT, unsmob<Grob> (get_property ("currentMusicalColumn")));
      announce_end_grob (s, SCM_EOL);
    }

  for (vsize i = 0; i < objects_to_acknowledge_.size (); i++)
    Slur::auxiliary_acknowledge_extra_object (objects_to_acknowledge_[i], slurs_, end_slurs_);

  note_slurs_[LEFT].clear ();
  note_slurs_[RIGHT].clear ();
  objects_to_acknowledge_.clear ();
  end_slurs_.clear ();
  start_events_.clear ();
  stop_events_.clear ();
}

// no ADD_ACKNOWLEDGER / ADD_TRANSLATOR macro calls
// since this class is abstract
