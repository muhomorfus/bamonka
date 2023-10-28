list_find(List, Elem) :- list_find(List, Elem, _, _).

list_find([], _, _, _) :- !.
list_find([H|T], Group, [_|TResist], Resist) :- H \= Group, !, list_find(T, Group, TResist, Resist).
list_find([_|_], _, [HResist|_], Resist):- Resist #= HResist ,!.

plate_by_group(Group, Name, Ap, Fn, Cost) :- plate_properties(PlateGroup, List, _),
	                                         list_find(List, Group),
	                                         plate(PlateGroup, Name, Ap, Fn, Cost).

plate_by_group(Group, Name, Ap, Fn, Cost, Resistance) :- plate_properties(PlateGroup, List, ListResistance),
	                                                     list_find(List, Group, ListResistance, Resistance),
	                                                     plate(PlateGroup, Name, Ap, Fn, Cost).

gradou(X, XStart, XEnd, Y, YStart, YEnd):- Y #= (X*YEnd - X*YStart - XStart*YEnd + XStart*YStart) // (XEnd - XStart) + YStart.

vc(Metal, Plate, Cover, ResF, ResV, ResA) :- metal(Metal),
                                metal_properties(Group, Metal, _, _),
                                plate_by_group(Group, Plate, ap(_, _, ResA), fn(_, ResF, _), _),
                                metal_h(Group, h(FnMin, FnMax)),
                                metal_v(Metal, Cover, v(VMin, VMax)),
                                gradou(ResF, FnMin, FnMax, ResV, VMin, VMax).

q(Metal, Plate, Cover, Res) :- metal(Metal),
                               vc(Metal, Plate, Cover, F, V, A),
                               Res #= F * V * A // 10000.

divide(A, B, C) :- A mod B is 0, C #= A // B, !.
divide(A, B, C) :- C #= A // B + 1.

time(Metal, Plate, Cover, Volume, Time) :- metal(Metal),
                                           q(Metal, Plate, Cover, Q),
                                           divide(Volume, Q, Time).

cost(Metal, Plate, Cover, Volume, Cost, Count) :- metal(Metal),
                                           metal_properties(Group, Metal, _, _),
                                           plate_by_group(Group, Plate, _, _, cost(CostItem), Resistance),
                                           time(Metal, Plate, Cover, Volume, Time),
                                           divide(Time, Resistance, Count),
                                           Cost #= CostItem * Count.

cost_border(Metal, Plate, Cover, Volume, Cost, Count) :- metal(Metal),
                                           metal_properties(Group, Metal, _, _),
                                           plate_by_group(Group, Plate, _, _, cost(CostItem), Resistance),
                                           time(Metal, Plate, Cover, Volume, Time),
                                           divide(Time, Resistance, Count),
                                           Time > 0,
                                           Count #> 0,
                                           Cost #>= CostItem * Count.

listWithCosts(Metal, Plate, Cover, Volume, Cost, Count, List) :- findall(struct(Metal, Plate, Cover, Volume, Cost, Count), cost(Metal, Plate, Cover, Volume, Cost, Count), List).

compare(struct(_, _, _, _, Cost1, _), struct(_, _, _, _, Cost2, _)) :- Cost1 #< Cost2.

minCost(Metal, Plate, Cover, Volume, Cost, Count) :- listWithCosts(Metal, Plate, Cover, Volume, Cost, Count, List),
                                              min_member(compare, struct(Metal, Plate, Cover, Volume, Cost, Count), List).