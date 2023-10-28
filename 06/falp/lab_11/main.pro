domains 
 list = integer*.

predicates
 nondeterm len1(list, integer, integer).
 nondeterm len(list, integer).
 
 nondeterm sum1(list, integer, integer).
 nondeterm sum(list, integer).
 
 nondeterm sum_odd_pos1(list, integer, integer).
 nondeterm sum_odd_pos(list, integer).
 
 nondeterm list_bigger(list, integer, list).
 
 nondeterm rm_all(list, integer, list).
 nondeterm rm_one(list, integer, list).
 
 nondeterm union(list, list, list).
 
clauses
 len1([], Acc, Acc) :- !.
 len1([_ | T], Acc, Res) :- Acc1 = Acc + 1, len1(T, Acc1, Res).
 len(L, Res) :- len1(L, 0, Res).
 
 sum1([], Acc, Acc) :- !.
 sum1([H | T], Acc, Res) :- Acc1 = Acc + H, sum1(T, Acc1, Res).
 sum(L, Res) :- sum1(L, 0, Res).
 
 sum_odd_pos1([], Acc, Res) :- Res = Acc, !.
 sum_odd_pos1([_, H | T], Acc, Res) :- Acc1 = Acc + H, sum_odd_pos1(T, Acc1, Res).
 sum_odd_pos(L, Res) :- sum_odd_pos1(L, 0, Res).
 
 list_bigger([], _, []) :- !.
 list_bigger([H | T], N, [H | ResT]) :- H > N, !, list_bigger(T, N, ResT).
 list_bigger([_ | T], N, Res) :- list_bigger(T, N, Res).
 
 rm_all([], _, []) :- !.
 rm_all([H | T], N, Res) :- H = N, !, rm_one(T, N, Res).
 rm_all([H | T], N, [H | ResT]) :- rm_one(T, N, ResT).
 
 rm_one([], _, []) :- !.
 rm_one([H | T], N, T) :- H = N, !.
 rm_one([H | T], N, [H | ResT]) :- rm_one(T, N, ResT).
 
 union([], L, L) :- !.
 union([H | T], L, [H | ResT]) :- !, union(T, L, ResT).

goal
 % len([1, 2, 3], Len).
 % sum([1, 2, 3], Sum).
 % sum_odd_pos([1, 2, 3, 4, 5], Sum).
 % list_bigger([1, 2, 3, 4, 5], 3, List).
 % rm_all([1, 2, 2, 3, 4, 2, 4], 2, List).
 % rm_one([1, 2, 2, 3, 4, 2, 4], 2, List).
 % union([1, 2, 3, 4], [5, 6, 7], List).