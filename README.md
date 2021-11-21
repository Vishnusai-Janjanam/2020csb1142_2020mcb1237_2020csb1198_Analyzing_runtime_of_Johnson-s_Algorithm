Group Members:
 1.Harpreet Singh: 2020MCB1237
 2.Rishabh Jain: 2020CSB1198
 3.Vishnu Sai: 2020CSB1142

Course Instructor: Dr. Anil Shukla
TA: Sravanthi Chede


Project: Runtime Analysis of Johnson’s Algorithm using various Heaps(Fibonacci Heap,Binomial Heap,Binary Heap)

Summary: 
Johnson’s Algorithm is used to find all pair shortest paths in a graphcontaining no negative cycles. It uses the Bellman-Ford and Dijkstra’s algorithm for the same. We have analysed the runtime of this algorithm using different typesof heaps (binary, fibonacci and binomial heaps).


Procedure for Running Code:

    1. Compile the code written in c as follows: gcc Code_file_Group_2.c and Execute the program as follows: a.exe 
    NOTE: If User is running code against input file containing test cases availalbe along with repository as follows: a.exe <<file_name>
    2. Program will ask for number of nodes. 
    3. Then you have to enter which type of groph it is, Directed or Undirected. For Directed Type 'D' and 'U' for undirected. 
    4. Then you have to input edges of graph in following format:
                (u,v,w): 
                       which implies that there is an edge from u to v which has a weight=w
        NOTE: Please enter the values of u and v in range (1,n) only.
    5. Then Program will  ask user choice to use Heap. Type 1 for Binomial Heap , 2 for Binary Heap and 3 for Fibonacci Heap.
    6. After this program will run and give cost of travelling from source to every node or will output NOT REACHABLE if not reachable.
    7. At the end, program will output time taken for executing Dijkstra wrt Heap choosen by user. 
