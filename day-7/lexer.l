%{
#include <stdbool.h>
#include "common.h"
%}

%%
no /* Ignore */
other /* Ignore */
bags? /* Ignore */
contain { return ST_NEXT; }
[a-z]+ { return ST_COLOR; }
[0-9]+ { return ST_NUMBER; }
[.] { return ST_NEXT; }
, { return ST_NEXT; }
. /* Ignore */
\n /* Ignore */
%%
