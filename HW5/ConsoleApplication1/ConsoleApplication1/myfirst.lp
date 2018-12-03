\ Model first
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  x0 + 2 x1 + 5 x2 + 7 x3 + 3.5 x4
Subject To
 1_constraint: 1.05 x0 + x1 - 4 x2 + x3 >= 0
 2_constraint: 1.05 x0 + 2 x1 + 7 x2 - 5 x3 >= 0
 3_constraint: 1.05 x0 + 2 x1 + 9 x2 - 9 x3 + 5 x4 >= 0
Bounds
End
