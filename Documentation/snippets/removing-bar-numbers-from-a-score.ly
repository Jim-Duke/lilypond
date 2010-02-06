%% Do not edit this file; it is automatically
%% generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.10"

\header {
  lsrtags = "rhythms, contexts-and-engravers"

%% Translation of GIT committish: 4385ed4cc738e164a95798862580b4b86703356f
  texidoces = "

Se pueden eliminar completamente los números de compás quitando el
grabador @code{Bar_number_engraver} del contexto de @code{Score}.

"

  doctitlees = "Suprimir los números de compás de toda la partitura"


%% Translation of GIT committish: d96023d8792c8af202c7cb8508010c0d3648899d
  texidocde = "
Taktnummern können vollkommen aus den Noten entfernt werden, indem
man den @code{Bar_number_engraver} aus dem @code{Score}-Kontext
entfernt.

"
  doctitlede = "Entfernung von Taktnummern in einer Partitur"


%% Translation of GIT committish: 21c8461ea87cd670a35a40b91d3ef20de03a0409
  texidocfr = "
Désactiver le graveur concerné --- @code{Bar_number_engraver} ---
donnera une partition  --- contexte @code{Score} --- sans numéros de
mesure.

"
  doctitlefr = "Supprimer les numéros de mesure d'une partition"


  texidoc = "
Bar numbers can be removed entirely by removing the
@code{Bar_number_engraver} from the @code{Score} context.

"
  doctitle = "Removing bar numbers from a score"
} % begin verbatim

\layout {
  \context {
    \Score
    \remove "Bar_number_engraver"
  }
}

\relative c'' {
  c4 c c c \break
  c4 c c c
}

