
\version "2.19.21"

\header {texidoc = "Dynamic letters are kerned, and their weight
matches that of the hairpin signs.  The dynamic scripts should be
horizontally centered on the note head.  Scripts that should appear
closer to the note head (staccato, accent) are reckoned with.
"  }


\relative {
  c''1\ff
  e,-.-> \f
  e \rfz		      
  c' \mf
  c \mp \> 
  c\!\ppppp \< 
  c\sfp\!
}

