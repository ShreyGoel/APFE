\ Model first
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  C0 + 2 C1 + 5 C2 + 7 C3 + 3.5 C4
Subject To
 1_constraint: 1.05 C0 + C1 - 4 C2 + C3 >= 0
 2_constraint: 1.05 C0 + 2 C1 + 7 C2 - 5 C3 >= 0
 3_constraint: 1.05 C0 + 2 C1 + 9 C2 - 9 C3 + 5 C4 >= 0
 4_constraint: - C1 + C5 >= 0
 5_constraint: - C2 + C6 >= 0
 6_constraint: - C3 + C7 >= 0
 7_constraint: - C4 + C8 >= 0
 8_constraint: - C5 - C6 - C7 - C8 >= -2
Bounds
Binaries
 C5 C6 C7 C8
End
