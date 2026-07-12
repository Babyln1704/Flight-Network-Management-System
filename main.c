#include <stdio.h>
#include <stdlib.h>
#include "flight_network.h"

int main() {
    Graph g;
    initGraph(&g);
    loadRoutesFromFile(&g, "routes.txt");

    int choice, time;
    char src[50], dest[50];

    while (1) {
        printf(CYAN "\n=== FLIGHT NETWORK MENU ===\n" RESET);
        printf("1. Add Flight Route\n2. Remove Flight Route\n3. Display Network\n4. Check Connectivity (BFS)\n5. Find Shortest Path\n6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Source Destination Time(hr): ");
                scanf("%s %s %d", src, dest, &time);
                addFlightRoute(&g, src, dest, time);
                printf(GREEN "✅ Route added successfully!\n" RESET);
                break;
            case 2:
                printf("Enter Source Destination to remove: ");
                scanf("%s %s", src, dest);
                removeFlightRoute(&g, src, dest);
                printf(YELLOW "🗑️ Route removed successfully!\n" RESET);
                break;
            case 3:
                displayFlightNetwork(&g);
                break;
            case 4:
                printf("Enter start city: ");
                scanf("%s", src);
                bfsConnectivity(&g, src);
                break;
            case 5:
                printf("Enter Start and End City: ");
                scanf("%s %s", src, dest);
                findFlightPath(&g, src, dest);
                printf(CYAN "\n✅ Shortest path highlighted in graph_output.txt\n" RESET);
                break;
            case 6:
                printf(YELLOW "\nExiting... Safe travels!\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice!\n" RESET);
        }
    }
    return 0;
}
