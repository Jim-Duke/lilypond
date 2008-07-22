%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.52"

\header {
  lsrtags = "editorial-annotations, chords, keyboards, fretted-strings"

  texidoces = "
Se puede controlar con precisión la colocación de los números de digitación.

"
  doctitlees = "Controlar la colocación de las digitaciones de acordes"

  texidoc = "
The placement of fingering numbers can be controlled precisely.

"
  doctitle = "Controlling the placement of chord fingerings"
} % begin verbatim
\relative c' {
  \set fingeringOrientations = #'(left)
  <c-1 e-3 a-5>4
  \set fingeringOrientations = #'(down)
  <c-1 e-3 a-5>4
  \set fingeringOrientations = #'(right)
  <c-1 e-3 a-5>4
  \set fingeringOrientations = #'(up)
  <c-1 e-3 a-5>4
  \set fingeringOrientations = #'(left down)
  <c-1 e-3 a-5>2
  \set fingeringOrientations = #'(up right down)
  <c-1 e-3 a-5>2
}
