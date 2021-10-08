%{
#include "lexer.h"
%}

%s ST_ALLERGEN

%%
contains /* Ignore */
<ST_ALLERGEN>[a-z]+ { return LX_ALLERGEN; }
[a-z]+ { return LX_INGREDIENT; }
\( { BEGIN ST_ALLERGEN; }
\) { BEGIN 0; return LX_EOL; }
. /* Ignore */
\n /* Ignore */
%%
