
d)lib kpi.kpi 
 Library for working with qkdb internal api
 q).import.module`kpi 
 q).import.module`kpi.kpi
 q).import.module"%kpi%/qlib/kpi/kpi.q"

.kpi.summary:{} 

d)fnc kpi.kpi.summary 
 Give a summary of this function
 q) .kpi.summary[] 

if[not`.kpi.lib.type~key`.kpi.lib.type;.kpi.lib.type:`Release];  / default is release

.kpi.lib.defPath:`$.bt.print[":%kpi%/qlib/kpi/cpp/kpi.def"] .import.repository.con;
.kpi.lib.def:update fname:`${"." sv string x}'[flip(repo;nsp;name)] from update repo:`${".",1_first "_" vs x}@'name,nsp:`${first 1_"_" vs x}@'name,name:`${"_" sv  2_"_" vs x}@'name,orig:`$name from {select from x where not null num} flip`name`num!("*j";";") 0: read0 .kpi.lib.defPath
.kpi.lib.dll:`$.bt.print[":%kpi%/qlib/kpi/cpp/bin/%os%/%type%/kpi"] .import.repository.con,.kpi.lib,.bt.md[`os] .z.o

{@[x`repo;x`nsp;:;](1#`)!(1#{})}@'key select by repo,nsp from .kpi.lib.def;
{.[x`repo;x`nsp`name;:;].kpi.lib.dll 2: x`orig`num }@'.kpi.lib.def;