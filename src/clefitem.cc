#include "clefitem.hh"
#include "string.hh"
#include "molecule.hh"
#include "paper.hh"
#include "lookup.hh"
#include "clef.hh"

Clef_item::Clef_item()
{
    read("violin");
}
void
Clef_item::read(String t)
{
    type = t;
    if (type == "violin")
	y_off = 2;
    if (type == "bass")
	y_off = 6;
}
void
Clef_item::read(Clef k)
{
    read(k.clef_type);
}

void
Clef_item::preprocess()
{
    Symbol s = paper()->lookup_->clef(type);
    output = new Molecule(Atom(s));
    output->translate(Offset(0, paper()->interline()/2 * y_off));
}

