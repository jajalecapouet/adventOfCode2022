# adventOfCode2022

My solvers are used to be more optimized for the machine and modular than fast and easy to code.

Performance Tasks :
- day12 : replace multimap<int, Node*> by map<std::pair<int, coord>, Node*> to get highly better performance.
- day13 : stock MyTuple container in personnal library and rename it like "MyTree".
- day15.2 : create Circle's member's function which return the (x, y) position of next point out of his border and inside the square [(0, 0)-(4 000 000, 4 000 000)].
- day16 : define all sequences of valves opening then simulate for all those sequences. (Unstead of my gaz factory).