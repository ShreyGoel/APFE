\ Model first
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  C0 + 2 C1 + 4.3 C2 + 2.26 C3 + 1.33 C4 + 2.2 C5 + 9 C6 + 10 C7
Subject To
 1_constraint: 1.05 C0 + 3.05 C1 + 2.5 C2 + C3 + 0.4 C4 + 3 C5 + 8.5 C6
   + 11 C7 >= 0
 2_constraint: 1.05 C0 + 1.1 C1 + 3.1 C2 + 9.8 C3 + 1.2 C4 + 0.011 C5
   + 0.3 C6 + 1.4 C7 >= 0
 3_constraint: 1.05 C0 + 1.2 C1 + 26 C2 + 10 C3 + 2.5 C4 + 1.5 C5 + 11 C6
   + 3.3 C7 >= 0
 4_constraint: - C1 + C8 >= 0
 5_constraint: - C2 + C9 >= 0
 6_constraint: - C3 + C10 >= 0
 7_constraint: - C4 + C11 >= 0
 8_constraint: - C5 + C12 >= 0
 9_constraint: - C6 + C13 >= 0
 10_constraint: - C7 + C14 >= 0
 11_constraint: - C8 - C9 - C10 - C11 - C12 - C13 - C14 >= -5
Bounds
Binaries
 C8 C9 C10 C11 C12 C13 C14
End
