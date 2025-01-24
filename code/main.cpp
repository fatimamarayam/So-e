#include <iostream>
#include <string>
#include <vector>
using namespace std;
class AntGroup {
public:
    string identifier;
    string classification;
    int laborers;
    int fighters;
    int enemyDefeats;
    vector<string> conqueredGroups;
    int survivalDuration;
    bool isActive;
    string eliminator;
AntGroup(const string& groupName, const string& groupType) {
        identifier = groupName;
        classification = groupType;
        laborers = 0;
        fighters = 0;
        enemyDefeats = 0;
        survivalDuration = 0;
        isActive = true;
        eliminator = "";}
void augmentPopulation(int laborCount, int fighterCount) {
        if (!isActive) return;
        laborers += laborCount;
        fighters += fighterCount;}
void recordEnemyDefeats(int victories) {
        if (!isActive) return;
        enemyDefeats += victories;}
void registerConquest(const string& groupName) {
        if (!isActive) return;
        conqueredGroups.push_back(groupName);}
void advanceTime() {
        if (isActive) survivalDuration++;}
void terminate(const string& defeatSource) {
        isActive = false;
        eliminator = defeatSource;}
void displayStatus() const {
        cout << "Type: " << classification << endl;
        cout << "Workers: " << laborers << endl;
        cout << "Warriors: " << fighters << endl;
        cout << "Eliminations: " << enemyDefeats << endl;
        cout << "Conquests: " << conqueredGroups.size();
        if (!conqueredGroups.empty()) {
            cout << " (";
            for (size_t i = 0; i < conqueredGroups.size(); ++i) {
                cout << conqueredGroups[i];
                if (i < conqueredGroups.size() - 1) cout << ", ";
            }
            cout << ")";
        }
        cout << endl;
        cout << "Survival Time: " << survivalDuration << endl;
        cout << "Status: " << (isActive ? "Active" : "Eliminated by " + eliminator) << endl;
    }
};
class EcosystemTerritory {
private:
    static EcosystemTerritory* singularInstance;
    AntGroup** populations;
    int populationCount;
    static const int POPULATION_LIMIT = 10;
public:
    EcosystemTerritory() {
        populations = new AntGroup*[POPULATION_LIMIT];
        populationCount = 0;}
~EcosystemTerritory() {
        for (int i = 0; i < populationCount; ++i) {
            delete populations[i];}
        delete[] populations;}
static EcosystemTerritory* obtainInstance() {
        if (!singularInstance) {
            singularInstance = new EcosystemTerritory();}
        return singularInstance;}
void createPopulation(const string& name, const string& species) {
        if (populationCount < POPULATION_LIMIT) {
            populations[populationCount] = new AntGroup(name, species);
            populationCount++;
            cout << "Population " << name << " of type " << species << " established." << endl;
        } else {
            cout << "Population limit reached. Cannot create more groups." << endl;}
    }
void supplyResources(int groupIndex, int laborers, int fighters) {
        if (groupIndex < 0 || groupIndex >= populationCount) {
            cout << "Invalid group selection." << endl;
            return;}
        populations[groupIndex]->augmentPopulation(laborers, fighters);
        cout << "Resources allocated to " << populations[groupIndex]->identifier << "." << endl;}
void progressTimeline() {
        for (int i = 0; i < populationCount; ++i) {
            populations[i]->advanceTime();}
        cout << "Time progression completed." << endl;}
void initiateConflict(int aggressorIndex, int defenderIndex) {
        if (aggressorIndex < 0 || aggressorIndex >= populationCount || defenderIndex < 0 || defenderIndex >= populationCount) {
            cout << "Invalid group selection." << endl;
            return;}
AntGroup* aggressor = populations[aggressorIndex];
AntGroup* defender = populations[defenderIndex];
if (!aggressor->isActive || !defender->isActive) {
            cout << "Both populations must be active for conflict." << endl;
            return;}
if (aggressor->fighters > defender->fighters) {
            aggressor->recordEnemyDefeats(defender->laborers + defender->fighters);
            aggressor->registerConquest(defender->identifier);
            defender->terminate(aggressor->identifier);
            cout << aggressor->identifier << " defeated " << defender->identifier << "!" << endl;
        } else {
            cout << defender->identifier << " successfully defended." << endl;}
    }
void displayPopulationStatus(int groupIndex) {
        if (groupIndex < 0 || groupIndex >= populationCount) {
            cout << "Invalid group selection." << endl;
            return;}
        populations[groupIndex]->displayStatus();}
void enumeratePopulations() {
        cout << "Current Populations:" << endl;
        for (int i = 0; i < populationCount; ++i) {
            cout << i + 1 << ". " << populations[i]->identifier << " (" << populations[i]->classification << ")" << endl;}}
};
EcosystemTerritory* EcosystemTerritory::singularInstance = nullptr;
void presentInteractionMenu() {
    cout << endl << "--- Ecosystem Population Simulation ---" << endl;
    cout << "1. Establish Population" << endl;
    cout << "2. Allocate Resources" << endl;
    cout << "3. Progress Timeline" << endl;
    cout << "4. Initiate Conflict" << endl;
    cout << "5. Display Population Status" << endl;
    cout << "6. List Populations" << endl;
    cout << "7. Exit" << endl;
    cout << "Select action: ";
}
void processUserInteraction() {
    EcosystemTerritory* ecosystem = EcosystemTerritory::obtainInstance();
    bool systemRunning = true;
    while (systemRunning) {
        presentInteractionMenu();
        int selectedAction;
        cin >> selectedAction;
        switch (selectedAction) {
        case 1: {
            string name, species;
            cout << "Population Name: ";
            cin >> name;
            cout << "Population Type: ";
            cin >> species;
            ecosystem->createPopulation(name, species);
            break;
        }
        case 2: {
            int index, laborers, fighters;
            ecosystem->enumeratePopulations();
            cout << "Select Population Index: ";
            cin >> index;
            cout << "Laborers Count: ";
            cin >> laborers;
            cout << "Fighters Count: ";
            cin >> fighters;
            ecosystem->supplyResources(index - 1, laborers, fighters);
            break;}
        case 3:
            ecosystem->progressTimeline();
            break;
        case 4: {
            int aggressor, defender;
            ecosystem->enumeratePopulations();
            cout << "Aggressor Population Index: ";
            cin >> aggressor;
            cout << "Defender Population Index: ";
            cin >> defender;
            ecosystem->initiateConflict(aggressor - 1, defender - 1);
            break;}
        case 5: {
            int index;
            ecosystem->enumeratePopulations();
            cout << "Population Index for Status: ";
            cin >> index;
            ecosystem->displayPopulationStatus(index - 1);
            break;}
        case 6:
            ecosystem->enumeratePopulations();
            break;
        case 7:
            systemRunning = false;
            cout << "Simulation Terminating. Farewell!" << endl;
            break;
        default:
            cout << "Invalid Selection. Retry." << endl;
        }
    }
}
int main() {
    processUserInteraction();
    return 0;
}
