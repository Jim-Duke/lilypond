#include "request.hh"
#include "swalker.hh"
#include "stcol.hh"
#include "sccol.hh"

Staff_walker::~Staff_walker() {}

Staff_walker::Staff_walker(Staff * s, PScore*ps )
    : PCursor<Staff_column*> (s->cols)
{
    staff_ = s;
    pscore_ = ps;
    break_status = BREAK_END - BREAK_PRE;
}

Real
Staff_walker::when() const
{
    return (* (PCursor<Staff_column*> *) this)->when();
}

void
Staff_walker::process()
{
    break_status = BREAK_END - BREAK_PRE;

    for (int i = 0 ; i < (*this)->s_commands.sz(); i++) {
	process_command((*this)->s_commands[i]);
    }

    process_requests();
}


void
Staff_walker::process_command(Command*com)
{
    switch (com->code){
    case BREAK_PRE:
    case BREAK_MIDDLE:
    case BREAK_POST:
    case BREAK_END:
	(*this)->score_column->set_breakable();
	break_status = com->code- BREAK_PRE;
	break;
    case INTERPRET:
	do_INTERPRET_command(com);
	break;
	
    case TYPESET:
	do_TYPESET_command(com);
	break;
   
    default :
	break;
    }
}

void
Staff_walker::operator++(int i)
{
    PCursor<Staff_column*>::operator++(i);
    reset();
}
