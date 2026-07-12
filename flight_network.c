#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight_network.h"

// ---------------- BASIC GRAPH FUNCTIONS ----------------
void initGraph(Graph* g) {
    g->cityCount = 0;
    for (int i = 0; i < MAX_CITIES; i++)
        g->adjList[i] = NULL;
}

Node* createNode(const char* city, int time) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->city, city);
    newNode->time = time;
    newNode->next = NULL;
    return newNode;
}

int findCityIndex(Graph* g, const char* city) {
    for (int i = 0; i < g->cityCount; i++)
        if (strcmp(g->cities[i], city) == 0)
            return i;
    return -1;
}

// ---------------- QUEUE ----------------
void initQueue(Queue* q) { q->front = q->rear = 0; }
int isEmpty(Queue* q) { return q->front == q->rear; }
void enqueue(Queue* q, int val) { q->data[q->rear++] = val; }
int dequeue(Queue* q) { return q->data[q->front++]; }

// ---------------- FILE UPDATES ----------------
void saveAdjacencyToFile(Graph* g) {
    FILE* f = fopen("adjacency.txt", "w");
    fprintf(f, "=== FLIGHT NETWORK (Adjacency List) ===\n\n");
    for (int i = 0; i < g->cityCount; i++) {
        fprintf(f, "%s → ", g->cities[i]);
        Node* temp = g->adjList[i];
        if (!temp) fprintf(f, "No direct flights");
        while (temp) {
            fprintf(f, "%s(%d hrs)", temp->city, temp->time);
            if (temp->next) fprintf(f, ", ");
            temp = temp->next;
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

// ---------------- GRAPH OUTPUT (MAP VIEW) ----------------
void saveGraphToFile(Graph* g, const char* highlightPath[][2], int highlightCount) {
    FILE* f = fopen("graph_output.txt", "w");
    fprintf(f, "==============================\n");
    fprintf(f, "       ✈️  FLIGHT MAP VIEW\n");
    fprintf(f, "==============================\n\n");

    if (highlightCount > 0) {
        fprintf(f, "🟥 Shortest Path Highlighted:\n");
        for (int i = 0; i < highlightCount; i++) {
            fprintf(f, "%s → %s", highlightPath[i][0], highlightPath[i][1]);
            if (i != highlightCount - 1) fprintf(f, " → ");
        }
        fprintf(f, "\n\n──────────────────────────────\n\n");
    }

    fprintf(f, "City Connections:\n\n");
    for (int i = 0; i < g->cityCount; i++) {
        Node* temp = g->adjList[i];
        int hasRoute = 0;
        while (temp) {
            int isHighlighted = 0;
            for (int h = 0; h < highlightCount; h++) {
                if (strcmp(g->cities[i], highlightPath[h][0]) == 0 && strcmp(temp->city, highlightPath[h][1]) == 0)
                    isHighlighted = 1;
            }
            if (isHighlighted)
                fprintf(f, "%-12s ==> [%s] (%d hrs)\n", g->cities[i], temp->city, temp->time);
            else
                fprintf(f, "%-12s ──▶ %s (%d hrs)\n", g->cities[i], temp->city, temp->time);
            hasRoute = 1;
            temp = temp->next;
        }
        if (!hasRoute)
            fprintf(f, "%-12s ── No direct flights\n", g->cities[i]);
        fprintf(f, "\n");
    }

    fprintf(f, "==============================\n");
    fclose(f);
}

// ---------------- LOAD ROUTES ----------------
void loadRoutesFromFile(Graph* g, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf(RED "⚠️ Could not open %s, starting with empty network.\n" RESET, filename);
        return;
    }

    char src[50], dest[50];
    int time;
    while (fscanf(f, "%s %s %d", src, dest, &time) == 3) {
        int srcIdx = findCityIndex(g, src);
        int destIdx = findCityIndex(g, dest);

        if (srcIdx == -1) {
            strcpy(g->cities[g->cityCount], src);
            g->adjList[g->cityCount] = NULL;
            srcIdx = g->cityCount++;
        }
        if (destIdx == -1) {
            strcpy(g->cities[g->cityCount], dest);
            g->adjList[g->cityCount] = NULL;
            destIdx = g->cityCount++;
        }

        // Avoid duplicates
        Node* temp = g->adjList[srcIdx];
        int exists = 0;
        while (temp) {
            if (strcmp(temp->city, dest) == 0) { exists = 1; break; }
            temp = temp->next;
        }
        if (!exists) {
            Node* newNode = createNode(dest, time);
            newNode->next = g->adjList[srcIdx];
            g->adjList[srcIdx] = newNode;
        }
    }
    fclose(f);
    printf(GREEN "✅ Routes loaded successfully from %s\n" RESET, filename);
    saveAdjacencyToFile(g);
    saveGraphToFile(g, NULL, 0);
}

// ---------------- ADD ROUTE ----------------
void addFlightRoute(Graph* g, const char* src, const char* dest, int time) {
    int srcIdx = findCityIndex(g, src);
    int destIdx = findCityIndex(g, dest);

    if (srcIdx == -1) { strcpy(g->cities[g->cityCount], src); g->adjList[g->cityCount] = NULL; srcIdx = g->cityCount++; }
    if (destIdx == -1) { strcpy(g->cities[g->cityCount], dest); g->adjList[g->cityCount] = NULL; destIdx = g->cityCount++; }

    Node* temp = g->adjList[srcIdx];
    while (temp) {
        if (strcmp(temp->city, dest) == 0) return; // prevent duplicates
        temp = temp->next;
    }

    Node* newNode = createNode(dest, time);
    newNode->next = g->adjList[srcIdx];
    g->adjList[srcIdx] = newNode;

    FILE* fr = fopen("routes.txt", "a");
    fprintf(fr, "%s %s %d\n", src, dest, time);
    fclose(fr);

    saveAdjacencyToFile(g);
    saveGraphToFile(g, NULL, 0);
}

// ---------------- REMOVE ROUTE ----------------
void removeFlightRoute(Graph* g, const char* src, const char* dest) {
    int srcIdx = findCityIndex(g, src);
    if (srcIdx == -1) return;

    Node *temp = g->adjList[srcIdx], *prev = NULL;
    while (temp && strcmp(temp->city, dest) != 0) { prev = temp; temp = temp->next; }
    if (!temp) return;

    if (prev) prev->next = temp->next; else g->adjList[srcIdx] = temp->next;
    free(temp);

    FILE* f = fopen("routes.txt", "w");
    for (int i = 0; i < g->cityCount; i++) {
        Node* t = g->adjList[i];
        while (t) { fprintf(f, "%s %s %d\n", g->cities[i], t->city, t->time); t = t->next; }
    }
    fclose(f);

    saveAdjacencyToFile(g);
    saveGraphToFile(g, NULL, 0);
}

// ---------------- DISPLAY ----------------
void displayFlightNetwork(Graph* g) {
    printf(CYAN "\n=== CURRENT FLIGHT NETWORK ===\n" RESET);
    for (int i = 0; i < g->cityCount; i++) {
        printf("%s → ", g->cities[i]);
        Node* temp = g->adjList[i];
        if (!temp) printf("No direct flights");
        while (temp) {
            printf("%s(%d hrs)", temp->city, temp->time);
            if (temp->next) printf(", ");
            temp = temp->next;
        }
        printf("\n");
    }
}

// ---------------- BFS CONNECTIVITY ----------------
void bfsConnectivity(Graph* g, const char* startCity) {
    int startIdx = findCityIndex(g, startCity);
    if (startIdx == -1) {
        printf(RED "Invalid city!\n" RESET);
        return;
    }

    int visited[MAX_CITIES] = {0};
    Queue q;
    initQueue(&q);
    enqueue(&q, startIdx);
    visited[startIdx] = 1;

    FILE* fout = fopen("output.txt", "w");
    if (!fout) {
        printf(RED "⚠️ Could not open output.txt for writing!\n" RESET);
        return;
    }

    fprintf(fout, "==============================\n");
    fprintf(fout, "       ✈️  FLIGHT CONNECTIVITY\n");
    fprintf(fout, "==============================\n\n");
    fprintf(fout, "Starting city: %s\n\n", startCity);
    fprintf(fout, "Reachable cities (BFS traversal order):\n");

    printf(YELLOW "\nReachable cities from %s:\n" RESET, startCity);

    while (!isEmpty(&q)) {
        int curr = dequeue(&q);
        printf("%s ", g->cities[curr]);
        fprintf(fout, "%s ", g->cities[curr]);

        Node* temp = g->adjList[curr];
        while (temp) {
            int idx = findCityIndex(g, temp->city);
            if (!visited[idx]) {
                enqueue(&q, idx);
                visited[idx] = 1;
            }
            temp = temp->next;
        }
    }

    printf("\n");
    fprintf(fout, "\n\n==============================\n");
    fprintf(fout, "Connectivity updated successfully!\n");
    fclose(fout);

    printf(GREEN "✅ Connectivity details written to output.txt\n" RESET);
}


// ---------------- FIND FLIGHT PATH ----------------
void findFlightPath(Graph* g, const char* startCity, const char* endCity) {
    int startIdx = findCityIndex(g, startCity);
    int endIdx = findCityIndex(g, endCity);
    if (startIdx == -1 || endIdx == -1) { printf(RED "Invalid city name!\n" RESET); return; }

    int visited[MAX_CITIES] = {0}, parent[MAX_CITIES];
    for (int i = 0; i < MAX_CITIES; i++) parent[i] = -1;

    Queue q; initQueue(&q);
    enqueue(&q, startIdx);
    visited[startIdx] = 1;

    while (!isEmpty(&q)) {
        int curr = dequeue(&q);
        Node* temp = g->adjList[curr];
        while (temp) {
            int idx = findCityIndex(g, temp->city);
            if (!visited[idx]) {
                parent[idx] = curr;
                enqueue(&q, idx);
                visited[idx] = 1;
                if (idx == endIdx) break;
            }
            temp = temp->next;
        }
    }

    if (!visited[endIdx]) {
        printf(RED "❌ No route found!\n" RESET);
        return;
    }

    int path[MAX_CITIES], count = 0;
    for (int i = endIdx; i != -1; i = parent[i])
        path[count++] = i;

    printf(GREEN "\n🛫 Shortest route: " RESET);
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", g->cities[path[i]]);
        if (i != 0) printf(" → ");
    }
    printf("\n");

    // Build highlight list for graph output
    const char* highlights[MAX_CITIES][2];
    for (int i = count - 1, j = 0; i > 0; i--, j++) {
        highlights[j][0] = g->cities[path[i]];
        highlights[j][1] = g->cities[path[i - 1]];
    }

    saveGraphToFile(g, highlights, count - 1);
}
