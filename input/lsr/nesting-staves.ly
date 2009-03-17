%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "staff-notation, contexts-and-engravers, tweaks-and-overrides"

  texidoces = "
Se puede utilizar la propiedad
@code{systemStartDelimiterHierarchy} para crear grupos de
pentagramas anidados de forma más compleja. La instrucción
@code{\\set StaffGroup.systemStartDelimiterHierarchy} toma una
lista alfabética del número de pentagramas producidos. Se puede
proporcionar antes de cada pentagrama un delimitador de comienzo
de sistema. Se debe encerrar entre corchetes y admite tantos
pentagramas como encierren las llaves. Se pueden omitir los
elementos de la lista, pero el primer corchete siempre abarca
todos los pentagramas. Las posibilidades son
@code{SystemStartBar}, @code{SystemStartBracket},
@code{SystemStartBrace} y @code{SystemStartSquare}.

"
  doctitlees = "Anidado de grupos de pentagramas"

%% Translation of GIT committish :<0364058d18eb91836302a567c18289209d6e9706>  
  texidocde = "
Die Eigenschaft @code{systemStartDelimiterHierarchy} kann eingesetzt
werden, um komplizierte geschachtelte Systemklammern zu erstellen.  Der
Befehl @code{\\set StaffGroup.systemStartDelimiterHierarchy} nimmt eine
Liste mit der Anzahl der Systeme, die ausgegeben werden, auf.  Vor jedem
System kann eine Systemanfangsklammer angegeben werden.  Sie muss in Klammern eingefügt
werden und umfasst so viele Systeme, wie die Klammer einschließt.  Elemente
in der Liste können ausgelassen werden, aber die erste Klammer umfasst immer
die gesamte Gruppe.  Die Möglichkeiten der Anfangsklammer sind: @code{SystemStartBar},
@code{SystemStartBracket}, @code{SystemStartBrace} und
@code{SystemStartSquare}.

"
  doctitlede = "Systeme schachteln"

  texidoc = "
The property @code{systemStartDelimiterHierarchy} can be used to make
more complex nested staff groups. The command @code{\\set
StaffGroup.systemStartDelimiterHierarchy} takes an alphabetical list of
the number of staves produced. Before each staff a system start
delimiter can be given. It has to be enclosed in brackets and takes as
much staves as the brackets enclose. Elements in the list can be
omitted, but the first bracket takes always the complete number of
staves. The possibilities are @code{SystemStartBar},
@code{SystemStartBracket}, @code{SystemStartBrace}, and
@code{SystemStartSquare}.

"
  doctitle = "Nesting staves"
} % begin verbatim

\new StaffGroup
\relative c'' <<
  \set StaffGroup.systemStartDelimiterHierarchy
    = #'(SystemStartSquare (SystemStartBrace (SystemStartBracket a
                             (SystemStartSquare b)  ) c ) d)
  \new Staff { c1 }
  \new Staff { c1 }
  \new Staff { c1 }
  \new Staff { c1 }
  \new Staff { c1 }
>>
