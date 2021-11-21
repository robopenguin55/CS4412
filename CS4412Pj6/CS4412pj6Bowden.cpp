
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*
Jason Bowden
Shortest Path Assignment
*/

/* Used for storing properties about a node */
struct Vertex
{
    int number;
    int distance;
    string color;
    Vertex* parent;
};

/*
Jason Bowden
PriorityQueue class implementation using an array.
The node count is stored in the first index of the queue array.
*/
class PriorityQueue
{
private:
    unsigned int const size = pow(10, 3);
    unsigned int count;
    Vertex** queue;

    int SwapAndReturnChild(int parent, int child)
    {
        Vertex * temp = queue[parent];

        // swap parent and child values
        queue[parent] = queue[child];
        queue[child] = temp;

        return child;
    }
public:
    PriorityQueue()
    {
        queue = (Vertex**)calloc(size, sizeof(Vertex*));
        count = 0;
    }

    /* Insert a value into the queue and bubble up as needed */
    void Insert(Vertex * v)
    {
        queue[++count] = v;

        while (count > 1 && queue[count]->distance < queue[(count >> 1)]->distance)
        {
            Vertex * temp = queue[count];

            // swap parent and child values
            queue[count] = queue[(count >> 1)];
            queue[(count >> 1)] = temp;

            count = (count >> 1);
        }
    }

    /* Extract root of array and bubble down as needed */
    Vertex * ExtractMin()
    {
        // pull root out
        Vertex * min = queue[1];

        if (!min) return min;

        // move right most leaf to root
        queue[1] = queue[count];
        queue[count--] = NULL; // set right most leaf node to NULL (and decrease queue size)

        int currentPosition = 1;
        int leftChild = currentPosition * 2;
        int rightChild = ((double)currentPosition + (double).5) * 2;

        if (queue[currentPosition])
        {
            while (currentPosition < count && (queue[leftChild] && queue[currentPosition]->distance > queue[leftChild]->distance) || (queue[rightChild] && queue[currentPosition]->distance > queue[rightChild]->distance))
            {
                leftChild = currentPosition * 2;
                rightChild = ((double)currentPosition + (double).5) * 2;

                if (queue[leftChild] && queue[leftChild]->distance > 0 && (queue[rightChild] && queue[rightChild]->distance > 0))
                {
                    if (queue[leftChild]->distance < queue[rightChild]->distance) // bubble down left edge
                    {
                        currentPosition = SwapAndReturnChild(currentPosition, leftChild);
                    }
                    else if (queue[rightChild]->distance <= queue[leftChild]->distance) // right edge
                    {
                        currentPosition = SwapAndReturnChild(currentPosition, rightChild);
                    }
                }
                else if (queue[rightChild] && !queue[leftChild])
                {
                    if (queue[rightChild]->distance < queue[currentPosition]->distance)
                    {
                        currentPosition = SwapAndReturnChild(currentPosition, rightChild);;
                    }
                }
                else if (queue[leftChild] && !queue[rightChild])
                {
                    if (queue[leftChild]->distance < queue[currentPosition]->distance)
                    {
                        currentPosition = SwapAndReturnChild(currentPosition, leftChild);
                    }
                }
            }
        }

        return min;
    }

    void DecreaseKey(Vertex * vertex)
    {
        int i = 1;

        while (Vertex * v = queue[i])
        {
            if (v->number == vertex->number)
            {
                int leftChild = i * 2;
                int rightChild = ((double)i + (double).5) * 2;



                v = vertex;
            }

            i++;
        }
    }

    // Print each level of a tree with each node separated by spaces
    // a b-tree has 2^(n)/2 leaf nodes at a given height n
    void PrintQueue()
    {
        unsigned int height = log2(count) + 1;
        unsigned int leafCount = pow(2, height) / 2;

        cout << "Length: " << count << endl;
        cout << "Tree Height: " << height << endl;
        cout << "Leaf count: " << leafCount << endl;

        int t = 1;
        for (unsigned int i = 1; i <= height; i++)
        {
            for (unsigned int j = 1; j <= (pow(2, i) / 2); j++)
            {
                if (queue[t] == NULL)
                {
                    cout << " ";
                }
                else
                    cout << "V" << queue[t]->number << ":" << queue[t]->distance << "  ";

                t++;
            }

            cout << endl;
        }

        cout << endl;
    }
};

/* Custom implementation of a standard queue */
class Queue
{
private:
    unsigned int const size = pow(10, 3);
    unsigned int count;
    unsigned int bottomIndex;
    Vertex** queue;

public:
    Queue()
    {
        queue = (Vertex**)calloc(size, sizeof(Vertex*));
        count = 0;
        bottomIndex = 0;
    }

    void Enqueue(Vertex * item)
    {
        queue[count++] = item;
    }

    Vertex* Dequeue()
    {
        Vertex * item = queue[bottomIndex];

        queue[bottomIndex++] = NULL;

        count--;

        return item;
    }
};

/* Custom implementation of a standard stack */
class Stack
{
private:
    unsigned int const size = pow(10, 3);
    unsigned int count;
    Vertex** stack;

public:
    Stack()
    {
        stack = (Vertex**)calloc(size, sizeof(Vertex*));
        count = 0;
    }

    void Push(Vertex* item)
    {
        stack[count++] = item;
    }

    Vertex* Pop()
    {
        if (count <= 0)
            return NULL;

        Vertex* item = stack[--count];

        stack[count] = NULL;

        return item;
    }
};

/* Hard coded 2D 8x8 array with 30% of the columns as 1s and the rest as -1s */
int eightByEight[8][8] = {
{-1,1,1,1,-1,-1,1,1},
{1,-1,-1,1,1,-1,1,1},
{-1,-1,-1,-1,-1,1,-1,-1},
{1,-1,-1,-1,1,1,1,-1},
{-1,-1,-1,-1,-1,1,1,1},
{1,1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,1,-1,-1},
{-1,-1,1,-1,1,-1,1,-1}
};

/* 
Hard coded 2D 10x10 array with 30% of the values 
equal to a number between 1 and 20, and the rest as -1s.
Random number generation is SLOW... so I hope you don't mind this.
*/
int tenByTen[10][10] = {
{-1,1,6,1,-1,-1,-1,9,7,-1},
{1,-1,-1,1,1,-1,7,1,3,-1},
{-1,-1,-1,-1,-1,1,-1,-1,1,1},
{1,-1,-1,-1,2,1,5,-1,19,1},
{-1,-1,-1,-1,-1,20,4,1,6,-1},
{-1,-1,-1,-1,15,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,1,-1,-1,1,1},
{-1,-1,3,-1,15,-1,19,-1,1,1},
{-1,-1,5,-1,1,-1,1,-1,-1,1},
{-1,-1,7,-1,9,-1,8,-1,-1,-1}
};

/* View the color, distance, vertex #, and prior vertex in a table */
void DisplayVertices(Vertex** vertices, int columns)
{
    for (int j = 0; j < columns; j++)
    {
        if (vertices[j])
            cout << vertices[j]->color << " |";
        else
            cout << "W |";
    }

    cout << endl;
    cout << "--------------------------------" << endl;

    for (int j = 0; j < columns; j++)
    {
        if (vertices[j])
            if (vertices[j]->distance < 10)
                cout << vertices[j]->distance << " |";
            else 
                cout << vertices[j]->distance << "|";
        else
            cout << "~ |";
    }

    cout << endl;
    cout << "--------------------------------" << endl;

    for (int j = 0; j < columns; j++)
    {
        if (vertices[j])
            if (vertices[j]->number < 10)
                cout << vertices[j]->number << " |";
            else 
                cout << vertices[j]->number << "|";
        else
            cout << j + 1 << " |";
    }

    cout << endl;
    cout << "--------------------------------" << endl;

    for (int j = 0; j < columns; j++)
    {
        if (vertices[j] && vertices[j]->parent)
            cout << "V" << vertices[j]->parent->number << "|";
        else
            cout << "~ |";
    }

    cout << endl;
    cout << "--------------------------------" << endl;
}


/* 
Shortest Path implementation for 10x10 that follows:
     dist[source] <- 0  // initialization
     create vertex priority queue Q

     foreach vertex v in graph
         if v <> source
             dist[v] <- INFINITY
             prev[v] <- UNDEFINED

         Q.addWithPriority(v, dist[v])


     while Q is not empty
         u <- Q.extract_min()
         for each neighbor v of u
             alt <- dist[u] + length(u, v)
*/
void ShortestPathForTenByTen()
{
    PriorityQueue* queue = new PriorityQueue();
    Vertex** vertices = (Vertex**)calloc(100, sizeof(Vertex*));

    // set v1
    Vertex* v1 = new Vertex();
    v1->distance = 0;
    v1->number = 1;
    v1->color = "G";
    v1->parent = NULL;

    vertices[v1->number - 1] = v1;

    queue->Insert(v1);

    while (Vertex* min = queue->ExtractMin())
    {
        // process node children
        for (int i = 0; i < 10; i++)
        {
            if (tenByTen[min->number - 1][i] > 0)
            {
                if (vertices[i] && vertices[i]->color.compare("B") == 0)
                    continue;

                if (vertices[i] && vertices[i]->color.compare("G") == 0)
                {
                    if (vertices[i]->distance > (min->distance + tenByTen[min->number - 1][i])) // check existing distance against new distance
                    {
                        vertices[i]->distance = min->distance + tenByTen[min->number - 1][i];
                        vertices[i]->parent = min;

                        // decrease key
                        queue->DecreaseKey(vertices[i]);
                    }
                }
                else
                {
                    int d = tenByTen[min->number - 1][i];
                    // i = vertex #
                    Vertex* v = new Vertex();
                    v->distance = d + min->distance;
                    v->number = i + 1;
                    v->color = "G";
                    v->parent = min;

                    vertices[v->number - 1] = v;

                    queue->Insert(v);
                }
            }
        }

        queue->PrintQueue();
        DisplayVertices(vertices, 10);

        min->color = "B";
        vertices[min->number - 1] = min;
    }
}

/* Shortest Path implementation that attempts to follow:
     dist[source] <- 0  // initialization
     create vertex priority queue Q

     foreach vertex v in graph
         if v <> source
             dist[v] <- INFINITY
             prev[v] <- UNDEFINED

         Q.addWithPriority(v, dist[v])


     while Q is not empty
         u <- Q.extract_min()
         for each neighbor v of u
             alt <- dist[u] + length(u, v)


    Search time is: O(V^2)
*/
void ShortestPath(int ** graph, int childCount)
{
    PriorityQueue* queue = new PriorityQueue();
    Vertex** vertices = (Vertex**)calloc(100, sizeof(Vertex*));

    // set v1
    Vertex* v1 = new Vertex();
    v1->distance = 0;
    v1->number = 1;
    v1->color = "G";
    v1->parent = NULL;

    vertices[v1->number - 1] = v1;

    queue->Insert(v1);

    while (Vertex* min = queue->ExtractMin())
    {
        // process node children
        for (int i = 0; i < childCount; i++)
        {
            if (graph[min->number - 1][i] > 0)
            {
                if (vertices[i] && vertices[i]->color.compare("B") == 0)
                    continue;

                if (vertices[i] && vertices[i]->color.compare("G") == 0)
                {
                    if (vertices[i]->distance > (min->distance + graph[min->number - 1][i])) // check existing distance against new distance
                    {
                        vertices[i]->distance = min->distance + graph[min->number - 1][i];
                        vertices[i]->parent = min;

                        // decrease key
                        queue->DecreaseKey(vertices[i]);
                    }
                }
                else
                {
                    int d = graph[min->number - 1][i];
                    // i = vertex #
                    Vertex* v = new Vertex();
                    v->distance = d + min->distance;
                    v->number = i + 1;
                    v->color = "G";
                    v->parent = min;

                    vertices[v->number - 1] = v;

                    queue->Insert(v);
                }
            }
        }

        queue->PrintQueue();
        DisplayVertices(vertices, childCount);

        min->color = "B";
        vertices[min->number - 1] = min;
    }
}

/* Custom DFS function using a stack 
    Search time is: O(|Vertices|+ |Edges)
*/
void DepthFirstSearch(int childCount)
{
    Stack* stack = new Stack();
    Vertex** vertices = (Vertex**)calloc(100, sizeof(Vertex*));

    Vertex* v1 = new Vertex();
    v1->distance = 0;
    v1->number = 1;
    v1->parent = NULL;
    v1->color = "G";

    stack->Push(v1);
    vertices[v1->number - 1] = v1;

    while (Vertex* item = stack->Pop())
    {
        for (int i = 0; i < childCount; i++)
        {
            // if a positive weighted edge exists
            if (eightByEight[item->number - 1][i] > 0)
            {
                if (item->number == 8) // path to 8 has been found
                {
                    Vertex* found = item;
                    cout << "Path found :";

                    while (found)
                    {
                        if (found->number > 1)
                            cout << "V" << found->number << "<-";
                        else
                            cout << "V" << found->number;

                        found = found->parent;
                    }

                    cout << endl;

                    return;
                }

                if (vertices[i] && vertices[i]->color.compare("B") == 0)
                    continue;

                Vertex* v = new Vertex();
                v->distance = eightByEight[item->number - 1][i];
                v->number = i + 1;
                v->parent = item;
                v->color = "G";

                stack->Push(v);
            }
        }

        item->color = "B";
    }
}

/* Custom BFS using a queue 
   Search time is: O(|Vertices|+ |Edges) 
   */
void BreadthFirstSearch(int childCount)
{
    Queue* queue = new Queue();
    Vertex** vertices = (Vertex**)calloc(100, sizeof(Vertex*));

    Vertex* v1 = new Vertex();
    v1->distance = 0;
    v1->number = 1;
    v1->parent = NULL;
    v1->color = "G";

    queue->Enqueue(v1);
    vertices[v1->number - 1] = v1;

    while (Vertex* item = queue->Dequeue())
    {
        for (int i = 0; i < childCount; i++)
        {
            // if a positive weighted edge exists
            if (eightByEight[item->number - 1][i] > 0)
            {
                if (item->number == 8) // path to 8 has been found
                {
                    Vertex* found = item;
                    cout << "Path found :";

                    while (found)
                    {
                        if (found->number > 1)
                            cout << "V" << found->number << "<-";
                        else
                            cout << "V" << found->number;

                        found = found->parent;
                    }

                    cout << endl;

                    return;
                }

                if (vertices[i] && vertices[i]->color.compare("B") == 0)
                    continue;

                Vertex* v = new Vertex();
                v->distance = eightByEight[item->number - 1][i];
                v->number = i + 1;
                v->parent = item;
                v->color = "G";

                queue->Enqueue(v);
            }
        }

        item->color = "B";
    }
}

/* Display the contents of the EightByEight 2D array */
void print2DArray(int rowSize, int columnSize)
{
	for (int i = 0; i < rowSize; i++)
	{
		for (int j = 0; j < columnSize; j++) cout << eightByEight[i][j] << " ";

		cout << endl;
	}

	cout << endl;
}

/* Holds the contents of the file we read in for part 2 */
int** fileContents;

/* Create matrix from file contents */
void readFileIntoArray()
{
    string line = "";
    ifstream file("project6DataFile.txt");
    int position = 0;
    int n = 0;
    int rows = 0;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            // get NxN value from file
            if (position == 0)
            {
                n = stoi(line);

                fileContents = new int*[n];

                for (int i = 0; i < n; i++)
                {
                    fileContents[i] = new int[n];
                }
            }

            if (!line.empty() && position > 0)
            {
                size_t position = 0;
                int columns = 0;
                string token;

                // swap out tabs for spaces before reading in to our array
                while ((position = line.find("\t")) != std::string::npos)
                {
                    line.replace(position, 1, " ");
                }

                while ((position = line.find(" ")) != std::string::npos)
                {
                    fileContents[rows][columns] = stoi(line.substr(0, position));
                    line.erase(0, position + 1);
                    columns++;
                }

                if (!line.empty())
                    fileContents[rows][columns] = stoi(line);

                rows++;
            }
            
            position++;
        }

        file.close();
    }
    else cout << "Unable to open file";
}

/* Display the contents of the fileContents 2D pointer array */
void Display(int rowCount, int columnCount, int** a)
{
    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            cout << fileContents[i][j] << " ";
        }

        cout << endl;
    }
}

/* Table for viewing Eight by Eight 2D array */
void DisplayEightByEight()
{
    cout << "-------------------------------------" << endl;

    for (int i = 0; i <= 8; i++)
    {
        if (i == 0)
            cout << " | ~";
        else
            cout << " | " << i;

        if (i == 8)
            cout << " |";
    }

    cout << endl;
    cout << "-------------------------------------" << endl;

    for (int i = 0; i < 8; i++)
    {
        cout << " |V" << i + 1;

        for (int j = 0; j < 8; j++)
        {
            if (eightByEight[i][j] > 0)
                cout << " | " << eightByEight[i][j];
            else
                cout << " |" << eightByEight[i][j];
        }

        cout << " |";
        cout << endl;
        cout << "-------------------------------------" << endl;
    }
}

/* Where we handle the requirements in Part 1 of the Assignment 
    1. Generate an 8x8 array with 30% of the cells initialized to 1...
    2. Depth first search from 0 to 9
    3. Breadth first search from 0 to 9
*/
void ExecutePart1()
{
    DisplayEightByEight();

    int childCount = 8;

    cout << "Find path with DFS:" << endl;

    /* Depth first search */
    DepthFirstSearch(childCount);

    cout << "Find path with BFS:" << endl;

    /* Breadth first search */
    BreadthFirstSearch(childCount);
}


/* Where we handle the requirements in Part 2 of the Assignment 
   4. Read in txt file...
   5. Shortest Path through positive weighted directed graph...
*/
void ExecutePart2()
{
    // Read in NxN matrix
    readFileIntoArray();

    ShortestPath(fileContents, 10);
}

/* Where we handle the requirements in Part 3 of the Assignment */
void ExecutePart3()
{
    ShortestPathForTenByTen();
}

/* Application start point */
int main()
{
    string selection;
    do
    {
        cout << "Enter One of the following (1,2,3,q):" << endl;
        cout << "1 = Part 1" << endl;
        cout << "2 = Part 2" << endl;
        cout << "3 = Part 3" << endl;
        cout << "q = Quit" << endl;
        cout << ":";
        cin >> selection;

        if (selection != "q")
        {
            if (selection == "1") ExecutePart1();
            else if (selection == "2") ExecutePart2();
            else if (selection == "3") ExecutePart3();
        }

    } while (selection != "q");
}