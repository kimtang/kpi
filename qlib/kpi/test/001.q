
/ name:localhost:9081::


.qtx.testSuite[`kpi1;`repo`lib`file!`kpi`kpi`001;"test kpi"]
  .qtx.before[{.import.module`kpi}]
  .qtx.testCase[`xp;"test .kpi.api.xp"][
    .qtx.shouldTrue[`0;"same pointer1";{[a] .kpi.api.xp[a]  ~ .kpi.api.xp a}]
    .qtx.shouldTrue[`1;"different pointer";{[a;b] not .kpi.api.xp[b]  ~ .kpi.api.xp a }]
    .qtx.nil
    ]
  .qtx.testCase[`xn;"test .kpi.api.xn"][
    .qtx.shouldTrue[`0;"get length";{[d] 4 ~ .kpi.api.xn d}]
    .qtx.nil
    ]
  .qtx.argument[`a`b`c`d!(0 1; 2; "str";(1;2;3;4f))]
  .qtx.nil
  ;


/

(::)result:.qtx.execute .qtx.con

