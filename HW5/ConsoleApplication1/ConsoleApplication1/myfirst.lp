\ Model first
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  C0 + 2 C1 + 3.3 C2 - 2 C3 + 5 C4
Subject To
 1_constraint: 1.05 C0 + 1.05 C1 - 2.5 C2 + 8.5 C3 + 6 C4 >= 0
 2_constraint: 1.05 C0 + 1.1 C1 + 3.1 C2 + 2.3 C3 + 7 C4 >= 0
 3_constraint: - C1 + C5 >= 0
 4_constraint: - C2 + C6 >= 0
 5_constraint: - C3 + C7 >= 0
 6_constraint: - C4 + C8 >= 0
 7_constraint: - C5 - C6 - C7 - C8 >= -2
Bounds
Binaries
 C5 C6 C7 C8
End
