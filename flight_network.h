#ifndef FLIGHT_NETWORK_H
#define FLIGHT_NETWORK_H

#define MAX_CITIES 50

// --- Structures ---
typedef struct Node {
    char city[50];
    int time;
    struct Node* next;
} Node;

typedef struct {
    char cities[MAX_CITIES][50];
    Node* adjList[MAX_CITIES];
    int cityCount;
} Graph;

typedef struct {
    int data[MAX_CITIES];
    int front, rear;
} Queue;

// --- Utility Colors ---
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// --- Function Declarations ---
void initGraph(Graph* g);
Node* createNode(const char* city, int time);
int findCityIndex(Graph* g, const char* city);

void addFlightRoute(Graph* g, const char* src, const char* dest, int time);
void removeFlightRoute(Graph* g, const char* src, const char* dest);
void displayFlightNetwork(Graph* g);
void bfsConnectivity(Graph* g, const char* startCity);
void findFlightPath(Graph* g, const char* startCity, const char* endCity);

void loadRoutesFromFile(Graph* g, const char* filename);
void saveAdjacencyToFile(Graph* g);
void saveGraphToFile(Graph* g, const char* highlightPath[][2], int highlightCount);

void initQueue(Queue* q);
int isEmpty(Queue* q);
void enqueue(Queue* q, int val);
int dequeue(Queue* q);

#endif
