<<<<<<< HEAD
# Sliding Puzzle Solver (in progress...)
=======
# Sliding Puzzle Solver (in progress)
>>>>>>> ae347522ed09453d12469135242287e8b7f5d2ec

A sliding puzzle is a combination puzzle that challenges a player to slide (frequently flat) pieces along certain routes (usually on a board) to establish a certain end-configuration.

Input
--
An n x n array/list comprised of integer values ranging from 0 to n^2 - 1 (inclusive), which represents a square grid of tiles. Note that there will always be one empty tile (represented by 0) to allow for movement of adjacent tiles.

Output
--
An array/list comprised of any (but not necessarily all) of the integers from 1 to n^2 - 1, inclusive. This represents the sequence of tile moves for a successful transition from the initial unsolved state to the solved state. If the puzzle is unsolvable, return null(JavaScript, Java, PHP) or None(Python) or the vector {0} (C++).

The range of values for n are: 10 >= n >= 3
