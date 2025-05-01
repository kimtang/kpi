/ name:localhost:9081::

.qtx.testSuiter[`kpi1;`repo`lib`file!`kpi`kpi`001;"test kpi"]
  .qtx.before[{.import.module`kpi}]
  .qtx.testCase[`api;"successfully loaded"][
    .qtx.shouldTrue[`0;"all loaded functions are present";{
      min {x~key x}@'exec fname from .kpi.lib.def
    }]
    .qtx.nil
    
  ]
  .qtx.testCase[`xp;"test .kpi.api.xp"][
    .qtx.shouldTrue[`0;"same pointe4d";{[a] .kpi.api.xp[a]  ~ .kpi.api.xp a}]
    .qtx.shouldTrue[`1;"different pointer";{[a;b] not .kpi.api.xp[b]  ~ .kpi.api.xp a }]
    .qtx.nil
    ]
  .qtx.testCase[`xn;"test .kpi.api.xn"][
    .qtx.shouldTrue[`0;"get length";{[d] count[d] ~ .kpi.api.xn d}]
    .qtx.nil
    ]
  .qtx.argument[`a`b`c`d!(14 9; 10; "str";(1;2;3;4f))]
  .qtx.nil
  ;

/


asd