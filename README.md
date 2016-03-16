# MetaheruisticsQtApp
Developed at University for the course of Metaheuristics using Qt and C++ language.

## Folders
* Deployment: Contains the executables of all the exercises, with all the dependencies necessary to execute them.
* Documentation: Contains the PDF files with more information regarding the algorithms developed and the results of test executions, all of them in Spanish language.
* Instancias: Contains all the instances used for the testing of the application
* Meta*: Contains all the source code of the application for that exercise, including self developed metaheuristics algorithms.

## Algorithm included with each exercise
* Meta1a: Greedy, Local search and Tabu search.
* Meta1b: Local search, GRASP (Greedy Randomized Adaptive Search Procedure) and ILS (Iterative Local Search).
* Meta2: Local search and two genetic algorithms.
* Meta3: Local search and three versions of a memetic algorithm.

Further information can be found in the PDF files included in the Documentation folder (all of them in Spanish language).

## Deployement
For deploying the application, first you will need to install Qt. Then, once installed, open Qt program and from within just open the project (with the option OpenProject) selecting inside the folder Meta3 the file with .pro extension.

Once opened, build and execute will show the same interface as the executable file in the Deployment folder.

For the final deploymente, you can use the method you prefer. For instance, I used a simple method extracting the executable file and all the Qt dependencies using [this tutorial](http://wiki.qt.io/Deploy_an_Application_on_Windows).

