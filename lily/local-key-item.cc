/*
  local-key-item.cc -- implement Local_key_item, Musical_pitch

  source file of the GNU LilyPond music typesetter

  (c)  1997--1999 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/
#include "local-key-item.hh"
#include "molecule.hh"
#include "scalar.hh"
#include "lookup.hh"
#include "paper-def.hh"
#include "musical-request.hh"
#include "note-head.hh"
#include "misc.hh"

Local_key_item::Local_key_item ()
{
  c0_position_i_ = 0;
}

void
Local_key_item::add_pitch (Musical_pitch p, bool cautionary)
{
  for (int i=0; i< accidental_arr_.size(); i++)
    if (!Musical_pitch::compare (p, accidental_arr_[i].pitch_))
      return;

  Local_key_cautionary_tuple t;
  t.pitch_ = p;
  t.cautionary_b_ = cautionary;
  accidental_arr_.push (t);
}


void
Local_key_item::do_pre_processing()
{
  accidental_arr_.sort (Local_key_cautionary_tuple::compare);
  Note_head_side::do_pre_processing ();
}

Molecule*
Local_key_item::do_brew_molecule_p() const
{
  Molecule*output = new Molecule;
  Real note_distance = staff_line_leading_f ()/2;
  Molecule *octave_mol_p = 0;
  int lastoct = -100;
  
  for  (int i = 0; i <  accidental_arr_.size(); i++) 
    {
      Musical_pitch p (accidental_arr_[i].pitch_);
      // do one octave
      if (p.octave_i_ != lastoct) 
	{
	  if (octave_mol_p)
	    {
	      Real dy =lastoct*7* note_distance;
	      octave_mol_p->translate_axis (dy, Y_AXIS);
	      output->add_molecule (*octave_mol_p);
	      delete octave_mol_p;
	    }
	  octave_mol_p= new Molecule;
	}
      
      lastoct = p.octave_i_;
      Real dy =
	(c0_position_i_ + p.notename_i_)
	* note_distance;
      Molecule m (lookup_l ()->accidental (p.accidental_i_, 
					   accidental_arr_[i].cautionary_b_));

      m.translate_axis (dy, Y_AXIS);
      octave_mol_p->add_at_edge (X_AXIS, RIGHT, m, 0);
    }

  if (octave_mol_p)
    {
      Real dy =lastoct*7*note_distance;
      octave_mol_p->translate_axis (dy, Y_AXIS);
      output->add_molecule (*octave_mol_p);
      delete octave_mol_p;
    }
  
 if (accidental_arr_.size()) 
    {
      Box b(Interval (0, 0.6 * note_distance), Interval (0,0));
      Molecule m (lookup_l ()->fill (b));
      output->add_at_edge (X_AXIS, RIGHT, m, 0);
    }

  return output;
}

void
Local_key_item::do_substitute_element_pointer (Score_element *o, Score_element*n)
{
  Note_head_side::do_substitute_element_pointer (o,n);
  Staff_symbol_referencer::do_substitute_element_pointer (o,n);
  
}
