# Project Overview

## Problem
Students often do not know which practice questions to solve first or how to fit practice into limited study time.

## Proposed solution
A C++ learning assistant that selects valuable questions under a time budget, creates non-overlapping study sessions, allocates time across topics, compares learning sequences, and demonstrates graph-based topic paths.

## Scope for the first release
- Console interface
- Text-file persistence
- Question bank
- Algorithm demonstrations
- Recommendation based on estimated minutes and question score

## Important design note
The initial sample data uses `score` as a simplified learning-value measure. A real recommendation score should later incorporate topic weakness, question difficulty, recency, and student history.
