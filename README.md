# AdaptiveLearningSystem

An adaptive learning and question recommendation desktop application for
CSE students. The system analyses a student's quiz performance,
identifies weaker topics, and recommends what to practise next. It also
demonstrates selected algorithms from the **Algorithm Design and
Analysis** course.

> **Project status:** This README describes the agreed project direction
> and a suggested implementation structure. Update the build commands,
> Qt version, screenshots, and file names to match the final source code
> before publishing.

## Table of Contents

-   [Overview](#overview)
-   [Goals](#goals)
-   [Main Features](#main-features)
-   [How the System Works](#how-the-system-works)
-   [Recommendation Workflow](#recommendation-workflow)
-   [Algorithms](#algorithms)
-   [Application Architecture](#application-architecture)
-   [Suggested Project Structure](#suggested-project-structure)
-   [Technology Stack](#technology-stack)
-   [Setup and Installation](#setup-and-installation)
-   [Building and Running](#building-and-running)
-   [Data Storage](#data-storage)
-   [User Roles](#user-roles)
-   [Example Workflow](#example-workflow)
-   [Testing](#testing)
-   [Team Responsibilities](#team-responsibilities)
-   [Future Improvements](#future-improvements)
-   [Contributing](#contributing)

## Overview

AdaptiveLearningSystem is a C++ desktop application designed to support
personalised learning for students studying Computer Science and
Engineering (CSE).

Instead of presenting the same learning path to every student, the
application uses quiz results and topic-level performance to estimate
each student's learning needs. It then selects suitable topics or
questions for the student's next practice session.

The project combines a practical educational use case with algorithm
demonstrations and visual explanations.

## Goals

-   Identify a student's strong and weak topics using quiz performance.
-   Recommend practice topics based on performance and available study
    time.
-   Demonstrate how selected course algorithms can solve practical
    problems.
-   Provide a graphical interface for students and teachers.
-   Keep the first version manageable as a local desktop application.

## Main Features

### Student

-   Take diagnostic or topic-based quizzes.
-   View scores and topic-wise performance.
-   Identify topics that need more practice.
-   Receive a prioritised list of recommended topics or questions.
-   Review previous attempts and progress.

### Teacher

-   View student performance summaries.
-   Review topic-level strengths and weaknesses.
-   Manage or organise questions and topics, if this module is included
    in the final implementation.

### Algorithm demonstration

-   Show how the recommendation and scheduling algorithms work.
-   Display relevant inputs, selected items, and outputs.
-   Explain the practical purpose of each algorithm through the
    application.

> The exact teacher permissions and question-management features depend
> on the final implementation.

## How the System Works

At a high level, the application follows this process:

1.  A student selects a subject or starts a diagnostic quiz.
2.  The student answers questions.
3.  The application evaluates the answers.
4.  Results are grouped by topic.
5.  The system calculates topic performance and identifies weaker areas.
6.  Topics or practice activities receive priority values.
7.  The recommendation module selects a useful set of activities within
    the student's available study time.
8.  The application displays the recommended practice plan.
9.  New attempts update the student's performance data and influence
    future recommendations.

## Recommendation Workflow

``` text
            ┌─────────────────────┐
            │   Student Login     │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │ Select Subject /    │
            │ Start Diagnostic    │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │     Take Quiz       │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │ Evaluate Answers    │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │ Analyse Performance │
            │ Topic by Topic      │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │ Calculate Topic     │
            │ Priorities          │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │ 0/1 Knapsack-based  │
            │ Recommendation      │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │ Display Study Plan  │
            └──────────┬──────────┘
                       │
                       ▼
            ┌─────────────────────┐
            │ Record New Attempts │
            │ and Update Progress │
            └─────────────────────┘
```

### Topic performance

A topic's performance can be estimated from the student's previous
answers. For example:

-   Higher accuracy indicates stronger performance.
-   Lower accuracy indicates a topic that may need more practice.
-   A topic with little or no attempt history can be treated as
    unassessed.

The exact scoring formula should be documented in the implementation
once it is finalised. Avoid treating a single incorrect answer as proof
that a student is weak in a topic.

### Recommendation selection

Each candidate activity can be assigned:

-   **Time cost:** estimated minutes needed.
-   **Priority/value:** how useful the activity is for the student.
-   **Topic:** the subject area it belongs to.

The 0/1 Knapsack algorithm can then select a combination of activities
that maximises total priority without exceeding the student's available
study time.

## Algorithms

The project is based on the algorithm topics covered in the course. The
MVP should focus on algorithms that directly support the application.

  -----------------------------------------------------------------------
  Algorithm               Planned use             Priority
  ----------------------- ----------------------- -----------------------
  0/1 Knapsack            Select a high-value set Core
                          of practice activities  
                          within a time limit     

  Activity Selection      Schedule                Core
                          non-overlapping study   
                          activities or sessions  

  Fractional Knapsack     Demonstrate a           Secondary
                          comparison with the 0/1 
                          recommendation approach 
                          or allocate divisible   
                          study resources         

  Longest Common          Optional comparison of  Optional
  Subsequence (LCS)       answer sequences or     
                          learning-pattern        
                          demonstration           

  Coin Change             Course algorithm        Not required for the
                          demonstration, if time  MVP
                          permits                 

  Bellman-Ford            Course algorithm        Not required for the
                          demonstration, if time  MVP
                          permits                 
  -----------------------------------------------------------------------

All required algorithm implementations should be written from scratch
for learning and demonstration purposes rather than hidden behind a
library implementation.

### 0/1 Knapsack

**Purpose:** Select practice activities that provide the highest total
priority while staying within a student's available time.

-   Weight: estimated time required for an activity.
-   Value: recommendation priority.
-   Capacity: available study time.
-   Output: selected activities.

### Activity Selection

**Purpose:** Choose a set of compatible activities when activities have
start and finish times.

### Fractional Knapsack

**Purpose:** Demonstrate a greedy strategy when a resource can be
divided. This is not the same as selecting indivisible practice
questions, so it should be presented as a secondary demonstration rather
than the main recommender.

### Optional algorithms

LCS, Coin Change, and Bellman-Ford can be added as independent course
demonstrations if the team has enough time. They are not necessary for
the first working recommendation flow.

## Application Architecture

The planned first version is a **C++17 desktop application with a Qt
Widgets graphical interface**.

It is a local application, not a web application with a separate online
server. The UI, application logic, recommendation services, and
algorithm implementations run within the desktop program.

``` text
┌─────────────────────────────────────────────┐
│             Qt Widgets Frontend             │
│ Login │ Dashboard │ Quiz │ Results │ Admin   │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             Application Services            │
│ Quiz Service │ Progress │ Recommendation     │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│              Algorithm Module               │
│ Knapsack │ Activity Selection │ Other Demos  │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             Models and Data Layer           │
│ Users │ Topics │ Questions │ Attempts        │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│            Local File Storage                │
│             CSV / Text Files                 │
└─────────────────────────────────────────────┘
```

### Frontend

The frontend is built with Qt Widgets. It is responsible for:

-   Displaying screens and navigation.
-   Collecting user input.
-   Showing quiz questions and answer feedback.
-   Displaying performance summaries and recommendations.
-   Providing controls for algorithm demonstrations.

The UI should call service functions rather than implementing
recommendation calculations directly inside button-click handlers or
screen classes.

### Backend / application logic

In this desktop architecture, "backend" means the C++ application logic
running behind the interface. It includes:

-   Authentication and user handling.
-   Quiz evaluation.
-   Topic performance analysis.
-   Recommendation generation.
-   Progress updates.
-   Reading and writing local data.

There is no separate REST API or remote backend server in the planned
MVP.

## Suggested Project Structure

The following is a suggested structure. Keep the actual repository
consistent with it, or revise this section to reflect the folders that
exist in the final project.

``` text
AdaptiveLearningSystem/
│
├── CMakeLists.txt
├── README.md
├── .gitignore
│
├── assets/
│   ├── icons/
│   ├── images/
│   └── styles/
│
├── data/
│   ├── users.csv
│   ├── subjects.csv
│   ├── topics.csv
│   ├── questions.csv
│   └── attempts.csv
│
├── include/
│   ├── models/
│   │   ├── User.h
│   │   ├── Subject.h
│   │   ├── Topic.h
│   │   ├── Question.h
│   │   └── Attempt.h
│   │
│   ├── algorithms/
│   │   ├── Knapsack.h
│   │   ├── ActivitySelection.h
│   │   ├── FractionalKnapsack.h
│   │   └── LCS.h
│   │
│   ├── services/
│   │   ├── AuthService.h
│   │   ├── QuizService.h
│   │   ├── ProgressService.h
│   │   ├── RecommendationService.h
│   │   └── DataService.h
│   │
│   └── ui/
│       ├── LoginWindow.h
│       ├── DashboardWindow.h
│       ├── QuizWindow.h
│       ├── ResultsWindow.h
│       └── TeacherWindow.h
│
├── src/
│   ├── main.cpp
│   │
│   ├── models/
│   ├── algorithms/
│   ├── services/
│   └── ui/
│
├── tests/
│   ├── algorithm_tests.cpp
│   ├── recommendation_tests.cpp
│   └── quiz_tests.cpp
│
└── docs/
    ├── workflow.md
    └── algorithm-notes.md
```

### Folder responsibilities

  -----------------------------------------------------------------------
  Folder                              Responsibility
  ----------------------------------- -----------------------------------
  `assets/`                           Icons, images, and UI styles

  `data/`                             Local CSV/text data files

  `include/models/`                   Data structures representing
                                      application entities

  `include/algorithms/`               Algorithm declarations

  `include/services/`                 Application service declarations

  `include/ui/`                       Qt window and widget declarations

  `src/`                              C++ implementations

  `tests/`                            Algorithm and application tests

  `docs/`                             Additional project documentation
  -----------------------------------------------------------------------

Do not put all application logic in `main.cpp`. Keep UI, services,
models, algorithms, and file operations separated so each part can be
developed and tested independently.

## Technology Stack

  Technology         Purpose
  ------------------ -----------------------------------
  C++17              Main programming language
  Qt Widgets         Desktop graphical interface
  CMake              Suggested build-system choice
  CSV / text files   Local data persistence
  Git and GitHub     Version control and collaboration

The exact Qt release and build-system configuration should match the
repository's actual configuration.

## Setup and Installation

### 1. Install development tools

You need:

-   A C++17-compatible compiler.
-   Qt with the Widgets module.
-   CMake, if the project uses the suggested CMake setup.
-   Git.
-   Qt Creator or another editor configured for Qt development.

On Fedora, the general package installation command is:

``` bash
sudo dnf install gcc-c++ cmake make git
```

Install the Qt development packages and Qt Creator using the package
names available for your Fedora release, or use the official Qt
installer. The project must be configured to use the same Qt
installation when building.

### 2. Clone the repository

``` bash
git clone <repository-url>
cd AdaptiveLearningSystem
```

Replace `<repository-url>` with the actual Git repository URL.

### 3. Configure the project

If the repository uses CMake, configure it with:

``` bash
cmake -S . -B build
```

If CMake cannot locate Qt, set `CMAKE_PREFIX_PATH` to the Qt
installation directory or configure the Qt kit in Qt Creator.

## Building and Running

For a CMake-based project:

``` bash
cmake --build build
```

Run the executable produced by the build. Its exact path and filename
depend on the target name and build configuration.

For example, if the executable is named `AdaptiveLearningSystem` and is
placed directly in the build directory:

``` bash
./build/AdaptiveLearningSystem
```

Alternatively:

1.  Open the project in Qt Creator.
2.  Configure a kit with a C++17 compiler and the matching Qt version.
3.  Select the build configuration.
4.  Build the project.
5.  Run the application.

> These commands assume a CMake project. If the repository uses a `.pro`
> qmake project or another build setup, replace this section with the
> commands for that build system.

## Data Storage

The planned MVP uses local CSV or text files rather than a database or
remote API.

Possible data files:

  -----------------------------------------------------------------------
  File                                Stores
  ----------------------------------- -----------------------------------
  `users.csv`                         User IDs, names, roles, and locally
                                      required account information

  `subjects.csv`                      Available subjects

  `topics.csv`                        Topics belonging to each subject

  `questions.csv`                     Questions, options, correct
                                      answers, and topic references

  `attempts.csv`                      Student answers, scores, and
                                      attempt history
  -----------------------------------------------------------------------

### Data handling guidelines

-   Use a stable unique ID for each user, topic, and question.
-   Link questions to topics using IDs rather than relying on row
    positions.
-   Validate data when loading files.
-   Handle missing or malformed files without crashing.
-   Keep file reading and writing inside the data service.
-   Do not store plain-text passwords. If local authentication is
    implemented, use an appropriate password-hashing approach.

The final CSV column names and formats should be documented once the
data model is implemented.

## User Roles

### Student

The student-facing workflow focuses on learning:

1.  Sign in.
2.  Select a subject.
3.  Take a diagnostic or practice quiz.
4.  Review the result.
5.  View weak topics.
6.  Follow the recommended practice plan.
7.  Repeat quizzes to track improvement.

### Teacher

The teacher-facing workflow focuses on monitoring and learning content:

1.  Sign in with a teacher account.
2.  View available students or performance summaries.
3.  Review topic-level results.
4.  Manage learning content if the final version includes that feature.

## Example Workflow

Suppose a student has 30 minutes available for practice.

  Activity                 Estimated time   Priority
  ---------------------- ---------------- ----------
  Array practice                   10 min          5
  Linked-list practice             15 min          9
  Stack practice                   10 min          7
  Graph practice                   20 min         10

The recommendation module treats each activity as an item:

-   Its estimated time is the weight.
-   Its priority is the value.
-   The student's 30 minutes is the knapsack capacity.

The 0/1 Knapsack algorithm selects the combination with the highest
total priority that fits the available time. The actual result depends
on the implemented scoring formula and activity data.

## Testing

Test each part independently before testing the full application.

### Algorithm tests

-   Check known input/output examples.
-   Test empty input and boundary cases.
-   Test cases where no item fits the knapsack capacity.
-   Test duplicate or equal activity times.
-   Verify that selected activities do not exceed the time limit.

### Application tests

-   Verify quiz scoring.
-   Verify topic-wise performance calculations.
-   Verify that attempts are saved and loaded correctly.
-   Verify recommendations for students with different performance
    histories.
-   Verify behaviour when a data file is missing or invalid.
-   Test navigation between screens and invalid user input.

## Team Responsibilities

The project is planned for a four-member team with a short development
period. A practical division is:

  -----------------------------------------------------------------------
  Work area                           Main responsibility
  ----------------------------------- -----------------------------------
  UI / frontend                       Qt screens, navigation, and visual
                                      presentation

  Algorithms                          Implement and test the required
                                      algorithms

  Models and data                     Data structures, CSV handling, and
                                      sample data

  Integration and testing             Connect UI to services, test
                                      workflows, and prepare
                                      documentation
  -----------------------------------------------------------------------

Team members can share tasks, but agree on function signatures and data
structures before integrating separate modules.

## Future Improvements

Possible improvements beyond the MVP:

-   More detailed progress charts.
-   Better recommendation scoring using longer-term performance.
-   Question difficulty levels.
-   More subjects and larger question banks.
-   Exportable student progress reports.
-   A database-backed version.
-   A web or mobile client connected to a central server.
-   Additional algorithm visualisations.

These are future possibilities, not requirements for the first working
version.

## Contributing

1.  Create a separate branch for your task.
2.  Keep changes focused on one module.
3.  Follow the existing folder and naming conventions.
4.  Do not duplicate algorithm logic inside UI classes.
5.  Build and test before committing.
6.  Use clear commit messages.
7.  Open a pull request and explain what changed.

Example:

``` bash
git checkout -b feature/recommendation-module
git add .
git commit -m "Implement topic recommendation logic"
git push -u origin feature/recommendation-module
```

## Project Notes

-   The application is intended as a C++17 desktop project.
-   Qt Widgets is the planned graphical frontend.
-   Local CSV/text storage is the planned MVP data layer.
-   0/1 Knapsack is the main recommendation algorithm.
-   Activity Selection is intended for scheduling.
-   Fractional Knapsack is secondary; LCS is optional.
-   Coin Change and Bellman-Ford are not required for the MVP.
-   The project should remain understandable, modular, and achievable
    within the course timeline.

------------------------------------------------------------------------

**AdaptiveLearningSystem** --- personalised practice through
algorithm-based recommendations.
