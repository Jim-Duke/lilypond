%% Do not edit this file; it is automatically
%% generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.10"

\header {
  lsrtags = "rhythms"

%% Translation of GIT committish: 4385ed4cc738e164a95798862580b4b86703356f
  texidoces = "

Es posible aplicar la barrita que cruza la barra de las
acciaccaturas, en otras situaciones.

"

  doctitlees = "Utilizar la barra que tacha las notas de adorno con notas normales"


%% Translation of GIT committish: 21c8461ea87cd670a35a40b91d3ef20de03a0409
  texidocfr = "
Le trait que l'on trouve sur les hampes des acciaccatures peut
être appliqué dans d'autres situations.

"

  doctitlefr = "Utilisation de hampe barrée pour une note normale"

  texidoc = "
The slash through the stem found in acciaccaturas can be applied in
other situations.

"
  doctitle = "Using grace note slashes with normal heads"
} % begin verbatim

\relative c'' {
  \override Stem #'stroke-style = #"grace"
  c8( d2) e8( f4)
}

