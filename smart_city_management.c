#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NODES 10


struct Vehicle {
    int id;
    char type[20];
    int currentNode; 
    int destination;
    bool isEmergency; 
    struct Vehicle *next;
};

struct Vehicle *head = NULL;
int graph[MAX_NODES][MAX_NODES];
char trafficLights[MAX_NODES][10]; 

void addVehicle(int id, char type[], int startNode, int destination, bool isEmergency) {
    struct Vehicle *newVehicle = (struct Vehicle*) malloc(sizeof(struct Vehicle));
    newVehicle->id = id;
    strcpy(newVehicle->type, type);
    newVehicle->currentNode = startNode;
    newVehicle->destination = destination;
    newVehicle->isEmergency = isEmergency;
    newVehicle->next = head;
    head = newVehicle;
}

void displayVehicles() {
    struct Vehicle *temp = head;
    while (temp) {
        printf("Vehicle ID: %d, Type: %s, Current Node: %d, Destination: %d, Emergency: %s\n",
               temp->id, temp->type, temp->currentNode, temp->destination,
               temp->isEmergency ? "Yes" : "No");
        temp = temp->next;
    }
}


void inputGraph() {
    int edges, u, v, weight;
    printf("Enter number of edges: ");
    scanf("%d", &edges);
    for (int i = 0; i < edges; i++) {
        printf("Enter edge (u, v, weight): ");
        scanf("%d %d %d", &u, &v, &weight);
        graph[u][v] = weight;
        graph[v][u] = weight; 
    }
}


void manageTrafficLights() {
    printf("\nSetting traffic lights for active nodes:\n");
    for (int i = 0; i < MAX_NODES; i++) {
        bool hasEdge = false;
        for (int j = 0; j < MAX_NODES; j++) {
            if (graph[i][j] > 0) { 
                hasEdge = true;
                break;
            }
        }
        if (hasEdge) {
            printf("Enter traffic light status for Node %d (Green/Red): ", i);
            scanf("%s", trafficLights[i]);
        }
    }
}


void dijkstra(int start, int dist[], int prev[]) {
    bool visited[MAX_NODES] = {false};
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (int count = 0; count < MAX_NODES - 1; count++) {
        int minDist = INT_MAX, minIndex;
        for (int v = 0; v < MAX_NODES; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                minIndex = v;
            }
        }
        visited[minIndex] = true;
        for (int v = 0; v < MAX_NODES; v++) {
            if (!visited[v] && graph[minIndex][v] && dist[minIndex] + graph[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + graph[minIndex][v];
                prev[v] = minIndex;
            }
        }
    }
}


void moveVehicles() {
    struct Vehicle *temp = head;
    while (temp) {
        printf("\nVehicle %d (Type: %s) is at Node %d\n", temp->id, temp->type, temp->currentNode);

        int dist[MAX_NODES], prev[MAX_NODES];
        dijkstra(temp->currentNode, dist, prev);

        if (dist[temp->destination] == INT_MAX) {
            printf("No path to destination for vehicle %d.\n", temp->id);
        } else {
            int nextNode = prev[temp->destination];

            if (temp->isEmergency || strcmp(trafficLights[temp->currentNode], "Green") == 0) {
                printf("Vehicle %d moving from Node %d to Node %d\n", temp->id, temp->currentNode, nextNode);
                temp->currentNode = nextNode;
            } else {
                printf("Vehicle %d is waiting at Node %d due to Red light.\n", temp->id, temp->currentNode);
            }
        }
        temp = temp->next;
    }
}

int main() {

    memset(graph, 0, sizeof(graph));

    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Vehicle\n");
        printf("2. Display Vehicles\n");
        printf("3. Enter Road Network\n");
        printf("4. Manage Traffic Lights\n");
        printf("5. Move Vehicles\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int id, startNode, destination;
            char type[20];
            char isEmergencyInput[4];
            bool isEmergency = false;

            printf("Enter Vehicle ID: ");
            scanf("%d", &id);
            printf("Enter Vehicle Type: ");
            scanf("%s", type);
            printf("Enter starting node: ");
            scanf("%d", &startNode);
            printf("Enter destination node: ");
            scanf("%d", &destination);
            printf("Is the vehiclE an emergency (yes/no): ");
            scanf("%s", isEmergencyInput);

            if (strcmp(isEmergencyInput, "yes") == 0) {
                isEmergency = true;
            }

            addVehicle(id, type, startNode, destination, isEmergency);
        } else if (choice == 2) {
            displayVehicles();
        } else if (choice == 3) {
            inputGraph();
        } else if (choice == 4) {
            manageTrafficLights();
        } else if (choice == 5) {
            moveVehicles();
        } else if (choice == 6) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice!!! Try again.\n");
        }
    }
    return 0;
}
