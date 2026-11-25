
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>
#include <fstream>
#include <cmath>

using namespace std;

// ==================== CONSTANTS ====================

const int MAX_VEHICLES = 100;
const int TOTAL_SLOTS = 10;
const int GRAPH_NODES = 14;
const int INF = numeric_limits<int>::max();


// ==================== STRUCTURES ====================

struct Vehicle {
    string number;
    string owner;
};

struct ParkingSlot {
    int slotNumber;
    int distance;
    bool isAllocated;
    bool isInside;
    string vehicleNumber;
};

struct ParkingPriority {
    int slotNumber;
    int distance;
};

struct CompareDistance {
    bool operator()(const ParkingPriority& a,
                    const ParkingPriority& b) const {

        if (a.distance == b.distance)
            return a.slotNumber > b.slotNumber;

        return a.distance > b.distance;
    }
};

struct HistoryNode {
    string message;
    HistoryNode* next;
};

struct BSTNode {
    string number;
    string owner;
    BSTNode* left;
    BSTNode* right;
};

struct Edge {
    int destination;
    int weight;
};


// ==================== GLOBAL VARIABLES ====================

Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

ParkingSlot slots[TOTAL_SLOTS];

unordered_map<string, int> vehicleIndex;

queue<string> waitingQueue;

priority_queue<ParkingPriority,
               vector<ParkingPriority>,
               CompareDistance> parkingPriority;

HistoryNode* historyHead = nullptr;
HistoryNode* historyTail = nullptr;

BSTNode* bstRoot = nullptr;

vector<Edge> graph[GRAPH_NODES];


// ==================== FUNCTION DECLARATIONS ====================

BSTNode* insertBST(BSTNode* root, string number, string owner);
BSTNode* searchBST(BSTNode* root, string number);

void addHistory(string message, bool save = true);

void saveVehicles();
void saveParking();
void saveHistory();

int findVehicle(string number);


// ==================== LINKED LIST ====================

void addHistory(string message, bool save) {

    HistoryNode* newNode = new HistoryNode;

    newNode->message = message;
    newNode->next = nullptr;

    if (historyHead == nullptr) {
        historyHead = newNode;
        historyTail = newNode;
    }
    else {
        historyTail->next = newNode;
        historyTail = newNode;
    }

    if (save)
        saveHistory();
}


void viewParkingHistory() {

    if (historyHead == nullptr) {
        cout << "\nNo parking history available!\n";
        return;
    }

    cout << "\n===== PARKING HISTORY (LINKED LIST) =====\n";

    HistoryNode* current = historyHead;
    int count = 1;

    while (current != nullptr) {

        cout << count << ". " << current->message << endl;

        current = current->next;
        count++;
    }

    cout << "\nEnd of Linked List (NULL)\n";
}


void clearHistory() {

    HistoryNode* current = historyHead;

    while (current != nullptr) {

        HistoryNode* temp = current;
        current = current->next;

        delete temp;
    }

    historyHead = nullptr;
    historyTail = nullptr;
}


// ==================== BINARY SEARCH TREE ====================

BSTNode* insertBST(BSTNode* root, string number, string owner) {

    if (root == nullptr) {

        BSTNode* newNode = new BSTNode;

        newNode->number = number;
        newNode->owner = owner;
        newNode->left = nullptr;
        newNode->right = nullptr;

        return newNode;
    }

    if (number < root->number)
        root->left = insertBST(root->left, number, owner);

    else if (number > root->number)
        root->right = insertBST(root->right, number, owner);

    return root;
}


BSTNode* searchBST(BSTNode* root, string number) {

    if (root == nullptr || root->number == number)
        return root;

    if (number < root->number)
        return searchBST(root->left, number);

    return searchBST(root->right, number);
}


void inorderTraversal(BSTNode* root) {

    if (root == nullptr)
        return;

    inorderTraversal(root->left);

    cout << "Vehicle: " << root->number
         << " | Owner: " << root->owner << endl;

    inorderTraversal(root->right);
}


void preorderTraversal(BSTNode* root) {

    if (root == nullptr)
        return;

    cout << "Vehicle: " << root->number
         << " | Owner: " << root->owner << endl;

    preorderTraversal(root->left);
    preorderTraversal(root->right);
}


void postorderTraversal(BSTNode* root) {

    if (root == nullptr)
        return;

    postorderTraversal(root->left);
    postorderTraversal(root->right);

    cout << "Vehicle: " << root->number
         << " | Owner: " << root->owner << endl;
}


void searchVehicleBST() {

    string number;

    cout << "\nEnter Vehicle Number: ";
    cin >> number;

    BSTNode* result = searchBST(bstRoot, number);

    if (result == nullptr) {
        cout << "\nVehicle not found in BST!\n";
    }
    else {
        cout << "\n===== BST SEARCH RESULT =====\n";
        cout << "Vehicle Number: " << result->number << endl;
        cout << "Owner Name: " << result->owner << endl;
    }
}


void displayBSTStructure(BSTNode* root, int space = 0) {

    if (root == nullptr)
        return;

    space += 8;

    displayBSTStructure(root->right, space);

    cout << endl;

    for (int i = 8; i < space; i++)
        cout << " ";

    cout << root->number << endl;

    displayBSTStructure(root->left, space);
}


void showBSTStructure() {

    if (bstRoot == nullptr) {
        cout << "\nBST is empty!\n";
        return;
    }

    cout << "\n===== SMARTPARK BST STRUCTURE =====\n";

    displayBSTStructure(bstRoot);

    cout << "\nRoot Node: " << bstRoot->number << endl;
}


void clearBST(BSTNode* root) {

    if (root == nullptr)
        return;

    clearBST(root->left);
    clearBST(root->right);

    delete root;
}


// ==================== PARKING INITIALIZATION ====================

void initializeSlots() {

    int distances[TOTAL_SLOTS] = {
        20, 5, 15, 10, 25, 8, 18, 12, 30, 6
    };

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        slots[i].slotNumber = i + 1;
        slots[i].distance = distances[i];
        slots[i].isAllocated = false;
        slots[i].isInside = false;
        slots[i].vehicleNumber = "";
    }
}


// ==================== HASH MAP ====================

int findVehicle(string number) {

    auto it = vehicleIndex.find(number);

    if (it != vehicleIndex.end())
        return it->second;

    return -1;
}


bool isAlreadyWaiting(string number) {

    queue<string> temp = waitingQueue;

    while (!temp.empty()) {

        if (temp.front() == number)
            return true;

        temp.pop();
    }

    return false;
}


// ==================== FILE HANDLING ====================

// SAVE REGISTERED VEHICLES
// Format: vehicleNumber|owner name

void saveVehicles() {

    ofstream file("data/vehicles.txt");

    if (!file) {
        cout << "\nError saving vehicles.txt!\n";
        return;
    }

    for (int i = 0; i < vehicleCount; i++) {

        file << vehicles[i].number << "|"
             << vehicles[i].owner << "\n";
    }

    file.close();
}


// SAVE PARKING SLOTS AND WAITING QUEUE

void saveParking() {

    ofstream file("data/parking.txt");

    if (!file) {
        cout << "\nError saving parking.txt!\n";
        return;
    }

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        file << slots[i].slotNumber << " "
             << slots[i].isAllocated << " "
             << slots[i].isInside << " "
             << (slots[i].vehicleNumber.empty()
                 ? "-" : slots[i].vehicleNumber)
             << "\n";
    }

    queue<string> temp = waitingQueue;

    file << temp.size() << "\n";

    while (!temp.empty()) {

        file << temp.front() << "\n";
        temp.pop();
    }

    file.close();
}


// SAVE PARKING HISTORY

void saveHistory() {

    ofstream file("data/history.txt");

    if (!file) {
        cout << "\nError saving history.txt!\n";
        return;
    }

    HistoryNode* current = historyHead;

    while (current != nullptr) {

        file << current->message << "\n";
        current = current->next;
    }

    file.close();
}


// LOAD REGISTERED VEHICLES
// Supports both old and new file formats.

void loadVehicles() {

    ifstream file("data/vehicles.txt");

    if (!file)
        return;

    string line;

    while (getline(file, line)) {

        if (vehicleCount >= MAX_VEHICLES)
            break;

        if (line.empty())
            continue;

        string number;
        string owner;

        size_t separator = line.find('|');

        if (separator != string::npos) {

            number = line.substr(0, separator);
            owner = line.substr(separator + 1);
        }
        else {

            // Support old records with a single-word owner.
            size_t space = line.find(' ');

            if (space == string::npos)
                continue;

            number = line.substr(0, space);
            owner = line.substr(space + 1);
        }

        if (number.empty() || owner.empty())
            continue;

        if (vehicleIndex.find(number) != vehicleIndex.end())
            continue;

        vehicles[vehicleCount].number = number;
        vehicles[vehicleCount].owner = owner;

        vehicleIndex[number] = vehicleCount;

        bstRoot = insertBST(
            bstRoot,
            number,
            owner
        );

        vehicleCount++;
    }

    file.close();
}


// LOAD PARKING SLOTS AND WAITING QUEUE

void loadParking() {

    ifstream file("data/parking.txt");

    if (!file)
        return;

    int slotNumber;
    int allocated, inside;
    string number;

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (!(file >> slotNumber >> allocated >> inside >> number))
            break;

        if (slotNumber < 1 || slotNumber > TOTAL_SLOTS)
            continue;

        int index = slotNumber - 1;

        slots[index].isAllocated = (allocated != 0);
        slots[index].isInside = (inside != 0);

        slots[index].vehicleNumber =
            (number == "-" ? "" : number);

        if (!slots[index].isAllocated) {

            slots[index].isInside = false;
            slots[index].vehicleNumber = "";
        }
        else if (findVehicle(slots[index].vehicleNumber) == -1) {

            // Ignore a parking record for an unregistered vehicle.
            slots[index].isAllocated = false;
            slots[index].isInside = false;
            slots[index].vehicleNumber = "";
        }
    }

    int waitingCount = 0;

    if (file >> waitingCount) {

        for (int i = 0; i < waitingCount; i++) {

            if (file >> number) {

                if (findVehicle(number) != -1 &&
                    !isAlreadyWaiting(number)) {

                    waitingQueue.push(number);
                }
            }
        }
    }

    file.close();
}


// LOAD PARKING HISTORY

void loadHistory() {

    ifstream file("data/history.txt");

    if (!file)
        return;

    string message;

    while (getline(file, message)) {

        if (!message.empty())
            addHistory(message, false);
    }

    file.close();
}


// LOAD ALL DATA AND REBUILD MIN HEAP

void loadAllData() {

    loadVehicles();

    loadParking();

    loadHistory();

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (!slots[i].isAllocated) {

            parkingPriority.push({
                slots[i].slotNumber,
                slots[i].distance
            });
        }
    }
}


// ==================== VEHICLE REGISTRATION ====================

void registerVehicle() {

    if (vehicleCount >= MAX_VEHICLES) {

        cout << "\nRegistration limit reached!\n";
        return;
    }

    string number;
    string owner;

    cout << "\nEnter Vehicle Number: ";
    cin >> number;

    if (findVehicle(number) != -1) {

        cout << "\nVehicle is already registered!\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Owner Name: ";
    getline(cin, owner);

    if (owner.empty()) {

        cout << "\nOwner name cannot be empty!\n";
        return;
    }

    vehicles[vehicleCount].number = number;
    vehicles[vehicleCount].owner = owner;

    vehicleIndex[number] = vehicleCount;

    bstRoot = insertBST(
        bstRoot,
        number,
        owner
    );

    vehicleCount++;

    saveVehicles();

    addHistory(number + " registered successfully");

    cout << "\nVehicle registered and saved successfully!\n";
}


// ==================== PARKING ALLOCATION ====================

void allocateToWaitingVehicle(int releasedSlot) {

    int index = releasedSlot - 1;

    if (waitingQueue.empty()) {

        parkingPriority.push({
            slots[index].slotNumber,
            slots[index].distance
        });

        saveParking();
        return;
    }

    string waitingVehicle = waitingQueue.front();
    waitingQueue.pop();

    slots[index].isAllocated = true;
    slots[index].isInside = false;
    slots[index].vehicleNumber = waitingVehicle;

    cout << "\nWaiting vehicle " << waitingVehicle
         << " allocated Slot " << releasedSlot
         << " automatically!\n";

    addHistory(
        waitingVehicle + " allocated Slot " +
        to_string(releasedSlot) + " from waiting queue"
    );

    saveParking();
}


void allocateParkingSlot() {

    string number;

    cout << "\nEnter Vehicle Number: ";
    cin >> number;

    if (findVehicle(number) == -1) {

        cout << "\nVehicle is not registered!\n";
        return;
    }

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (slots[i].vehicleNumber == number &&
            slots[i].isAllocated) {

            cout << "\nVehicle already has a parking slot!\n";
            return;
        }
    }

    if (isAlreadyWaiting(number)) {

        cout << "\nVehicle already in waiting queue!\n";
        return;
    }

    if (parkingPriority.empty()) {

        waitingQueue.push(number);

        cout << "\nParking lot is full!\n";
        cout << "Vehicle added to waiting queue.\n";

        addHistory(number + " added to waiting queue");

        saveParking();
        return;
    }

    ParkingPriority nearestSlot = parkingPriority.top();
    parkingPriority.pop();

    int index = nearestSlot.slotNumber - 1;

    slots[index].isAllocated = true;
    slots[index].isInside = false;
    slots[index].vehicleNumber = number;

    cout << "\nParking slot allocated successfully!\n";
    cout << "Vehicle Number: " << number << endl;
    cout << "Allocated Slot: " << nearestSlot.slotNumber << endl;
    cout << "Distance: " << nearestSlot.distance << " metres\n";

    addHistory(
        number + " allocated Slot " +
        to_string(nearestSlot.slotNumber)
    );

    saveParking();
}


// ==================== VEHICLE ENTRY ====================

void vehicleEntry() {

    string number;

    cout << "\nEnter Vehicle Number: ";
    cin >> number;

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (slots[i].vehicleNumber == number &&
            slots[i].isAllocated) {

            if (slots[i].isInside) {

                cout << "\nVehicle is already inside!\n";
                return;
            }

            slots[i].isInside = true;

            cout << "\nVehicle entry recorded successfully!\n";

            addHistory(
                number + " entered Slot " +
                to_string(slots[i].slotNumber)
            );

            saveParking();
            return;
        }
    }

    cout << "\nNo parking allocation found!\n";
}


// ==================== VEHICLE EXIT ====================

void vehicleExit() {

    string number;

    cout << "\nEnter Vehicle Number: ";
    cin >> number;

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (slots[i].vehicleNumber == number &&
            slots[i].isAllocated) {

            if (!slots[i].isInside) {

                cout << "\nVehicle has not entered yet!\n";
                return;
            }

            int releasedSlot = slots[i].slotNumber;

            cout << "\nVehicle exited successfully!\n";
            cout << "Released Slot: " << releasedSlot << endl;

            addHistory(
                number + " exited Slot " +
                to_string(releasedSlot)
            );

            slots[i].isAllocated = false;
            slots[i].isInside = false;
            slots[i].vehicleNumber = "";

            allocateToWaitingVehicle(releasedSlot);

            saveParking();
            return;
        }
    }

    cout << "\nVehicle is not currently parked!\n";
}


// ==================== HASH MAP SEARCH ====================

void searchVehicle() {

    string number;

    cout << "\nEnter Vehicle Number: ";
    cin >> number;

    int index = findVehicle(number);

    if (index == -1) {

        cout << "\nVehicle not found!\n";
        return;
    }

    cout << "\n===== HASHING SEARCH RESULT =====\n";
    cout << "Vehicle Number: " << vehicles[index].number << endl;
    cout << "Owner Name: " << vehicles[index].owner << endl;

    bool found = false;

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (slots[i].vehicleNumber == number &&
            slots[i].isAllocated) {

            cout << "Parking Slot: " << slots[i].slotNumber << endl;
            cout << "Distance: " << slots[i].distance << " metres\n";

            cout << "Status: "
                 << (slots[i].isInside ? "Occupied\n" : "Reserved\n");

            found = true;
            break;
        }
    }

    if (!found) {

        if (isAlreadyWaiting(number))
            cout << "Status: Waiting in Queue\n";
        else
            cout << "Status: Registered, not currently allocated\n";
    }
}


// ==================== VIEW PARKING SLOTS ====================

void viewAvailableSlots() {

    cout << "\n===== PARKING SLOT DETAILS =====\n";

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        cout << "\nSlot Number: " << slots[i].slotNumber;
        cout << "\nDistance: " << slots[i].distance << " metres";
        cout << "\nStatus: ";

        if (slots[i].isInside)
            cout << "Occupied";
        else if (slots[i].isAllocated)
            cout << "Reserved";
        else
            cout << "Available";

        cout << endl;
    }
}


// ==================== WAITING QUEUE ====================

void viewWaitingQueue() {

    if (waitingQueue.empty()) {

        cout << "\nWaiting queue is empty!\n";
        return;
    }

    queue<string> temp = waitingQueue;

    cout << "\n===== WAITING QUEUE (FIFO) =====\n";

    int position = 1;

    while (!temp.empty()) {

        cout << position << ". " << temp.front() << endl;

        temp.pop();
        position++;
    }
}


// ==================== BUBBLE SORT ====================

void sortParkingSlots() {

    int sortedSlots[TOTAL_SLOTS];

    for (int i = 0; i < TOTAL_SLOTS; i++)
        sortedSlots[i] = i;

    for (int i = 0; i < TOTAL_SLOTS - 1; i++) {

        for (int j = 0; j < TOTAL_SLOTS - i - 1; j++) {

            if (slots[sortedSlots[j]].distance >
                slots[sortedSlots[j + 1]].distance) {

                swap(sortedSlots[j], sortedSlots[j + 1]);
            }
        }
    }

    cout << "\n===== PARKING SLOTS SORTED BY DISTANCE =====\n";
    cout << "\nSlot\tDistance\tStatus\n";

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        int index = sortedSlots[i];

        cout << slots[index].slotNumber << "\t"
             << slots[index].distance << "m\t\t";

        if (slots[index].isInside)
            cout << "Occupied";
        else if (slots[index].isAllocated)
            cout << "Reserved";
        else
            cout << "Available";

        cout << endl;
    }
}


// ==================== STEP 14: TWO POINTERS ====================

// Find two available slots whose combined distance
// is closest to the target distance.

void findClosestSlotPair() {

    vector<int> availableSlots;

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (!slots[i].isAllocated)
            availableSlots.push_back(i);
    }

    if (availableSlots.size() < 2) {

        cout << "\nAt least two available slots are required!\n";
        return;
    }

    sort(
        availableSlots.begin(),
        availableSlots.end(),
        [](int a, int b) {
            return slots[a].distance < slots[b].distance;
        }
    );

    int target;

    cout << "\nEnter target combined distance in metres: ";
    cin >> target;

    int left = 0;
    int right = (int)availableSlots.size() - 1;

    int bestLeft = -1;
    int bestRight = -1;
    int bestDifference = INF;
    int bestSum = 0;

    while (left < right) {

        int firstIndex = availableSlots[left];
        int secondIndex = availableSlots[right];

        int sum = slots[firstIndex].distance +
                  slots[secondIndex].distance;

        int difference = abs(sum - target);

        if (difference < bestDifference) {

            bestDifference = difference;

            bestLeft = firstIndex;
            bestRight = secondIndex;

            bestSum = sum;
        }

        if (sum == target)
            break;

        if (sum < target)
            left++;
        else
            right--;
    }

    cout << "\n===== TWO POINTERS RESULT =====\n";

    cout << "Target Distance: " << target << " metres\n";

    cout << "First Slot: " << slots[bestLeft].slotNumber
         << " (" << slots[bestLeft].distance << " metres)\n";

    cout << "Second Slot: " << slots[bestRight].slotNumber
         << " (" << slots[bestRight].distance << " metres)\n";

    cout << "Combined Distance: " << bestSum << " metres\n";

    cout << "Difference from Target: "
         << bestDifference << " metres\n";

    cout << "\nCalculated using the Two Pointers technique.\n";
}


// ==================== STEP 14: SLIDING WINDOW ====================

// Find the group of consecutive available slots
// with the lowest average distance.

void slidingWindowAnalysis() {

    vector<int> availableSlots;

    for (int i = 0; i < TOTAL_SLOTS; i++) {

        if (!slots[i].isAllocated)
            availableSlots.push_back(i);
    }

    if (availableSlots.empty()) {

        cout << "\nNo available parking slots!\n";
        return;
    }

    sort(
        availableSlots.begin(),
        availableSlots.end(),
        [](int a, int b) {
            return slots[a].distance < slots[b].distance;
        }
    );

    int k;

    cout << "\nNumber of slots in each window: ";
    cin >> k;

    if (k <= 0 || k > (int)availableSlots.size()) {

        cout << "\nInvalid window size!\n";
        return;
    }

    int windowSum = 0;

    // Calculate the first window.
    for (int i = 0; i < k; i++) {
        windowSum += slots[availableSlots[i]].distance;
    }

    int bestSum = windowSum;
    int bestStart = 0;

    cout << "\n===== SLIDING WINDOW ANALYSIS =====\n";

    cout << "Window 1: ";

    for (int i = 0; i < k; i++) {

        cout << "Slot "
             << slots[availableSlots[i]].slotNumber << " ";
    }

    cout << "| Average: "
         << (double)windowSum / k << " metres\n";

    // Slide the window one position at a time.
    for (int i = k; i < (int)availableSlots.size(); i++) {

        windowSum += slots[availableSlots[i]].distance;

        windowSum -= slots[availableSlots[i - k]].distance;

        int windowNumber = i - k + 2;

        cout << "Window " << windowNumber << ": ";

        for (int j = i - k + 1; j <= i; j++) {

            cout << "Slot "
                 << slots[availableSlots[j]].slotNumber << " ";
        }

        cout << "| Average: "
             << (double)windowSum / k << " metres\n";

        if (windowSum < bestSum) {

            bestSum = windowSum;
            bestStart = i - k + 1;
        }
    }

    cout << "\n===== BEST PARKING GROUP =====\n";

    for (int i = bestStart; i < bestStart + k; i++) {

        cout << "Slot "
             << slots[availableSlots[i]].slotNumber
             << " (" << slots[availableSlots[i]].distance
             << " metres)\n";
    }

    cout << "Lowest Average Distance: "
         << (double)bestSum / k << " metres\n";

    cout << "\nCalculated using the Sliding Window technique.\n";
}


// ==================== GRAPH AND DIJKSTRA ====================

void addEdge(int source, int destination, int weight) {

    graph[source].push_back({destination, weight});
    graph[destination].push_back({source, weight});
}


void initializeGraph() {

    addEdge(0, 1, 4);

    addEdge(1, 2, 3);
    addEdge(1, 3, 5);

    addEdge(2, 4, 4);
    addEdge(2, 5, 2);
    addEdge(2, 6, 5);
    addEdge(2, 7, 7);
    addEdge(2, 8, 8);

    addEdge(3, 9, 2);
    addEdge(3, 10, 4);
    addEdge(3, 11, 3);
    addEdge(3, 12, 6);
    addEdge(3, 13, 1);
}


string getNodeName(int node) {

    if (node == 0) return "Entrance";
    if (node == 1) return "Main Junction";
    if (node == 2) return "Left Lane";
    if (node == 3) return "Right Lane";

    return "Slot " + to_string(node - 3);
}


void dijkstraShortestPath() {

    int destinationSlot;

    cout << "\nEnter destination parking slot (1-10): ";
    cin >> destinationSlot;

    if (destinationSlot < 1 || destinationSlot > TOTAL_SLOTS) {

        cout << "\nInvalid parking slot number!\n";
        return;
    }

    int source = 0;
    int destination = destinationSlot + 3;

    vector<int> distance(GRAPH_NODES, INF);
    vector<int> parent(GRAPH_NODES, -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {

        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;

        pq.pop();

        if (currentDistance > distance[currentNode])
            continue;

        for (const Edge& edge : graph[currentNode]) {

            int nextNode = edge.destination;
            int newDistance = currentDistance + edge.weight;

            if (newDistance < distance[nextNode]) {

                distance[nextNode] = newDistance;
                parent[nextNode] = currentNode;

                pq.push({newDistance, nextNode});
            }
        }
    }

    if (distance[destination] == INF) {

        cout << "\nNo route found!\n";
        return;
    }

    vector<int> path;

    int current = destination;

    while (current != -1) {

        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    cout << "\n===== SHORTEST PARKING ROUTE =====\n";

    for (int i = 0; i < (int)path.size(); i++) {

        cout << getNodeName(path[i]);

        if (i != (int)path.size() - 1)
            cout << " -> ";
    }

    cout << "\n\nTotal Shortest Distance: "
         << distance[destination] << " metres\n";

    cout << "\nRoute calculated using Dijkstra's Algorithm.\n";
}


// ==================== MAIN FUNCTION ====================

int main() {

    initializeSlots();

    loadAllData();

    initializeGraph();

    cout << "\n===== SMARTPARK DATA LOADED =====\n";
    cout << "Registered Vehicles: " << vehicleCount << endl;

    int choice;

    do {

        cout << "\n\n========================================\n";
        cout << "       SMARTPARK MANAGEMENT SYSTEM\n";
        cout << "========================================\n";

        cout << "1. Register Vehicle\n";
        cout << "2. Allocate Parking Slot\n";
        cout << "3. View Available Slots\n";
        cout << "4. Vehicle Entry\n";
        cout << "5. Vehicle Exit\n";
        cout << "6. Search Vehicle (Hashing)\n";
        cout << "7. View Waiting Queue\n";
        cout << "8. View Parking History (Linked List)\n";
        cout << "9. Sort Parking Slots by Distance\n";
        cout << "10. Search Vehicle Using BST\n";
        cout << "11. Inorder Traversal\n";
        cout << "12. Preorder Traversal\n";
        cout << "13. Postorder Traversal\n";
        cout << "14. Display BST Structure\n";
        cout << "15. Find Shortest Route (Dijkstra)\n";
        cout << "16. Find Closest Slot Pair (Two Pointers)\n";
        cout << "17. Sliding Window Parking Analysis\n";
        cout << "18. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        if (cin.fail()) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nPlease enter a valid number!\n";
            continue;
        }

        switch (choice) {

            case 1:
                registerVehicle();
                break;

            case 2:
                allocateParkingSlot();
                break;

            case 3:
                viewAvailableSlots();
                break;

            case 4:
                vehicleEntry();
                break;

            case 5:
                vehicleExit();
                break;

            case 6:
                searchVehicle();
                break;

            case 7:
                viewWaitingQueue();
                break;

            case 8:
                viewParkingHistory();
                break;

            case 9:
                sortParkingSlots();
                break;

            case 10:
                searchVehicleBST();
                break;

            case 11:

                if (bstRoot == nullptr)
                    cout << "\nBST is empty!\n";
                else {
                    cout << "\n===== BST INORDER TRAVERSAL =====\n";
                    inorderTraversal(bstRoot);
                }

                break;

            case 12:

                if (bstRoot == nullptr)
                    cout << "\nBST is empty!\n";
                else {
                    cout << "\n===== BST PREORDER TRAVERSAL =====\n";
                    preorderTraversal(bstRoot);
                }

                break;

            case 13:

                if (bstRoot == nullptr)
                    cout << "\nBST is empty!\n";
                else {
                    cout << "\n===== BST POSTORDER TRAVERSAL =====\n";
                    postorderTraversal(bstRoot);
                }

                break;

            case 14:
                showBSTStructure();
                break;

            case 15:
                dijkstraShortestPath();
                break;

            case 16:
                findClosestSlotPair();
                break;

            case 17:
                slidingWindowAnalysis();
                break;

            case 18:

                saveVehicles();
                saveParking();
                saveHistory();

                cout << "\nAll SmartPark data saved successfully!\n";
                cout << "Thank you for using SmartPark!\n";

                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 18);

    clearHistory();
    clearBST(bstRoot);

    return 0;
}
