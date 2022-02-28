% Knowledge Base

flight(canakkale, erzincan, 6).
flight(erzincan, antalya, 3).
flight(antalya, diyarbakir, 4).
flight(antalya, izmir, 2).
flight(diyarbakir, ankara, 8).
flight(izmir, ankara, 6).
flight(izmir, istanbul, 2).
flight(ankara, istanbul, 1).
flight(ankara, rize, 5).
flight(ankara, van, 4).
flight(istanbul, rize, 4).
flight(van, gaziantep, 3).
flight(rize, ankara, 5).

% Rules

route(X, Y, C) :-
    shortest(X, Y, C).

route(X, Y, C) :-
    shortest(Y, X, C).
%%%
shortest(X, Y, C) :-     % For all X and Y cities, there is a shortest path with the cost of C where:
    path(X, Y, C),       % There exists a path from X to Y...
    \+ (path(X, Y, C1),  % ...and there isn't any other path...
    C1 < C).             % ...where its cost is smaller than the first path

path(X, Y, C) :- path(X, Y, C, []).  % [] is the list of visited cities

path(X, Y, C, Visited) :-
    \+ member(X, Visited),      % X is not visited...
    flight(X, Y, C).            % ...and there is a flight from X to Y.
    
% Next rule works recursively.
path(X, Y, C, Visited) :-
    \+ member(X, Visited),          % X is not visited...
    flight(X, Z, C1),               % ...and there exists a flight from X to Z...
    path(Z, Y, C2, [X|Visited]),    % ...and there exists a path from Z to Y. X is added to visited list.
    C is C1 + C2.                   % Total cost is C1 + C2.

