    0    0               The Geometry
  SPH       bh     0.0       0.0       0.0   5000000.0
  SPH      vac     0.0       0.0       0.0   1000000.0
  RPP    calor   -50.0      50.0     -50.0      50.0       0.0      50.0
  XYP      p00     2.5
  XYP      p01     5.0
  XYP      p02     7.5
  XYP      p03    10.0
  XYP      p04    12.5
  XYP      p05    15.0
  XYP      p06    17.5
  XYP      p07    20.0
  XYP      p08    22.5
  XYP      p09    25.0
  XYP      p10    27.5
  XYP      p11    30.0
  XYP      p12    32.5
  XYP      p13    35.0
  XYP      p14    37.5
  XYP      p15    40.0
  XYP      p16    42.5
  XYP      p17    45.0
  XYP      p18    47.5
  END
* region 1 - black hole
       BH   5  +bh -vac
* region 2 - vacuum around
   VACUUM   5  +vac -calor
  LAYER00   5  +calor +p00
  LAYER01   5  +calor -p00  +p01
  LAYER02   5  +calor -p01  +p02
  LAYER03   5  +calor -p02  +p03
  LAYER04   5  +calor -p03  +p04
  LAYER05   5  +calor -p04  +p05
  LAYER06   5  +calor -p05  +p06
  LAYER07   5  +calor -p06  +p07
  LAYER08   5  +calor -p07  +p08
  LAYER09   5  +calor -p08  +p09
  LAYER10   5  +calor -p09  +p10
  LAYER11   5  +calor -p10  +p11
  LAYER12   5  +calor -p11  +p12
  LAYER13   5  +calor -p12  +p13
  LAYER14   5  +calor -p13  +p14
  LAYER15   5  +calor -p14  +p15
  LAYER16   5  +calor -p15  +p16
  LAYER17   5  +calor -p16  +p17
  LAYER18   5  +calor -p17  +p18
  LAYER19   5  +calor -p18
  END
